
#include "..\Common\LocalData.h"
#include <Windows.h>
#include "Debug.h"
#include "SomeData.h"
#include <Version.h>
#include <YRPP.h>


void __cdecl ShowIEDialog(void)
{
	GameData::ShowInternalErrorDialog();
}

bool __cdecl OpenWebsite(const char* Site)
{
	return GameData::OpenWebsite(Site, TRUE);
}

std::unordered_map<std::string, FuncInfo>IHCore_Funcs
{
	{"ShowIEDialog",FuncInfo(ShowIEDialog ,FuncType::Procedure)},
	{"OpenWebsite",FuncInfo(OpenWebsite ,FuncType::Default)},//bool (__cdecl*)(const char*)
};

FuncInfo* IHCore_GetFunc(const char* Name, int Version)
{
	if (Version != DoNotCheckVersion && PRODUCT_VERSION < Version)return nullptr;
	auto it = IHCore_Funcs.find(Name);
	if (it == IHCore_Funcs.end())return nullptr;
	else return &it->second;
}
