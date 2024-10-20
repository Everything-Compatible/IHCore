
#include "ExtCsf.h"
#include "ExtFile.h"
#include "ToolFunc.h"
#include "Debug.h"
#include <CCINIClass.h>
#include <YRPP.h>
#include <Helpers/Macro.h>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <array>

int IHCSFLoader::CSFCount = 0;
int IHCSFLoader::NextValueIndex = 0;
std::unordered_map<std::string, const CSFString*> IHCSFLoader::DynamicStrings;
std::unordered_map<std::string, IHExtCSF> IHCSFLoader::DynamicCSFs;


//CANNOT BE USED
const CSFString* IHCSFLoader::FindDynamic(const char* pLabelName)
{
	if (pLabelName)
	{
		auto it = DynamicStrings.find(pLabelName);
		if (it != DynamicStrings.end())
		{
			return it->second;
		}
	}

	// failed
	return nullptr;
}

//CANNOT BE USED
const wchar_t* IHCSFLoader::GetDynamicString(const char* pLabelName, const wchar_t* pPattern, const char* pDefault)
{
	const CSFString* String = IHCSFLoader::FindDynamic(pLabelName);

	if (!String)
	{
		CSFString* NewString = GameCreate<CSFString>();
		wsprintfW(NewString->Text, pPattern, pDefault);

		StringTable::LastLoadedString().Push(NewString);
		/*if (Ares::bOutputMissingStrings)
		{
			Debug::Log("[CSFLoader] Added label \"%s\" with value \"%ls\".\n", pLabelName, NewString->Text);
		}*/

		DynamicStrings[pLabelName] = NewString;
		String = NewString;
	}

	return String->Text;
}

const IHExtCSF* IHCSFLoader::FindCSF(const char* Name)
{
	if (Name)
	{
		auto it = DynamicCSFs.find(Name);
		if (it != DynamicCSFs.end())
		{
			return std::addressof(it->second);
		}
	}

	// failed
	return nullptr;
}

const wchar_t* IHCSFLoader::GetDynamicString(const char* pCsfName, const char* pEntry)
{
	if (!pCsfName)return nullptr;

	std::string SCsfName { pCsfName };
	std::for_each(SCsfName.begin(), SCsfName.end(), [](char& c) { c = (char)std::tolower(c); });

	auto it = DynamicCSFs.find(SCsfName);
	if (it == DynamicCSFs.end())
	{
		DynamicCSFs[pCsfName].Load<ExtCCFile>((std::string(pCsfName) + ".csf").c_str());
		//if (!Im.second)return nullptr;
		it = DynamicCSFs.find(pCsfName);
	}

	auto& Csf = it->second;

	auto String = Csf.GetStr_Opt(pEntry);
	if (String != nullptr)
	{
		return String->str.c_str();
	}
	else return nullptr;

}

const size_t STRSIZE = 2000;

const wchar_t* IHCSFLoader::AnalyzeDynamicString(const char* pEntry)//without "EXTS:"
{
	static char gb1[STRSIZE + 1], gb2[STRSIZE + 1];

	memset(gb1, 0, STRSIZE + 1);
	memset(gb2, 0, STRSIZE + 1);
	sscanf_s(pEntry, "%[^:]:%s", gb1, STRSIZE, gb2, STRSIZE);
	return IHCSFLoader::GetDynamicString(gb1, gb2);
}

std::array<std::wstring, IHCSFLoader::MaxEntries> HexStrings;
int NHexStr = 0;

void WriteHexString(std::wstring& Target, const char* pEntry)
{
	static wchar_t gb1[STRSIZE / 4 + 1];
	int Len = strnlen(pEntry, STRSIZE);
	int RealLen = Len / 4;
	bool Valid = true;
	if (RealLen <= 0)Valid = false;
	else
	{
		int Tmp;
		memset(gb1, 0, STRSIZE / 4 + 1);
		for (int i = 0; i < RealLen; i++)
		{
			sscanf_s(pEntry + i * 4, "%04X", &Tmp);
			if (Tmp == 0)
			{
				Valid = false;
				break;
			}
			gb1[i] = Tmp;
		}
	}
	if (Valid)
	{
		Target = gb1;
	}
	else
	{
		Target = L"ERROR:INVALID HEX";
	}
}

const wchar_t* IHCSFLoader::AnalyzeHexString(const char* pEntry)//without "HEX:"
{
	if (NHexStr >= IHCSFLoader::MaxEntries)return nullptr;
	WriteHexString(HexStrings[NHexStr], pEntry);
	return HexStrings[NHexStr++].c_str();
}

std::unordered_map<std::string, std::wstring> GBKMap;
std::unordered_map<std::string, std::wstring> UTF8Map;
std::unordered_map<std::string, std::wstring> NoStrMap;

void ReanalyzeStr(std::wstring& ws)
{
	if (ws.size() < 2)return;
	std::wstring Tmp;
	size_t Last = 0;
	for (size_t i = 0; i < ws.size()-1; i++)
	{
		if (ws[i] == L'\\')
		{
			if (ws[i + 1] == L'n')
			{
				ws[i] = L'\n';
				if (Last <= i)Tmp.append(ws.begin() + Last, ws.begin() + i + 1);
				Last = i + 2;
			}
		}
	}
	if (Last < ws.size())
		Tmp.append(ws.begin() + Last, ws.end());
	ws.swap(Tmp);
}

const wchar_t* IHCSFLoader::AnalyzeGBKString(const char* pEntry)
{
	//::MessageBoxA(NULL, pEntry, "CSF_Entry_GBK", MB_OK);
	//Debug::Log("GBK: \"%hs\"\n", pEntry);
	auto& ws = GBKMap[pEntry];
	if (ws.empty())
	{
		ws = ANSItoUnicode(pEntry);
		ReanalyzeStr(ws);
		ws.back() = 0;
	}
	//::MessageBoxW(NULL, ws.c_str(), L"CSF_Entry", MB_OK);
	/*
	Debug::Log("UTF16: \"%hs\"\n", UnicodetoANSI(ws).c_str());
	Debug::Log("HEX: ");
	for (auto w : ws)
	{
		Debug::Log("%04X ",(unsigned)w);
	}Debug::Log("\n");*/
	return ws.c_str();
}

const wchar_t* IHCSFLoader::AnalyzeUTF8String(const char* pEntry)
{
	//::MessageBoxA(NULL, UTF8toANSI(pEntry).c_str(), "CSF_Entry_UTF8", MB_OK);
	//Debug::Log("UTF8: \"%hs\"\n", UTF8toANSI(pEntry).c_str());
	//Debug::Log("UTF8_HEX: ");
	//for(const char* s=pEntry;*s;s++)
	//{
	//	Debug::Log("%02X ", (unsigned)*s);
	//}Debug::Log("\n");
	auto& ws = UTF8Map[pEntry];
	if (ws.empty())
	{
		ws = UTF8toUnicode(pEntry);
		ReanalyzeStr(ws);
		ws.back() = 0;
	}
	//::MessageBoxW(NULL, ws.c_str(), L"CSF_Entry", MB_OK);
	//Debug::Log("UTF16: \"%hs\"\n", UnicodetoANSI(ws).c_str());
	//Debug::Log("HEX: ");
	//for (auto w : ws)
	//{
	//	Debug::Log("%04X ", (unsigned)w);
	//}Debug::Log("\n");
	return ws.c_str();
}



std::unordered_map<std::string, const wchar_t*> RegStrMap;

bool AddRegStr(std::string&& s, const wchar_t* ss)
{
	RegStrMap[std::move(s)] = ss;
	return true;
}


const wchar_t* IHCSFLoader::AnalyzeNoStr(const char* pEntry)
{
	auto& ws = NoStrMap[pEntry];
	if (ws.empty())ws = ANSItoUnicode(pEntry);
	return ws.c_str();
}

const wchar_t* AnalyzeMyString(const char* Name)
{
	auto it = RegStrMap.find(Name);
	if (it != RegStrMap.end())
	{
		return it->second;
	}

	if (!strncmp(Name, "NOSTR:", 6))
	{
		return IHCSFLoader::AnalyzeNoStr(Name + 6);
	}

	if (!strncmp(Name, "EXTS:", 5))
	{
		return IHCSFLoader::AnalyzeDynamicString(Name + 5);
	
	}

	if (!strncmp(Name, "HEX:", 4))
	{
		return IHCSFLoader::AnalyzeHexString(Name + 4);
	}

	if (!strncmp(Name, "(£º", 3))
	{
		return IHCSFLoader::AnalyzeGBKString(Name + 3);
	}

	if (!strncmp(Name, (const char*)u8"(£º", 4))
	{
		return IHCSFLoader::AnalyzeUTF8String(Name + 4);
	}
	return nullptr;
}

bool IsMyString(const char* Name)
{
	if (!strncmp(Name, "NOSTR:", 6))return true;
	if (!strncmp(Name, "EXTS:", 5))return true;
	if (!strncmp(Name, "HEX:", 4))return true;
	if (!strncmp(Name, "(£º", 3))return true;
	if (!strncmp(Name, (const char*)u8"(£º", 4))return true;
	return false;
}


IHExtCSF::sdata IHExtCSF::GetStr(const std::string& Key)
{
	auto it = CsfMap.find(Key);
	if (it == CsfMap.end())return { L"","" };
	else return it->second;
}

const IHExtCSF::sdata* IHExtCSF::GetStr_Opt(const std::string& Key)
{
	auto it = CsfMap.find(Key);
	if (it == CsfMap.end())return nullptr;
	else return &it->second;
}

const IHExtCSF::sdata* IHExtCSF::GetStr_DefaultOpt(const std::string& Key, bool& Missing)
{
	auto it = CsfMap.find(Key);
	Missing = false;
	if (it == CsfMap.end())
	{
		auto ws = AnalyzeMyString(Key.c_str());
		auto& psd = CsfMap[Key];
		if (ws)psd.str = ws;
		else
		{
			Missing = true;
			psd.str = L"MISSING: \"" + ANSItoUnicode(Key) + L"\"";
		}
		return &psd;
	}
	else return &it->second;
}

void IHExtCSF::Merge(IHExtCSF&& Another)
{
	if (!Another.Loaded)return;
	for (auto& p : Another.CsfMap)
	{
		CsfMap.insert_or_assign(p.first, std::move(p.second));
	}
	Another.CsfMap.clear();
	Another.Loaded = false;
}
void IHExtCSF::Merge(const IHExtCSF& Another)
{
	if (!Another.Loaded)return;
	for (auto& p : Another.CsfMap)
	{
		CsfMap.insert_or_assign(p.first, p.second);
	}
}
void IHExtCSF::Merge(const std::string& Key, std::wstring&& value)
{
	CsfMap.insert_or_assign(Key, std::move(sdata{ std::move(value),""}));
}
void IHExtCSF::Merge(const std::string& Key, const std::wstring& value, const std::string& Extra)
{
	CsfMap.insert_or_assign(Key, sdata{ value,Extra });
}
void IHExtCSF::Merge(const std::string& Key,const sdata& Val)
{
	CsfMap.insert_or_assign(Key, Val);
}
void IHExtCSF::Merge(const std::string& Key, sdata&& Val)
{
	CsfMap.insert_or_assign(Key, std::move(Val));
}
void IHExtCSF::Log()const
{
	Debug::Log("[IH] Output %s\n", Name.c_str());
	for (auto& p : CsfMap)
	{
		Debug::Log("[IH] \"%s\" %X =\"%s\"\n", p.first.c_str(), UpperHash()(p.first), UnicodetoUTF8(p.second.str).c_str());
	}
}
void IHExtCSF::Assign(const IHExtCSF& rhs)
{
	*this = rhs;
}

/*
//FOR EXT & JSON
DEFINE_HOOK(0x734E83, CSF_LoadStringA, 6)
{
	GET(char*, Name, EBX);
	//IH_ENTER_FN_LOG(CSF_LoadStringA, AnalyzeMyString);
	const wchar_t* string = AnalyzeMyString(Name);
	if (string != nullptr)
	{
		R->EAX(string);
		return 0x734F0F;
	}
	//IH_EXIT_FN_LOG(CSF_LoadStringA, AnalyzeMyString);
	return 0;
}

DEFINE_HOOK(0x410B58, CSF_UINameLabelFix, 6)
{
	static char Stack[200010];
	GET_STACK(INIClass*, pINI, 0x440);
	GET(ObjectTypeClass*, pObj, EBX);
	try
	{
		if (!pINI || !pObj)return 0;
		Stack[pINI->ReadString(pObj->ID, "UIName", Stack, Stack, 200000)] = 0;
		//Debug::Log("UIName Entry=\"%s\"\n", Stack);
		const wchar_t* string = AnalyzeMyString(Stack);
		if (string)R->EAX(string);
	}
	catch (std::exception& e)
	{
		Debug::Log("IHCore Error : %s", e.what());
	}
	return 0;

	bool IHExtCSF::LoadFromCCFile(const char* name)
{
	static char gbuf[10000];

	CCFileClass* pFile = GameCreate<CCFileClass>(name);
	if (!pFile->Exists())return Loaded = false;
	if (!pFile->Open(FileAccessMode::Read))return Loaded = false;


	gbuf[4] = 0;
	pFile->ReadBytes(gbuf,4);

	if (std::string(gbuf) != " FSC")
		{ pFile->Close(); GameDelete(pFile); return Loaded = false; }
	pFile->Read(Version);
	pFile->Read(LblN);
	pFile->Read(StrN);
	pFile->Read(Language);
	pFile->Read(Reserved);

	uint32_t NLblStr, StrLen;
	std::string Key, Ext;
	std::wstring Value;
	bool HasExt;
	for (size_t i = 0; i < LblN; i++)
	{
		gbuf[4] = 0;
		pFile->ReadBytes(gbuf, 4);
		if (std::string(gbuf) != " LBL")
			{ pFile->Close(); GameDelete(pFile); return Loaded = false; }
		pFile->Read(NLblStr);
		pFile->Read(StrLen);
		gbuf[StrLen] = 0;
		pFile->ReadBytes(gbuf, StrLen);
		Key = gbuf;
		//cout<<Key<<endl;

		gbuf[4] = 0;
		pFile->ReadBytes(gbuf, 4);
		if (std::string(gbuf) != " RTS" && std::string(gbuf) != "WRTS")
			{ pFile->Close(); GameDelete(pFile); return Loaded = false; }
		HasExt = (std::string(gbuf) == "WRTS");
		pFile->Read(StrLen);
		gbuf[StrLen << 1] = gbuf[StrLen << 1 | 1] = 0;
		pFile->ReadBytes(gbuf, StrLen << 1);
		for (size_t j = 0; j < (StrLen << 1); j++)gbuf[j] = ~gbuf[j];
		Value = (const wchar_t*)gbuf;

		if (HasExt)
		{
			pFile->Read(StrLen);
			gbuf[StrLen] = 0;
			pFile->ReadBytes(gbuf, StrLen);
			Ext = gbuf;
			//cout<<Ext<<endl;
		}
		else
		{
			Ext.clear();
		}

		CsfMap[Key] = { Value,Ext };
	}
	pFile->Close();
	GameDelete(pFile);
	return Loaded = true;
}
}*/
