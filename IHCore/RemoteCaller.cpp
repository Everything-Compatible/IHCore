#include "ECInterprocess.h"

bool RemoteComponent::HasMethod(const std::u8string& MethodName) const
{
	return BasicInfo.SupportedMethods.contains(MethodName);
}

bool RemoteComponent::Register(const InitInput& Input)//generate InitResult
{
	auto F{ RemoteComponentManager::GenerateRegisterContext(Input) };
	auto Result = CallMethod(RemoteCallSendInfo{
		.Source = u8"",
		.Component = RegName,
		.Method = RemoteCallInfoBase::GetRegisterMethodName(),
		.Version = RemoteCallInfoBase::GetInternalMethodVersion(),
		.Context = F.GetObj()
	});

	if (Result.ErrorMessage)
		return false;

	if(!BasicInfo.Load(Result.ResponseData))
		return false;

	if (BasicInfo.SupportedMethods.count(RemoteCallInfoBase::GetInitMethodName()) == 0)return false;
	if (BasicInfo.SupportedMethods.count(RemoteCallInfoBase::GetPingMethodName()) == 0)return false;
	if (BasicInfo.SupportedMethods.count(RemoteCallInfoBase::GetResponseMethodName()) == 0)return false;

	Register_VerInfo.Version = BasicInfo.Version;
	Register_VerInfo.LowestSupportedVersion = BasicInfo.LowestSupportedVersion;
	Register_VerInfo.LibName = conv RegName.c_str();
	Register_VerInfo.Description = BasicInfo.Description.c_str();

	Register_Dependencies.clear();
	for (const auto& Dep : BasicInfo.Dependencies)
	{
		Register_Dependencies.push_back(InitDependency{
			.LibName = conv Dep.LibName.c_str(),
			.Version = Dep.Version,
			.LowestRequiredVersion = Dep.LowestRequiredVersion,
			.RequiredLoadSequence = Dep.RequiredLoadSequence
		});
	}

	Register_InitRes.GetFunc = nullptr;//remote component do not provide direct function pointer
	Register_InitRes.Info = &Register_VerInfo;
	Register_InitRes.OrderedInit = nullptr;//use RemoteComponent::OrderedInit
	Register_InitRes.Dependencies = Register_Dependencies;
}
void RemoteComponent::OrderedInit()
{
	JsonFile F;
	CallMethod(RemoteCallSendInfo{
		.Source = u8"",
		.Component = RegName,
		.Method = RemoteCallInfoBase::GetInitMethodName(),
		.Version = RemoteCallInfoBase::GetInternalMethodVersion(),
		.Context = F.GetObj()
	});
}
bool RemoteComponent::IsConnected() const
{
	return Connected;
}

const std::u8string& RemoteComponent::GetName() const
{
	return RegName;
}
InitResult& RemoteComponent::GetInitResult()
{
	return Register_InitRes;
}
