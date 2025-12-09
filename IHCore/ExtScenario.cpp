#include <Syringe.h>
#include <Helpers/Macro.h>
#include <CCINIClass.h>
#include <ScenarioClass.h>
#include "Debug.h"
#include "ToolFunc.h"
#include "..\Common\IHLoader\EC.Listener.h"

/*
DEFINE_HOOK(0x686B29, RedirectINI, 6)
{
	GET(CCINIClass*, pMap, ECX);
	Debug::Log("[IH]Scenario File : %s", UnicodetoUTF8(ScenarioClass::Instance->Name).c_str());
	return 0;

	//Debug::Log("[IH]Scenario File : %s\n", pMapName);
	//for (auto pSec = ScenarioINI.Sections.First(); pSec != ScenarioINI.Sections.Last(); pSec = pSec->Next())
	//	Debug::Log("IHCore : %s\n",pSec->Name);
}*/


namespace RedirectData
{
	CCINIClass* NewRules{ nullptr };
	CCINIClass* OldRules{ nullptr };
	CCINIClass* NewArt{ nullptr };
	bool HasOldArt{ false };
	CCINIClass OldArt{ noinit_t{} };
}

DEFINE_HOOK(0x683CD1, RedirectINI, 7)
{
	static char StrBuf[MAX_PATH];
	GET(const char*, pMapName, EBP);
	REF_STACK(CCINIClass, ScenarioINI, 0x1C);

	if (RedirectData::OldRules)CCINIClass::INI_Rules() = RedirectData::OldRules;
	if (RedirectData::NewRules)GameDelete(RedirectData::NewRules);
	RedirectData::OldRules = RedirectData::NewRules = nullptr;
	if(RedirectData::HasOldArt)memcpy(&CCINIClass::INI_Art(), &RedirectData::OldArt, sizeof(CCINIClass));
	if (RedirectData::NewArt)GameDelete(RedirectData::NewArt);
	RedirectData::NewArt = nullptr;
	RedirectData::HasOldArt = false;

	auto pSec = ScenarioINI.GetSection("ExtraFiles");
	if (pSec)
	{
		Debug::Log("IHCore : Extra Files Found.\n");
		memset(StrBuf, 0, sizeof(StrBuf));
		ScenarioINI.GetString("ExtraFiles", "NewRules", StrBuf);
		if (StrBuf[0])
		{
			Debug::Log("IHCore : New Rules : %s\n", StrBuf);
			auto pExtCC = GameCreate<CCFileClass>(StrBuf);
			RedirectData::NewRules = GameCreate<CCINIClass>();
			RedirectData::NewRules->ReadCCFile(pExtCC);
			RedirectData::OldRules = CCINIClass::INI_Rules();
			CCINIClass::INI_Rules() = RedirectData::NewRules;
			GameDelete(pExtCC);
		}
		memset(StrBuf, 0, sizeof(StrBuf));
		ScenarioINI.GetString("ExtraFiles", "NewArt", StrBuf);
		if (StrBuf[0])
		{
			Debug::Log("IHCore : New Art : %s\n", StrBuf);
			auto pExtCC = GameCreate<CCFileClass>(StrBuf);
			RedirectData::NewArt = GameCreate<CCINIClass>();
			RedirectData::NewArt->ReadCCFile(pExtCC);
			memcpy(&RedirectData::OldArt, &CCINIClass::INI_Art(), sizeof(CCINIClass));
			memcpy(&CCINIClass::INI_Art(), RedirectData::NewArt, sizeof(CCINIClass));
			RedirectData::HasOldArt = true;
			GameDelete(pExtCC);
		}
	}
	else
	{
		//Debug::Log("IHCore : No Extra Files.\n");
	}

	for (auto hd : ECListener::GetAll("EC::OnLoadScenario"))
		if(hd)((ECListener::Listener_LoadScenario)(hd))();

	return 0;
}