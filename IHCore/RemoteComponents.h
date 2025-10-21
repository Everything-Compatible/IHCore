#pragma once
#include "RemoteCommBase.h"

template<typename T>
concept CommTypeFinal = RemoteCommType<T> && !std::is_base_of_v<RemoteComponentBase<T>, T>;

template <CommTypeFinal T>
class RemoteComponent_CommType : public RemoteComponent
{
	T CommBase;
	RemoteComponentBase<T>* Base()
	{
		return reinterpret_cast<RemoteComponentBase<T>*>(this);
	}
public:
	RemoteComponent_CommType(RemoteComponentNameType&& Info) :
		RemoteComponent(std::move(Info))
	{
	}

	virtual ~RemoteComponent_CommType()
	{
		Disconnect();
	}

	virtual bool Connect()
	{
		Base()->Connect();
	}
	virtual void Disconnect()
	{
		Base()->Disconnect();
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
			Base()->SendCallUnchecked(Info, CallID, false);
		return CallID;
	}
	virtual void PostCall(const RemoteCallSendInfo& Info)
	{
		int64_t CallID = Info.GenerateCallID();
		if (CheckHasMethod(Info, CallID))
			Base()->SendCallUnchecked(Info, CallID, true);
	}
	virtual RemoteCallRecvInfo ReceiveCall()
	{
		return Base()->ReceiveCall();
	}
	virtual bool HasReceivedCall()
	{
		return Base()->HasReceivedCall();
	}
};

template <CommTypeFinal T>
bool RemoteComponent_CommType<T>::CheckHasMethod(const RemoteCallSendInfo& Info, int64_t CallID)
{
	if (HasMethod(Info.Method))return true;
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
		Base()->InterruptRecv(std::move(recv));
	}
}

using RemoteComponent_NamedPipe = RemoteComponent_CommType<RemoteComm_NamedPipe>;
using RemoteComponent_TCP = RemoteComponent_CommType<RemoteComm_TCP>;