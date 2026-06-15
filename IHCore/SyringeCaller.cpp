#include "SyringeCaller.h"
#include "LocalData.h"

//GenCallRetType ProcessSyringeRequest(const std::string& Method, JsonFile&& Arguments);

bool SyringeHasMethod(const char* MethodName)
{
	//HasCommand -Command <string>
	const char* HasMethodFnName = "HasCommand";

	JsonFile Arguments;
	Arguments.GetObj().AddString("Command", MethodName);

	auto ResultVariant = ProcessSyringeRequestToJSON(HasMethodFnName, std::move(Arguments));

	if (std::holds_alternative<std::monostate>(ResultVariant))
	{
		return false;
	}
	else if (std::holds_alternative<std::string>(ResultVariant))
	{
		//Error
		return false;
	}
	else
	{
		auto& ResultJson = std::get<JsonFile>(ResultVariant);
		if (!ResultJson.Available())
			return false;
		auto Obj = ResultJson.GetObj();
		if (!Obj.HasItem("Response"))
			return false;
		return IsTrueString(Obj.ItemString("Response"));
	}
}