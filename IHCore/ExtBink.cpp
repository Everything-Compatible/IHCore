#include "ExtBink.h"
#include <stdexcept>
#include "Patch.h"
#include "Debug.h"
#include "ExtCD.h"
#include <EC.Misc.h>

static const char* BinkDLLName = "BINKW32.DLL";
static HMODULE BinkDLLHandle = nullptr;
static void (__stdcall* BinkSetIO_Fn) (BinkIOInitFn callback) = nullptr;
static int (__stdcall* BinkOpen_Fn) (DWORD Handle, DWORD Flags) = nullptr;

HMODULE GetBinkW32()
{
	if(!BinkDLLHandle)
		BinkDLLHandle = LoadLibraryA(BinkDLLName);
	if (!BinkDLLHandle)
		throw std::runtime_error("ExtBink.cpp : Cannot open BINKW32.DLL");
	return BinkDLLHandle;
}

void __stdcall ExtBink_SetIO(BinkIOInitFn callback)
{
	if (!BinkSetIO_Fn)
	{
		auto hModule = GetBinkW32();
		if(hModule)
			BinkSetIO_Fn = (void(__stdcall*)(BinkIOInitFn))GetProcAddress(hModule, "_BinkSetIO@4");
	}
	//never be caught but let syringe see
	if(!BinkSetIO_Fn)
		throw std::runtime_error("ExtBink.cpp : Cannot find BinkSetIO in BINKW32.DLL");
	BinkSetIO_Fn(callback);
}

int __stdcall ExtBink_Open(DWORD Handle, DWORD Flags)
{
	if (!BinkOpen_Fn)
	{
		auto hModule = GetBinkW32();
		if (hModule)
			BinkOpen_Fn = (int(__stdcall*)(DWORD, DWORD))GetProcAddress(hModule, "_BinkOpen@8");
	}
	//never be caught but let syringe see
	if (!BinkOpen_Fn)
		throw std::runtime_error("ExtBink.cpp : Cannot find BinkOpen in BINKW32.DLL");
	return BinkOpen_Fn(Handle, Flags);
}

std::unordered_map<BINKIO*, BinkIOUserData> BinkIOUserDataMap;
BINKIO* LastBinkIO = nullptr;

// Global .bik filename for proxy audio replacement
static char g_currentBikName[MAX_PATH] = "";

extern "C" __declspec(dllexport) const char* __stdcall ExtBink_GetCurrentBikName()
{
    return g_currentBikName;
}

namespace ExtBink
{
	bool EnableExtBink = false;
	CCFileClass* ExtBinkFile = nullptr;

	struct DUMMY
	{
		DWORD dwReserved[3];
		const char* pFileName;
	}dummy;

	// ============================================================================
// slot 0: seek_read — 最高频，Bink 所有数据读取的入口
//   
//   pos >= 0  →  先 Seek(pos, SET) 再读
//   pos == -1 →  从当前位置继续读
//
//   返回实际读取字节数，0=EOF，负值=错误
// ============================================================================
	int __stdcall BinkIO_seek_read(BINKIO* io, int pos, void* buf, int size)
	{
		auto pFile = io->GetFile();
		EnterCriticalSection(io->GetCS());
		if (pos >= 0) {
			pFile->Seek(pos, FileSeekMode::Set);
		}
		int read = pFile->ReadBytes(buf, size);
		LeaveCriticalSection(io->GetCS());

		return read;
	}

	// ============================================================================
	// slot 1: read_async — 指定偏移的预读
	//
	//   BinkOpen 中调用，用于从指定 offset 读数据到 Bink 提供的缓冲区。
	//   与 seek_read 的区别：offset 总是绝对值，且带 flags 参数。
	// ============================================================================
	int __stdcall BinkIO_read_async(BINKIO* io, int frame, int offset, void* buf, int size)
	{
		auto pFile = io->GetFile();
		EnterCriticalSection(io->GetCS());
		pFile->Seek(offset, FileSeekMode::Set);
		int read = pFile->ReadBytes(buf, size);
		LeaveCriticalSection(io->GetCS());
		return read;
	}

	// ============================================================================
	// slot 2: io_size — 告诉 Bink 数据源总大小
	//
	//   BinkOpen 中调用，返回值用于：
	//     - 判断是否需要全量预读 buffer（size < 某阈值 → 全部读到内存）
	//     - 决定 buffer 分配策略
	//
	//   param: Bink 内部期望值，一般忽略，直接返回真实文件大小即可
	//
	//   返回 0 也行（Bink 走保守策略），返回真实值更好
	// ============================================================================
	int __stdcall BinkIO_io_size(BINKIO* io, int param)
	{
		return param;
	}

	// ============================================================================
	// slot 3: prepare — 填充 Bink 提供的预读缓冲区
	//
	//   Bink 已分配好 buf（大小 maxSize 字节），期望你从当前位置读 size 字节填入。
	//   类似 "commit" 语义——Bink 给你内存，你填数据。
	//
	//   size:  本次期望填充量
	//   maxSize: 缓冲区总容量
	//   flags:  BinkOpen 的 flags（可能含额外预读策略位）
	//
	//   无返回值（Bink 不检查），空了也不报错
	// ============================================================================
	void __stdcall BinkIO_prepare(BINKIO* io, void* buf, int size, int maxSize, int flags)
	{
		auto pFile = io->GetFile();

		// 从当前文件位置顺序读
		int total = 0;
		int remaining = size;
		char* dst = (char*)buf;

		EnterCriticalSection(io->GetCS());
		while (remaining > 0) {
			int n = pFile->ReadBytes(dst, remaining);
			if (n <= 0) break;          // EOF 或错误 → 停止填充
			dst += n;
			total += n;
			remaining -= n;
		}
		LeaveCriticalSection(io->GetCS());
	}

	// ============================================================================
	// slot 4: pending — Bink 轮询是否有后台 IO 正在进行
	//
	//   Bink 在发起异步操作后会循环调用此函数：
	//     while (ioState->pending(ioState)) ;   // 自旋等待
	//
	//   对于同步文件 IO：始终返回 0（即刻完成，无挂起操作）
	//   对于异步 IO：返回非零表示还有数据没读完
	// ============================================================================
	int __stdcall BinkIO_pending(BINKIO* io)
	{
		return 0; 
	}


	void __stdcall BinkIO_close(BINKIO* io)
	{
		auto& ud = io->GetUserData();
		if (ud.pFile) {
			ud.pFile->Close();
		}
		ud.pFile = nullptr;
		DeleteCriticalSection(ud.cs);
		delete ud.cs;
		ud.cs = nullptr;
		io->DestroyUserData();
	}


	int __stdcall BinkIO_InitFn(BINKIO* io, DWORD Handle, int Flags)
	{
		io->seek_read = BinkIO_seek_read;
		io->read_async = BinkIO_read_async;
		io->io_size = BinkIO_io_size;
		io->prepare = BinkIO_prepare;
		io->pending = BinkIO_pending;
		io->close = BinkIO_close;

		auto& UserData = io->GetUserData();
		UserData.pFile = (CCFileClass*)Handle;
		UserData.cs = new CRITICAL_SECTION();
		InitializeCriticalSection(UserData.cs);
		return 1;
	}

	bool __fastcall MixFileClass_Offset_Impl(const char* filename, void** data, MixFileClass** mixfile, int* offset, int* length)
	{
		//Debug::Log("[IH] ExtBink IN %s\n", filename);

		// Save original .bik filename for proxy audio replacement
		if (filename && filename[0])
			strncpy_s(g_currentBikName, sizeof(g_currentBikName), filename, _TRUNCATE);

		auto pCC = GameCreate<CCFileClass>(filename);
		pCC->Open(FileAccessMode::Read);
		//Debug::LogFormat("[IH] ExtBink CCFileClass {} Name {} Exists {} ExtPtr {}\n", (void*)pCC, pCC->GetFileName(), pCC->Exists(), (void*)pCC->IHExtPtr);
		if (pCC && pCC->IHExtPtr && pCC->Exists())
		{
			//Debug::Log("[IH] ExtBink ON\n");
			EnableExtBink = true;
			ExtBinkFile = pCC;
			dummy.pFileName = pCC->GetFileName();
			if(mixfile) *mixfile = (MixFileClass*) &dummy;
			if (offset) *offset = 0;
			return true;
		}
		else if (pCC)
		{
			GameDelete(pCC);
			return MixFileClass::Offset(filename, data, mixfile, offset, length);
		}
		return MixFileClass::Offset(filename, data, mixfile, offset, length);
	}
	HANDLE __stdcall CreateFileA_Impl(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		if (EnableExtBink)
		{
			HANDLE hNul = CreateFileA(
				"NUL",                       // 特殊设备名
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			return hNul;
		}
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	DWORD __stdcall SetFilePointer_Impl(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
	{
		if (EnableExtBink) return lDistanceToMove;
		return SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
	}
	int __stdcall BinkOpen_Impl(DWORD Handle, DWORD Flags)
	{
		//Debug::Log("[IH] ExtBink OUT\n");
		if (EnableExtBink)
		{
			ExtBink_SetIO(ExtBink::BinkIO_InitFn);
			auto pFile = ExtBinkFile;
			auto Ret = ExtBink_Open((DWORD)pFile, BINKIOPROCESSOR);
			ExtBinkFile = nullptr;
			EnableExtBink = false;
			LastBinkIO = nullptr;
			g_currentBikName[0] = '\0';  // clear after use
			if (!Ret && pFile)
			{
				// BinkOpen 失败 → close(栈BINKIO)只调了Close(), ownsFile=false不删
				GameDelete(pFile);
			}
			//Debug::Log("[IH] ExtBink OFF result=%p\n", Ret);
			return Ret;
		}
		g_currentBikName[0] = '\0';
		return ExtBink_Open(Handle, Flags);
	}
}


BinkIOUserData& BINKIO::GetUserData()
{
	if (BinkIOUserDataMap.contains(this))
	{
		LastBinkIO = this;
		return BinkIOUserDataMap.at(this);
	}
	else if (LastBinkIO)
	{
		BinkIOUserDataMap[this] = BinkIOUserDataMap.at(LastBinkIO);
		BinkIOUserDataMap[this].ownsFile = true;  // 迁移到堆BINKIO, 现在拥有pFile
		BinkIOUserDataMap.erase(LastBinkIO);
		LastBinkIO = this;
		return BinkIOUserDataMap.at(this);
	}
	else
	{
		return BinkIOUserDataMap[this];
	}
}
void BINKIO::DestroyUserData()
{
	BinkIOUserDataMap.erase(this);
	LastBinkIO = nullptr;
}

void ExtBink_InitBeforeEverything()
{
	Patch::Apply_CALL(0x432803, ExtBink::MixFileClass_Offset_Impl);
	Patch::Apply_CALL6(0x432824, ExtBink::CreateFileA_Impl);
	Patch::Apply_CALL6(0x43283A, ExtBink::SetFilePointer_Impl);
	Patch::Apply_CALL6(0x432849, ExtBink::BinkOpen_Impl);
}