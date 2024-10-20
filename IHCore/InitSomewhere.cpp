#include "StringManagerExt.h"
#include "ExtCD.h"

void ExtIni_InitBeforeEverything();
void ExtFont_InitBeforeEverything();

void InitBeforeEverything()
{
	ExtIni_InitBeforeEverything();
	ExtCD_InitBeforeEverything();
	StringManagerExt::Apply();
}

void InitOnReadingRules(CCINIClass* pIni)
{
	if (!pIni)return;
	StringManagerExt::MergeFromIni(pIni);
}