#pragma once

#include <vector>
#include <unordered_map>
#include "ToolFunc.h"
#include <IH.h>
#include <YRPPCore.h>
#include <memory>
#include <variant>
#include <optional>


class IHException :public std::exception
{
	std::string Info;
public:
	IHException(const char* info);
	IHException(std::string&& info);
	const char* what() const noexcept override;
};
void IHLogAndThrow(const char* str);

namespace IHLibList
{
	bool IsAvailable();
	bool Initialize();//如果没有调用该函数，使用时会自动补上调用
	bool RegisterToLibList(LibFuncHandle Init);
	int GetMaxLibs();//-1 for error
	const PArray<LibFuncHandle> GetEntries();
};


FuncInfo* IHCore_GetFunc(const char* Name, int Version);
FuncInfo* Internal_GetFunc(const char* Name, int Version);
std::vector<std::string> GetInternalSupportedFunctions();
std::u8string FormatMessageU8(DWORD ErrorValue);
GenCallRetType ProcessSyringeRequest(const std::string& Method, JsonFile&& Arguments);
GenCallRetType ProcessSyringeRequestAlt(const std::string& Method, GeneratorParam Arg);

namespace Local
{
	extern PArray<LibFuncHandle> InitFn;
	struct LibType
	{
		InitInput In;
		InitResult* Out;
		bool Available;
		BasicLibData* Basic;
		LibInputFnTable Tbl;
		bool RemoteComponent{ false };

		struct DependencyProcessData
		{
			std::vector<std::string> ValidDep;
			int DFN;
			int LOW;
			bool PD;
			bool VIS;
		}Dep;
	};
	struct BinderType
	{
		Binder_t Func;
		const char* BindType;
	};
	struct ActiveExecutor
	{
		GeneralExecutor Exec;
		ContextIndex Idx;
		int NextRunTime;
		JsonObject CurrentContext;
		bool Finished;
		int Counter_NLoop;

		void Destroy();
		void Init(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind);
		ActiveExecutor() = delete;
		ActiveExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind);
		ActiveExecutor(noinit_t);
		~ActiveExecutor();

		bool ShouldDestroy() const;
		bool ShouldExecute() const;
		void Execute();
	private:
		void Init_Impl(const GeneratorParam& Param, bool DirectBind);
	};
	struct ActiveRoutine
	{
		RoutineSet Set;
		bool Paused;
		int NextRunTime;
		int PauseTime;
		bool ResetParamPerGame;
		RoutineParam Original;
		bool First;
		bool OrigPaused;
		int OrigInitialDelay;
		RoutineParam _Param;

		~ActiveRoutine();
		void Init(Routine_t, const RoutineParam& Param);
		ActiveRoutine(Routine_t, const RoutineParam& Param);
		ActiveRoutine(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam);
		void Uninit(bool ShouldDestroy);
		bool Execute();
		void InstantRun();
		bool ShouldRun();
		void Pause();
		void Resume();
		void Delay(int);
		void Reset();
		inline RoutineParam& Param() { return _Param; }
	};

	struct VClass
	{
		DWORD vptr;
		size_t Size;
	};

	extern std::vector<LibType> Libs;
	extern std::unordered_map<std::string, LibType*> LibMap;
	extern std::unordered_map<std::string, VClass> IHFileStreamer;
	extern std::unordered_map<std::string, std::string, UpperHash, UpperEqualPred> IHFileBinder;//name to stream
	extern std::unordered_map<std::string, LibFuncHandle, UpperHash, UpperEqualPred> IHFileFilter;
	extern std::unordered_map<std::string, std::vector<FuncInfo*>> NamedFunc;
	extern std::unordered_map<std::string, ContextFunc_t> ContextMap;
	extern std::unordered_map<std::string, BinderType> BinderMap;
	extern std::unordered_multimap<std::string, BindingGenerator_t> GeneratorMap;
	extern std::unordered_map<std::string, DirectBinder_t> DirectBinderMap;
	extern std::unordered_map<std::string, std::unique_ptr<ActiveRoutine>> RoutineMap;
	extern LibInputFnTable IHCoreFnTable;
	extern std::vector<std::string> InitOrder;


	void LocalInit();
	BasicLibData* GetLib(const char* Name);
	BasicLibData* GetAvailableLib(const char* Name);
	FuncInfo* QueryFunction(BasicLibData* Lib, const char* Name, int Version);


	//没有找到//找到且唯一//找到但不唯一
	std::variant<std::monostate, FuncInfo*, std::unordered_map<std::string, FuncInfo*>>
		PickFunctionForCommand(const char* pLib, const char* pFunc, int Version);


	//FuncInfo
	FuncInfo* GetFuncFromLib(const char* pLib, const char* pFunc, int Version);
	PArray<FuncInfo*> GetFuncByName(const char* pFunc);
	int GetVersion();
	int GetLSV();
	void RegisterContextProcessor(const char* Type, ContextFunc_t pProcessor);
	void RegisterBinder(const char* Type, const char* BindType, Binder_t pBinder);
	void RegisterGenerator(const char* BindType, BindingGenerator_t pGenerator);
	void RegisterDirectBinder(const char* Type, const char* BindType, DirectBinder_t pDirectBinder);
	bool RegisterRoutine(const char* Name, Routine_t Routine);


	void GenerateContext(JsonObject pContext);
	void GenerateBinder(JsonObject pContext, JsonObject pBinding, const char* Type);
	void GenerateBinding(JsonObject pBinding, const char* Type, const GeneratorParam& Param);
	void GenerateDirectBinder(JsonObject pContext, const char* BindType, const GeneratorParam& Param);

	//0,1 for bool other return 2; default return -1
	GenCallRetType GeneralCall(const FuncInfo& Fn, JsonObject Context);
	GenCallRetType GeneralCallAlt(const FuncInfo& Fn, const GeneratorParam& Param);

	JsonObject DirectBindContextTo(JsonObject Context, const ContextIndex& Idx);
	JsonObject DirectBindTextTo(const char* Text, const ContextIndex& Idx);
	JsonObject BindContextTo(JsonObject Context, const ContextIndex& Idx);
	JsonObject GetContextByIdx(const ContextIndex& Idx);
	void DeleteContextByIdx(const ContextIndex& Idx);
	RoutineHandle GetRoutine(const char* Name);
	void InstantRunRoutine(RoutineHandle Routine);
	void PauseRoutine(RoutineHandle Routine);
	void ResumeRoutine(RoutineHandle Routine);
	void DelayRoutine(RoutineHandle Routine,int Delay);
	void DeleteRoutine(const char* Name);

	//若没有Destructor，填补nullptr
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

	void AddExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind);
	ActiveExecutor* GetExecutor(const ContextIndex& Idx);
	void RemoveExecutor(const ContextIndex& Idx);
	void ExecutorRunLoop();
	RoutineParam* GetRoutineParam(RoutineHandle Routine);
	void ResetGetFunctionBuffer();
	bool RegisterIHFileStream(const char* StreamName, VClass vtbl);

	

	//FuncInfo
	FuncInfo* __cdecl Export_GetFuncFromLib(const char* pLib, const char* pFunc, int Version);
	PArray<FuncInfo*> __cdecl Export_GetFuncByName(const char* pFunc);
	int  __cdecl Export_GetVersion();
	//PLACEHOLDER 2

	//注册函数
	void __cdecl Export_RegisterContextProcessor(const char* Type, ContextFunc_t pProcessor);
		//void Export_RegisterBinder(const char* Type, const char* BindType, Binder_t pBinder);
		//void Export_RegisterGenerator(const char* BindType, BindingGenerator_t pGenerator);
	void __cdecl Export_RegisterDirectBinder(const char* Type, const char* BindType, DirectBinder_t pDirectBinder);
	bool __cdecl Export_RegisterRoutine(const char* Name, Routine_t Routine, RoutineParam Param);
	bool __cdecl Export_RegisterRoutineSet(const char* Name, const RoutineSet& Routine, RoutineParam Param, bool Paused, int InitialDelay, bool ResetParam, CreateMode Mode);
	//0,1 for bool other return 2; default return -1
	GenCallRetType __cdecl Export_GeneralCall(const FuncInfo& Fn, JsonObject Context);
	//Context
	JsonObject __cdecl Export_DirectBindContextTo(JsonObject Context, const ContextIndex& Idx);
	JsonObject __cdecl Export_DirectBindTextTo(const char* Text, const ContextIndex& Idx);
		//JsonObject Export_BindContextTo(JsonObject Context, const ContextIndex& Idx);
		//PLACEHOLDER 6
		//PLACEHOLDER 7
	JsonObject __cdecl Export_GetContextByIdx(const ContextIndex& Idx);
	void __cdecl Export_DeleteContextByIdx(const ContextIndex& Idx);
	RoutineHandle __cdecl Export_GetRoutine(const char* Name);
	void __cdecl Export_InstantRunRoutine(RoutineHandle Routine);
	void __cdecl Export_PauseRoutine(RoutineHandle Routine);
	void __cdecl Export_ResumeRoutine(RoutineHandle Routine);
	void __cdecl Export_DelayRoutine(RoutineHandle Routine, int Delay);
	void __cdecl Export_DeleteRoutine(const char* Name);

	//若没有Destructor，填补nullptr
	bool __cdecl Export_MakeExecutor_BaseText(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Text, const char* Type, const char* ExecType, SwizzleExecutor_t Swizzle, FuncInfo* Destructor);
	bool __cdecl Export_MakeExecutor_Base(ExecutorBase& Base, FuncInfo* Action, int Delay, const char* Type, const char* ExecType, JsonObject Context, FuncInfo* Destructor);
		//PLACEHOLDER 8
		//PLACEHOLDER 9
	//填充
	bool __cdecl Export_MakeExecutor_Trigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool __cdecl  Export_MakeExecutor_InfiniteTrigger(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool __cdecl Export_MakeExecutor_Once(GeneralExecutor& Target);
	bool __cdecl Export_MakeExecutor_InfiniteLoop(GeneralExecutor& Target, int Interval);
	bool __cdecl Export_MakeExecutor_WhileLoop(GeneralExecutor& Target, FuncInfo* Condition, int Interval);
	bool __cdecl Export_MakeExecutor_NLoop(GeneralExecutor& Target, int N, int Interval);
		//PLACEHOLDER 10
		//PLACEHOLDER 11
	//填充 EX
	bool __cdecl Export_MakeExecutorEx_Trigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool __cdecl Export_MakeExecutorEx_InfiniteTrigger(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool __cdecl Export_MakeExecutorEx_Once(GeneralExecutor& Target, const ExecutorBase& Base);
	bool __cdecl Export_MakeExecutorEx_InfiniteLoop(GeneralExecutor& Target, const ExecutorBase& Base, int Interval);
	bool __cdecl Export_MakeExecutorEx_WhileLoop(GeneralExecutor& Target, const ExecutorBase& Base, FuncInfo* Condition, int Interval);
	bool __cdecl Export_MakeExecutorEx_NLoop(GeneralExecutor& Target, const ExecutorBase& Base, int N, int Interval);
		//PLACEHOLDER 12
		//PLACEHOLDER 13
	//添加
	void __cdecl Export_AddExecutor(const GeneralExecutor& GenExec, const GeneratorParam& Param, bool DirectBind);
	bool __cdecl Export_HasExecutor(const ContextIndex& Idx);
	void __cdecl Export_RemoveExecutor(const ContextIndex& Idx);

	//PLACEHOLDER 14
	RoutineParam* __cdecl Export_GetRoutineParam(RoutineHandle Routine);
	void __cdecl Export_ResetGetFunctionBuffer();

	UTF8_CString __cdecl Export_GetTextDrawValue(UTF8_CString Key);
	void* __cdecl Export_IHCore_Malloc(size_t Size);
	BasicLibData* __cdecl Export_GetLib(const char* Name);
	BasicLibData* __cdecl Export_GetAvailableLib(const char* Name);
	FuncInfo* __cdecl Export_QueryFunction(BasicLibData* Lib, const char* Name, int Version);

	void __cdecl Export_IHCore_Free(void* p);
	void __cdecl Export_DbgFunc_ReturnString(UTF8_CString Str);
	void __cdecl Export_DbgFunc_ReturnError(UTF8_CString Str, int Code);
	void __cdecl Export_DbgFunc_DoNotEcho();
	void __cdecl Export_DbgFunc_ReturnStdError(long Code);

	void __cdecl Export_DbgFunc_SetGlobalVar(UTF8_CString Key, UTF8_CString Value);
	void __cdecl Export_DbgFunc_SetErrorCode(int Code);
	int __cdecl Export_DbgFunc_GetErrorCode();
	void __cdecl Export_DbgFunc_GetLastResult(UTF8_CString& Ret, UTF8_CString& ErrorStr, int& ErrorCode);
	UTF8_CString __cdecl Export_DbgFunc_GetVar(UTF8_CString Key);

	void __cdecl Export_RemoteReturnInfo_Destroy(RemoteReturnInfo* Info);
	UTF8_CString __cdecl Export_RemoteReturnInfo_GetErrorMessage(const RemoteReturnInfo* Info);
	bool __cdecl Export_RemoteReturnInfo_Succeeded(const RemoteReturnInfo* Info);
	JsonObject __cdecl Export_RemoteReturnInfo_GetResponseData(const RemoteReturnInfo* Info);
	bool __cdecl Export_PostAsyncRemoteCall(const char* pLib, const char* pFunc, int Version, JsonObject Context);
}