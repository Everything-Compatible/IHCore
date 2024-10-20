#pragma once
#include <vector>
#include <string>
#include <type_traits>
#include <functional>
#include "IHInitialLoad.h"
#include "ExtJson.h"
#include <YRPP.h>


#ifndef PARRAY_DEFINITION
#define PARRAY_DEFINITION
template<typename T>
struct PArray
{
	size_t N;
	const T* Data;

	PArray() :N(0), Data(nullptr) {}
	PArray(const std::vector<T>& p) :N(p.size()), Data(p.data()) {}
	template<size_t _N>
	PArray(const T p[_N]) : N(_N), Data(p) {}
};
#endif

using LibFuncHandle = void*;
using CSFHandle = void*;
struct LibInputFnTable;
struct CSFClass_ITable;

template<typename T>
T* AsType(LibFuncHandle Handle)
{
	return (T*)Handle;
}

struct GeneratorParam
{
	int ExecutorVersion;
	void* Ptr;
	AbstractType Type;
	int Reserved;
};

struct RoutineParam
{
	char Unknown[16];
};
const RoutineParam NullRoutineParam{};

enum class FuncType
{
	Default = 0,//idk type
	Condition = 1,//bool (__cdecl *)(JsonObject Context)
	Action = 2,//void (__cdecl *)(JsonObject Context)
	Callback = 3,//void (__cdecl *)(JsonObject Context)
	Procedure = 4,//void (__cdecl *)(void)
	Comm = 5,//void* (__cdecl *)(void*)
	ConditionAlt = 6,//bool (__cdecl *)(GeneratorParam Param)
	ActionAlt = 7,//void (__cdecl *)(GeneratorParam Param)
	CommAlt = 8//RoutineParam (__cdecl*)(RoutineParam)
};
using FuncType_Condition = bool(__cdecl*)(JsonObject Context);
using FuncType_Action = void(__cdecl*)(JsonObject Context);
using FuncType_Callback = void(__cdecl*)(JsonObject Context);
using FuncType_Procedure = void(__cdecl*)(void);
using FuncType_Comm = void*(__cdecl*)(void*);
using FuncType_ConditionAlt = bool(__cdecl*)(GeneratorParam Param);
using FuncType_ActionAlt = void(__cdecl*)(GeneratorParam Param);
using FuncType_CommAlt = RoutineParam(__cdecl*)(RoutineParam);



//For Executor&Routine
enum class CreateMode
{
	ProcReplace = 0,  //替换Proc 替换Context/Param 会触发Destructor、Uninit等
	ProcMerge = 1,    //替换Proc 合并Context 更新重复项 对Routine同Replace 不会触发Destructor、Uninit等
	ProcReserve = 2,  //替换Proc 保留Context/Param 不会触发Destructor、Uninit等
	CtxReplace = 4,   //保留Proc 替换Context/Param 会触发Destructor、Uninit等
	CtxMerge = 5,     //保留Proc 合并Context 更新重复项 对Routine同Replace 不会触发Destructor、Uninit等
	CtxReserve = 6,   //保留Proc 保留Context/Param 不会触发Destructor、Uninit等
};

struct FuncInfo
{
	static const int GClassVersion{ 1 };
	int ClassVersion{ GClassVersion };

	LibFuncHandle Func;
	FuncType Type;
	FuncInfo(LibFuncHandle F, FuncType T) :ClassVersion(GClassVersion), Func(F), Type(T) {}
	FuncInfo() = default;
	template<typename T>
	T* SeeAsType()
	{
		return (T*)Func;
	}
};


struct GeneralExecutor;
using SwizzleExecutor_t = void(__cdecl*)(GeneralExecutor* Exec, const GeneratorParam* Param, JsonObject CurrentContext);

struct ExecutorBase
{
	static const int GClassVersion{ 1 };
	int ClassVersion{ GClassVersion };
	
	SwizzleExecutor_t Swizzle;
	JsonObject Context;
	const char* Type;
	const char* ExecType;
	FuncInfo* Action;
	FuncInfo* Destructor;
	int Delay;
};
static_assert(sizeof(ExecutorBase) == 32);

struct GeneralExecutor
{
	enum class type
	{
		Trigger=0,
		Once=1,
		InfiniteLoop=2,
		WhileLoop=3,
		NLoop=4,
		InfiniteTrigger=5,
	}Type;
	
	ExecutorBase Base;
	FuncInfo* Condition;
	int Interval;

	int Reserved[5];
};
static_assert(sizeof(GeneralExecutor) == 64);

struct ContextIndex
{
	const char* Type;
	const char* ExecType;
	GeneratorParam Param;
};

struct LibVersionInfo
{
	static const int GClassVersion{ 1 };
	int ClassVersion{ GClassVersion };

	const char* LibName;
	int Version;
	int LowestSupportedVersion;
	const char8_t* Description;//UTF8
	//For C++ Version Lower than C++20 ,replace it with const char*

	int Reserved[3];
};
static_assert(sizeof(LibVersionInfo) == 32);

enum class GenCallRetType
{
	False = 0,
	True = 1,
	Void = 2,
	Default = -1
};

using GetFunc_t = FuncInfo*(__cdecl*)(const char*, int);
using ContextFunc_t = bool(__cdecl*)(JsonObject Context);
using BindingGenerator_t = bool(__cdecl*)(JsonObject Binding,const GeneratorParam& Param);
using Binder_t = bool(__cdecl*)(JsonObject Binding, JsonObject Context);
using DirectBinder_t = bool(__cdecl*)(JsonObject Context, const GeneratorParam& Param);
using Routine_t = int(__cdecl*)(RoutineParam*);
using RoutineHandle = void*;

const int ExitThisRoutine = INT_MAX;
const int PauseThisRoutine = INT_MAX - 1;
const int DoNotCheckVersion = INT_MAX;

struct InitDependency
{
	const char* LibName;
	int Version;
	int LowestRequiredVersion;
	bool RequiredLoadSequence;
	int Reserved[4];
};
static_assert(sizeof(InitDependency) == 32);

struct InitResult
{
	static const int GClassVersion{ 1 };
	int ClassVersion{ GClassVersion };

	LibVersionInfo* Info;
	GetFunc_t GetFunc;
	PArray<InitDependency> Dependencies;
	FuncType_Procedure OrderedInit;

	int Reserved[2];
};
static_assert(sizeof(InitResult) == 32);

struct RoutineSet
{
	Routine_t Init;
	Routine_t Loop;
	Routine_t Uninit;
	Routine_t Exit;
};

struct CSFEntry
{
	const wchar_t* Value;
	const char* Extra;//无extra传空串
};


struct InitInput
{
	static const int GClassVersion{ 1 };
	const int ClassVersion{ GClassVersion };
	LibInputFnTable* FunctionTable;

	int Reserved[6];
};
static_assert(sizeof(InitInput) == 32);



using InitFunc_t = InitResult * (__cdecl*)(InitInput*);

namespace Init
{
	bool Initialize();
	bool __cdecl RegisterEntry(LibFuncHandle Entry);
	PArray<IHInitialLoadService> __cdecl QueryServiceRequest(const char* Name);
	
	extern InitInput* LibInput;
	extern InitResult Result;
}

bool IHAvailable();

struct BasicLibData
{
	bool Available;
	InitInput* In;
	InitResult* Out;
	void* ReservedA;
	void* ReservedB;
	int Reserved[3];
};

//在Init之后才可用的东西
namespace Ext
{
	inline LibInputFnTable& FnTable()
	{
		return *Init::LibInput->FunctionTable;
	}

	CSFClass_ITable& CSFClass_GetITable();

	class CallInterface
	{
	private:
		std::string FunctionName;
		PArray<FuncInfo*> Funcs;
	public:
		CallInterface() = delete;
		CallInterface(const CallInterface&) = delete;
		CallInterface(CallInterface&&) = delete;
		CallInterface(const std::string& FuncName);
		CallInterface(const char* FuncName);

		void Reload(const std::string& FuncName);
		void Reload(const char* FuncName);
		void Call(JsonObject Context);
		PArray<FuncInfo*> Get();
	};

	class DispatchInterface
	{
	private:
		std::string Lib, Proc;
		int Version;
		FuncInfo* Info;
	public:
		DispatchInterface() = delete;
		DispatchInterface(const DispatchInterface&) = delete;
		DispatchInterface(DispatchInterface&&) = delete;
		DispatchInterface(const std::string& Lib, const std::string& Proc, int Version);
		void Reset(const std::string& Lib, const std::string& Proc, int Version);
		void Reset();
		FuncInfo* GetInfo();
		LibFuncHandle GetFunc();
		template<typename T>
		T* Func()
		{
			return (T*)GetFunc();
		}
		template<typename Ret, class... TArgs>
		Ret Call(TArgs&&... Args)
		{
			return ((Ret(*)(TArgs))GetFunc())(std::forward(Args));
		}
	};

	//假如你不想用Executor
	class ActiveContext
	{
		ContextIndex Idx;
		JsonObject CurContext;
	public:
		inline ContextIndex& Index() { return Idx; }
		inline JsonObject& Context() { return CurContext; }

		ActiveContext() = delete;
		ActiveContext(const ActiveContext&) = delete;
		ActiveContext(ActiveContext&&);
		ActiveContext(noinit_t);
		ActiveContext(const ContextIndex&);
		void Init(const ContextIndex& Idx);
		~ActiveContext();

		void Bind(JsonObject Context);
		void Bind(const char* Text);
		std::pair<ContextIndex, JsonObject> Release();
	};

	class ActiveRoutine
	{
		std::string _Name;
		RoutineHandle Handle;
		Routine_t _Routine;
	public:
		ActiveRoutine() = delete;
		ActiveRoutine(const ActiveRoutine&) = delete;
		ActiveRoutine(ActiveRoutine&&);
		ActiveRoutine(const char*, Routine_t,const RoutineParam&);
		ActiveRoutine(const std::string&, Routine_t, const RoutineParam&);
		ActiveRoutine(noinit_t);
		void Init(const char*, Routine_t, const RoutineParam&);
		void Init(const std::string&, Routine_t, const RoutineParam&);
		void Destroy();
		~ActiveRoutine();

		inline RoutineHandle GetHandle() { return Handle; }

		bool Available() const;
		void InstantRun() const;//立刻继续（若暂停）并清零等待时间
		void Pause() const;
		void Resume() const;
		void Delay(int NFrames) const;
		RoutineParam* GetParam() const;
	};

	class ActiveExecutor
	{
		GeneralExecutor Exec;
		ContextIndex Idx;
		bool Running;
	public:
		ActiveExecutor() = delete;
		ActiveExecutor(const ActiveExecutor&) = delete;
		ActiveExecutor(ActiveExecutor&&) = delete;
		ActiveExecutor(noinit_t);
		ActiveExecutor(const GeneralExecutor&);
		void Init(const GeneralExecutor&);
		void Destroy();
		~ActiveExecutor();
		
		void Start(const GeneratorParam& Param, bool DirectBind = true);
		inline const ContextIndex& Index() { return Idx; }
		JsonObject GetContext();
		bool IsRunning();
	};

	class LibData
	{
		BasicLibData* Lib;
	public:
		LibData(BasicLibData*);
		LibData() = delete;

		bool Available();
		FuncInfo* QueryFunction(const char* Name, int Version);
		int Version();
		const char* LibName();
		LibInputFnTable* LibFnTable();
		LibFuncHandle GetTableFn(int FuncIdx);//似乎可以在低版本SDK访问高版本IH的接口
		void SetTableFn(int FuncIdx, LibFuncHandle Fn);
		LibFuncHandle GetTableCustormFn(int FuncIdx);
		void SetTableCustomFn(int FuncIdx, LibFuncHandle Fn);

		inline BasicLibData* GetRaw() { return Lib; }
		inline void* GetBack() { return Lib ? Lib->ReservedA : nullptr; }
	};

	class CSFClass
	{
		CSFHandle Handle;
	public:
		uint32_t Version, LabelCount, StringCount, Reserved;
		CSFLanguages Language;

		CSFClass();
		~CSFClass();
		CSFClass(CSFClass&&);
		CSFClass(const CSFClass&);
		void Assign(const CSFClass&);
		inline CSFClass& operator=(const CSFClass& rhs)
		{
			Assign(rhs);
			return *this;
		}

		bool LoadAsCCFile(const char* name);
		bool LoadAsCCFile(CCFileClass* pCC);
		bool LoadAsExternalFile(const char* name);
		CSFEntry GetString(const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
		CSFEntry GetStringDefault(const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
		void Merge(const CSFClass* Another);
		void Merge(const char* Key, const wchar_t* value, const char* Extra);
		bool Available();
		void Clear();
	};


	FuncInfo* GetFuncFromLib(const char* pLib, const char* pFunc, int Version);
	PArray<FuncInfo*> GetFuncByName(const char* pFunc);
	void RegisterContextProcessor(const char* Type, ContextFunc_t pProcessor);
	void RegisterDirectBinder(const char* Type, const char* BindType, DirectBinder_t pDirectBinder);
	bool RegisterRoutine(const char* Name, Routine_t Routine, RoutineParam Param);//false 0 true CtxReserve

	GenCallRetType GeneralCall(const FuncInfo& Fn, JsonObject Context = NullJsonObject);
	JsonObject DirectBindContextTo(JsonObject Context, const ContextIndex& Idx);
	JsonObject DirectBindTextTo(const char* Text, const ContextIndex& Idx);
	JsonObject GetContextByIdx(const ContextIndex& Idx);
	void DeleteContextByIdx(const ContextIndex& Idx);

	RoutineHandle GetRoutine(const char* Name);
	void InstantRunRoutine(RoutineHandle Routine);
	void PauseRoutine(RoutineHandle Routine);
	void ResumeRoutine(RoutineHandle Routine);
	void DelayRoutine(RoutineHandle Routine, int Delay);

	void DeleteRoutine(const char* Name);
	bool MakeExecutor_BaseText(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Text, const char* Type, const char* ExecType, SwizzleExecutor_t Swizzle, FuncInfo* Destructor);
	bool MakeExecutor_Base(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Type, const char* ExecType, JsonObject Context, FuncInfo* Destructor);
	bool MakeExecutor_Trigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool MakeExecutor_InfiniteTrigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval);

	bool MakeExecutor_Once(GeneralExecutor& Target);
	bool MakeExecutor_InfiniteLoop(GeneralExecutor& Target, int Interval);
	bool MakeExecutor_WhileLoop(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool MakeExecutor_NLoop(GeneralExecutor& Target, int N, int Interval);
	bool MakeExecutorEx_Trigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	
	bool MakeExecutorEx_InfiniteTrigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool MakeExecutorEx_Once(GeneralExecutor& Target, const ExecutorBase& Base);
	bool MakeExecutorEx_InfiniteLoop(GeneralExecutor& Target, const ExecutorBase& Base, int Interval);
	bool MakeExecutorEx_WhileLoop(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool MakeExecutorEx_NLoop(GeneralExecutor& Target, const ExecutorBase& Base, int N, int Interval);
	
	void AddExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind = true);
	bool HasExecutor(const ContextIndex& Idx);
	void RemoveExecutor(const ContextIndex& Idx);
	RoutineParam* GetRoutineParam(RoutineHandle Routine);
	void ResetGetFunctionBuffer();

	bool RegisterRoutineSet(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam, CreateMode Mode);
	LibData GetLib(const char* Name);
	LibData GetAvailableLib(const char* Name);
	FuncInfo* QueryFunction(BasicLibData* Lib, const char* Name, int Version);//DoNotCheckVersion还是匹配当前版本 但大部分检查都不会工作 甚至不检查传出的是不是FuncInfo*
	void* CustomFunction(int FuncIdx);
}

struct CSFClass_ITable
{
	void(__cdecl* Constructor)(Ext::CSFClass* This);
	void(__cdecl* Destructor)(Ext::CSFClass* This);
	void(__cdecl* MoveAssign)(Ext::CSFClass* This, Ext::CSFClass* Another);
	void(__cdecl* Assign)(Ext::CSFClass* This, const Ext::CSFClass* Another);
	bool(__cdecl* LoadAsCCFile_A)(Ext::CSFClass* This, const char* name);
	bool(__cdecl* LoadAsCCFile_B)(Ext::CSFClass* This, CCFileClass* pCC);
	bool(__cdecl* LoadAsExternalFile)(Ext::CSFClass* This, const char* name);
	CSFEntry(__cdecl* GetString)(Ext::CSFClass* This, const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
	CSFEntry(__cdecl* GetStringDefault)(Ext::CSFClass* This, const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
	void(__cdecl* Merge_A)(Ext::CSFClass* This, const Ext::CSFClass* Another);
	void(__cdecl* Merge_B)(Ext::CSFClass* This, const char* Key, const wchar_t* value, const char* Extra);
	bool(__cdecl* Available)(Ext::CSFClass* This);
	void(__cdecl* Clear)(Ext::CSFClass* This);
};

struct LibInputFnTable
{
	static const int GClassVersion{ 1 };
	int ClassVersion{ GClassVersion };
	int Size{ sizeof(LibInputFnTable) };

	//FuncIdx 0~4
	FuncInfo* (__cdecl* GetFuncFromLib)(const char* pLib, const char* pFunc, int Version);
	PArray<FuncInfo*>(__cdecl* GetFuncByName)(const char* pFunc);
	void* PlaceHolder_1;
	void* PlaceHolder_2;
	void(__cdecl* RegisterContextProcessor)(const char* Type, ContextFunc_t pProcessor);

	//FuncIdx 5~9
	void* PlaceHolder_3;
	void* PlaceHolder_4;
	void(__cdecl* RegisterDirectBinder)(const char* Type, const char* BindType, DirectBinder_t pDirectBinder);
	bool(__cdecl* RegisterRoutine)(const char* Name, Routine_t Routine, RoutineParam Param);
	bool(__cdecl* RegisterRoutineSet)(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam, CreateMode Mode);

	//FuncIdx 10~14
	GenCallRetType(__cdecl* GeneralCall)(const FuncInfo& Fn, JsonObject Context);
	JsonObject(__cdecl* DirectBindContextTo)(JsonObject Context, const ContextIndex& Idx);
	JsonObject(__cdecl* DirectBindTextTo)(const char* Text, const ContextIndex& Idx);
	void* PlaceHolder_5;
	void* PlaceHolder_6;

	//FuncIdx 15~19
	void* PlaceHolder_7;
	JsonObject(__cdecl* GetContextByIdx)(const ContextIndex& Idx);
	void(__cdecl* DeleteContextByIdx)(const ContextIndex& Idx);
	RoutineHandle(__cdecl* GetRoutine)(const char* Name);
	void(__cdecl* InstantRunRoutine)(RoutineHandle Routine);

	//FuncIdx 20~24
	void(__cdecl* PauseRoutine)(RoutineHandle Routine);
	void(__cdecl* ResumeRoutine)(RoutineHandle Routine);
	void(__cdecl* DelayRoutine)(RoutineHandle Routine, int Delay);
	void(__cdecl* DeleteRoutine)(const char* Name);
	bool(__cdecl* MakeExecutor_BaseText)(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Text, const char* Type, const char* ExecType, SwizzleExecutor_t Swizzle, FuncInfo* Destructor);

	//FuncIdx 25~29
	bool(__cdecl* MakeExecutor_Base)(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Type, const char* ExecType, JsonObject Context, FuncInfo* Destructor);
	void* PlaceHolder_8;
	void* PlaceHolder_9;
	bool(__cdecl* MakeExecutor_Trigger)(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool(__cdecl* MakeExecutor_InfiniteTrigger)(GeneralExecutor& Target, FuncInfo* Condition, int Interval);

	//FuncIdx 30~34
	bool(__cdecl* MakeExecutor_Once)(GeneralExecutor& Target);
	bool(__cdecl* MakeExecutor_InfiniteLoop)(GeneralExecutor& Target, int Interval);
	bool(__cdecl* MakeExecutor_WhileLoop)(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool(__cdecl* MakeExecutor_NLoop)(GeneralExecutor& Target, int N, int Interval);
	void* PlaceHolder_10;

	//FuncIdx 35~39
	void* PlaceHolder_11;
	bool(__cdecl* MakeExecutorEx_Trigger)(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool(__cdecl* MakeExecutorEx_InfiniteTrigger)(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool(__cdecl* MakeExecutorEx_Once)(GeneralExecutor& Target, const ExecutorBase& Base);
	bool(__cdecl* MakeExecutorEx_InfiniteLoop)(GeneralExecutor& Target, const ExecutorBase& Base, int Interval);

	//FuncIdx 40~44
	bool(__cdecl* MakeExecutorEx_WhileLoop)(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool(__cdecl* MakeExecutorEx_NLoop)(GeneralExecutor& Target, const ExecutorBase& Base, int N, int Interval);
	void* PlaceHolder_12;
	void* PlaceHolder_13;
	void(__cdecl* AddExecutor)(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind);

	//FuncIdx 45~49
	bool(__cdecl* HasExecutor)(const ContextIndex& Idx);
	void(__cdecl* RemoveExecutor)(const ContextIndex& Idx);
	void* PlaceHolder_14;
	RoutineParam* (__cdecl* GetRoutineParam)(RoutineHandle Routine);
	void(__cdecl* ResetGetFunctionBuffer)();

	//FuncIdx 50~54
	void* PlaceHolder_15;
	void* PlaceHolder_16;
	BasicLibData* (__cdecl* GetLib)(const char* Name);
	BasicLibData* (__cdecl* GetAvailableLib)(const char* Name);
	FuncInfo* (__cdecl* QueryFunction)(BasicLibData* Lib, const char* Name, int Version);

	//FuncIdx 55~59
	void* Custom_1;
	void* Custom_2;
	void* Custom_3;
	void* Custom_4;
	void* Custom_5;

	CSFClass_ITable* CSFClass_pITable;
	void* PlaceHolder_17;
	void* PlaceHolder_18;
	void* PlaceHolder_19;
	void* PlaceHolder_20;
};



#define GENERATOR_PARAM(name) \
inline GeneratorParam GetGeneratorParam(name ## Class* pClass)\
	{ return GeneratorParam{ FuncInfo::GClassVersion,pClass,AbstractType:: name ,0 }; }

class CampaignClass;//Actually there is
class IsotileClass;
class IsotileTypeClass;
class SpecialClass;
class ActionClass;
class EventClass;//Actually there is
class TacticalMapClass;
class AITriggerClass;
class VeinholeMonsterClass;//Actually there is
class NavyTypeClass;

inline GeneratorParam GetGeneratorParam(void* pClass)
{ 
	return GeneratorParam{ FuncInfo::GClassVersion,pClass,AbstractType::None ,0 }; 
}
GENERATOR_PARAM(Unit)
GENERATOR_PARAM(Aircraft)
GENERATOR_PARAM(AircraftType)
GENERATOR_PARAM(Anim)
GENERATOR_PARAM(AnimType)
GENERATOR_PARAM(Building)
GENERATOR_PARAM(BuildingType)
GENERATOR_PARAM(Bullet)
GENERATOR_PARAM(BulletType)
GENERATOR_PARAM(Campaign)//
GENERATOR_PARAM(Cell)
GENERATOR_PARAM(Factory)
GENERATOR_PARAM(House)
GENERATOR_PARAM(HouseType)
GENERATOR_PARAM(Infantry)
GENERATOR_PARAM(InfantryType)
GENERATOR_PARAM(Isotile)//
GENERATOR_PARAM(IsotileType)//
GENERATOR_PARAM(BuildingLight)
GENERATOR_PARAM(Overlay)
GENERATOR_PARAM(OverlayType)
GENERATOR_PARAM(Particle)
GENERATOR_PARAM(ParticleType)
GENERATOR_PARAM(ParticleSystem)
GENERATOR_PARAM(ParticleSystemType)
GENERATOR_PARAM(Script)
GENERATOR_PARAM(ScriptType)
GENERATOR_PARAM(Side)
GENERATOR_PARAM(Smudge)
GENERATOR_PARAM(SmudgeType)
GENERATOR_PARAM(Special)//
GENERATOR_PARAM(SuperWeaponType)
GENERATOR_PARAM(TaskForce)
GENERATOR_PARAM(Team)
GENERATOR_PARAM(TeamType)
GENERATOR_PARAM(Terrain)
GENERATOR_PARAM(TerrainType)
GENERATOR_PARAM(Trigger)
GENERATOR_PARAM(TriggerType)
GENERATOR_PARAM(UnitType)
GENERATOR_PARAM(VoxelAnim)
GENERATOR_PARAM(VoxelAnimType)
GENERATOR_PARAM(Wave)
GENERATOR_PARAM(Tag)
GENERATOR_PARAM(TagType)
GENERATOR_PARAM(Tiberium)
GENERATOR_PARAM(Action)//
GENERATOR_PARAM(Event)//
GENERATOR_PARAM(WeaponType)
GENERATOR_PARAM(WarheadType)
GENERATOR_PARAM(Waypoint)
GENERATOR_PARAM(Abstract)
GENERATOR_PARAM(Tube)
GENERATOR_PARAM(LightSource)
GENERATOR_PARAM(EMPulse)
GENERATOR_PARAM(TacticalMap)//
GENERATOR_PARAM(Super)
GENERATOR_PARAM(AITrigger)//
GENERATOR_PARAM(AITriggerType)
GENERATOR_PARAM(Neuron)
GENERATOR_PARAM(FoggedObject)
GENERATOR_PARAM(AlphaShape)
GENERATOR_PARAM(VeinholeMonster)//
GENERATOR_PARAM(NavyType)//
GENERATOR_PARAM(SpawnManager)
GENERATOR_PARAM(CaptureManager)
GENERATOR_PARAM(Parasite)
GENERATOR_PARAM(Bomb)
GENERATOR_PARAM(RadSite)
GENERATOR_PARAM(Temporal)
GENERATOR_PARAM(Airstrike)
GENERATOR_PARAM(SlaveManager)
GENERATOR_PARAM(DiskLaser)


namespace InitialLoad
{
	template<typename ParamType, typename... TArgs>
	IHInitialLoadService CreateRequest(const char* Name, TArgs&&... args)//创建后永不销毁
	{
		static_assert(std::is_base_of<InitialLoadParam, ParamType>::value, "ParamType 必须派生自 InitialLoadParam ！");
		return IHInitialLoadService{ Name,new ParamType(std::forward<TArgs>(args)...) };
	}
	void ServiceRequest(IHInitialLoadService IService);
	template<typename ParamType, typename... TArgs>
	void CreateRequestAndSubmit(const char* Name, TArgs&&... args)//创建后永不销毁
	{
		static_assert(std::is_base_of<InitialLoadParam, ParamType>::value, "ParamType 必须派生自 InitialLoadParam ！");
		ServiceRequest(IHInitialLoadService{ Name,new ParamType(std::forward<TArgs>(args)...) });
	}
	template<typename ParamType>
	class Service
	{
		static_assert(std::is_base_of<InitialLoadParam, ParamType>::value, "ParamType 必须派生自 InitialLoadParam ！");
		std::string ServiceName;
		PArray<IHInitialLoadService> Requests;
		bool Requested{ false };
	public:
		Service() = delete;
		Service(const Service&) = delete;
		Service(Service&&) = delete;

		Service(const std::string& Name) : ServiceName(Name), Requests() {}
		Service(const char* Name) : ServiceName(Name), Requests() {}
		void ProcessOnce(const std::function<void(const ParamType&)>& Func)//如果列表为空则填补并处理
		{
			if (!Requested)
			{
				Refresh();
				for (size_t i = 0; i < Requests.N; i++)
					if (Requests.Data[i].Param)
						Func(*static_cast<const ParamType* const>(Requests.Data[i].Param));
				Requested = true;
			}
		}
		void Process(const std::function<void(const ParamType&)>& Func)//如果列表为空仍会尝试refresh
		{
			if (!Requests.N)Refresh();
			for (size_t i = 0; i < Requests.N; i++)
				if (Requests.Data[i].Param)
					Func(*static_cast<const ParamType* const>(Requests.Data[i].Param));
		}
		void Refresh()
		{
			Requests = Init::QueryServiceRequest(ServiceName.c_str());
		}
		void RefreshAndProcess(const std::function<void(const ParamType&)>& Func)
		{
			Refresh();
			Process(Func);
		}
	};
}
