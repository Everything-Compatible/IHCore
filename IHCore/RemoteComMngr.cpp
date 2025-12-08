#include "ECInterprocess.h"
#include "ECDbgConsole.h"
#include <thread>
#include <shared_mutex>
#include <future>
#include <chrono>
#include "ToolFunc.h"
#include "Debug.h"
#include "Global.h"
#include "InfoStack.h"
#include "RemoteCommType.h"


/*
Output Parameters:
NOTE : Equivalent to InitResult
{
	"Version": <int>,
	"LowestSupportedVersion": <int>,
	"Description": <string>,
	"SupportedMethods": [ <string>, ... ],
	"Dependencies": [
		{
			"LibName": <string>,
			"Version": <int>,
			"LowestRequiredVersion": <int>,
			"RequiredLoadSequence": <bool>
		},
		...
	]
}
*/
bool InitRemoteComponentInfo::Load(JsonObject obj)
{
	auto oVer = obj.GetObjectItem("Version");
	auto oLSV = obj.GetObjectItem("LowestSupportedVersion");
	auto oDesc = obj.GetObjectItem("Description");
	auto oSMeth = obj.GetObjectItem("SupportedMethods");
	if (!oVer || !oLSV || !oDesc || !oSMeth) return false;
	if (!oVer.IsTypeNumber() || !oLSV.IsTypeNumber() || !oDesc.IsTypeString() || !oSMeth.IsTypeArray()) return false;

	Version = oVer.GetInt();
	LowestSupportedVersion = oLSV.GetInt();
	Description = ~oDesc.GetString();

	SupportedMethods.clear();
	auto Arr = oSMeth.GetArrayString();
	for (auto& s : Arr)SupportedMethods.insert(~s);

	auto oDeps = obj.GetObjectItem("Dependencies");
	Dependencies.clear();
	if (!oDeps) return true;//optional
	if (!oDeps.IsTypeArray()) return true;//optional

	auto DepArr = oDeps.GetArrayObject();
	for (auto& depObj : DepArr)
	{
		auto oLibName = depObj.GetObjectItem("LibName");
		auto oVer = depObj.GetObjectItem("Version");
		auto oLRV = depObj.GetObjectItem("LowestRequiredVersion");
		auto oRLS = depObj.GetObjectItem("RequiredLoadSequence");

		if (!oLibName || !oVer || !oLRV || !oRLS) continue;
		if (!oLibName.IsTypeString() || !oVer.IsTypeNumber() || !oLRV.IsTypeNumber() || !oRLS.IsTypeBool()) continue;

		InitRemoteComponentDependency Dep;
		Dep.LibName = ~oLibName.GetString();
		Dep.Version = oVer.GetInt();
		Dep.LowestRequiredVersion = oLRV.GetInt();
		Dep.RequiredLoadSequence = oRLS.GetBool();
		Dependencies.push_back(std::move(Dep));
	}

	return true;
}

template<typename RemoteInfo>
std::u8string RemoteCallInfoTraits<RemoteInfo>::GenerateText() const
{
	//Send : Source, Component, Method, Version, CallID, Context
	JsonObject Ctx = inst()->Context;
	auto CtxString = Ctx.GetText();
	return ~std::format(
R"({{
  "Source": "{}",
  "Component": "{}",
  "Method": "{}",
  "Version": {},
  "CallID": "{}",
  "Context": {}
}})"
		, ~inst()->Source, ~inst()->Component, ~inst()->Method, inst()->Version, inst()->GetCallID(), CtxString);
}

template<typename RemoteInfo>
std::u8string RemoteCallInfoTraits<RemoteInfo>::GenerateText(int64_t CallID) const
{
	//Send : Source, Component, Method, Version, CallID, Context
	JsonObject Ctx = inst()->Context;
	auto CtxString = Ctx.GetText();
	return ~std::format(
		R"({{
  "Source": "{}",
  "Component": "{}",
  "Method": "{}",
  "Version": {},
  "CallID": "{}",
  "Context": {}
}})"
, ~inst()->Source, ~inst()->Component, ~inst()->Method, inst()->Version, CallID, CtxString);
}

template<typename RemoteInfo>
std::u8string RemoteCallInfoTraits<RemoteInfo>::GenerateCompactText() const
{
	// Send : Source, Component, Method, Version, CallID, Context
		JsonObject Ctx = inst()->Context;
	auto CtxString = Ctx.GetCompactText();
	return ~std::format(
		R"({{"Source": "{}","Component": "{}","Method": "{}","Version": {},"CallID": "{}","Context": {}}})", 
		~inst()->Source, ~inst()->Component, ~inst()->Method, inst()->Version, inst()->CallID, CtxString);
}

int64_t RemoteCallSendInfo::GenerateCallID() const
{
	//NOTE : 32bits only

	//generate by hash & time
	auto hSrc = (int64_t)std::hash<std::u8string_view>()(Source);
	auto hCom = (int64_t)std::hash<std::u8string_view>()(Component);
	auto hMet = (int64_t)std::hash<std::u8string_view>()(Method);
	auto hVer = (int64_t)std::hash<int>()(Version);
	auto hCtx = (int64_t)std::hash<cJSON*>()(Context.GetRaw());
	auto hNow = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	int64_t hCall = hCom << 32 | (hMet & 0xFFFFFFFF);
	int64_t hArg = hVer << 32 | (hCtx & 0xFFFFFFFF);
	
	return hSrc ^ hCall ^ hArg ^ hNow;
}

std::optional<GeneratorParam> RemoteCallSendInfo::ToGeneratorParam() const
{
	if (!Context.IsTypeString()) return std::nullopt;
	auto Str = Context.GetString();
	return ECCommand::GetGeneratorParamFromStr(Str);
}

/*
Response Format :

{
	"ErrorMessage" : <string|null>
	"ResponseData" : <object>
}
*/
JsonFile RemoteCallReturnInfo::GenerateContext(RemoteCallReturnInfo&& Info)
{
	JsonFile J;
	auto Obj = J.GetObj();
	if (Info.ErrorMessage)
		Obj.AddString("ErrorMessage", ~(*Info.ErrorMessage));
	else
		Obj.AddNull("ErrorMessage");
	Obj.AddObjectItem("ResponseData", std::move(Info.ResponseData));
	return J;
}

/*
Response Format :

{
	"ErrorMessage" : <string|null>
	"ResponseData" : <object>
}
*/
std::optional<RemoteCallReturnInfo> RemoteCallRecvInfo::AsReturnInfo() const
{
	auto Ctx = Context.GetObj();
	auto Err = Ctx.GetObjectItem("ErrorMessage");
	auto Resp = Ctx.GetObjectItem("ResponseData");
	if (!Err || !Resp) return std::nullopt;
	if (!(Err.IsTypeString() || Err.IsTypeNull())) return std::nullopt;

	RemoteCallReturnInfo Ret;

	if (Err.IsTypeString())Ret.ErrorMessage = ~Err.GetString();
	else Ret.ErrorMessage = std::nullopt;

	Ret.ResponseData.DuplicateFromObject(Resp, true);

	return Ret;
}

RemoteCallSendInfo RemoteCallRecvInfo::Forward() const {
	return RemoteCallSendInfo{
		.Source = Source,
		.Component = Component,
		.Method = Method,
		.Version = Version,
		.Context = Context.GetObj()
	};
}

namespace RemoteComponentManager
{
	//DO NOT WRITE IT BETWEEN INITIALIZE & UNINITIALIZE
	std::unordered_map<std::u8string, std::unique_ptr<RemoteComponent>> RemoteComponents;
	//NOTE: 初始化期间加锁写，之后视为const变量无锁读
	std::mutex RemoteComponents_InitLock;
	bool IninitProcess{ false };

	std::unique_ptr<std::jthread> RemoteReceiverThread;
	std::unique_ptr<std::jthread> FrameInactiveMessageHandler;

	std::unordered_map<int64_t, std::promise<RemoteCallReturnInfo>> PendingRecvCalls;
	std::mutex PendingRecvCallsMutex;// multiple write & one read

	std::vector<std::future<void>> WaitInit;

	void WaitAllInitialized()
	{
		for (auto& f : WaitInit)
		{
			f.wait();
		}
		WaitInit.clear();
		IninitProcess = false;
	}

	bool Initialize(const InitInput& Input)
	{
		IninitProcess = true;
		auto Comps = ReadRemoteComponents();

		ServiceProcessComm::BeginCommSession();

		for (auto& c : Comps)
		{
			IPC_Log("[EC] RCM : Component Data - \n");
			IPC_Log("[EC]     RegName = %s\n", c.RegName.c_str());
			IPC_Log("[EC]     RunCommand = %s\n", c.RunCommand.c_str());
			IPC_Log("[EC]     Location = %s\n", c.Location.c_str());

			RegisterComponent(std::move(c), Input);
		}

		RemoteReceiverThread = std::make_unique<std::jthread>(RemoteReceiverThreadFunction);
		FrameInactiveMessageHandler = std::make_unique<std::jthread>(FrameInactiveMessageHandlerThreadFunction);

		return true;
	}

	void Uninitialize()
	{
		ServiceProcessComm::EndCommSession();

		if (RemoteReceiverThread)
		{
			RemoteReceiverThread->request_stop();
			RemoteReceiverThread->join();
			RemoteReceiverThread.reset();
		}

		if (FrameInactiveMessageHandler)
		{
			FrameInactiveMessageHandler->request_stop();
			FrameInactiveMessageHandler->join();
			FrameInactiveMessageHandler.reset();
		}

		void WSAGlobalUninit();
		WSAGlobalUninit();

		for (auto& [name, comp] : RemoteComponents)
		{
			((void)comp);
			UnregisterComponent(name);
		}
	}

	std::vector<InitResult*> GetRemoteComponentLibType()
	{
		std::vector<InitResult*> Res;
		for (auto& [Name, Comp] : RemoteComponents)
		{
			((void)Name);
			Res.push_back(&Comp->GetInitResult());
		}
		return Res;
	}

	/*
	-----------------------
	*/


	void RegisterComponent(RemoteComponentNameType&& Com, const InitInput& Input)
	{
		std::unique_ptr<RemoteComponent> Ptr;
		switch (Com.CommType)
		{
		case RemoteComponentNameType::Pipe:
		{
			IPC_Log("[EC] RCM : INIT Named Pipe Component %s\n", Com.RegName.c_str());
			Ptr.reset(new RemoteComponent_NamedPipe(std::move(Com)));
			break;
		}
		case RemoteComponentNameType::TCP:
		{
			IPC_Log("[EC] RCM : INIT TCP Component %s\n", Com.RegName.c_str());
			Ptr.reset(new RemoteComponent_TCP(std::move(Com)));
			break;
		}
		//Types are checked before here
		default:
			return;
		};

		WaitInit.push_back(std::async(std::launch::async, [rcp = std::move(Ptr), &Input] () mutable {

			IPC_Log("[EC] RCM : CONNECT Component %s\n", rcp->GetName().c_str());
			if (rcp->Connect())
			{
				IPC_Log("[EC] RCM : REGISTER Component %s\n", rcp->GetName().c_str());
				if (!rcp->Register(Input))
				{
					IPC_Log("[EC] RCM : Component %s Registration Failed! Disconnecting...\n", rcp->GetName().c_str());
					rcp->Disconnect();
					IPC_Log("[EC] RCM : Disconnected Component %s\n", rcp->GetName().c_str());
				}
			}

			if (rcp->IsConnected())
			{
				IPC_Log("[EC] RCM : Component %s Registered Successfully\n", rcp->GetName().c_str());
				std::lock_guard<std::mutex> lock(RemoteComponents_InitLock);
				RemoteComponents[rcp->GetName()] = std::move(rcp);
			}
				
		}));
	}

	RemoteComponent* GetComponentByName(const std::u8string& Name)
	{
		auto it = RemoteComponents.find(Name);
		if (it == RemoteComponents.end())return nullptr;
		else return it->second.get();
	}

	void UnregisterComponent(const std::u8string& Name)
	{
		auto it = RemoteComponents.find(Name);
		if (it != RemoteComponents.end())
		{
			IPC_Log("[EC] RCM : Unregister Remote Component %s\n", Name.c_str());
			it->second->Disconnect();
			RemoteComponents.erase(it);
		}
	}

	RemoteCallReturnInfo CallComponentMethod(const RemoteCallSendInfo& Info)
	{
		auto comp = GetComponentByName(std::u8string(Info.Component));

		if (!comp)
		{
			RemoteCallReturnInfo Ret;
			std::u8string str = u8"Component \"";
			str += Info.Component;
			str += u8"\" Not Found";
			Ret.ErrorMessage = std::move(str);
			return Ret;
		}
		if (!comp->IsConnected())
		{
			return ComponentDisconnected(Info);
		}

		return comp->CallMethod(Info);
	}

	bool PostComponentMethod(const RemoteCallSendInfo& Info)
	{
		auto comp = GetComponentByName(std::u8string(Info.Component));

		if (!comp)return false;
		if (!comp->IsConnected())return false;
		if (!comp->HasMethod(std::u8string(Info.Method)))return false;

		comp->PostMethod(Info);
		return true;
	}

	std::unordered_map<int64_t, FuncInfo> RemoteMethodInfoCache;

	int64_t GenerateInternalMethodName(const char* ComponentName, const char* MethodName, int Version)
	{
		auto h1 = (int64_t)std::hash<std::string>()(ComponentName);
		auto h2 = (int64_t)std::hash<std::string>()(MethodName);
		auto h3 = (int64_t)std::hash<int>()(Version);
		return (h1 << 32) | (h2 & 0xFFFFFFFF) ^ (h3 << 16);
	}

	FuncInfo* GetRemoteMethodInfo(const char* ComponentName, const char* MethodName, int Version)
	{
		auto key = GenerateInternalMethodName(ComponentName, MethodName, Version);

		IPC_Log(
			"[EC] RCM : GetRemoteMethodInfo : Component = %s, Method = %s, Version = %d, CacheKey = 0x%016llX\n",
			ComponentName, MethodName, Version, key
		);

		auto it = RemoteMethodInfoCache.find(key);
		if (it != RemoteMethodInfoCache.end())
			return &it->second;
		else
		{
			IPC_Log("[EC] RCM : Cache Miss. Generating...\n");
			auto& info = RemoteMethodInfoCache[key];
			info.Type = FuncType::Remote;
			info.Func = GetRemoteFunctionAddr(ComponentName, MethodName, Version);
			IPC_Log("[EC] RCM : Remote Function At 0x%08X\n", info.Func);
			return &info;
		}
	}

	bool IsAvailableRemoteComponent(const std::u8string& Name)
	{
		auto comp = GetComponentByName(Name);
		if (comp)
			return comp->IsConnected();
		else
			return false;
	}

	void OrderedInit(const std::u8string& ComponentName)
	{
		auto comp = GetComponentByName(ComponentName);
		if (comp)
			comp->OrderedInit();
	}


	/*
	-----------------------------------------------
	*/

	void RemoteReceiverProcess(RemoteComponent* comp)
	{
		if (comp->HasReceivedCall())
		{
			auto RecvInfo = comp->ReceiveCall();
			//(A)
			//可能是Source -> HOST 的请求
			//Target是HOST且不是响应
			//此时需要在本地执行请求，并向Source发送响应
			//(B)
			//可能是Target -> HOST 的响应
			//Target是HOST且是响应
			//此时需要将响应传递给等待的调用处，无需回复
			//(C)
			//可能是Source -> HOST -> Target 的转发 （此时不关心内容）
			//Target不是HOST

			auto& Source = comp->GetName();
			auto& Target = RecvInfo.Component;
			bool IsEmptyTarget = Target.empty();
			bool IsHost;
			bool TargetAvailable;
			bool IsResponse;

			if (IsEmptyTarget)
			{
				IsHost = true;
				TargetAvailable = true;
				//For Internal Functions, Set IsHost to true
			}
			else
			{
				auto it = Local::LibMap.find(~RecvInfo.Component);
				if (it == Local::LibMap.end())
				{
					IsHost = true;
					TargetAvailable = (RecvInfo.Component == u8"Syringe");
					//Target not exist or Target is Syringe
				}
				else
				{
					TargetAvailable = it->second->Available;
					IsHost = !it->second->RemoteComponent;
				}
			}

			if (!TargetAvailable)
			{
				//Error Handling
				auto targetComp = comp;
				auto ErrorStr{ ~std::format("ERROR Request from {} : Target {} unavailable.",
					~comp->GetName(), ~RecvInfo.Component) };
				//Debug::Log("[EC] RCM : %s\n", ErrorStr.c_str());
				//Debug::Log("[EC] Request : %s", RecvInfo.GenerateText().c_str());
				auto ErrorCtx = RemoteCallReturnInfo::GenerateContext(GenerateErrorRetInfo(ErrorStr));
				targetComp->SendCall(RemoteCallSendInfo{
					.Source = u8"",
					.Component = Source,
					.Method = RecvInfo.GetResponseMethodName(),
					.Version = RecvInfo.GetInternalMethodVersion(),
					.Context = ErrorCtx
					}, RecvInfo.CallID);
				return;
			}

			IPC_Log("[EC] RCM : Received Call : %s\n", RecvInfo.GenerateText().c_str());
			IPC_Log("[EC] RCM : IsEmptyTarget = %s, IsHost = %s, TargetAvailable = %s, IsResponse = %s\n",
				IsEmptyTarget ? "true" : "false",
				IsHost ? "true" : "false",
				TargetAvailable ? "true" : "false",
				RecvInfo.MethodIsResponse() ? "true" : "false"
			);

			IsResponse = RecvInfo.MethodIsResponse();
			if (!IsHost)
			{
				//(C)
				auto targetComp = GetComponentByName(Target);
				//assert targetComp is not null since TargetAvailable is true
				targetComp->SendCall(RecvInfo.Forward(), RecvInfo.CallID);
			}
			else if (IsResponse)
			{
				//(B)
				auto ReturnInfo = RecvInfo.AsReturnInfo();
				if (ReturnInfo)
				{
					auto& RetVal = ReturnInfo.value();

					auto CallID = RecvInfo.GetCallID();
					PendingRecvCallsMutex.lock();
					auto it = PendingRecvCalls.find(CallID);
					if (it != PendingRecvCalls.end())
					{
						it->second.set_value(std::move(RetVal));
						PendingRecvCalls.erase(it);
						PendingRecvCallsMutex.unlock();
					}
					else
					{
						PendingRecvCallsMutex.unlock();
						//Unexpected Response
						//Note : No response to source since it's a response
						auto ErrorStr{ ~std::format("ERROR Request from {} : Unexpected Call ID {}.",
							~comp->GetName(), RecvInfo.CallID) };
						//Debug::Log("[EC] RCM : %s\n", ErrorStr.c_str());
						//Debug::Log("[EC] Request : %s", RecvInfo.GenerateText().c_str());
					}

				}
				else
				{
					//Response Syntax Error
					//Note : No response to source since it's a response
					auto ErrorStr{ ~std::format("ERROR Request from {} : Response Syntax Error.",
						~comp->GetName()) };
					//Debug::Log("[EC] RCM : %s\n", ErrorStr.c_str());
					//Debug::Log("[EC] Request : %s", RecvInfo.GenerateText().c_str());
				}
			}
			else
			{
				//(A)
				PostAsyncLocalTask(std::move(RecvInfo));
			}
		}
	}

	void RemoteReceiverThreadFunction(std::stop_token stok)
	{
		while (!stok.stop_requested())
		{
			for (auto& [name, comp] : RemoteComponents)
			{
				((void)name);
				RemoteReceiverProcess(comp.get());
			}
			for (auto& Recv : FetchAsyncLocalResponses())
			{
				IPC_Log("[EC] RCM : Processing Local Async Response : \n%s\n", Recv.GenerateText().c_str());

				//(A)
				//可能是Source -> HOST 的请求
				//Target是HOST且不是响应
				//此时需要在本地执行请求，并向Source发送响应

				auto targetComp = GetComponentByName(Recv.Source);
				//assume targetComp != nullptr since it's local task response
				targetComp->SendCall(Recv.Forward(), Recv.CallID);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void FrameInactiveMessageHandlerThreadFunction(std::stop_token stok)
	{
		while (!stok.stop_requested())
		{
			if (IsLocalTaskAsyncHandling())
			{
				RemoteCommFrameUpdate();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			else std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}

	bool IsLocalTaskAsyncHandling()
	{
		return true;
		//return !Game::IsActive;
	}

	/*
	--------------------------------------------------------
	*/

	RemoteCallRecvInfo RespondLocalCall(const RemoteCallSendInfo& Info, int64_t CallID)
	{
		RemoteCallReturnInfo Ret;
		auto ReturnInfo = [Info, CallID, &Ret]() -> RemoteCallRecvInfo
			{
				if (Ret.ErrorMessage)
				{
					auto& ErrorStr = *Ret.ErrorMessage;
					//Debug::Log("[EC] RCM : %s\n", ErrorStr.c_str());
					//Debug::Log("[EC] Request : %s", Info.GenerateText(CallID).c_str());
				}
				RemoteCallRecvInfo Result;
				Result.Source = Info.Component;
				Result.Component = Info.Source;
				Result.Method = Info.GetResponseMethodName();
				Result.CallID = CallID;
				Result.Version = Info.GetInternalMethodVersion();
				Result.Context = Ret.GenerateContext(std::move(Ret));
				return Result;
			};

		bool IsSyringe = (Info.Component == u8"Syringe");

		if (IsSyringe)
		{
			JsonFile J;
			J.DuplicateFromObject(Info.Context, true);

			ECCommandRemoteBuf::Enter();
			auto retval = ProcessSyringeRequest(~std::u8string(Info.Method), std::move(J));
			Ret = FromECCommandRemote(retval);
			ECCommandRemoteBuf::Exit();

			return ReturnInfo();
		}

		auto pInfo = Local::GetFuncFromLib(conv Info.Component.data(), conv Info.Method.data(), Info.Version);
		if (!pInfo)
		{
			Ret = MethodNotSupported(Info);
			return ReturnInfo();
		}

		switch (pInfo->Type)
		{
		case FuncType::Remote:
			break;
		case FuncType::Condition:
		case FuncType::Action:
		case FuncType::Callback:
		case FuncType::Procedure:
		{
			ECCommandRemoteBuf::Enter();
			auto retval = Local::GeneralCall(*pInfo, Info.Context);
			Ret = FromECCommandRemote(retval);
			ECCommandRemoteBuf::Exit();
		}
			break;
		case FuncType::ConditionAlt:
		case FuncType::ActionAlt:
		{
			auto GenParam = Info.ToGeneratorParam();
			if (GenParam)
			{
				ECCommandRemoteBuf::Enter();
				auto retval = Local::GeneralCallAlt(*pInfo, *GenParam);
				Ret = FromECCommandRemote(retval);
				ECCommandRemoteBuf::Exit();
			}
			else Ret = CannotConvertToGenParam(Info);
		}
			break;
		case FuncType::Comm:
		case FuncType::CommAlt:
			Ret = CannotInvokeCommMethod(Info);
			break;
		case FuncType::Default:
		default:
			Ret = CannotInvokeDefaultMethod(Info);
			break;
		}
		return ReturnInfo();
	}

	RemoteCallReturnInfo GenerateErrorRetInfo(const std::u8string& ErrMsg)
	{
		RemoteCallReturnInfo Ret;
		Ret.ErrorMessage = ErrMsg;
		return Ret;
	}

	RemoteCallReturnInfo MethodNotSupported(const RemoteCallSendInfo& Info)
	{
		return GenerateErrorRetInfo(
			~std::format("ERROR Request from {} : Method \"{}\" version {} not supported in Component \"{}\".",
				~Info.Source, ~Info.Method, Info.Version, ~Info.Component)
		);
	}

	RemoteCallReturnInfo CannotInvokeDefaultMethod(const RemoteCallSendInfo& Info)
	{
		return GenerateErrorRetInfo(
			~std::format("ERROR Request from {} : Cannot invoke Method \"{}\" with Func Type Default in Component \"{}\".",
				~Info.Source, ~Info.Method, ~Info.Component)
		);
	}
	RemoteCallReturnInfo CannotInvokeCommMethod(const RemoteCallSendInfo& Info)
	{
		return GenerateErrorRetInfo(
			~std::format("ERROR Request from {} : Cannot invoke Method \"{}\" with Func Type Comm/CommAlt in Component \"{}\".",
				~Info.Source, ~Info.Method, ~Info.Component)
		);
	}
	RemoteCallReturnInfo CannotConvertToGenParam(const RemoteCallSendInfo& Info)
	{
		return GenerateErrorRetInfo(
			~std::format("ERROR Request from {} : Cannot convert Context to GeneratorParam for Method \"{}\" version {} in Component \"{}\".",
				~Info.Source, ~Info.Method, Info.Version, ~Info.Component)
		);
	}

	RemoteCallReturnInfo ComponentDisconnected(const RemoteCallSendInfo& Info)
	{
		RemoteCallReturnInfo Ret;
		std::u8string str = u8"Component \"";
		str += Info.Component;
		str += u8"\" Not Connected";
		Ret.ErrorMessage = std::move(str);
		return Ret;
	}

	RemoteCallReturnInfo FromECCommandRemote(GenCallRetType Ret)
	{
		RemoteCallReturnInfo info;
		if (!ECCommandRemoteBuf::ErrorStr.empty())
		{
			info.ErrorMessage = ECCommandRemoteBuf::ErrorStr;
		}
		else if (ECCommandRemoteBuf::ErrorCode != 0)
		{
			info.ErrorMessage = ~std::format("Error Code : {}", ECCommandRemoteBuf::ErrorCode);
		}
		else
		{
			info.ErrorMessage = std::nullopt;
		}

		auto Obj = info.ResponseData.GetObj();

		if (ECCommandRemoteBuf::ReturnedValue)
		{
			Obj.AddInt("ErrorCode", ECCommandRemoteBuf::ErrorCode);
			//try to parse returned value
			JsonFile RetFile;
			auto ErrorStr = RetFile.ParseChecked(~ECCommandRemoteBuf::Ret, "");
			if (ErrorStr.empty())
			{
				//parsed successfully
				Obj.AddObjectItem("Value", std::move(RetFile));
			}
			else
			{
				//parsing error, return as string
				Obj.AddString("Value", ~ECCommandRemoteBuf::Ret);
			}

		}
		else
		{
			Obj.AddInt("ErrorCode", ECCommandRemoteBuf::ErrorCode);
			switch (Ret)
			{
			case GenCallRetType::Default:
			case GenCallRetType::Void:
				Obj.AddNull("Value");
				break;
			case GenCallRetType::True:
				Obj.AddBool("Value", true);
				break;
			case GenCallRetType::False:
				Obj.AddBool("Value", false);
				break;
			}
		}

		return info;
	}

	/*
	Input Parameters:
	NOTE : Equivalent to InitInput
	{
		"FrameworkVersion": <int>,
		"InternalMethods": [ <string>, ... ]
	}
	*/
	JsonFile GenerateRegisterContext(const InitInput& In)
	{
		int FrameworkVersion = Local::GetVersion();
		std::vector<std::string> InternalMethods = GetInternalSupportedFunctions();
		JsonFile Ctx;
		auto Obj = Ctx.GetObj();
		Obj.AddInt("FrameworkVersion", FrameworkVersion);
		Obj.AddArrayString("InternalMethods", InternalMethods);
		return Ctx;
	}

	/*
	
	------------------------------------------
	
	*/

	InfoStack<RemoteCallRecvInfo> AsyncLocalRecv;
	InfoStack<RemoteCallRecvInfo> AsyncLocalPost;

	void PostAsyncLocalTask(RemoteCallRecvInfo&& Info)
	{
		AsyncLocalPost.Push(std::move(Info));
	}

	std::vector<RemoteCallRecvInfo> FetchAsyncLocalResponses()
	{
		return AsyncLocalRecv.Release();
	}

	void ProcessAsyncLocalTask()
	{
		for (auto&& Post : AsyncLocalPost.Release())
		{
			AsyncLocalRecv.Push(RespondLocalCall(Post.Forward(), Post.CallID));
		}
	}
}

void RemoteCommFrameUpdate()
{
	struct __IMPL
	{
		__IMPL() { ServiceProcessComm::IsCurrentThreadOnConsole = true; }
		~__IMPL() { ServiceProcessComm::IsCurrentThreadOnConsole = false; }
	}impl;

	ECDebug::FlushCommands();
	RemoteComponentManager::ProcessAsyncLocalTask();
}

RemoteCallReturnInfo RemoteComponent::CallMethod(const RemoteCallSendInfo& Info)
{
	ServiceProcessComm::CommTargetHalper(
		ServiceProcessComm::IsCurrentThreadOnConsole, 
		ServiceProcessComm::Target::Service, 
		RegName
	);

	auto CallID = SendCall(Info);

	IPC_Log("[EC] RCM : Sent Call : \n%s\n", Info.GenerateText(CallID));

	RemoteComponentManager::PendingRecvCallsMutex.lock();
	auto RecvInfo = RemoteComponentManager::PendingRecvCalls[CallID].get_future();
	RemoteComponentManager::PendingRecvCallsMutex.unlock();

	IPC_Log("[EC] RCM : Receiving Response for Call ID %lld\n", CallID);

	auto rinfo = RecvInfo.get();

	IPC_Log("[EC]      ErrorMessage : %s\n", rinfo.ErrorMessage ? (*rinfo.ErrorMessage).c_str() : u8"null");
	IPC_Log("[EC]      Rsaponse : %s\n", rinfo.ResponseData.GetObj().GetText().c_str());

	return rinfo;
}

void RemoteComponent::PostMethod(const RemoteCallSendInfo& Info)
{
	PostCall(Info);
}

bool RemoteComponent::Ping()
{
	if (!IsConnected())return false;

	JsonFile F;
	auto Info = RemoteCallSendInfo{
		.Source = u8"",
		.Component = RegName,
		.Method = RemoteCallInfoBase::GetPingMethodName(),
		.Version = RemoteCallInfoBase::GetInternalMethodVersion(),
		.Context = F.GetObj()
	};

	auto CallID = SendCall(Info);

	RemoteComponentManager::PendingRecvCallsMutex.lock();
	auto RecvInfo = RemoteComponentManager::PendingRecvCalls[CallID].get_future();
	RemoteComponentManager::PendingRecvCallsMutex.unlock();

	auto Result = RecvInfo.wait_for(std::chrono::milliseconds(RemoteComponentMaxPingMilliseconds));

	return (Result == std::future_status::ready);
}