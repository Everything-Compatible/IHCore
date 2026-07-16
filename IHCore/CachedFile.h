#pragma once
#include <IH.File.h>
#include <IH.Loader.h>

//Set to 1 when testing Cached File Manager
//#define CFM_DebugMode 1
#if CFM_DebugMode
#define CFM_Log  Debug::LogFormat
#else
#define CFM_Log(...)
#endif

namespace CachedFile
{
	static constexpr size_t CachePageSize = 16 * 1024 * 1024; //8MB per page

	size_t GenerateToken(const std::string& Class, const char* pFileName);

	int Seek(size_t Token, IHFileClass* File, int& Position, int Offset);

	int ReadBytes(size_t Token, IHFileClass* File, FileIterationType IterType, int& Position, void* Buffer, int Size);

	void ClearCache(size_t Token);

	void TakeOverIHFile(IHExtPtrType* pExt);

	IHExtPtrType* ReleaseIHFile(size_t Token);

	bool CanAcceptIHFile(size_t Token);

	bool CanReleaseIHFile(size_t Token);

	bool CanAcceptIHExtPtr(IHExtPtrType* pExt);

	void SetShuttingDown();
}
