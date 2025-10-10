
#include <YRPP.h>
#include <CRT.h>
#include <Helpers/Macro.h>
#include <ToolFunc.h>
#include "Debug.h"
#include "Global.h"
#include "ExtIni.h"
#include "Patch.h"
#include "LocalData.h"

void InitBeforeEverything();
void InitOnReadingRules(CCINIClass* pIni);

void AllowMultipleInstances_Fix()
{
	InitBeforeEverything();
}

DEFINE_HOOK(0x6BBE56, AllowMultipleInstances, 5)
{
	IH_ENTER_FN_LOG(AllowMultipleInstances, AllowMultipleInstances_Fix);
	AllowMultipleInstances_Fix();
	IH_EXIT_FN_LOG(AllowMultipleInstances, AllowMultipleInstances_Fix);
	return 0x6BBED6;
}


DEFINE_HOOK(0x668BF0, RulesClass_ReadFile, 5)
{
	IH_ENTER_FN_LOG(RulesClass_ReadFile, InitOnReadingRules);
	GET_STACK(CCINIClass*, pIni, 0x4);
	InitOnReadingRules(pIni);
	IH_EXIT_FN_LOG(RulesClass_ReadFile, InitOnReadingRules);
	return 0;
}

DEFINE_HOOK(0x6BC0CD, ECInitialize, 5)
{
	Local::LocalInit();
	return 0;
}
