
#include "LocalData.h"
#include <Windows.h>
#include <map>
#include "..\IHCore\Debug.h"
#include "..\IHCore\SomeData.h"
#include "..\IHCore\ECDbgConsole.h"
#include "..\IHCore\ECInterprocess.h"
#include "IHCore/Version.h"
#include <YRPP.h>

extern struct CSFFile_ITable CSFFile_InstTable;

const char8_t* GetTextDrawVariable(const std::u8string_view Key);

namespace IHLibList
{
	int(__cdecl* _GetNLibs)(void);
	int(__cdecl* _GetMaxLibs)(void);
	bool(__cdecl* _RegisterEntry)(LibFuncHandle);
	const LibFuncHandle* (__cdecl* _GetEntries)(void);
	bool Available{ false };
	HMODULE LibListDLL;

	bool IsAvailable()
	{
		return Available;
	}
	bool CheckAvailable()
	{
		if (!Available)
		{
			Initialize();
			if (!Available)return false;
		}
		return true;
	}
	bool Initialize()
	{
		if (Available)return true;
		LibListDLL = LoadLibraryW(L"Patches\\IHLibList.dll");
		if (LibListDLL == NULL)LibListDLL = LoadLibraryW(L"IHLibList.dll");
		if (LibListDLL == NULL)
		{
			Debug::Log("IHCore : Cannot initialize LibList\n");
			return false;
		}

#define ____GetFunc(x) {_ ## x=(decltype(_ ## x))GetProcAddress(LibListDLL, #x);\
			if(_ ## x==nullptr){Debug::Log("IHCore : GetProcAddress Error : Cannot Fetch \"" #x "\"\n");return false;}}
		____GetFunc(GetNLibs);
		____GetFunc(GetMaxLibs);
		____GetFunc(RegisterEntry);
		____GetFunc(GetEntries);
#undef ____GetFunc

		Available = true;
		return true;
	}
	bool RegisterToLibList(LibFuncHandle Init)
	{
		if (!CheckAvailable())return false;
		else return _RegisterEntry(Init);
	}
	int GetMaxLibs()
	{
		if (!CheckAvailable())return -1;
		else return _GetMaxLibs();
	}
	const PArray<LibFuncHandle> GetEntries()
	{
		if (!CheckAvailable())return GetNullPArray<LibFuncHandle>();
		else return PArray<LibFuncHandle>{ (size_t)_GetNLibs(), (_GetEntries()) };
	}
};

//template<>
bool operator<(const GeneratorParam& lhs, const GeneratorParam& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(GeneratorParam)) < 0;
}

namespace Local
{
	const std::string Delim{ "_^$|!@:_" };

	/*
			RoutineSet Set;
		bool Paused;
		int NextRunTime;
		int PauseTime;
		bool ResetParamPerGame;
		RoutineParam Original;
		bool First;
		RoutineParam _Param;
	*/
	void ActiveRoutine::Init(Routine_t Fn, const RoutineParam& Param)
	{
		memset(&Set, 0, sizeof(Set));
		Set.Loop = Fn;
		Paused = false;
		NextRunTime = 0;
		PauseTime = 0;
		ResetParamPerGame = true;
		Original = Param;
		First = true;
		OrigPaused = false;
		OrigInitialDelay = 0;
		_Param = Param;
	}
	ActiveRoutine::ActiveRoutine(Routine_t Fn, const RoutineParam& Param)
		:Set({ nullptr, Fn, nullptr, nullptr }), 
		Paused(false), 
		NextRunTime(0), 
		PauseTime(0), 
		ResetParamPerGame(true),
		Original(Param),
		First(true),
		OrigPaused(false),
		OrigInitialDelay(0),
		_Param(Param) {}
	ActiveRoutine::ActiveRoutine(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam)
		:Set(Routine),
		Paused(Paused),
		NextRunTime(InitialDelay),
		PauseTime(0),
		ResetParamPerGame(ResetParam),
		Original(Param),
		First(true),
		OrigPaused(Paused),
		OrigInitialDelay(InitialDelay),
		_Param(Param) {}
	ActiveRoutine::~ActiveRoutine()
	{
		if (!First)
		{
			if(Set.Exit)Set.Exit(&_Param);
			First = true;
		}
	}
	void ActiveRoutine::Reset()
	{
		Paused = OrigPaused;
		NextRunTime = OrigInitialDelay;
		PauseTime = 0;
		if (!First)
		{
			if (Set.Exit)Set.Exit(&_Param);
			First = true;
		}
		if (ResetParamPerGame)
		{
			_Param = Original;
		}
	}
	void ActiveRoutine::Uninit(bool ShouldDestroy)
	{
		Paused = OrigPaused;
		NextRunTime = OrigInitialDelay;
		PauseTime = 0;
		if (!First)
		{
			if (ShouldDestroy)
			{
				if (Set.Uninit)Set.Uninit(&_Param);
			}
			First = true;
		}
		if (ResetParamPerGame)
		{
			_Param = Original;
		}
	}
	bool ActiveRoutine::Execute()
	{
		if (!Set.Loop)return false;
		if (First)
		{
			if (Set.Init)Set.Init(&_Param);
		}
		auto i = Set.Loop(&_Param);
		if (i == PauseThisRoutine)
		{
			Pause();
			return true;
		}
		else
		{
			NextRunTime += i;
			return i != INT_MAX;
		}
		
	}
	void ActiveRoutine::InstantRun()
	{
		Paused = false;
		NextRunTime = Unsorted::CurrentFrame();
	}
	bool ActiveRoutine::ShouldRun()
	{
		if (Paused)return false;
		return NextRunTime <= Unsorted::CurrentFrame();
	}
	void ActiveRoutine::Pause()
	{
		if (Paused)return;
		Paused = true;
		PauseTime = Unsorted::CurrentFrame();
	}
	void ActiveRoutine::Resume()
	{
		if (!Paused)return;
		Paused = false;
		NextRunTime += Unsorted::CurrentFrame() - PauseTime;
	}
	void ActiveRoutine::Delay(int NFrame)
	{
		NextRunTime += NFrame;
	}

	
	
	//FuncInfo
	FuncInfo* GetFuncFromLib(const char* pLib, const char* pFunc, int Version)
	{
		if (!strlen(pLib))return Internal_GetFunc(pFunc, Version);
		
		auto it = LibMap.find(pLib);
		if (it != LibMap.end() && !it->second->RemoteComponent)
		{
			if (!it->second->Available)return nullptr;
			if (Version != DoNotCheckVersion && Version > it->second->Out->Info->Version)return nullptr;
			auto pInfo = it->second->Out->GetFunc(pFunc, (Version == DoNotCheckVersion ? it->second->Out->Info->Version : Version));
			if (!pInfo)return nullptr;
			if (pInfo->ClassVersion > FuncInfo::GClassVersion)return nullptr;
			return pInfo;
		}
		else if(RemoteComponentManager::GetComponentByName(conv pLib))
			return RemoteComponentManager::GetRemoteMethodInfo(pLib, pFunc, Version);
		else return nullptr;
	}
	//std::unordered_map<std::string, std::vector<LibFuncHandle>> NamedFunc;

	std::unordered_map<std::string, FuncInfo*> GetFuncMap(const char* pFunc)
	{
		std::unordered_map<std::string, FuncInfo*> Result;

		auto pInt = Internal_GetFunc(pFunc, DoNotCheckVersion);
		if (pInt)Result[""] = pInt;

		for (auto& Lib : Libs)
		{
			if (!Lib.Available)continue;
			FuncInfo* pInfo;
			if (Lib.RemoteComponent)pInfo = RemoteComponentManager::GetRemoteMethodInfo(Lib.Out->Info->LibName, pFunc, Lib.Out->Info->Version);
			else pInfo = Lib.Out->GetFunc(pFunc, Lib.Out->Info->Version);
			if (pInfo && pInfo->ClassVersion <= FuncInfo::GClassVersion)
				Result[Lib.Out->Info->LibName] = pInfo;
		}
		return Result;
	}

	PArray<FuncInfo*> GetFuncByName(const char* pFunc)
	{
		auto it = NamedFunc.find(pFunc);
		if (it != NamedFunc.end())return PArray<FuncInfo*>(it->second);
		auto& pVec = NamedFunc[pFunc];

		auto pInt = Internal_GetFunc(pFunc, DoNotCheckVersion);
		if (pInt)pVec.push_back(pInt);

		for (auto& Lib : Libs)
		{
			if (!Lib.Available)continue;
			if (Lib.RemoteComponent)
			{
				auto pInfo = RemoteComponentManager::GetRemoteMethodInfo(Lib.Out->Info->LibName, pFunc, Lib.Out->Info->Version);
				if (pInfo && pInfo->ClassVersion <= FuncInfo::GClassVersion)
					pVec.push_back(pInfo);
			}
			else
			{
				auto pInfo = Lib.Out->GetFunc(pFunc, Lib.Out->Info->Version);
				if (pInfo && pInfo->ClassVersion <= FuncInfo::GClassVersion)
					pVec.push_back(pInfo);
			}
		}
		return PArray<FuncInfo*>(pVec);
	}

	//没有找到//找到且唯一//找到但不唯一
	std::variant<std::monostate, FuncInfo*, std::unordered_map<std::string, FuncInfo*>>
		PickFunctionForCommand(const char* pLib, const char* pFunc, int Version)
	{
		if (!strlen(pLib))
		{
			auto Funcs = GetFuncMap(pFunc);
			std::vector<FuncInfo*> Result;
			std::string LastName;
			for (auto& [Name, pf] : Funcs)
			{
				if (!pf->IsConsiderAsCommand())continue;
				LastName = Name;
				Result.push_back(pf);
			}

			if (Result.size() == 1)
			{
				auto pf = GetFuncFromLib(LastName.c_str(), pFunc, Version);
				if (pf) return pf;
				//此处分离出nullptr
				//在类型上区分
				else return std::monostate{};
			}
			else if(Result.size() == 0)
			{
				return std::monostate{};
			}
			else
			{
				return Funcs;
			}
		}
		else
		{
			auto pf = GetFuncFromLib(pLib, pFunc, Version);
			if (pf && pf->IsConsiderAsCommand()) return pf;
			else return std::monostate{};
		}
	}

	int GetVersion()
	{
		return PRODUCT_VERSION;
	}

	int GetLSV()
	{
		return PRODUCT_LOWEST_SUPPORTED_VERSION;
	}

	void RegisterContextProcessor(const char* Type, ContextFunc_t pProcessor)
	{
		ContextMap[Type] = pProcessor;
	}
	void RegisterBinder(const char* Type, const char* BindType, Binder_t pBinder)
	{
		BinderMap[Type + Delim + BindType] = { pBinder, BindType };
	}
	void RegisterGenerator(const char* BindType, BindingGenerator_t pGenerator)
	{
		GeneratorMap.insert({ BindType,pGenerator });
	}
	void RegisterDirectBinder(const char* Type, const char* BindType, DirectBinder_t pDirectBinder)
	{
		DirectBinderMap[Type + Delim + BindType] = pDirectBinder;
	}
	bool RegisterRoutine(const char* Name, Routine_t Routine, RoutineParam Param)
	{
		auto it = RoutineMap.find(Name);
		if (it != RoutineMap.end())return false;
		RoutineMap[Name].reset(new ActiveRoutine(Routine, Param));
		return true;
	}
	bool RegisterRoutineSet(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam, CreateMode Mode)
	{
		auto it = RoutineMap.find(Name);
		auto RMode = (CreateMode)((int)Mode - (int)CreateMode::CtxReplace);
		if (RMode == CreateMode::ProcMerge)RMode = CreateMode::ProcReplace;
		if (it != RoutineMap.end())
		{
			if (Mode >= CreateMode::CtxReplace)
			{
				if (RMode == CreateMode::ProcReserve)return false;
				else
				{
					it->second->Param() = Param;
					return true;
				}
			}
			else
			{
				if (RMode == CreateMode::ProcReserve)
				{
					auto& ptr = it->second;
					auto par = ptr->Param();
					ptr->Uninit(false);
					ptr.reset(new ActiveRoutine(Name, Routine, par, Paused, InitialDelay, ResetParam));
				}
				else
				{
					auto& ptr = it->second;
					ptr->Uninit(true);
					ptr.reset(new ActiveRoutine(Name, Routine, Param, Paused, InitialDelay, ResetParam));
				}
			}
		}
		else
		{
			auto& ptr = RoutineMap[Name];
			ptr.reset(new ActiveRoutine(Name, Routine, Param, Paused, InitialDelay, ResetParam));
		}
		return true;
	}


	void __SXWProcess(JsonObject pContext, const char* Type)
	{
		if (!strncmp(Type, "CTX:", 4))
		{
			auto it = ContextMap.find(Type + 4);
			if (it == ContextMap.end())
			{
				Debug::Log("Bad Context \"%s\"\n", Type + 4);
				pContext.SetNull();
			}
			else
			{
				if (it->second)it->second(pContext);
			}
		}
	}

	JsonObject _Binding;
	const char* _pType;
	GeneratorParam _Param;
	void __BindProcess(JsonObject pContext, const char* Type)
	{
		if (!strncmp(Type, "BIND:", 5))
		{
			auto it = BinderMap.find(std::string(Type + 5) + Delim + _pType);
			if (it == BinderMap.end())
			{
				Debug::Log("Bad Binder \"%s\"\n", Type + 5);
				pContext.SetNull();
			}
			else
			{
				if (it->second.Func)it->second.Func(_Binding, pContext);
			}
		}
	}

	void __DirectBindProcess(JsonObject pContext, const char* Type)
	{
		if (!strncmp(Type, "BIND:", 5))
		{
			auto it = DirectBinderMap.find(std::string(Type + 5) + Delim + _pType);
			if (it == DirectBinderMap.end())
			{
				Debug::Log("Bad DirectBinder \"%s\"\n", Type + 5);
				pContext.SetNull();
			}
			else
			{
				if (it->second)it->second(pContext, _Param);
			}
		}
	}

	void __SXWHelper(JsonObject pContext, void (*Process)(JsonObject, const char*))
	{
		if (!pContext.Available())return;
		if (pContext.IsTypeObject())
		{
			for (auto p : pContext.GetMapObject())
			{
				__SXWHelper(p.second, Process);
			}
		}
		else if (pContext.IsTypeArray())
		{
			for (auto p : pContext.GetArrayObject())
			{
				__SXWHelper(p, Process);
			}
		}
		else if (pContext.IsTypeString())
		{
			const char* pc = pContext.GetCString();
			Process(pContext, pc);
		}
	}

	void GenerateContext(JsonObject pContext)
	{
		if (!pContext.Available())return;
		__SXWHelper(pContext, __SXWProcess);
	}
	void GenerateBinder(JsonObject pContext, JsonObject pBinding, const char* Type)
	{
		if (!pContext.Available())return;
		if (!pBinding.Available())return;
		_Binding = pBinding;
		_pType = Type;
		__SXWHelper(pContext, __BindProcess);
	}
	void GenerateBinding(JsonObject pBinding, const char* Type, const GeneratorParam& Param)
	{
		if (!pBinding.Available())return;
		auto rg = GeneratorMap.equal_range(Type);
		auto it = GeneratorMap.find(Type);
		if (it == GeneratorMap.end())
		{
			Debug::Log("Bad BindGenerator \"%s\"\n", Type);
		}
		else
		{
			for (auto it2 = rg.first; it2 != rg.second; ++it2)
			{
				if (it2->second)it2->second(pBinding, Param);
			}

		}
	}
	void GenerateDirectBinder(JsonObject pContext, const char* BindType, const GeneratorParam& Param)
	{
		if (!pContext.Available())return;
		_pType = BindType;
		_Param = Param;
		__SXWHelper(pContext, __DirectBindProcess);
	}

	//0,1 for bool other return 2; default return -1
	GenCallRetType GeneralCall(const FuncInfo& Fn, JsonObject Context)
	{
		if (!Fn.Func)return GenCallRetType::Default;
		switch (Fn.Type)
		{
		case FuncType::Condition:
		{
			auto pf = (FuncType_Condition)Fn.Func;
			if (Fn.Func)
			{
				JsonFile File;
				if (!Context.Available())return GenCallRetType::Default;
				File.DuplicateFromObject(Context, true);
				GenerateContext(File.GetObj());
				return pf(File.GetObj()) ? GenCallRetType::True : GenCallRetType::False;
			}
		}break;
		case FuncType::Action:
		{
			auto pf = (FuncType_Action)Fn.Func;
			JsonFile File;
			if (!Context.Available())return GenCallRetType::Default;
			File.DuplicateFromObject(Context, true);
			GenerateContext(File.GetObj());
			pf(File.GetObj());
			return GenCallRetType::Void;
		}break;
		case FuncType::Callback:
		{
			auto pf = (FuncType_Callback)Fn.Func;
			JsonFile File;
			if (!Context.Available())return GenCallRetType::Default;
			File.DuplicateFromObject(Context, true);
			GenerateContext(File.GetObj());
			pf(File.GetObj());
			return GenCallRetType::Void;
		}break;
		case FuncType::Procedure:
		{
			auto pf = (FuncType_Procedure)Fn.Func;
			pf();
			return GenCallRetType::Void;
		}break;
		case FuncType::Default:
		default:
			return GenCallRetType::Default;
		}
		return GenCallRetType::Default;
	}

	GenCallRetType GeneralCallAlt(const FuncInfo& Fn, const GeneratorParam& Param)
	{
		if (!Fn.Func)return GenCallRetType::Default;
		switch (Fn.Type)
		{
		case FuncType::ConditionAlt:
		{
			auto pf = (FuncType_ConditionAlt)Fn.Func;
			return pf(Param) ? GenCallRetType::True : GenCallRetType::False;
		}break;
		case FuncType::ActionAlt:
		{
			auto pf = (FuncType_ActionAlt)Fn.Func;
			pf(Param);
			return GenCallRetType::Void;
		}break;
		case FuncType::Procedure:
		{
			auto pf = (FuncType_Procedure)Fn.Func;
			pf();
			return GenCallRetType::Void;
		}break;
		default:
			return GenCallRetType::Default;
		}
		return GenCallRetType::Default;
	}


	std::unordered_map <std::string, std::map<GeneratorParam, JsonFile>>ObjList;
	std::unordered_map <std::string, std::map<GeneratorParam, ActiveExecutor>>ExecList;
	JsonObject DirectBindContextTo(JsonObject Context, const ContextIndex& Idx)
	{
		if (!Context.Available())return Context;
		auto file = ObjList[Idx.Type + Delim + Idx.ExecType][Idx.Param];
		file.DuplicateFromObject(Context, true);
		auto CtxD = file.GetObj();
		GenerateDirectBinder(CtxD, Idx.Type, Idx.Param);
		return CtxD;
	}
	JsonObject DirectBindTextTo(const char* Text, const ContextIndex& Idx)
	{
		if (!Text)return NullJsonObject;
		auto file = ObjList[Idx.Type + Delim + Idx.ExecType][Idx.Param];
		file.Parse(Text);
		auto CtxD = file.GetObj();
		return CtxD;
	}
	JsonObject BindContextTo(JsonObject Context, const ContextIndex& Idx)
	{
		if (!Context.Available())return Context;
		auto file = ObjList[Idx.Type + Delim + Idx.ExecType][Idx.Param];
		file.DuplicateFromObject(Context, true);
		auto CtxD = file.GetObj();
		JsonFile Binding(new cJSON);
		GenerateBinding(Binding, Idx.Type, Idx.Param);
		GenerateBinder(CtxD, Binding, Idx.Type);
		return CtxD;
	}
	JsonObject GetContextByIdx(const ContextIndex& Idx)
	{
		auto it1 = ObjList.find(Idx.Type + Delim + Idx.ExecType);
		if (it1 == ObjList.end())return NullJsonObject;
		auto it2 = it1->second.find(Idx.Param);
		if (it2 == it1->second.end())return NullJsonObject;
		return it2->second.GetObj();
	}
	void DeleteContextByIdx(const ContextIndex& Idx)
	{
		auto it1 = ObjList.find(Idx.Type + Delim + Idx.ExecType);
		if (it1 == ObjList.end())return;
		if (it1->second.find(Idx.Param) != it1->second.end())
			it1->second.erase(Idx.Param);
	}
	RoutineHandle GetRoutine(const char* Name)
	{
		auto it = RoutineMap.find(Name);
		if (it == RoutineMap.end())return nullptr;
		else return it->second.get();
	}
	void InstantRunRoutine(RoutineHandle Routine)
	{
		if (!Routine)return;
		((ActiveRoutine*)Routine)->InstantRun();
	}
	void PauseRoutine(RoutineHandle Routine)
	{
		if (!Routine)return;
		((ActiveRoutine*)Routine)->Pause();
	}
	void ResumeRoutine(RoutineHandle Routine)
	{
		if (!Routine)return;
		((ActiveRoutine*)Routine)->Resume();
	}
	void DelayRoutine(RoutineHandle Routine, int DelayTime)
	{
		if (!Routine)return;
		((ActiveRoutine*)Routine)->Delay(DelayTime);
	}
	void DeleteRoutine(const char* Name)
	{
		auto it = RoutineMap.find(Name);
		if (it != RoutineMap.end())
		{
			it->second->Uninit(true);
			RoutineMap.erase(Name);
		}
			
	}
	RoutineParam* GetRoutineParam(RoutineHandle Routine)
	{
		if (!Routine)return nullptr;
		return &((ActiveRoutine*)Routine)->Param();
	}



	//若没有Destructor，填补nullptr
	bool MakeExecutor_BaseText(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Text, const char* Type, const char* ExecType, SwizzleExecutor_t Swizzle, FuncInfo* Destructor)
	{
		if (!Action)return false;
		if (Action->Type != FuncType::Action && Action->Type != FuncType::Callback && Action->Type != FuncType::Procedure)return false;
		if (!Text)return false;
		if (!Type)return false;
		if (!ExecType)return false;
		if (Destructor)
		{
			if (Destructor->Type != FuncType::Action && Destructor->Type != FuncType::Callback && Destructor->Type != FuncType::Procedure)return false;
		}
		Base.Action = Action;
		Base.ExecType = ExecType;
		Base.Context = JsonObject((cJSON*)Text);
		Base.Type = Type;
		Base.Delay = Delay;
		Base.Swizzle = Swizzle;
		Base.Destructor = Destructor;
		return true;
	}
	bool MakeExecutor_Base(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Type, const char* ExecType, JsonObject Context, FuncInfo* Destructor)
	{
		if (!Action)return false;
		if (Action->Type != FuncType::Action && Action->Type != FuncType::Callback && Action->Type != FuncType::Procedure)return false;
		if (!Context.Available())return false;
		if (!Type)return false;
		if (!ExecType)return false;
		if (Destructor)
		{
			if (Destructor->Type != FuncType::Action && Destructor->Type != FuncType::Callback && Destructor->Type != FuncType::Procedure)return false;
		}
		Base.Action = Action;
		Base.ExecType = ExecType;
		Base.Context = Context;
		Base.Type = Type;
		Base.Delay = Delay;
		Base.Swizzle = nullptr;
		Base.Destructor = Destructor;
		return true;
	}






	bool MakeExecutor_Trigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		if (!Target.Base.Action)return false;
		if (!Condition)return false;
		if (Condition->Type != FuncType::Condition)return false;
		Target.Condition = Condition;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::Trigger;
		return true;
	}
	bool MakeExecutor_InfiniteTrigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		auto Ret = MakeExecutor_Trigger(Target, Condition, Interval);
		Target.Type = GeneralExecutor::type::InfiniteTrigger;
		return Ret;
	}
	bool MakeExecutor_Once(GeneralExecutor& Target)
	{
		if (!Target.Base.Action)return false;
		Target.Type = GeneralExecutor::type::Once;
		return true;
	}
	bool MakeExecutor_InfiniteLoop(GeneralExecutor& Target, int Interval)
	{
		if (!Target.Base.Action)return false;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::InfiniteLoop;
		return true;
	}
	bool MakeExecutor_WhileLoop(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		if (!Target.Base.Action)return false;
		if (!Condition)return false;
		if (Condition->Type != FuncType::Condition)return false;
		Target.Condition = Condition;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::WhileLoop;
		return true;
	}
	bool MakeExecutor_NLoop(GeneralExecutor& Target, int N, int Interval)
	{
		if (!Target.Base.Action)return false;
		Target.Condition = (FuncInfo*)N;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::NLoop;
		return true;
	}





	bool MakeExecutorEx_Trigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		if (!Base.Action)return false;
		if (!Condition)return false;
		if (Condition->Type != FuncType::Condition)return false;
		Target.Condition = Condition;
		Target.Base = Base;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::Trigger;
		return true;
	}
	bool MakeExecutorEx_InfiniteTrigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		auto Ret = MakeExecutorEx_Trigger(Target, Base, Condition, Interval);
		Target.Type = GeneralExecutor::type::InfiniteTrigger;
		return Ret;
	}
	bool MakeExecutorEx_Once(GeneralExecutor& Target, const ExecutorBase& Base)
	{
		if (!Base.Action)return false;
		Target.Base = Base;
		Target.Type = GeneralExecutor::type::Once;
		return true;
	}
	bool MakeExecutorEx_InfiniteLoop(GeneralExecutor& Target, const ExecutorBase& Base, int Interval)
	{
		if (!Base.Action)return false;
		Target.Base = Base;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::InfiniteLoop;
		return true;
	}
	bool MakeExecutorEx_WhileLoop(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		if (!Base.Action)return false;
		if (!Condition)return false;
		if (Condition->Type != FuncType::Condition)return false;
		Target.Condition = Condition;
		Target.Base = Base;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::WhileLoop;
		return true;
	}
	bool MakeExecutorEx_NLoop(GeneralExecutor& Target, const ExecutorBase& Base, int N, int Interval)
	{
		if (!Base.Action)return false;
		Target.Condition = (FuncInfo*)N;
		Target.Base = Base;
		Target.Interval = Interval;
		Target.Type = GeneralExecutor::type::NLoop;
		return true;
	}



	void ActiveExecutor::Init_Impl(const GeneratorParam& Param, bool DirectBind)
	{
		Idx.Param = Param;
		Idx.Type = Exec.Base.Type;
		Idx.ExecType = Exec.Base.ExecType;
		NextRunTime = Unsorted::CurrentFrame() + Exec.Base.Delay;
		if (Exec.Base.Swizzle)
		{
			CurrentContext = DirectBindTextTo((const char*)Exec.Base.Context.GetRaw(), Idx);
			Exec.Base.Swizzle(&Exec, &Idx.Param, CurrentContext);
		}
		else
		{
			if (DirectBind)
			{
				CurrentContext = DirectBindContextTo(Exec.Base.Context, Idx);
			}
			else
			{
				CurrentContext = BindContextTo(Exec.Base.Context, Idx);
			}
		}
	}
	void ActiveExecutor::Init(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind)
	{
		Exec = GenExec;
		Finished = false;
		Counter_NLoop = 0;
		Init_Impl(Param, DirectBind);
	}
	ActiveExecutor::ActiveExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind)
		:Exec(GenExec), Finished(false), Counter_NLoop(0)
	{
		Init_Impl(Param, DirectBind);
	}
	ActiveExecutor::ActiveExecutor(noinit_t) {}
	ActiveExecutor::~ActiveExecutor() { Destroy(); }
	void ActiveExecutor::Destroy()
	{
		if (Exec.Base.Destructor != nullptr)
		{
			GeneralCall(*Exec.Base.Destructor, CurrentContext);
		}
		DeleteContextByIdx(Idx);
	}
	bool ActiveExecutor::ShouldDestroy() const
	{
		return Finished;
	}
	bool ActiveExecutor::ShouldExecute() const
	{
		return NextRunTime <= Unsorted::CurrentFrame();
	}
	void ActiveExecutor::Execute()
	{
		switch (Exec.Type)
		{
		case GeneralExecutor::type::Trigger:
		{
			if (GeneralCall(*Exec.Condition, CurrentContext) == GenCallRetType::True)
			{
				GeneralCall(*Exec.Base.Action, CurrentContext);
				Finished = true;
			}
			else
			{
				NextRunTime += Exec.Interval;
			}
		}break;
		case GeneralExecutor::type::Once:
		{
			GeneralCall(*Exec.Base.Action, CurrentContext);
			Finished = true;
		}break;
		case GeneralExecutor::type::InfiniteLoop:
		{
			GeneralCall(*Exec.Base.Action, CurrentContext);
			NextRunTime += Exec.Interval;
		}break;
		case GeneralExecutor::type::WhileLoop:
		{
			if (GeneralCall(*Exec.Condition, CurrentContext) != GenCallRetType::True)Finished = true;
			else
			{
				GeneralCall(*Exec.Base.Action, CurrentContext);
				NextRunTime += Exec.Interval;
			}
		}break;
		case GeneralExecutor::type::NLoop:
		{
			if (Counter_NLoop >= (int)Exec.Condition)Finished = true;
			else
			{
				GeneralCall(*Exec.Base.Action, CurrentContext);
				NextRunTime += Exec.Interval;
				++Counter_NLoop;
			}
		}break;
		case GeneralExecutor::type::InfiniteTrigger:
		{
			if (GeneralCall(*Exec.Condition, CurrentContext) == GenCallRetType::True)
				GeneralCall(*Exec.Base.Action, CurrentContext);
			NextRunTime += Exec.Interval;
		}break;
		default:
			Finished = true; 
			break;
		};
	}

	//如有swizzle，Param应该是swizzle后的
	void AddExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param,bool DirectBind)
	{
		bool Found = true;
		auto idx1 = GenExec.Base.Type + Delim + GenExec.Base.ExecType;
		auto it1 = ExecList.find(idx1);
		if (it1 == ExecList.end())Found = false;
		else
		{
			auto it2 = it1->second.find(Param);
			if (it2 == it1->second.end())Found = false;
			it2->second.Destroy();
			it2->second.Init(GenExec, Param, DirectBind);
		}
		if (!Found)
		{
			auto pp = ExecList[idx1].emplace(Param, ActiveExecutor(noinit_t()));
			if (pp.second)pp.first->second.Init(GenExec, Param, DirectBind);
		}
	}

	ActiveExecutor* GetExecutor(const ContextIndex& Idx)
	{
		auto it1 = ExecList.find(Idx.Type + Delim + Idx.ExecType);
		if (it1 == ExecList.end())return nullptr;
		auto it2 = it1->second.find(Idx.Param);
		if (it2 == it1->second.end())return nullptr;
		return &it2->second;
	}

	bool HasExecutor(const ContextIndex& Idx)
	{
		return GetExecutor(Idx) != nullptr;
	}

	void RemoveExecutor(const ContextIndex& Idx)
	{
		auto it1 = ExecList.find(Idx.Type + Delim + Idx.ExecType);
		if (it1 == ExecList.end())return;
		if (it1->second.find(Idx.Param) != it1->second.end())it1->second.erase(Idx.Param);
	}

	void ExecutorRunLoop()
	{
		std::vector<std::string> RoutineToErase;
		for (auto& pr : RoutineMap)
		{
			if (pr.second->ShouldRun())
				if (!pr.second->Execute())
					RoutineToErase.push_back(pr.first);
		}
		for (auto& es : RoutineToErase)
		{
			RoutineMap.erase(es);
		}
		
		std::vector<std::string> MapToErase;
		std::vector<GeneratorParam> ExecToErase;
		for (auto& pm : ExecList)
		{
			ExecToErase.clear();
			for (auto& pe : pm.second)
			{
				auto& e = pe.second;
				if (e.ShouldExecute())
				{
					e.Execute();
				}
				if (e.ShouldDestroy())
				{
					ExecToErase.push_back(pe.first);
				}
			}
			for (auto& ex : ExecToErase)
			{
				pm.second.erase(ex);
			}
			if (pm.second.empty())
			{
				MapToErase.push_back(pm.first);
			}
		}
		for (auto& es : MapToErase)
		{
			ExecList.erase(es);
		}
	}
	void ResetGetFunctionBuffer()
	{
		NamedFunc.clear();
	}

	void RemoteReturnInfo_Destroy(RemoteReturnInfo* Info)
	{
		ReturnInfoPtr& info = *(ReturnInfoPtr*)Info;
		info.Release();
	}

	UTF8_CString RemoteReturnInfo_GetErrorMessage(const RemoteReturnInfo* Info)
	{
		const ReturnInfoPtr& info = *(const ReturnInfoPtr*)Info;
		return info.GetErrorMessage();
	}

	bool RemoteReturnInfo_Succeeded(const RemoteReturnInfo* Info)
	{
		const ReturnInfoPtr& info = *(const ReturnInfoPtr*)Info;
		return info.Succeeded();
	}

	JsonObject RemoteReturnInfo_GetResponseData(const RemoteReturnInfo* Info)
	{
		const ReturnInfoPtr& info = *(const ReturnInfoPtr*)Info;
		return info.GetResponseData();
	}







	void ExitClear()
	{
		/*
		auto List = GetFuncByName("IHCore::Exit");
		Debug::Log("List N=%d Data=%X\n", List.N, List.Data);
		if(List.Data)
			for (size_t i = 0; i < List.N; i++)
		{
			if (List.Data[i]->Type == FuncType::Procedure)
			{
				Debug::Log("Calling");
				((void(__cdecl*)(void))List.Data[i]->Func)();
				Debug::Log("Condfngfhid");
			}
		}

		RoutineMap.clear();

		ObjList.clear();
		ExecList.clear();
		*/
	}


	void ResetAll()
	{
		static bool First = true;
		if (First) {
			(void)0;
		}
		for (auto& p : RoutineMap)
		{
			p.second->Reset();
		}


		ObjList.clear();
		ExecList.clear();

		auto List = GetFuncByName("IHCore::Reset");
		for (size_t i = 0; i < List.N; i++)
		{
			if (List.Data[i]->Type == FuncType::Procedure)
			{
				((void(__cdecl*)(void))List.Data[i]->Func)();
			}
		}
	}

	void LocalFrameUpdate()
	{
		auto List = GetFuncByName("IHCore::FrameUpdate");
		for (size_t i = 0; i < List.N; i++)
		{
			if (List.Data[i]->Type == FuncType::Procedure)
			{
				((void(__cdecl*)(void))List.Data[i]->Func)();
			}
		}
	}

	bool RegisterIHFileStream(const char* StreamName, VClass vc)
	{
		IHFileStreamer[StreamName] = vc;
		return true;
	}

	bool PostAsyncRemoteCall(const char* pLib, const char* pFunc, int Version, JsonObject Context)
	{
		RemoteCallSendInfo Info;
		Info.Source = u8"";
		Info.Component = conv pLib;
		Info.Version = Version;
		Info.Method = conv pFunc;
		Info.Context = Context;
		return RemoteComponentManager::PostComponentMethod(Info);
	}

	/*
	void ReturnString(const std::u8string& Str);

	void ReturnError(const std::u8string& Str, int Code);

	void DoNotEcho();

	void ReturnStdError(long Code);

	void SetGlobalVar(const std::u8string& Key, const std::u8string& Value);

	void SetErrorCode(int Code);

	int GetErrorCode();

	void GetLastResult(std::u8string& Ret, std::u8string& ErrorStr, int& ErrorCode);

	const std::u8string& GetVar(const std::u8string& Key);
	*/
	//UTF8_CharType
	//UTF8_CString

	void IHCore_Free(void* p)
	{
		free(p);
	}
	void DbgFunc_ReturnString(UTF8_CString Str)
	{
		ECCommand::ReturnString(Str);
	}
	void DbgFunc_ReturnError(UTF8_CString Str, int Code)
	{
		ECCommand::ReturnError(Str, Code);
	}
	void DbgFunc_DoNotEcho()
	{
		ECCommand::DoNotEcho();
	}
	void DbgFunc_ReturnStdError(long Code)
	{
		ECCommand::ReturnStdError(Code);
	}
	void DbgFunc_SetGlobalVar(UTF8_CString Key, UTF8_CString Value)
	{
		ECCommand::SetGlobalVar(Key, Value);
	}
	void DbgFunc_SetErrorCode(int Code)
	{
		ECCommand::SetErrorCode(Code);
	}
	int DbgFunc_GetErrorCode()
	{
		return ECCommand::GetErrorCode();
	}
	void DbgFunc_GetLastResult(UTF8_CString& Ret, UTF8_CString& ErrorStr, int& ErrorCode)
	{
		std::u8string _Ret, _ErrorStr;
		int _ErrorCode;
		ECCommand::GetLastResult(_Ret, _ErrorStr, _ErrorCode);
		char8_t* p1 = (char8_t*)malloc(_Ret.size() + 1);
		memcpy(p1, _Ret.c_str(), _Ret.size() + 1);
		char8_t* p2 = (char8_t*)malloc(_ErrorStr.size() + 1);
		memcpy(p2, _ErrorStr.c_str(), _ErrorStr.size() + 1);
		Ret = p1;
		ErrorStr = p2;
		ErrorCode = _ErrorCode;
	}
	UTF8_CString DbgFunc_GetVar(UTF8_CString Key)
	{
		auto& Str = ECCommand::GetVar(Key);
		char8_t* p = (char8_t*)malloc(Str.size() + 1);
		memcpy(p, Str.c_str(), Str.size() + 1);
		return p;
	}
















	//FuncInfo
	FuncInfo* __cdecl Export_GetFuncFromLib(const char* pLib, const char* pFunc, int Version)
	{
		return GetFuncFromLib(pLib, pFunc, Version);
	}
	PArray<FuncInfo*> __cdecl Export_GetFuncByName(const char* pFunc)
	{
		return GetFuncByName(pFunc);
	}
	int  __cdecl Export_GetVersion()
	{
		return GetVersion();
	}
	//PLACEHOLDER 2

	//注册函数
	void __cdecl Export_RegisterContextProcessor(const char* Type, ContextFunc_t pProcessor)
	{
		RegisterContextProcessor(Type, pProcessor);
	}
	//void Export_RegisterBinder(const char* Type, const char* BindType, Binder_t pBinder);
	//void Export_RegisterGenerator(const char* BindType, BindingGenerator_t pGenerator);
	void __cdecl Export_RegisterDirectBinder(const char* Type, const char* BindType, DirectBinder_t pDirectBinder)
	{
		RegisterDirectBinder(Type, BindType, pDirectBinder);
	}
	bool __cdecl Export_RegisterRoutine(const char* Name, Routine_t Routine, RoutineParam Param)
	{
		return RegisterRoutine(Name, Routine, Param);
	}
	//0,1 for bool other return 2; default return -1
	GenCallRetType __cdecl Export_GeneralCall(const FuncInfo& Fn, JsonObject Context)
	{
		return GeneralCall(Fn, Context);
	}
	//Context
	JsonObject __cdecl Export_DirectBindContextTo(JsonObject Context, const ContextIndex& Idx)
	{
		return DirectBindContextTo(Context, Idx);
	}
	JsonObject __cdecl Export_DirectBindTextTo(const char* Text, const ContextIndex& Idx)
	{
		return DirectBindTextTo(Text, Idx);
	}
	//JsonObject Export_BindContextTo(JsonObject Context, const ContextIndex& Idx);
	
	//PLACEHOLDER 6
	//PLACEHOLDER 7

	JsonObject __cdecl Export_GetContextByIdx(const ContextIndex& Idx)
	{
		return GetContextByIdx(Idx);
	}
	void __cdecl Export_DeleteContextByIdx(const ContextIndex& Idx)
	{
		DeleteContextByIdx(Idx);
	}
	RoutineHandle __cdecl Export_GetRoutine(const char* Name)
	{
		return GetRoutine(Name);
	}
	void __cdecl Export_InstantRunRoutine(RoutineHandle Routine)
	{
		InstantRunRoutine(Routine);
	}
	void __cdecl Export_PauseRoutine(RoutineHandle Routine)
	{
		PauseRoutine(Routine);
	}
	void __cdecl Export_ResumeRoutine(RoutineHandle Routine)
	{
		ResumeRoutine(Routine);
	}
	void __cdecl Export_DelayRoutine(RoutineHandle Routine, int Delay)
	{
		DelayRoutine(Routine, Delay);
	}
	void __cdecl Export_DeleteRoutine(const char* Name)
	{
		DeleteRoutine(Name);
	}
	//若没有Destructor，填补nullptr
	bool __cdecl Export_MakeExecutor_BaseText(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Text, const char* Type, const char* ExecType, SwizzleExecutor_t Swizzle, FuncInfo* Destructor)
	{
		return MakeExecutor_BaseText(Base, Action, Delay, Text, Type, ExecType, Swizzle, Destructor);
	}
	bool __cdecl Export_MakeExecutor_Base(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Type, const char* ExecType, JsonObject Context, FuncInfo* Destructor)
	{
		return MakeExecutor_Base(Base, Action, Delay, Type, ExecType, Context, Destructor);
	}

	//PLACEHOLDER 8
	//PLACEHOLDER 9

	//填充
	bool __cdecl Export_MakeExecutor_Trigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		return MakeExecutor_Trigger(Target, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutor_InfiniteTrigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		return MakeExecutor_InfiniteTrigger(Target, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutor_Once(GeneralExecutor& Target)
	{
		return MakeExecutor_Once(Target);
	}
	bool __cdecl Export_MakeExecutor_InfiniteLoop(GeneralExecutor& Target, int Interval)
	{
		return MakeExecutor_InfiniteLoop(Target, Interval);
	}
	bool __cdecl Export_MakeExecutor_WhileLoop(GeneralExecutor& Target, FuncInfo* Condition, int Interval)
	{
		return MakeExecutor_WhileLoop(Target, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutor_NLoop(GeneralExecutor& Target, int N, int Interval)
	{
		return MakeExecutor_NLoop(Target, N, Interval);
	}

	//PLACEHOLDER 10
	//PLACEHOLDER 11

	//填充 EX
	bool __cdecl Export_MakeExecutorEx_Trigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		return MakeExecutorEx_InfiniteTrigger(Target, Base, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutorEx_InfiniteTrigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		return MakeExecutorEx_InfiniteTrigger(Target, Base, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutorEx_Once(GeneralExecutor& Target, const ExecutorBase& Base)
	{
		return MakeExecutorEx_Once(Target, Base);
	}
	bool __cdecl Export_MakeExecutorEx_InfiniteLoop(GeneralExecutor& Target, const ExecutorBase& Base, int Interval)
	{
		return MakeExecutorEx_InfiniteLoop(Target, Base, Interval);
	}
	bool __cdecl Export_MakeExecutorEx_WhileLoop(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval)
	{
		return MakeExecutorEx_WhileLoop(Target, Base, Condition, Interval);
	}
	bool __cdecl Export_MakeExecutorEx_NLoop(GeneralExecutor& Target, const ExecutorBase& Base, int N, int Interval)
	{
		return MakeExecutorEx_NLoop(Target, Base, N, Interval);
	}

	//PLACEHOLDER 12
	//PLACEHOLDER 13

	//添加
	void __cdecl Export_AddExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind)
	{
		AddExecutor(GenExec, Param, DirectBind);
	}
	bool __cdecl Export_HasExecutor(const ContextIndex& Idx)
	{
		return HasExecutor(Idx);
	}
	void __cdecl Export_RemoveExecutor(const ContextIndex& Idx)
	{
		RemoveExecutor(Idx);
	}
	//PLACEHOLDER 15
	RoutineParam* __cdecl Export_GetRoutineParam(RoutineHandle Routine)
	{
		return GetRoutineParam(Routine);
	}
	void __cdecl Export_ResetGetFunctionBuffer()
	{
		ResetGetFunctionBuffer();
	}
	bool __cdecl Export_RegisterRoutineSet(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam, CreateMode Mode)
	{
		return RegisterRoutineSet(Name, Routine, Param, Paused, InitialDelay, ResetParam, Mode);
	}
	void* __cdecl Export_IHCore_Malloc(size_t Size)
	{
		return malloc(Size);
	}
	BasicLibData* __cdecl Export_GetLib(const char* Name)
	{
		return GetLib(Name);
	}
	BasicLibData* __cdecl Export_GetAvailableLib(const char* Name)
	{
		return GetAvailableLib(Name);
	}
	FuncInfo* __cdecl Export_QueryFunction(BasicLibData* Lib, const char* Name, int Version)
	{
		return QueryFunction(Lib, Name, Version);
	}
	bool __cdecl Export_RegisterIHFileStream(const char* StreamName, VClass vtbl)
	{
		return RegisterIHFileStream(StreamName, vtbl);
	}

	void __cdecl Export_IHCore_Free(void* p)
	{
		IHCore_Free(p);
	}
	void __cdecl Export_DbgFunc_ReturnString(UTF8_CString Str)
	{
		DbgFunc_ReturnString(Str);
	}
	void __cdecl Export_DbgFunc_ReturnError(UTF8_CString Str, int Code)
	{
		DbgFunc_ReturnError(Str, Code);
	}
	void __cdecl Export_DbgFunc_DoNotEcho()
	{
		DbgFunc_DoNotEcho();
	}
	void __cdecl Export_DbgFunc_ReturnStdError(long Code)
	{
		DbgFunc_ReturnStdError(Code);
	}
	void __cdecl Export_DbgFunc_SetGlobalVar(UTF8_CString Key, UTF8_CString Value)
	{
		DbgFunc_SetGlobalVar(Key, Value);
	}
	void __cdecl Export_DbgFunc_SetErrorCode(int Code)
	{
		DbgFunc_SetErrorCode(Code);
	}
	int __cdecl Export_DbgFunc_GetErrorCode()
	{
		return DbgFunc_GetErrorCode();
	}
	void __cdecl Export_DbgFunc_GetLastResult(UTF8_CString& Ret, UTF8_CString& ErrorStr, int& ErrorCode)
	{
		DbgFunc_GetLastResult(Ret, ErrorStr, ErrorCode);
	}
	UTF8_CString __cdecl Export_DbgFunc_GetVar(UTF8_CString Key)
	{
		return DbgFunc_GetVar(Key);
	}

	UTF8_CString __cdecl Export_GetTextDrawValue(UTF8_CString Key)
	{
		return ::GetTextDrawVariable(Key);
	}

	void __cdecl Export_RemoteReturnInfo_Destroy(RemoteReturnInfo* Info)
	{
		RemoteReturnInfo_Destroy(Info);
	}

	UTF8_CString __cdecl Export_RemoteReturnInfo_GetErrorMessage(const RemoteReturnInfo* Info)
	{
		return RemoteReturnInfo_GetErrorMessage(Info);
	}

	bool __cdecl Export_RemoteReturnInfo_Succeeded(const RemoteReturnInfo* Info)
	{
		return RemoteReturnInfo_Succeeded(Info);
	}

	JsonObject __cdecl Export_RemoteReturnInfo_GetResponseData(const RemoteReturnInfo* Info)
	{
		return RemoteReturnInfo_GetResponseData(Info);
	}

	bool __cdecl Export_PostAsyncRemoteCall(const char* pLib, const char* pFunc, int Version, JsonObject Context)
	{
		return PostAsyncRemoteCall(pLib, pFunc, Version, Context);
	}

	LibInputFnTable IHCoreFnTable
	{
		LibInputFnTable::GClassVersion,
		sizeof(LibInputFnTable),

		Export_GetFuncFromLib,
		Export_GetFuncByName,
		Export_GetVersion,
		nullptr,
		Export_RegisterContextProcessor,

		nullptr,
		nullptr,
		Export_RegisterDirectBinder,
		Export_RegisterRoutine,
		Export_RegisterRoutineSet,

		Export_GeneralCall,
		Export_DirectBindContextTo,
		Export_DirectBindTextTo,
		nullptr,
		nullptr,

		nullptr,
		Export_GetContextByIdx,
		Export_DeleteContextByIdx,
		Export_GetRoutine,
		Export_InstantRunRoutine,

		Export_PauseRoutine,
		Export_ResumeRoutine,
		Export_DelayRoutine,
		Export_DeleteRoutine,
		Export_MakeExecutor_BaseText,

		Export_MakeExecutor_Base,
		nullptr,
		nullptr,
		Export_MakeExecutor_Trigger,
		Export_MakeExecutor_InfiniteTrigger,

		Export_MakeExecutor_Once,
		Export_MakeExecutor_InfiniteLoop,
		Export_MakeExecutor_WhileLoop,
		Export_MakeExecutor_NLoop,
		nullptr,

		nullptr,
		Export_MakeExecutorEx_Trigger,
		Export_MakeExecutorEx_InfiniteTrigger,
		Export_MakeExecutorEx_Once,
		Export_MakeExecutorEx_InfiniteLoop,

		Export_MakeExecutorEx_WhileLoop,
		Export_MakeExecutorEx_NLoop,
		nullptr,
		nullptr,
		Export_AddExecutor,

		Export_HasExecutor,
		Export_RemoveExecutor,
		nullptr,
		Export_GetRoutineParam,
		Export_ResetGetFunctionBuffer,

		Export_GetTextDrawValue,
		Export_IHCore_Malloc,
		Export_GetLib,
		Export_GetAvailableLib,
		Export_QueryFunction,

		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,

		(CSFClass_ITable*)& CSFFile_InstTable,
		nullptr,
		nullptr,
		nullptr,
		nullptr,

		Export_IHCore_Free,
		Export_DbgFunc_ReturnString,
		Export_DbgFunc_ReturnError,
		Export_DbgFunc_DoNotEcho,
		Export_DbgFunc_ReturnStdError,

		Export_DbgFunc_SetGlobalVar,
		Export_DbgFunc_SetErrorCode,
		Export_DbgFunc_GetErrorCode,
		Export_DbgFunc_GetLastResult,
		Export_DbgFunc_GetVar,

		Export_RemoteReturnInfo_Destroy,
		Export_RemoteReturnInfo_GetErrorMessage,
		Export_RemoteReturnInfo_Succeeded,
		Export_RemoteReturnInfo_GetResponseData,
		Export_PostAsyncRemoteCall
	};
}