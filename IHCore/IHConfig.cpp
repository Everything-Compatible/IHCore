#include "IHConfig.h"
#include "ExtFile.h"
#include "Debug.h"

JsonFile IHConfig;
IHConfigClass IHInstConfig;

bool IHConfig_Initialize()
{
	auto ErrorString = IHConfig.ParseTmpChecked(GetStringFromFile<ExtCCFile>("IHConfig.json"), "<SYNTAX ERROR>", nullptr);
	if (!ErrorString.empty())
	{
		Debug::Log("IHCore : Error on Initializing IHConfig.json. Error : \"\n%s\"", ErrorString.c_str());
		return false;
	}
	return true;
}