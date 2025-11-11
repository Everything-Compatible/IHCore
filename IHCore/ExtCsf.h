#pragma once
#include <StringTable.h>
#include <SyringeEx.h>
#include <unordered_map>
#include <string>
#include "ToolFunc.h"
#include "Debug.h"


class IHExtCSF
{
	bool Loaded { false };
	struct sdata
	{
		std::wstring str;
		std::string ext;
	};
	std::unordered_map<std::string, sdata, UpperHash, UpperEqualPred> CsfMap;
	std::string Name;
public:
	uint32_t Version, LblN, StrN, Language, Reserved;

	inline size_t Size() { return CsfMap.size(); }
	template<typename FileLoader>
	bool Load(const char* name);
	template<typename FileLoader>
	bool Load(FileLoader& ld);
	sdata GetStr(const std::string& Key);
	const sdata* GetStr_Opt(const std::string& Key);
	const sdata* GetStr_DefaultOpt(const std::string& Key, bool& Missing);
	void Merge(IHExtCSF&& Another);
	void Merge(const IHExtCSF& Another);
	void Merge(const std::string& Key, std::wstring&& value);
	void Merge(const std::string& Key, const std::wstring& value, const std::string& Extra);
	void Merge(const std::string& Key, const sdata& Val);
	void Merge(const std::string& Key, sdata&& Val);
	void Log()const;
	void Assign(const IHExtCSF&);

	inline bool Available() { return Loaded; }
	inline void Clear()
	{
		CsfMap.clear();
		Name = "";
		Version = LblN = StrN = Language = Reserved = 0u;
		Loaded = false;
	}
};


class IHCSFLoader
{
public:
	static auto const MaxEntries = 20000u;

	static int CSFCount;
	static int NextValueIndex;

	static std::unordered_map<std::string, const CSFString*> DynamicStrings;
	static std::unordered_map<std::string, IHExtCSF> DynamicCSFs;

	static const CSFString* FindDynamic(const char* name);//CANNOT BE USED
	static const IHExtCSF* FindCSF(const char* name);
	static const wchar_t* GetDynamicString(const char* name, const wchar_t* pattern, const char* def);//CANNOT BE USED
	static const wchar_t* GetDynamicString(const char* pCsfName, const char* pEntry);
	static const wchar_t* AnalyzeNoStr(const char* pEntry);
	static const wchar_t* AnalyzeHexString(const char* pEntry);
	static const wchar_t* AnalyzeDynamicString(const char* pEntry);
	static const wchar_t* AnalyzeGBKString(const char* pEntry);
	static const wchar_t* AnalyzeUTF8String(const char* pEntry);
};

bool IsMyString(const char* Name);
const wchar_t* AnalyzeMyString(const char* Name);
bool AddRegStr(std::string&& s, const wchar_t* ss);
void ReanalyzeStr(std::wstring& ws);

template<typename FileLoader>
bool IHExtCSF::Load(FileLoader& fp)
{
	static char gbuf[100000];
	gbuf[4] = 0;
	fp.Read(gbuf, 1, 4);
	//Debug::Log("IHCore:A\n");
	if (std::string(gbuf) != " FSC")return Loaded = false;
	//Debug::Log("IHCore:B\n");
	fp.Read(&Version, 4, 1);
	fp.Read(&LblN, 4, 1);
	fp.Read(&StrN, 4, 1);
	fp.Read(&Reserved, 4, 1);
	fp.Read(&Language, 4, 1);
	//cout<<Version<<endl<<LblN<<endl<<StrN<<endl<<Language<<endl;
	uint32_t NLblStr, StrLen;
	std::string Key, Ext;
	std::wstring Value;
	bool HasExt;
	for (size_t i = 0; i < LblN; i++)
	{
		gbuf[4] = 0;
		fp.Read(gbuf, 1, 4);
		if (std::string(gbuf) != " LBL")
		{
			//Debug::Log("IHCore:C\n");
			return Loaded = false;
		}
		fp.Read(&NLblStr, 4, 1);
		fp.Read(&StrLen, 4, 1);
		gbuf[StrLen] = 0;
		fp.Read(gbuf, 1, StrLen);
		Key = gbuf;
		//cout<<Key<<endl;

		gbuf[4] = 0;
		fp.Read(gbuf, 1, 4);
		if (std::string(gbuf) != " RTS" && std::string(gbuf) != "WRTS")
		{
			//Debug::Log("IHCore:D\n");
			return Loaded = false;
		}
		HasExt = (std::string(gbuf) == "WRTS");
		fp.Read(&StrLen, 4, 1);
		gbuf[StrLen << 1] = gbuf[StrLen << 1 | 1] = 0;
		fp.Read(gbuf, 1, StrLen << 1);
		for (size_t j = 0; j < (StrLen << 1); j++)gbuf[j] = ~gbuf[j];
		Value = (const wchar_t*)gbuf;
		//cout<<UnicodetoMBCS(Value)<<endl;

		if (HasExt)
		{
			fp.Read(&StrLen, 4, 1);
			gbuf[StrLen] = 0;
			fp.Read(gbuf, 1, StrLen);
			Ext = gbuf;
			//cout<<Ext<<endl;
		}
		else
		{
			Ext.clear();
		}

		CsfMap[Key] = { Value,Ext };
	}
	//Debug::Log("IHCore:E\n");
	return Loaded = true;
}

template<typename FileLoader>
bool IHExtCSF::Load(const char* name)
{
	FileLoader fp;
	if (!fp.Open(name, FileLoader::GetBinaryReadSign()))return Loaded = false;
	return Load(fp);
}