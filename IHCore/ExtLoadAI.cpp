#include <YRPP.h>
#include <Helpers/Macro.h>

void __fastcall ScenarioClass_LoadAITeamFromINI(CCINIClass* pINI, BOOL Global)
{
	JMP_STD(0x6F19B0);
}

void __fastcall ScenarioClass_LoadAIScriptFromINI(CCINIClass* pINI, BOOL Global)
{
	JMP_STD(0x691970);
}

void __fastcall ScenarioClass_LoadTaskForcesFromINI(CCINIClass* pINI, BOOL Global)
{
	JMP_STD(0x6E8220);
}

void __fastcall ScenarioClass_LoadAITriggerFromINI(CCINIClass* pINI, BOOL Global)
{
	JMP_STD(0x41F2E0);
}

void __fastcall ScenarioClass_LoadTriggerFromINI(CCINIClass* pINI)
{
	JMP_STD(0x7275D0);
}

void __fastcall ScenarioClass_LoadTagsFromINI(CCINIClass* pINI)
{
	JMP_STD(0x6E5ED0);
}

/*
[68797A .. 6879E8)
  ScenarioClass_LoadAITeamFromINI(&AIMDIni, 1);
  ScenarioClass_LoadAITeamFromINI(pScenarioINI, 0);
  ScenarioClass_LoadAIScriptFromINI(&AIMDIni, 1);
  ScenarioClass_LoadAIScriptFromINI(pScenarioINI, 0);
  ScenarioClass_LoadTaskForcesFromINI(&AIMDIni, 1);
  ScenarioClass_LoadTaskForcesFromINI(pScenarioINI, 0);
  ScenarioClass_LoadTriggerFromINI(pScenarioINI);
  ScenarioClass_LoadTagsFromINI(pScenarioINI);
  ScenarioClass_LoadAITriggerFromINI(&AIMDIni, 1);
  ScenarioClass_LoadAITriggerFromINI(pScenarioINI, 0);
*/
//重写逻辑并插入自己的AI来源
DEFINE_HOOK(0x68797A, ScenarioClass_LoadAI, 5)
{
	GET(CCINIClass*, pScenarioINI, EDI);
	auto AIMD = &CCINIClass::INI_AI();

	ScenarioClass_LoadAITeamFromINI(AIMD, TRUE);
	ScenarioClass_LoadAITeamFromINI(pScenarioINI, FALSE);
	ScenarioClass_LoadAIScriptFromINI(AIMD, TRUE);
	ScenarioClass_LoadAIScriptFromINI(pScenarioINI, FALSE);
	ScenarioClass_LoadTaskForcesFromINI(AIMD, TRUE);
	ScenarioClass_LoadTaskForcesFromINI(pScenarioINI, FALSE);
	ScenarioClass_LoadTriggerFromINI(AIMD);
	ScenarioClass_LoadTriggerFromINI(pScenarioINI);
	ScenarioClass_LoadTagsFromINI(AIMD);
	ScenarioClass_LoadTagsFromINI(pScenarioINI);
	ScenarioClass_LoadAITriggerFromINI(AIMD, TRUE);
	ScenarioClass_LoadAITriggerFromINI(pScenarioINI, FALSE);

	return 0x6879E8;
}