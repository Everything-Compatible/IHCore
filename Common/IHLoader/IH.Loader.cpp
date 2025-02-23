#include "IH.Loader.h"
#include "IH.Config.h"
#include "IH.Initial.h"
#include "IH.InitialService.h"
#include "EC.Misc.h"
#include "SyringeEx.h"
#include <Windows.h>
#include <string_view>




#ifndef IHCore
void MyInit(InitResult& Result);
#endif

namespace SyringeData
{
	void InitRemoteData();
}

LibFuncHandle __cdecl ListenerAccess(const char* Name);
void RegisterLoadSaveInterface();
struct ECRTTIInfo;

namespace Init
{
	bool InitAvailable{ false };
	bool VeryFirst{ true };
	HMODULE LibListDLL;
	InitInput* LibInput;
	InitResult Result;
	bool Internal_EnableSaveLoad{ true };

	bool(__cdecl* _RegisterEntry)(LibFuncHandle);
	const LibFuncHandle* (__cdecl* _GetEntries)(void);
	void(__cdecl* _ServiceRequest)(IHInitialLoadService);
	void(__cdecl* _QueryServiceRequest)(const char*, PArray<IHInitialLoadService>&);
	ConfData* (__cdecl* _GetConfData)(void);
	unsigned int(__cdecl* _RandomUINT)(void);
	int (__cdecl* _GenerateID)(void);
	LPCVOID(__cdecl* _GetNamedPointer)(const char* Usage, const char* Name);
	void  (__cdecl* _SetNamedPointer)(const char* Usage, const char* Name, LPCVOID Pointer);
	const char* (__cdecl* _NamedPointer_GetName)(const char* Usage, LPCVOID Pointer);

	bool IsSyringeReadingHooks()
	{
		char Buf[1000];
		return GetEnvironmentVariableA("HERE_IS_SYRINGE", Buf, 1000) ? true : false;
	}

	bool LoaderLoaded{ false };
	struct IHLibListLoader
	{
		void Load()
		{
			if (LibListDLL || LoaderLoaded || IsSyringeReadingHooks())return;
			LibListDLL = LoadLibraryW(L"Patches\\IHLibList.dll");
			if (LibListDLL == NULL)LibListDLL = LoadLibraryW(L"IHLibList.dll");
			if (LibListDLL == NULL)return;

#define ____GetFunc(x) {_ ## x=(decltype(_ ## x))GetProcAddress(LibListDLL, #x);\
			if(_ ## x==nullptr)return;}
			____GetFunc(RegisterEntry);
			____GetFunc(GetEntries);
			____GetFunc(ServiceRequest);
			____GetFunc(QueryServiceRequest);
			____GetFunc(GetConfData);
			____GetFunc(RandomUINT);
			____GetFunc(GenerateID);
			____GetFunc(GetNamedPointer);
			____GetFunc(SetNamedPointer);
			____GetFunc(NamedPointer_GetName);
#undef ____GetFunc
			LoaderLoaded = true;
		}
		IHLibListLoader() { Load(); }
		void Rely() { Load(); };
}Loader;

#ifndef IHCore
	InitResult* __cdecl InitFn(InitInput* Input)
	{
		LibInput = Input;
		MyInit(Result);
#ifndef SIWIC
		if(Init::Internal_EnableSaveLoad)RegisterLoadSaveInterface();
#endif
		return &Result;
	}

	void RegisterListenerImpl()
	{
		InitialLoad::CreateRequestAndSubmit<InitialLoadParam_RegisterFunction>("EC::Internal::ListenerAccess", _strdup(RandStr(12).c_str()), ListenerAccess);
	}

	bool Initialize()
	{
		if (IsSyringeReadingHooks())return false;
		
		SyringeData::InitRemoteData();
		
		if (InitAvailable)return true;

		InitAvailable = true;
		RegisterEntry(InitFn);
		return true;
	}
#else
	void RegisterListenerImpl()
	{
		InitialLoad::CreateRequestAndSubmit<InitialLoadParam_RegisterFunction>("EC::Internal::ListenerAccess", "EC::Internal::IHCore", ListenerAccess);
		RegisterLoadSaveInterface();
	}

	bool Initialize()
	{
		if (IsSyringeReadingHooks())return false;

		SyringeData::InitRemoteData();

		if (InitAvailable)return true;

		InitAvailable = true;
		RegisterListenerImpl();
		return true;
	}
#endif
	bool __cdecl RegisterEntry(LibFuncHandle Entry)
	{
		if (InitAvailable)return _RegisterEntry(Entry);
		else return false;
	}

	ConfData* GetConfData()
	{
		if (_GetConfData)return _GetConfData();
		else return nullptr;
	}

	PArray<IHInitialLoadService> __cdecl QueryServiceRequest(const char* Name)
	{
		if (InitAvailable)
		{
			PArray<IHInitialLoadService> pa;
			_QueryServiceRequest(Name, pa);
			return pa;
		}
		else return PArray<IHInitialLoadService>();
	}
}

ConfData& GetConfigData()
{
	auto ptr = Init::GetConfData();
	if (!ptr)MessageBoxA(Game::hWnd, "Init::GetConfData() == nullptr", "EC SDK", MB_OK);
	return *ptr;
}
void UseOriginalFileClass(bool Option)
{
	GetConfigData().UseOriginalCCFile = Option;
}
bool UseOriginalFileClass()
{
	return GetConfigData().UseOriginalCCFile;
}
void DisableLoadSave()
{
	Init::Internal_EnableSaveLoad = false;
}

const char* Internal_GetLibRegName()
{
#ifdef IHCore
	return "IHCore";
#else
	return Init::Result.Info->LibName;
#endif
}

int Internal_GetLibVersion()
{
#ifdef IHCore
#include "..\IHCore\Version.h"
	return PRODUCT_VERSION;
#else
	return Init::Result.Info->Version;
#endif
}

void Internal_DebugLog(const char* pFormat, ...)
{
	JMP_STD(0x4068E0);
}

int Internal_GenerateID()
{
	return Init::_GenerateID();
}


void Internal_RegisterExportRTTI(const char* ClassName, const ECRTTIInfo* pInfo)
{
	Init::Loader.Rely();
	//if (!Init::_SetNamedPointer)MessageBoxA(Game::hWnd, "Init::_SetNamedPointer == nullptr", "EC SDK", MB_OK);

	Init::_SetNamedPointer("EC::RTTI", ClassName, pInfo);
}
const ECRTTIInfo* Internal_GetRTTIInfo(const char* ClassName)
{
	Init::Loader.Rely();
	//if (!Init::_GetNamedPointer)MessageBoxA(Game::hWnd, "Init::_GetNamedPointer == nullptr", "EC SDK", MB_OK);

	return reinterpret_cast<const ECRTTIInfo*>(Init::_GetNamedPointer("EC::RTTI", ClassName));
}
const char* Internal_GetRTTIClassName(const ECRTTIInfo* pInfo)
{
	Init::Loader.Rely();
	//if (!Init::_NamedPointer_GetName)MessageBoxA(Game::hWnd, "Init::_NamedPointer_GetName == nullptr", "EC SDK", MB_OK);
	return Init::_NamedPointer_GetName("EC::RTTI", pInfo);
}

namespace InitialLoad
{
	void ServiceRequest(IHInitialLoadService IService)
	{
		if (Init::InitAvailable)Init::_ServiceRequest(IService);
	}
	unsigned int RandomUINT()
	{
		if (Init::_RandomUINT)return Init::_RandomUINT();
		else return (unsigned)rand();//regardless of quality when IHLibList version is low
	}
}



bool IHAvailable()
{
	return Init::InitAvailable;
}

