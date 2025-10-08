#include "StringManagerExt.h"
#include "ExtCD.h"
#include "..\Common\IHLoader\EC.h"

void ExtIni_InitBeforeEverything();
void ConfigJson_InitBeforeEverything();
void HotKey_InitBeforeEverything();
void DebugConsole_InitBeforeEverything();

void InitBeforeEverything()
{
	//THIS MUST BE FIRST
	ConfigJson_InitBeforeEverything();

	ExtIni_InitBeforeEverything();
	ExtCD_InitBeforeEverything();
	HotKey_InitBeforeEverything();
	DebugConsole_InitBeforeEverything();
	StringManagerExt::Apply();

	for (auto hd : ECListener::GetAll("EC::InitBeforeEverything"))
	{
		AsType<void CALLBACK()>(hd)();
	}
}

void InitOnReadingRules(CCINIClass* pIni)
{
	if (!pIni)return;
	for (auto hd : ECListener::GetAll("EC::OnReadingRules"))
	{
		AsType<void CALLBACK(CCINIClass*)>(hd)(pIni);
	}
	StringManagerExt::MergeFromIni(pIni);
}