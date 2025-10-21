#pragma once
#include <ExtJson.h>
#include <ToolFunc.h>
#include <IH.Loader.h>
#include <unordered_set>
#include <stop_token>
#include <optional>
#include "LocalData.h"
#include <concepts>
#include <Mutex>
#include "InfoStack.h"

/*

Dynamic Functions Created at Runtime

*/

struct CallerContext;
struct ReturnInfoPtr;
using RemoteCaller_t = ReturnInfoPtr (__cdecl*)(JsonObject Obj);
using RemotePackedCaller_t = ReturnInfoPtr(__cdecl*)(JsonObject Obj, CallerContext* Ctx);

struct CallerContext
{
	std::u8string Target;
	std::u8string  Method;
	int Version;
	mutable RemoteCaller_t Function;

	int Reserved[4];

	bool operator< (const CallerContext&) const;
};

ReturnInfoPtr __cdecl ExternalFunctionCaller(JsonObject Obj, CallerContext* Ctx);

RemoteCaller_t GetRemoteFunctionAddr(const char* Target, const char* Method, int Version);

/*

Remote Component Registration

*/

/*

Examples as JSON:

"Components" : [
	{
	  "Name": "calculator",
	  "Run": "C:/services/calculator.exe",
	  "CommType": "Pipe",
	  "Location": "\\\\.\\pipe\\calculator_service"
	},
	{
	  "Name": "file_processor",
	  "Run": "C:/services/file_processor.py",
	  "CommType": "TCP",
	  "Location": "127.0.0.1:8081"
	}
]

*/
struct RemoteComponentNameType
{
	enum {
		Unknown,
		Pipe,
		TCP
	} CommType;
	std::u8string RegName;
	std::u8string RunCommand;
	std::u8string Location;

	void Load(JsonObject obj);
};

namespace ReadRemoteComponents_ErrorContext
{
	extern std::u8string CurrentFileName;
}

/*

Read From :
Game Root Dir (Current Dir)
& 
Root Dir/Components

Target :
*.json

*/
std::vector<RemoteComponentNameType> ReadRemoteComponents();


/*

Remote Component Management

*/

struct InitRemoteComponentDependency
{
	std::u8string LibName;
	int Version;
	int LowestRequiredVersion;
	bool RequiredLoadSequence;
};

/*

About Init Special Call:
1. The first Call-Response pair
2. Method Name Hardcoded as "_Init"
3. 4 Special Methods are required for Remote Components:
   a. _Register : Registration
   b. _Ping : Ping
   c. _Init : Initialization
   d. _Response : Response Wrapper
   or the component is considered invalid

Input Parameters:
NOTE : Equivalent to InitInput
{
	"FrameworkVersion": <int>,
	"InternalMethods": [ <string>, ... ]
}

Output Parameters:
NOTE : Equivalent to InitResult
	   Dependencies are optional
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
struct InitRemoteComponentInfo
{
	int Version;
	int LowestSupportedVersion;
	std::u8string Description;
	std::unordered_set<std::u8string> SupportedMethods;
	std::vector<InitRemoteComponentDependency> Dependencies;

	bool Load(JsonObject obj);
};

struct RemoteCallInfoBase
{
	static inline int GetInternalMethodVersion() { return DoNotCheckVersion; }
	static inline const char8_t* GetRegisterMethodName() { return u8"_Register"; }
	static inline const char8_t* GetPingMethodName() { return u8"_Ping"; }
	static inline const char8_t* GetInitMethodName() { return u8"_Init"; }
	static inline const char8_t* GetResponseMethodName() { return u8"_Response"; }
};

template<typename RemoteInfo>
struct RemoteCallInfoTraits
{
	RemoteInfo* inst() { return reinterpret_cast<RemoteInfo*>(this); }
	bool MethodIsRegister() const { return inst()->Method == u8"_Register"; }
	bool MethodIsPing() const { return inst()->Method == u8"_Ping"; }
	bool MethodIsInit() const { return inst()->Method == u8"_Init"; }
	bool MethodIsResponse() const { return inst()->Method == u8"_Response"; }
	int GetInternalMethodVersion() const { return RemoteCallInfoBase::GetInternalMethodVersion(); }
	const char8_t* GetRegisterMethodName() const { return RemoteCallInfoBase::GetRegisterMethodName(); }
	const char8_t* GetPingMethodName() const { return RemoteCallInfoBase::GetPingMethodName(); }
	const char8_t* GetInitMethodName() const { return RemoteCallInfoBase::GetInitMethodName(); }
	const char8_t* GetResponseMethodName() const { return RemoteCallInfoBase::GetResponseMethodName(); }

	//Send : Source, Component, Method, Version, CallID, Context
	//R"({{"Source": "{}","Component": "{}","Method": "{}","Version": {},"CallID": "{}","Context": {}}})", 
	std::u8string GenerateText() const;
	std::u8string GenerateCompactText() const;
};

struct RemoteCallSendInfo : public RemoteCallInfoTraits<RemoteCallSendInfo> // note : no data ownership
{
	std::u8string_view Source;
	std::u8string_view Component;
	std::u8string_view Method;
	int Version;
	JsonObject Context;

	int64_t GenerateCallID() const;
	int64_t GetCallID() const { return GenerateCallID(); }
	/*
	Generator Param:
	Member : AbsType Address
	Usage : Token of Game Objects

	Format : 
	Console Style : #Type@HEX
	*/
	std::optional<GeneratorParam> ToGeneratorParam() const;
};

/*

Response Format :

{
	"ErrorMessage" : <string|null> 
	"ResponseData" : <object>
}

*/
struct RemoteCallReturnInfo
{
	std::optional<std::u8string> ErrorMessage;// if has value, means error occurred
	JsonFile ResponseData;

	static JsonFile GenerateContext(RemoteCallReturnInfo&& Info);
};

struct RemoteCallRecvInfo : public RemoteCallInfoTraits<RemoteCallRecvInfo> // note : has data ownership
{
	std::u8string Source;
	std::u8string Component;
	std::u8string Method;
	int Version;
	int64_t CallID;
	JsonFile Context;

	std::optional<RemoteCallReturnInfo> AsReturnInfo() const;
	int64_t GetCallID() const { return CallID; }
	RemoteCallSendInfo Forward() const;
};

struct ReturnInfoPtr
{
	std::unique_ptr<RemoteCallReturnInfo> Ptr;

	void Release()
	{
		Ptr.reset();
	}

	UTF8_CString GetErrorMessage() const
	{
		if (Ptr->ErrorMessage.has_value())
			return Ptr->ErrorMessage->c_str();
		else
			return nullptr;
	}

	bool Succeeded() const
	{
		return !Ptr->ErrorMessage.has_value();
	}

	JsonObject GetResponseData() const
	{
		return Ptr->ResponseData.GetObj();
	}
};

class RemoteComponent
{
protected:
	std::u8string RegName;
	std::u8string RunCommand;
	std::u8string Location;
	InitRemoteComponentInfo BasicInfo;
	std::vector<InitDependency> Register_Dependencies;//auto generated
	LibVersionInfo Register_VerInfo;//auto generated
	InitResult Register_InitRes;//auto generated
	bool Connected{ false };
public:
	RemoteComponent(RemoteComponentNameType&& Info)
		:RegName(std::move(Info.RegName)),
		RunCommand(std::move(Info.RunCommand)),
		Location(std::move(Info.Location))
	{
	}

	virtual ~RemoteComponent() = default;

	virtual bool Connect() = 0;
	virtual void Disconnect() = 0;

	//Send : Source, Component, Method, Version, CallID, Context
	virtual int64_t SendCall(const RemoteCallSendInfo& Info) = 0; // note : auto generate CallID & not check Component Argument here
	virtual int64_t SendCall(const RemoteCallSendInfo& Info, int64_t CallID) = 0;//note : forwarding use
	virtual void PostCall(const RemoteCallSendInfo& Info) = 0;//async call & discard return value
	virtual RemoteCallRecvInfo ReceiveCall() = 0;// note : fetch latest received call (blocking)
	virtual bool HasReceivedCall() = 0;

	bool HasMethod(const std::u8string& MethodName) const;
	RemoteCallReturnInfo CallMethod(const RemoteCallSendInfo& Info);
	void PostMethod(const RemoteCallSendInfo& Info);
	bool Register(const InitInput& Input);//generate InitResult
	void OrderedInit();
	bool IsConnected() const;
	bool Ping();
	const std::u8string& GetName() const;
	InitResult& GetInitResult();
};

namespace RemoteComponentManager
{
	bool Initialize(const InitInput& Input);

	void Uninitialize();

	//Note : result->GetFunc should be nullptr to fallback to Local::GetFuncFromLib
	std::vector<InitResult*> GetRemoteComponentLibType();

	void OrderedInit(const std::u8string& ComponentName);

	FuncInfo* GetRemoteMethodInfo(const char* ComponentName, const char* MethodName, int Version);

	void PostAsyncLocalTask(RemoteCallRecvInfo&& Info);

	std::vector<RemoteCallRecvInfo> FetchAsyncLocalResponses();

	void ProcessAsyncLocalTask();

	/*
	-----------------------
	*/

	void RemoteReceiverThreadFunction(std::stop_token stok);

	void FrameInactiveMessageHandlerThreadFunction(std::stop_token stok);

	void RegisterComponent(RemoteComponentNameType&& Com, const InitInput& Input);

	void UnregisterComponent(const std::u8string& Name);

	RemoteComponent* GetComponentByName(const std::u8string& Name);

	RemoteCallReturnInfo CallComponentMethod(const RemoteCallSendInfo& Info);

	bool PostComponentMethod(const RemoteCallSendInfo& Info);

	bool IsAvailableRemoteComponent(const std::u8string& Name);

	bool IsLocalTaskAsyncHandling();

	RemoteCallRecvInfo RespondLocalCall(const RemoteCallSendInfo& Info);

	/*
	--------------------------------------------------------
	*/
	RemoteCallReturnInfo GenerateErrorRetInfo(const std::u8string& ErrMsg);
	RemoteCallReturnInfo MethodNotSupported(const RemoteCallSendInfo& Info);
	RemoteCallReturnInfo CannotInvokeDefaultMethod(const RemoteCallSendInfo& Info);
	RemoteCallReturnInfo CannotInvokeCommMethod(const RemoteCallSendInfo& Info);
	RemoteCallReturnInfo CannotConvertToGenParam(const RemoteCallSendInfo& Info);
	RemoteCallReturnInfo FromECCommandRemote(GenCallRetType Ret);
	
	JsonFile GenerateRegisterContext(const InitInput& In);
}

void RemoteCommFrameUpdate();


struct RemoteComm_NamedPipe
{
	bool Connect(std::u8string_view PipeName, std::u8string_view);
	void Disconnect();

	std::u8string ReadString();
	void WriteString(std::u8string_view Str);
	bool HasStringToRead();
};

struct RemoteComm_TCP
{
	bool Connect(std::u8string_view PipeName, std::u8string_view);
	void Disconnect();

	std::u8string ReadString();
	void WriteString(std::u8string_view Str);
	bool HasStringToRead();
};