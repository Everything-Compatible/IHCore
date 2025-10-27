#pragma once
#include "RemoteCommBase.h"

template<typename T>
concept CommTypeFinal = RemoteCommType<T> && std::is_base_of_v<RemoteComponentBase<T>, T>;

template <CommTypeFinal T>
class RemoteComponent_CommType : public RemoteComponent
{
	T CommBase;
public:
	RemoteComponent_CommType(RemoteComponentNameType&& Info) :
		RemoteComponent(std::move(Info))
	{
	}

	virtual ~RemoteComponent_CommType()
	{
		IPC_Log("[EC] RemoteComponent_CommType::DTOR");
		Disconnect();
	}

	virtual bool Connect()
	{
		//assume _Register is supported
		BasicInfo.SupportedMethods.insert(RemoteCallInfoBase::GetRegisterMethodName());

		Connected = CommBase.Connect(RegName, RunCommand, Location);
		return Connected;
	}
	virtual void Disconnect()
	{
		Connected = false;
		CommBase.Disconnect();
	}

	bool CheckHasMethod(const RemoteCallSendInfo& Info, int64_t CallID);

	// note : auto generate CallID & not check Component Argument here
	virtual int64_t SendCall(const RemoteCallSendInfo& Info)
	{
		int64_t CallID = Info.GenerateCallID();
		return SendCall(Info, CallID);
	}
	virtual int64_t SendCall(const RemoteCallSendInfo& Info, int64_t CallID)
	{
		if(CheckHasMethod(Info, CallID))
			CommBase.SendCallUnchecked(Info, CallID, false);
		return CallID;
	}
	virtual void PostCall(const RemoteCallSendInfo& Info)
	{
		int64_t CallID = Info.GenerateCallID();
		if (CheckHasMethod(Info, CallID))
			CommBase.SendCallUnchecked(Info, CallID, true);
	}
	virtual RemoteCallRecvInfo ReceiveCall()
	{
		return CommBase.ReceiveCall();
	}
	virtual bool HasReceivedCall()
	{
		return CommBase.HasReceivedCall();
	}
};

template <CommTypeFinal T>
bool RemoteComponent_CommType<T>::CheckHasMethod(const RemoteCallSendInfo& Info, int64_t CallID)
{
	if (!CommBase.BaseConnected())
	{
		Connected = false;

		//generate Connection Closed response
		//short circuit handling
		RemoteCallRecvInfo recv;
		recv.CallID = CallID;
		recv.Component = Info.Source;
		recv.Source = Info.Component;
		recv.Method = Info.GetResponseMethodName();
		recv.Version = Info.GetInternalMethodVersion();
		recv.Context = RemoteCallReturnInfo::GenerateContext(
			RemoteComponentManager::ComponentDisconnected(Info)
		);
		CommBase.InterruptRecv(std::move(recv));
	}
	else if (HasMethod(std::u8string(Info.Method)))return true;
	else
	{
		//generate not supported response
		//short circuit handling
		RemoteCallRecvInfo recv;
		recv.CallID = CallID;
		recv.Component = Info.Source;
		recv.Source = Info.Component;
		recv.Method = Info.GetResponseMethodName();
		recv.Version = Info.GetInternalMethodVersion();
		recv.Context = RemoteCallReturnInfo::GenerateContext(
			RemoteComponentManager::MethodNotSupported(Info)
		);
		CommBase.InterruptRecv(std::move(recv));

	}

	return false;
}

