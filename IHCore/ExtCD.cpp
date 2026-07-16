#include "ExtCD.h"
#include "Debug.h"
#include "Patch.h"
#include "ConfData.h"
#include "ToolFunc.h"
#include "InitialLoad.h"
#include "..\Common\LocalData.h"
#include <Helpers/Macro.h>
#include "CachedFile.h"

CDExt CDExt_Instance(&CDDrives::Instance);

CDExt::CDExt(CDDrives* p) :pDrives(p) {}

void CDExt::PushCustomPathToHead(const char* Path)
{
	PathHead.push_back(Path);
}
void CDExt::PushCustomPathToHead(const std::string& Path)
{
	PathHead.push_back(Path);
}
void CDExt::PushCustomPathToTail(const char* Path)
{
	PathTail.push_back(Path);
}
void CDExt::PushCustomPathToTail(const std::string& Path)
{
	PathTail.push_back(Path);
}
void CDExt::PushCustomPathToFirst(const char* Path)
{
	PathFirst.push_back(Path);
}
void CDExt::PushCustomPathToFirst(const std::string& Path)
{
	PathFirst.push_back(Path);
}

void CDExt::AddRedirect(const char* Old, const char* New)
{
	Redirect.insert({ Old,New });
}

const char* CDExt::TryRedirect(const char* Name)
{
	auto it = Redirect.find(Name);
	const char* New = Name;
	while (it != Redirect.end())
	{
		New = it->second.c_str();
		it = Redirect.find(it->second);
	}
	return New;
}

void CDExt::RegisterLoadedMix(const char* Name)
{
	LoadedMixFileNames.insert(Name);
}

bool CDExt::IsLoadedMix(const char* Name)
{
	return LoadedMixFileNames.contains(Name);
}

BiasedFileEntry* IHExtPtrType::GetEntry()
{
	return (BiasedFileEntry*)((((DWORD)this) + 0xF) & 0xFFFFFFF0);
}
IHFileClass* IHExtPtrType::GetIHExt()
{
	return GetEntry()->GetFile();
}

BiasedFileEntry* GetExtEntry(RawFileClass* pFile)
{
	return pFile->IHExtPtr ? pFile->IHExtPtr->GetEntry() : nullptr;
}

IHFileClass* GetIHExt(RawFileClass* pFile)
{
	return pFile->IHExtPtr ? pFile->IHExtPtr->GetIHExt() : nullptr;
}

int BiasedFileEntry::Seek(int Offset, FileSeekMode Mode)
{
	if (CachedAccess)
	{
		//Debug::LogFormat("[IH] Cached Seek Entry {} Ext {} Position {} Offset {} Mode {}\n", (void*)this, (void*)GetFile(), CachedAccessPosition, Offset, (int)Mode);
		switch (Mode)
		{
		case FileSeekMode::Set:
			return CachedFile::Seek(CachedAccessToken, GetFile(), CachedAccessPosition, Offset);
		case FileSeekMode::Current:
			return CachedFile::Seek(CachedAccessToken, GetFile(), CachedAccessPosition, CachedAccessPosition + Offset);
		case FileSeekMode::End:
			return CachedFile::Seek(CachedAccessToken, GetFile(), CachedAccessPosition, GetFile()->GetFileSize() + Offset);
		default:
			return 0;
		}
	}
	else return GetFile()->Seek(Offset, Mode);
}

int BiasedFileEntry::ReadBytes(void* Buffer, int Size)
{
	if(CachedAccess) return CachedFile::ReadBytes(CachedAccessToken, GetFile(), CachedAccessIterType, CachedAccessPosition, Buffer, Size);
	else return GetFile()->ReadBytes(Buffer, Size);
}

int BiasedFileEntry::Position()
{
	if(CachedAccess) return CachedAccessPosition;
	else return GetFile()->Position();
}

void BiasedFileEntry::CloseCache()
{
	if (CachedAccess)
	{
		CachedFile::ClearCache(CachedAccessToken);
		GetFile()->Seek(CachedAccessPosition, FileSeekMode::Set);
		CachedAccess = false;
	}
}

void IHExtPtrType::Destroy()
{
	auto Entry = GetEntry();
	if (Entry->Biased && Entry->BiasedFileName)
		CRT::free(Entry->BiasedFileName);
	auto Ext = Entry->GetFile();
	Ext->~IHFileClass();
}

void IHExtPtrTypeDeleter::operator()(IHExtPtrType* p)
{
	if (!p)return;
	p->Destroy();
	CRT::_delete(p);
}

void RemoveExt(RawFileClass* This)
{
	if (CachedFile::CanAcceptIHExtPtr(This->IHExtPtr))
	{
		CachedFile::TakeOverIHFile(This->IHExtPtr);
	}
	else
	{
		This->IHExtPtr->Destroy();
		CRT::_delete(This->IHExtPtr);
	}
	This->IHExtPtr = nullptr;
}


void SetName_Impl(CDFileClass* This, const std::string& Str,const char* pFileName)
{
	//Debug::Log("IHCore : Query File Stream %s for File %s\n", Str.c_str(), pFileName);
	auto jt = Local::IHFileStreamer.find(Str);
	if (jt != Local::IHFileStreamer.end())
	{
		This->IHExtPtr = nullptr;
		bool CachedAccess = Local::IHFileCachedAccess.contains(Str);
		if (CachedAccess)
		{
			auto Token = CachedFile::GenerateToken(Str, pFileName);
			if (CachedFile::CanReleaseIHFile(Token))
			{
				This->IHExtPtr = CachedFile::ReleaseIHFile(Token);
				auto Entry = GetExtEntry(This);
				auto Ext = GetIHExt(This);
				This->FileName = CRT::strdup(Ext->GetFileName());
				Entry->Biased = false;
				Entry->Seek(0, FileSeekMode::Set);
				return;
			}
		}


		//Debug::Log("PTR At %08X \n", This);
		auto sz = jt->second.Size + BiasedFileEntry::HeaderSize();
		auto aligned = (sz + 0xF) & 0xFFFFFFF0;
		This->IHExtPtr = (IHExtPtrType*)CRT::_new(aligned);
		memset(This->IHExtPtr, 0, aligned);

		auto Entry = GetExtEntry(This);
		auto Ext = GetIHExt(This);

		Entry->Biased = false;
		Entry->NeedsCachedAccess = false;
		Entry->CachedAccess = false;
		Entry->BiasedLength = 0;
		Entry->BiasedOffset = 0;
		Entry->BiasedFileName = nullptr;
		Entry->CachedAccessClosed = false;

		if (CachedAccess)
		{
			auto& Info = Local::IHFileCachedAccess[Str];
			Entry->CachedAccessThreshold = Info.Threshold;
			Entry->CachedAccessToken = CachedFile::GenerateToken(Str, pFileName);
			Entry->CachedAccessIterType = Info.IterType;
		}
		else
		{
			Entry->CachedAccessThreshold = 0xFFFFFFFF;
			Entry->CachedAccessToken = 0;
			Entry->CachedAccessIterType = FileIterationType::RandomAccess;
		}

		VTABLE_SET(Ext, jt->second.vptr);
		//Debug::Log("IHCore : File Stream %s for File %s at %08X binding to %08X\n", Str.c_str(), pFileName, Ext, This);
		Ext->Initialize();
		Ext->SetFileName(pFileName);
		This->FileName = CRT::strdup(Ext->GetFileName());
	}
}

void ClearPrevName_Impl(CDFileClass* This)
{
	if (This->IHExtPtr)
	{
		auto Ext = GetIHExt(This);
		Ext->~IHFileClass();
		CRT::_delete(This->IHExtPtr);
		This->IHExtPtr = nullptr;
	}
}

#include <CRC.h>

JsonObject GetIHCoreJson();

LPCVOID Internal_GetGlobalVarPtr(const char* Usage, const char* Key);

std::string GetBindingIHFile(const char* pFileName)
{
	auto it = Local::IHFileBinder.find(pFileName);
	if (it != Local::IHFileBinder.end())
		return it->second;
	for (auto& p : Local::IHFileFilter)
		if (p.second && ((bool(__cdecl*)(const char*))p.second)(pFileName))
			return p.first;
	return std::string();
}

bool HasBindingIHFile(const char* pFileName)
{
	return !GetBindingIHFile(pFileName).empty();
}

const char* FileClassExt::CDFileClass_SetFileName(char* pOriginalFileName)
{
	//Debug::Log("[IH] Requesting \"%s\"\n", pOriginalFileName);

	static bool First = true;
	if (First)
	{
		auto cfg = GetIHCoreJson();
		if (cfg)
		{
			auto Obj = cfg.GetObjectItem("ExtraPath_First");
			if (Obj.Available() && Obj.IsNotEmptyArray())
				for (const auto& s : Obj.GetArrayString())
				{
					CDExt_Instance.PushCustomPathToFirst(SyringeData::ExecutableDirectoryPath() + s);
					Debug::Log("IHCore : Adding Path From Config \"%hs%hs\"\n", SyringeData::ExecutableDirectoryPath().c_str(), s.c_str());
				}
			Obj = cfg.GetObjectItem("ExtraPath_Last");
			if (Obj.Available() && Obj.IsNotEmptyArray())
				for (const auto& s : Obj.GetArrayString())
				{
					CDExt_Instance.PushCustomPathToTail(SyringeData::ExecutableDirectoryPath() + s);
					Debug::Log("IHCore : Adding Path From Config \"%hs%hs\"\n", SyringeData::ExecutableDirectoryPath().c_str(), s.c_str());
				}
			Obj = cfg.GetObjectItem("FileRedirect");
			if (Obj.Available() && Obj.IsTypeObject())
				for (const auto& [Original,Target] : Obj.GetMapString())
				{
					CDExt_Instance.AddRedirect(Original.c_str(), Target.c_str());
					Debug::Log("IHCore : Adding Redirection From Config \"%hs\"->\"%hs\"\n", Original.c_str(), Target.c_str());
				}
		}

		Service_RegisterIHFile.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Register File Stream \"%hs\"\n", Param.Name);
		Local::RegisterIHFileStream(Param.Name, { Param.vptr, Param.Size }); });
		Service_BindToStream.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Register File \"%hs\" Binding to \"%hs\"\n", Param.OriginalName, Param.TargetName);
		Local::IHFileBinder.insert({ Param.OriginalName, Param.TargetName }); });
		Service_RegisterIHFileFilter.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Register File Stream \"%hs\"\n", Param.Name);
		Local::IHFileFilter.insert({ Param.Name, Param.Handle }); });
		Service_RedirectFile.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Adding Redirection \"%hs\"->\"%hs\"\n", Param.OriginalName, Param.TargetName);
		CDExt_Instance.AddRedirect(Param.OriginalName, Param.TargetName); });
		Service_CustomPathListHead.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Adding Path \"%hs\"\n", Param.Path); CDExt_Instance.PushCustomPathToHead(Param.Path); });
		Service_CustomPathListTail.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Adding Path \"%hs\"\n", Param.Path); CDExt_Instance.PushCustomPathToTail(Param.Path); });
		Service_CustomPathListFirst.RefreshAndProcess([](const auto& Param)
			{ Debug::Log("IHCore : Adding Path \"%hs\"\n", Param.Path); CDExt_Instance.PushCustomPathToFirst(Param.Path); });
		Service_RegisterIHFileTag.RefreshAndProcess([](const auto& Param)
			{
				auto Value = Internal_GetGlobalVarPtr(Param.TagType, Param.TagVar);
				Debug::Log("IHCore : Register File Stream \"%hs\" Tag \"%hs\" -> \"%hs\" Value 0x%08X(%u)\n", Param.Name, Param.TagType, Param.TagVar, Value, Value);
				if(!strcmp(Param.TagType, "IHFile::CachedAccessThreshold"))
					Local::IHFileCachedAccess[Param.Name].Threshold = (size_t)Value;
				if(!strcmp(Param.TagType, "IHFile::IterationType"))
					Local::IHFileCachedAccess[Param.Name].IterType = (FileIterationType)((int)Value);
			});
		First = false;
	}

	auto This = reinterpret_cast<CDFileClass*>(this);
	const char* pFileName = CDExt_Instance.TryRedirect(pOriginalFileName);
	char v5[MAX_PATH];


	if (!UseOriginalFileClass())
	{
		//if (pOriginalFileName != pFileName)
		//	Debug::Log("[IH] %s->%s", pOriginalFileName, pFileName);
		//Debug::Log("[IH] Requesting \"%s\"\n", pFileName);
		ClearPrevName_Impl(This);
		auto ClassName = GetBindingIHFile(pFileName);
		if(!ClassName.empty())
			SetName_Impl(This, ClassName, pFileName);
		if (This->IHExtPtr)return GetIHExt(This)->GetFileName();
	}
	

	for (auto& p : CDExt_Instance.PathFirst)
	{
		This->BufferIOFileClass::SetFileName((p + pFileName).c_str());
		if (This->BufferIOFileClass::Exists(0))
			return This->GetFileName();
	}

	This->BufferIOFileClass::SetFileName(pFileName);
	if (This->IsDisabled || CDDrives::Instance().Paths.Empty() || This->BufferIOFileClass::Exists(0))
		return This->GetFileName();
	auto PathLink = CDDrives::Instance().Paths.Header;
	if (!CDDrives::Instance().Paths.Empty())
	{
		for (auto& p : CDExt_Instance.PathHead)
		{
			This->BufferIOFileClass::SetFileName((p + pFileName).c_str());
			//不会悬垂，BufferIOFileClass::SetFileName会调用strdup复制一份
			if (This->BufferIOFileClass::Exists(0))
				return This->GetFileName();
		}
		while (1)
		{
			strcpy(v5, ((CDPath *)PathLink)->Path);
			strcat(v5, pFileName);
			This->BufferIOFileClass::SetFileName(v5);
			if (This->BufferIOFileClass::Exists(0))
				break;
			PathLink = PathLink->PreviousEntry;
			if (!PathLink)
				goto LABEL_7;
		}
	}
	else
	{
	LABEL_7:
		for (auto& p : CDExt_Instance.PathTail)
		{
			This->BufferIOFileClass::SetFileName((p + pFileName).c_str());
			if (This->BufferIOFileClass::Exists(0))
				return This->GetFileName();
		}
		This->BufferIOFileClass::SetFileName(pFileName);
	}
	return This->GetFileName();
}

const char* FileClassExt::RawFileClass_GetFileName()
{
	auto This = reinterpret_cast<RawFileClass*>(this);
	if (This->IHExtPtr && !UseOriginalFileClass())return GetIHExt(This)->GetFileName();
	return This->FileName;
}

bool FileClassExt::BufferIOFileClass_Exists(bool WriteShared)
{
	auto This = reinterpret_cast<BufferIOFileClass*>(this);
	auto Name = This->GetFileName();
	std::string_view p = Name;
	if (!p.ends_with(".mix") && !p.ends_with(".MIX") && !WhiteList.empty() && !WhiteList.count(PathFindFileNameA(Name)))
	{
		if (!MixFileClass::Offset(Name, nullptr, nullptr, nullptr, nullptr))
			Debug::Log("IHCore : Requested File \"%s\" is NOT in the WhiteList\n", Name);
		return false;
	}
	if (BlackList.count(PathFindFileNameA(Name)))return false;
	if (This->AsBufferedIOFile)return true;
	else return This->RawFileClass::Exists(false);
}

CCFileClass* FileClassExt::CCFileClass_Constructor_pFileName_InMixFile(const char* Source)
{
	Debug::LogFormat("[IH] Loading MIX file {}\n", Source);
	CDExt_Instance.RegisterLoadedMix(Source);
	return CCFileClass_Constructor_pFileName_Orig(Source);
}

BOOL FileClassExt::CCFileClass_Open(FileAccessMode Mode)
{
	auto This = reinterpret_cast<CCFileClass*>(this);
	//Dont assume its lifecycle since there is a OpenEx below
	std::string Name = This->GetFileName();
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, Name.c_str());

	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Entry = GetExtEntry(This);
		auto Ext = GetIHExt(This);
		if (Entry->CachedAccess && Entry->CachedAccessClosed)
		{
			Entry->CachedAccessClosed = false;
			return true;
		}
		else
		{
			auto Result = Ext->Open(Mode);
			auto Size = Ext->GetFileSize();
			if (Size > (int)Entry->CachedAccessThreshold)
				Entry->NeedsCachedAccess = true;
			return Result;
		}
	}

	int Size = 0;
	void* pBuffer = nullptr;
	MixFileClass* mixfile = nullptr;
	int Offset = 0;

	This->Close();
	if ( (Mode & FileAccessMode::Write) || This->BufferIOFileClass::Exists(false))
		return This->CDFileClass::Open(Mode);
	if (!MixFileClass::Offset(Name.c_str(), &pBuffer, &mixfile, &Offset, &Size))
		return This->CDFileClass::Open(Mode);
	if (pBuffer || !mixfile)
	{
		if (This != (CCFileClass*)((DWORD)-88))
			new (&This->Buffer) MemoryBuffer(pBuffer, Size);
		This->Position = 0;
		return TRUE;
	}
	if (!mixfile->FileName) return FALSE;

	if (This->OpenEx(mixfile->FileName, FileAccessMode::Read))
	{
		if (CDExt_Instance.IsLoadedMix(mixfile->FileName) && GetIHExt(This))
		{
			auto Entry = GetExtEntry(This);
			if (Entry->Biased)
			{
				auto ExternalName = This->GetFileName();
				auto DirectOuterSize = This->GetFileSize();
				//Debug::LogFormat("[IH] Bias On Bias {} In {} Offset {} Size {} DirectOuterSize {}\n",Name, ExternalName, Offset, Size, DirectOuterSize);

				//BiasedFileName修正到最内层，同时This->GetFileName()修正到最外层
				Entry->BiasedFileName = CRT::strdup(Name.c_str());
				auto OriginalOffset = Entry->BiasedOffset;
				if (Size < 0)Size = 0;
				if (Offset >= OriginalOffset + DirectOuterSize)
				{
					Offset = OriginalOffset + DirectOuterSize;
					Size = 0;
				}
				else
				{
					Size = std::min(Size, OriginalOffset + DirectOuterSize - Offset);
				}

				Entry->BiasedLength = Size;
				Entry->BiasedOffset = Offset;
				//Debug::LogFormat("[IH] Biased Final Offset {} Size {}\n", Entry->BiasedOffset, Entry->BiasedLength);
				//Debug::Log("Seek inner 1 : ");
				This->Seek(0, FileSeekMode::Set);
				This->FilePointer = Entry->BiasedOffset;
				//Debug::Log("Open inner Successfully\n");
				return TRUE;
			}
			else 
			{
				auto OriginalSize = This->GetFileSize();
				//Debug::LogFormat("[IH] Biased File {} In {} Offset {} Size {} OrigSize {}\n", Name.c_str(), mixfile->FileName, Offset, Size, OriginalSize);

				Entry->Biased = true;
				Entry->BiasedFileName = CRT::strdup(Name.c_str());
				if (Size < 0)Size = 0;
				if (Offset >= OriginalSize)
				{
					Offset = OriginalSize;
					Size = 0;
				}
				else
				{
					Size = std::min(Size, OriginalSize - Offset);
				}
				Entry->BiasedLength = Size;
				Entry->BiasedOffset = Offset;
				//Debug::LogFormat("[IH] Biased Final Offset {} Size {}\n", Entry->BiasedOffset, Entry->BiasedLength);
				//Debug::Log("Seek 1 : ");
				This->Seek(0, FileSeekMode::Set);
				This->FilePointer = Entry->BiasedOffset;
				//Debug::Log("Open Successfully\n");
				return TRUE;
			}
		}
		else
		{
			auto OriginalSize = This->GetFileSize();
			//Debug::LogFormat("[IH] Biased File {} In {} Offset {} Size {} OrigSize {}\n", Name.c_str(), mixfile->FileName, Offset, Size, OriginalSize);
			char* __FileName = CRT::strdup(This->GetFileName());
			This->IsDisabled = true;
			This->SetFileName(__FileName);
			This->IsDisabled = false;
			auto fp1 = This->FilePointer;
			This->Bias(0, -1);
			auto fp2 = This->FilePointer;
			This->Bias(Offset, Size);
			auto fp3 = This->FilePointer;
			This->Seek(0, FileSeekMode::Set);
			//Debug::LogFormat("[IH] Bias FP1 {} FP2 {} FP3 {}\n", fp1, fp2, fp3);
			free(__FileName);
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}


bool FileClassExt::CCFileClass_Exists(bool WriteShared)
{
	auto This = reinterpret_cast<CCFileClass*>(this);
	//Debug::LogFormat("[IH] CCFileClass_Exists {} \"{}\" Ext {}\n", (void*)This, This->GetFileName(), This->IHExtPtr);
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Result = GetIHExt(This)->Exists(WriteShared);
		//Debug::LogFormat("[IH] Custom CCFileClass_Exists {} -> {}\n", This->GetFileName(), Result);
		return Result;
		//return GetIHExt(This)->Exists(WriteShared);
	}

	if (This->Availablility == 1)
		return true;
	if (This->HasHandle())
	{
		This->Availablility = 1;
		return true;
	}
	else
	{
		auto Name = This->GetFileName();
		if (MixFileClass::Offset(Name, 0, 0, 0, 0))
		{
			This->Availablility = 1;
			return true;
		}
		else if (This->BufferIOFileClass::Exists(false))
		{
			This->Availablility = 1;
			return true;
		}
		else
		{
			This->Availablility = 2;
			return false;
		}
	}
}

void ExtCD_InitBeforeEverything()
{
	Patch::Apply_LJMP(0x47AE10, union_cast<void*>(&FileClassExt::CDFileClass_SetFileName));
	Patch::Apply_LJMP(0x401940, union_cast<void*>(&FileClassExt::RawFileClass_GetFileName));
	Patch::Apply_LJMP(0x431F10, union_cast<void*>(&FileClassExt::BufferIOFileClass_Exists));
	Patch::Apply_LJMP(0x473D10, union_cast<void*>(&FileClassExt::CCFileClass_Open));
	Patch::Apply_LJMP(0x473C50, union_cast<void*>(&FileClassExt::CCFileClass_Exists));
	Patch::Apply_CALL(0x5B3C8B, union_cast<void*>(&FileClassExt::CCFileClass_Constructor_pFileName_InMixFile));
};

int CCFileClass_ReadBytes_Ext(RawFileClass* This, LPVOID Buffer, size_t Size)
{
	auto Entry = GetExtEntry(This);
	auto Ext = GetIHExt(This);

	//Enable CachedAccess on first seek if the file is large enough
	if (Entry->NeedsCachedAccess && Entry->CachedAccessToken != 0 && !Entry->CachedAccess)
	{
		Entry->CachedAccess = true;
		Entry->CachedAccessPosition = Ext->Position();
		CFM_Log("[IH] Enabling CachedAccess on {} Position {} Threshold {}\n", This->GetFileName(), Entry->CachedAccessPosition, Entry->CachedAccessThreshold);
	}

	int BytesRead;
	//Debug::LogFormat("[IH] Reading {} Entry {} Ext {} Size {}\n", (void*)This, (void*)Entry, (void*)Ext, Size);
	if (Entry->Biased)
	{
		auto ActualPosition = Entry->Position();
		if (ActualPosition < Entry->BiasedOffset)
		{
			ActualPosition = Entry->Seek(Entry->BiasedOffset, FileSeekMode::Set);
		}
		auto BiasedEnd = Entry->BiasedOffset + Entry->BiasedLength;
		if (ActualPosition >= BiasedEnd)
			BytesRead = 0;
		else
		{
			auto RestSize = BiasedEnd - ActualPosition;
			if (RestSize < (int)Size) Size = RestSize;
			BytesRead = Entry->ReadBytes(Buffer, Size);
		}
	}
	else BytesRead = Entry->ReadBytes(Buffer, Size);
	if (Entry->CachedAccess)
	{
		//Debug::LogFormat("[IH] Cached Read Position {} BytesRead {}\n", Entry->CachedAccessPosition, BytesRead);
	}

	return BytesRead;
}

DEFINE_HOOK(0x65CCE0, RawFileClass_ReadBytes, 5)
{
	GET(RawFileClass*, This, ECX);
	GET_STACK(LPVOID, Buffer, 0x4);
	GET_STACK(size_t, Size, 0x8);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		int BytesRead = CCFileClass_ReadBytes_Ext(This, Buffer, Size);
		R->EAX(BytesRead);
		return 0x65CD0C;
	}
	return 0;
}
DEFINE_HOOK(0x473CD0, CCFileClass_HasHandle, 5)
{
	GET(RawFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(GetIHExt(This)->HasHandle());
		return 0x473CD9;
	}
	return 0;
}
DEFINE_HOOK(0x473B10, CCFileClass_ReadBytes, 5)
{
	GET(RawFileClass*, This, ECX);
	GET_STACK(LPVOID, Buffer, 0x4);
	GET_STACK(size_t, Size, 0x8);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		int BytesRead = CCFileClass_ReadBytes_Ext(This, Buffer, Size);
		R->EAX(BytesRead);
		return 0x473B9B;
	}
	return 0;
}
DEFINE_HOOK(0x473BA0, CCFileClass_Seek, 5)
{
	GET(RawFileClass*, This, ECX);
	GET_STACK(int, Offset, 0x4);
	GET_STACK(FileSeekMode , Mode, 0x8);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	//Debug::LogFormat("[IH] Seeking {} Offset {} Mode {}\n", (void*)This, Offset, (int)Mode);
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Entry = GetExtEntry(This);
		auto Ext = GetIHExt(This);

		//Enable CachedAccess on first seek if the file is large enough
		if (Entry->NeedsCachedAccess && Entry->CachedAccessToken != 0 && !Entry->CachedAccess)
		{
			Entry->CachedAccess = true;
			Entry->CachedAccessPosition = Ext->Position();
			CFM_Log("[IH] Enabling CachedAccess on {} Position {} Threshold {}\n", This->GetFileName(), Entry->CachedAccessPosition, Entry->CachedAccessThreshold);
		}


		auto BiasedEnd = Entry->BiasedOffset + Entry->BiasedLength;
		int Position;
		if (Entry->Biased)
		{
			bool UnknownMode = false;
			switch (Mode)
			{
			case FileSeekMode::Set:
				Position = Entry->Seek(Entry->BiasedOffset + Offset, FileSeekMode::Set);
				//Debug::LogFormat("[IH] Actual Offset {} Mode {} Position {}\n", Entry->BiasedOffset + Offset, (int)FileSeekMode::Set, Position);
				break;
			case FileSeekMode::Current:
				Position = Entry->Seek(Offset, FileSeekMode::Current);
				//Debug::LogFormat("[IH] Actual Offset {} Mode {} Position {}\n", Offset, (int)FileSeekMode::Current, Position);
				break;
			case FileSeekMode::End:
				Position = Entry->Seek(BiasedEnd + Offset, FileSeekMode::Set);
				//Debug::LogFormat("[IH] Actual Offset {} Mode {} Position {}\n", BiasedEnd + Offset, (int)FileSeekMode::Set, Position);
				break;
			default:
				UnknownMode = true;
				break;
			}
			if (UnknownMode)
			{
				//return 0 on error
				//like what gamemd does
				This->CDCheck(ERROR_INVALID_BLOCK, 0, This->GetFileName());
				Position = 0;
			}
			else
			{
				if (Position < Entry->BiasedOffset)
					Position = Entry->Seek(Entry->BiasedOffset, FileSeekMode::Set);
				if (Position > BiasedEnd)
					Position = Entry->Seek(BiasedEnd, FileSeekMode::Set);
				Position -= Entry->BiasedOffset;
			}
		}
		else Position = Entry->Seek(Offset, Mode);
		//Debug::LogFormat("[IH] Final Position {}\n", Position);
		R->EAX(Position);

		return 0x473BF8;
	}
	return 0;
}
DEFINE_HOOK(0x473C00, CCFileClass_GetFileSize, 7)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Entry = GetExtEntry(This);
		auto Ext = GetIHExt(This);
		int Size;
		if (Entry->Biased)
			Size = Entry->BiasedLength;
		else
			Size = Ext->GetFileSize();
		R->EAX(Size);
		return 0x473C10;
	}
	return 0;
}
DEFINE_HOOK(0x473AE0, CCFileClass_WriteBytes, 6)
{
	GET(RawFileClass*, This, ECX);
	GET_STACK(LPVOID, Buffer, 0x4);
	GET_STACK(size_t, Size, 0x8);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Entry = GetExtEntry(This);
		auto Ext = GetIHExt(This);
		if (Entry->CachedAccess)Entry->CloseCache();
		R->EAX(Ext->WriteBytes(Buffer, Size));
		return 0x473B0D;
	}
	return 0;
}




DEFINE_HOOK(0x65D150, RawFileClass_CreateFile, 5)
{
	GET(RawFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(GetIHExt(This)->CreateFile());
		return 0x65D187;
	}
	return 0;
}
DEFINE_HOOK(0x65D190, RawFileClass_DeleteFile, 5)
{
	GET(RawFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(GetIHExt(This)->DeleteFile());
		return 0x65D1EE;
	}
	return 0;
}
DEFINE_HOOK(0x473CE0, CCFileClass_Close, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		auto Entry = GetExtEntry(This);
		if (Entry->CachedAccess)
		{
			Entry->CachedAccessClosed = true;
		}
		else
		{
			GetIHExt(This)->Close();
		}
		return 0x473D02;
	}
	return 0;
}
DEFINE_HOOK(0x4019A0, CCFileClass_Destructor_III, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		RemoveExt(This);
	}
	return 0;
}
DEFINE_HOOK(0x535A60, CCFileClass_Destructor_II, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		RemoveExt(This);
	}
	return 0;
}
DEFINE_HOOK(0x535A70, CCFileClass_Destructor, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		RemoveExt(This);
	}
	return 0;
}
DEFINE_HOOK(0x431B80, BufferIOFileClass_Destructor_Static, 6)
{
	GET(BufferIOFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		RemoveExt(This);
	}
	return 0;
}
