#include "IHConfig.h"
#include "ExtFile.h"
#include "Debug.h"

JsonFile IHConfig;
IHConfigClass IHInstConfig;

bool IHConfig_Initialize()
{
	auto String = GetStringFromFile<ExtCCFile>("IHConfig.json");
	if (String.empty())return false;
	auto ErrorString = IHConfig.ParseTmpChecked(std::move(String), "<SYNTAX ERROR>", nullptr);
	if (!ErrorString.empty())
	{
		Debug::Log("IHCore : Error on Initializing IHConfig.json. Error : \"\n%s\"", ErrorString.c_str());
		return false;
	}
	return true;
}