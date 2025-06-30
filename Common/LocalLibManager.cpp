
#include "LocalData.h"
#include "ToolFunc.h"
#include <YRPP.h>
#include <locale>
#include <IH.Loader.h>
#include "..\IHCore\Debug.h"
#include "..\IHCore\Patch.h"
#include <SyringeEx.h>

namespace HPLocalData
{
	const HPDLLOutput* __cdecl LoadIFunction(const HPDLLInput*);
}

IHException::IHException(const char* info) : Info(info) {};
IHException::IHException(std::string&& info) : Info(std::move(info)) {};
const char* IHException::what() const noexcept
{
	return Info.c_str();
}
void IHLogAndThrow(const char* str)
{
	Debug::Log(str);
	Debug::Log("\n");
	throw IHException(str);
}


void IHHeapAllocInit();
char* __fastcall StrDup_Ex(const char* Str);

namespace Local
{
	PointerArray<FuncHandle> InitFn;
	std::vector<LibType> Libs;
	std::vector<BasicLibData> BasicLibs;
	std::unordered_map<std::string, LibType*> LibMap;
	std::unordered_map<std::string, VClass> IHFileStreamer;
	std::unordered_map<std::string, std::string, UpperHash, UpperEqualPred> IHFileBinder;
	std::unordered_map<std::string, FuncHandle, UpperHash, UpperEqualPred> IHFileFilter;
	std::unordered_map<std::string, std::vector<FuncInfo*>> NamedFunc;
	std::unordered_map<std::string, ContextFunc_t> ContextMap;
	std::unordered_map<std::string, BinderType> BinderMap;
	std::unordered_multimap<std::string, BindingGenerator_t> GeneratorMap;
	std::unordered_map<std::string, DirectBinder_t> DirectBinderMap;
	std::unordered_map<std::string, std::unique_ptr<ActiveRoutine>> RoutineMap;
	char IHExcBuf[10000];
	std::vector<LibType*> RingStack;
	int RingCheckIdx = 0;
	int ECInitStage{ 0 };
	std::vector<std::string> InitOrder;


	std::pair<bool, std::unordered_map<std::string, LibType*>::iterator> IsDependencyValid(const InitDependency& dep)
	{
		if (!dep.LibName)return { false,LibMap.end() };
		auto it = LibMap.find(dep.LibName);
		if (it == LibMap.end())return { false,it };
		if (!it->second || !it->second->Available)return { false,LibMap.end() };
		if (dep.LowestRequiredVersion != DoNotCheckVersion && dep.LowestRequiredVersion > it->second->Out->Info->Version)return { false,it };
		if (dep.Version != DoNotCheckVersion && dep.Version < it->second->Out->Info->LowestSupportedVersion)return { false,it };
		return { true,it };
	}

	void AddDependency(LibType& lib,const InitDependency& d)
	{
		auto [ok, it] = IsDependencyValid(d);
		if (!ok)
		{
			std::string DV = d.Version == DoNotCheckVersion ? "(No Requirements)" : std::to_string(d.Version);
			std::string DL = d.LowestRequiredVersion == DoNotCheckVersion ? "(No Requirements)" : std::to_string(d.LowestRequiredVersion);

			if (it == LibMap.end())
				sprintf_s(IHExcBuf, "IHCore: %hs needs dependency %hs \n(Required Version£º%hs or other compatible versions higher than %hs)"
					, lib.Out->Info->LibName, d.LibName, DV.c_str(), DL.c_str());
			else if (d.LowestRequiredVersion != DoNotCheckVersion && d.LowestRequiredVersion > it->second->Out->Info->Version)
				sprintf_s(IHExcBuf, "IHCore: Dependency of %hs : %hs version too low£¬\n(Current Version: %d Lowest Compatible Version: %d,Required Version£º%hs or other compatible versions higher than %hs)"
					, lib.Out->Info->LibName, d.LibName, it->second->Out->Info->Version, 
					it->second->Out->Info->LowestSupportedVersion, DV.c_str(), DL.c_str());
			else if (d.Version != DoNotCheckVersion && d.Version < it->second->Out->Info->LowestSupportedVersion)
				sprintf_s(IHExcBuf, "IHCore: Dependency of %hs : %hs version too high£¬\n(Current Version: %d Lowest Compatible Version: %d,Required Version£º%hs or other compatible versions higher than %hs)"
					, lib.Out->Info->LibName, d.LibName, it->second->Out->Info->Version, 
					it->second->Out->Info->LowestSupportedVersion, DV.c_str(), DL.c_str());
			IHLogAndThrow(IHExcBuf);
		}
		else
		{
			Debug::Log("IHCore : Add Dependency: %hs Requires %hs (%hs)\n", 
				lib.Out->Info->LibName, d.LibName, d.RequiredLoadSequence ? "Forced Sequence" : "Optional Sequence");
			if(d.RequiredLoadSequence)lib.Dep.ValidDep.push_back(d.LibName);
		}
	}

	void RingCheck(LibType& lib)
	{
		//Debug::Log("IHCore : Ring Check to %s ,", lib.Available ? lib.Out->Info->LibName : "<UNKNOWN>");
		//Debug::Log("Current DFN=%d LOW=%d\n",lib.Dep.DFN,lib.Dep.LOW);
		RingStack.push_back(&lib);
		lib.Dep.PD = true;
		lib.Dep.DFN = lib.Dep.LOW = RingCheckIdx;
		++RingCheckIdx;
		for (auto& s : lib.Dep.ValidDep)
		{
			auto& v = LibMap[s];
			if (!v)continue;
			if (!v->Available)continue;
			if (!v->Dep.DFN)
			{
				//Debug::Log("IHCore : Ring Check from %s\n", lib.Available ? lib.Out->Info->LibName : "<UNKNOWN>");
				RingCheck(*v);
				lib.Dep.LOW = std::min(lib.Dep.LOW, v->Dep.LOW);
			}
			else if (v->Dep.PD)
			{
				lib.Dep.LOW = std::min(lib.Dep.LOW, v->Dep.DFN);
			}
		}
		//Debug::Log("IHCore : %s After DFN=%d LOW=%d\n", lib.Available ? lib.Out->Info->LibName : "<UNKNOWN>"
		//	, lib.Dep.DFN, lib.Dep.LOW);
		if (lib.Dep.DFN == lib.Dep.LOW)
		{
			std::string sv;
			if (RingStack.empty() || RingStack.back() == &lib)
			{
				if(!RingStack.empty())RingStack.pop_back();
				return;
			}
			while (!RingStack.empty() && RingStack.back() != &lib)
			{
				auto& bk = RingStack.back();
				//if(!bk)
				if (!bk->Available)
				{
					sv += "(Invalid Library)->\n";
				}
				else
				{
					sv += bk->Out->Info->LibName;
					sv += " Version£º" + std::to_string(bk->Out->Info->Version);
					sv += "->\n";
				}
				//bk->PD = false;
				RingStack.pop_back();
			}
			sv += lib.Out->Info->LibName;
			sv += " Version£º" + std::to_string(lib.Out->Info->Version);
			sv += "\n";
			sprintf_s(IHExcBuf, "IHCore: Ring dependency formed. One of the ring is\n%s", sv.c_str());
			throw IHException(IHExcBuf);
		}
	}

	void MarkOrder(LibType& lib, std::vector<std::string>& ans)
	{
		for (auto s : lib.Dep.ValidDep) {
			auto& v = LibMap[s];
			if (!v)continue;
			if (v->Dep.VIS) continue;
			if (!v->Available)continue;
			MarkOrder(*v, ans);
		}
		lib.Dep.VIS = true;
		ans.push_back(lib.Out->Info->LibName);
	}

	std::vector<std::string> GetInitOrder()//chk available todo
	{
		for (auto& p : LibMap)
		{
			if (!p.second)continue;
			if (!p.second->Available)continue;
			if (!p.second->Out->Dependencies.N)continue;
			if (!p.second->Out->Dependencies.Data)continue;
			auto& d = p.second->Out->Dependencies;
			for (size_t i = 0; i < d.N; i++)
			{
				AddDependency(*p.second, d.Data[i]);
			}
		}
		ECInitStage = 20;
		for (auto& p : LibMap)
		{
			if (!p.second)continue;
			if (!p.second->Available)continue;
			if (!p.second->Dep.DFN)
				RingCheck(*p.second);
		}
		std::vector<std::string> Result;
		for (auto& p : LibMap)
		{
			if (!p.second)continue;
			if (!p.second->Available)continue;
			if (!p.second->Dep.VIS)
				MarkOrder(*p.second, Result);
		}
		return Result;
	}

	
	void ExitClear();

	void InitLibs()
	{
		Debug::Log("[EC] Initialize Libs\n");

		if (!IHLibList::Initialize())return;
		//IHLibList::RegisterToLibList(HPLocalData::LoadIFunction);
		InitFn = IHLibList::GetEntries();
		Libs.resize(InitFn.N);
		BasicLibs.resize(InitFn.N);
		for (size_t i = 0; i < InitFn.N; i++)
		{
			auto fn = (InitFunc_t)InitFn.Data[i];
			Libs[i].Tbl = IHCoreFnTable;
			Libs[i].In.FunctionTable = &Libs[i].Tbl;
			Libs[i].In.ECInitializeStage = &ECInitStage;
			Libs[i].Out = fn(&Libs[i].In);
			Libs[i].Available = true;
			if (!Libs[i].Out)Libs[i].Available = false;
			else {
				if (!Libs[i].Out->GetFunc)Libs[i].Available = false;
				if (!Libs[i].Out->Info)Libs[i].Available = false;
			}
			LibMap[Libs[i].Out->Info->LibName] = &Libs[i];
			if (Libs[i].Available)
			{
				Debug::Log("[EC] Loading Library \"%s\" Successfully\n", Libs[i].Out->Info->LibName);
			}
			else
			{
				Debug::Log("[EC] Failed to Load Library");
			}

			Libs[i].Basic = &BasicLibs[i];
			BasicLibs[i].Available = Libs[i].Available;
			BasicLibs[i].In = &Libs[i].In;
			BasicLibs[i].Out = Libs[i].Out;
			BasicLibs[i].ReservedA = (void*)&Libs[i];
			BasicLibs[i].Reserved[0] = i;
			if(Libs[i].Available)
				BasicLibs[i].ReservedB = (void*)Libs[i].Out->Info->LibName;
		}

		ECInitStage = 10;
		try
		{
			InitOrder = GetInitOrder();
			ECInitStage = 50;
			for (auto& s : InitOrder)
			{
				auto it = LibMap.find(s);
				if (it == LibMap.end())continue;
				if (!it->second->Available)continue;
				Debug::Log("[EC]  Call in the dependency order :\"%s\"\n", it->second->Out->Info->LibName);
				if (it->second->Out->OrderedInit)it->second->Out->OrderedInit();
			}
			ECInitStage = 100;
		}
		catch (IHException& e)
		{
			MessageBoxA(NULL, e.what(), "[EC] ERROR", MB_OK);
		}
	}

	BasicLibData* GetLib(const char* Name)
	{
		auto it = LibMap.find(Name);
		if (it == LibMap.end())return nullptr;
		else return it->second->Basic;
	}
	BasicLibData* GetAvailableLib(const char* Name)
	{
		auto it = LibMap.find(Name);
		if (it == LibMap.end())return nullptr;
		else if (!it->second->Available)return nullptr;
		return it->second->Basic;
	}
	FuncInfo* QueryFunction(BasicLibData* Lib, const char* Name, int Version)
	{
		if (!Lib)return nullptr;
		if (!Lib->Available)return nullptr;
		auto pInfo = Lib->Out->GetFunc(Name, (Version == DoNotCheckVersion ? Lib->Out->Info->Version : Version));
		return pInfo;
	}

	void __cdecl ExitClearImpl()
	{
		ExitClear();
	}



	void LocalInit()
	{
		atexit(ExitClearImpl);
		InitLibs();
	}


	void InitEx()
	{
		//Patch::Apply_RAW(0x474200, { 0x8B ,0xF1 ,0x8D ,0x54 ,0x24 ,0x0C });
		/*
	.text:00474200 0B4                 mov     esi, ecx
	.text:00474202 0B4                 lea     edx, [esp+0B4h+File2]


	.text:00474314 0A8                 add     esp, 0A8h
		*/
		//Patch::Apply_RAW(0x474314, { 0x81 ,0xC4 ,0xA8 ,0x00 ,0x00 ,0x00 });
		//Ext::GeneralCall(*Ext::GetFuncFromLib("IHCore", "ShowIEDialog", DoNotCheckVersion));
	}
	
}

DEFINE_HOOK(0x6BC0CD, ECInitialize, 5)
{
	Local::LocalInit();
	return 0;
}
