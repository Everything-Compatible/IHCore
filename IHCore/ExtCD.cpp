#include "ExtCD.h"
#include "Debug.h"
#include "Patch.h"
#include "ConfData.h"
#include "ToolFunc.h"
#include "InitialLoad.h"
#include "..\Common\LocalData.h"
#include <Helpers/Macro.h>

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

void SetName_Impl(CDFileClass* This, const std::string& Str,const char* pFileName)
{
	//Debug::Log("IHCore : Query File Stream %s for File %s\n", Str.c_str(), pFileName);
	auto jt = Local::IHFileStreamer.find(Str);
	if (jt != Local::IHFileStreamer.end())
	{
		//Debug::Log("PTR At %08X \n", This);
		This->IHExtPtr = CRT::_new(jt->second.Size);
		memset(This->IHExtPtr, 0, jt->second.Size);
		VTABLE_SET((This->IHExtPtr), jt->second.vptr);
		((IHFileClass*)This->IHExtPtr)->Initialize();
		((FileClass*)This->IHExtPtr)->SetFileName(pFileName);
	}
}

void ClearPrevName_Impl(CDFileClass* This)
{
	if (This->IHExtPtr)
	{
		((FileClass*)This->IHExtPtr)->~FileClass();
		CRT::_delete(This->IHExtPtr);
		This->IHExtPtr = nullptr;
	}
}

JsonObject GetIHCoreJson();

const char* FileClassExt::CDFileClass_SetFileName(char* pOriginalFileName)
{
	static bool First = true;
	if (First)
	{
		auto cfg = GetIHCoreJson();
		if (cfg.Available())
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
		First = false;
	}
	//Debug::Log("[IH] Requesting \"%s\"\n", pOriginalFileName);


	
	auto This = reinterpret_cast<CDFileClass*>(this);
	const char* pFileName = CDExt_Instance.TryRedirect(pOriginalFileName);
	char v5[MAX_PATH];

	if (!UseOriginalFileClass())
	{
		//if (pOriginalFileName != pFileName)
		//	Debug::Log("[IH] %s->%s", pOriginalFileName, pFileName);
		//Debug::Log("[IH] Requesting \"%s\"\n", pFileName);
		ClearPrevName_Impl(This);
		auto it = Local::IHFileBinder.find(pFileName);
		if (it != Local::IHFileBinder.end())
			SetName_Impl(This, it->second, pFileName);
		for (auto& p : Local::IHFileFilter)
			if (p.second && ((bool(__cdecl*)(const char*))p.second)(pFileName))
				SetName_Impl(This, p.first, pFileName);
		if (This->IHExtPtr)return ((FileClass*)This->IHExtPtr)->GetFileName();
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
			//����������BufferIOFileClass::SetFileName�����strdup����һ��
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
	if (This->IHExtPtr && !UseOriginalFileClass())return ((FileClass*)This->IHExtPtr)->GetFileName();
	return This->FileName;
}

void ExtCD_InitBeforeEverything()
{
	Patch::Apply_LJMP(0x47AE10, union_cast<void*>(&FileClassExt::CDFileClass_SetFileName));
	Patch::Apply_LJMP(0x401940, union_cast<void*>(&FileClassExt::RawFileClass_GetFileName));
};


DEFINE_HOOK(0x473C50, CCFileClass_Exists, 9)
{
	GET(RawFileClass*, This, ECX);
	GET_STACK(BOOL, WriteShared, 0x4);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(((FileClass*)This->IHExtPtr)->Exists((bool)WriteShared));
		return 0x473CC0;
	}
	return 0;
}
DEFINE_HOOK(0x473CD0, CCFileClass_HasHandle, 5)
{
	GET(RawFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(((FileClass*)This->IHExtPtr)->HasHandle());
		return 0x473CD9;
	}
	return 0;
}
DEFINE_HOOK(0x473D10, CCFileClass_Open, 5)
{
	GET(CCFileClass*, This, ECX);
	GET_STACK(FileAccessMode, Access, 0x4);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(((FileClass*)This->IHExtPtr)->Open(Access));
		return 0x473DF3;
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
		R->EAX(((FileClass*)This->IHExtPtr)->ReadBytes(Buffer,Size));
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
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		R->EAX(((FileClass*)This->IHExtPtr)->Seek(Offset, Mode));
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
		R->EAX(((FileClass*)This->IHExtPtr)->GetFileSize());
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
		R->EAX(((FileClass*)This->IHExtPtr)->WriteBytes(Buffer, Size));
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
		R->EAX(((FileClass*)This->IHExtPtr)->CreateFile());
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
		R->EAX(((FileClass*)This->IHExtPtr)->DeleteFile());
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
		((FileClass*)This->IHExtPtr)->Close();
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
		((FileClass*)This->IHExtPtr)->~FileClass();
		CRT::_delete(This->IHExtPtr);
		This->IHExtPtr = nullptr;
	}
	return 0;
}
DEFINE_HOOK(0x535A60, CCFileClass_Destructor_II, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		((FileClass*)This->IHExtPtr)->~FileClass();
		CRT::_delete(This->IHExtPtr);
		This->IHExtPtr = nullptr;
	}
	return 0;
}
DEFINE_HOOK(0x535A70, CCFileClass_Destructor, 6)
{
	GET(CCFileClass*, This, ECX);
	//Debug::Log(__FUNCTION__"  %08X \"%s\"\n", This, This->GetFileName());
	if (This->IHExtPtr && !UseOriginalFileClass())
	{
		((FileClass*)This->IHExtPtr)->~FileClass();
		CRT::_delete(This->IHExtPtr);
		This->IHExtPtr = nullptr;
	}
	return 0;
}
