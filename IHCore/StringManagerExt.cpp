#include "StringManagerExt.h"
#include "ExtIni.h"
#include "ToolFunc.h"
#include "ExtFile.h"
#include "InitialLoad.h"
#include "..\Common\IHLoader\EC.Listener.h"

//DEFINE_HOOK(0x734F50, StringManeger_InitialLoadString, 5)
//DEFINE_HOOK(0x734F80, StringManeger_SetUseCSFString, 5)
//DEFINE_HOOK(0x734FB0, StringManager_IsInternalUseCSFString, 5)
//DEFINE_HOOK(0x734FC0, StringManager_IsUseCSFString, 5)
//DEFINE_HOOK(0x734FD0, StringManager_IsLoaded, 5)





namespace StringManagerExt
{
	char FileNameBuf[260];
	IHExtCSF Ra2MD;
	Ini::IniFile StringMD;
	reference<const wchar_t, 0x84583C, 58> pFatal;
	const char* pStringMD = "stringmd.ini";
	bool FormattedOriginal{ false };
	bool UseAresStringTables{ true };

	void LoadStringMD()
	{
		
		StringMD.Load_ForCCINI<ExtCCFile>(pStringMD);
		if (StringMD.Available())
		{
			Debug::Log("IHCore : Found Stringmd.ini\n");
			MergeStringTable(pStringMD);
			auto Sec = StringMD.GetSection("StringTable");
			if (StringMD.IsFound(Sec))
			{
				for (auto& p : Sec->second.SecStr)
				{
					MergeStringTable(p.second.Value.str.c_str());
				}
			}
			Sec = StringMD.GetSection("Config");
			if (StringMD.IsFound(Sec))
			{
				auto L = Sec->second.GetLine("UseAresStringTables");
				if (Sec->second.IsFound(L))
				{
					UseAresStringTables = IsTrueString(L->second.Value.str);
				}
			}
		}
		else
		{
			//Debug::Log("IHCore : CANNOT Open Stringmd.ini\n");
		}
	}

	bool __fastcall LoadFile(char* FileName)
	{
		StringTable::Labels = GameCreateArray<CSFLabel>(IHCSFLoader::MaxEntries);
		StringTable::Values = GameCreateArray<wchar_t*>(IHCSFLoader::MaxEntries);
		StringTable::ExtraValues = GameCreateArray<char*>(IHCSFLoader::MaxEntries);
		
		if (StringTable::IsLoaded)return true;
		if (!FileName || !*FileName)return false;
		StringTable::FileName = FileName;
		StringTable::IsLoaded = true;
		strcpy(FileNameBuf, FileName);
		auto v2 = strchr(FileNameBuf, '.');
		if (v2)*v2 = 0;
		strcat(FileNameBuf, ".csf");
		Ra2MD.Load<ExtCCFile>(FileNameBuf);
		StringTable::Language = CSFLanguages(Ra2MD.Version < 2 ? 0 : Ra2MD.Language);
		StringTable::MaxLabelLen = 31;
		Debug::Log("\nIHCore :  Reading CSF File \"%s\"\n", FileNameBuf);
		Debug::Log("IHCore : MaxLabelLen = 31\nIHCore : Language: %s\n", StringTable::GetLanguageName(StringTable::Language));
		LoadStringMD();
		if (UseAresStringTables)
		{
			char name[260];
			for (int i = 0; i < 100; i++)
			{
				sprintf_s(name, "stringtable%02d.csf", i);
				IHExtCSF st;
				st.Load<ExtCCFile>(name);
				Ra2MD.Merge(std::move(st));
			}
			IHExtCSF st;
			st.Load<ExtCCFile>("ares.csf");
			Ra2MD.Merge(std::move(st));
		}
		Service_StringTable.Process([](const auto& Param) {MergeStringTable(Param.FileName); });
		Service_StringTableAddPair.Process([](const auto& Param) { Ra2MD.Merge(Param.Key, Param.Value, Param.Extra); });
		SessionClass::Instance->GetUIString();
		MapSeedClass::Instance->GetUIString_RandomMap();
		StringTable::IsLoaded = Ra2MD.Available();
		Debug::Log("IHCore : Value Count: %d\n", Ra2MD.Size());
		return Ra2MD.Available();
	}

	CSFString* Unload()
	{
		//Debug::Log("[IH] Unloading string table\n");
		StringTable::MissingString->Destroy();
		//CSFString* pPrev;
		StringTable::LastLoadedString->Destroy();
		StringTable::IsLoaded = false;
		GameDeleteArray(StringTable::Labels(), IHCSFLoader::MaxEntries);
		GameDeleteArray(StringTable::Values(), IHCSFLoader::MaxEntries);
		GameDeleteArray(StringTable::ExtraValues(), IHCSFLoader::MaxEntries);
		return (CSFString *)StringTable::LastLoadedString().Header;
	}

	const wchar_t* LoadCSFString_PreProcess(const char* pLabel, const char** pOutExtraData)
	{
		for (auto hd : ECListener::GetAll("EC::OnLoadCSFString"))
		{
			auto R = AsType<const wchar_t* CALLBACK(const char* pLabel)>(hd)(pLabel);
			if (R)
			{
				if (pOutExtraData)*pOutExtraData = 0;
				return R;
			}
		}
		if (!FormattedOriginal && StringTable::LabelCount)
		{
			for (int i = 0; i < StringTable::LabelCount(); i++)
				Ra2MD.Merge(StringTable::Labels[i].Name, StringTable::Values[StringTable::Labels[i].FirstValueIndex]);
			FormattedOriginal = true;
		}
		if (pOutExtraData)*pOutExtraData = 0;
		if (!Ra2MD.Available())
		{
			return pFatal();
		}
		return nullptr;
	}

	const wchar_t* __cdecl GetCSFString(const char* pLabel)
	{
		auto PreprocessedStr = LoadCSFString_PreProcess(pLabel, nullptr);
		if (PreprocessedStr)return PreprocessedStr;

		bool Missing = false;
		auto psd = Ra2MD.GetStr_DefaultOpt(pLabel, Missing);
		if (Missing) return nullptr;
		return psd->str.c_str();
	}

	UTF8_CString __cdecl TextDrawRouter_CSF(const char* Key)
	{
		static std::u8string Ret;
		Ret.clear();
		auto ws = GetCSFString(Key);
		if (!ws)return nullptr;
		Ret = ~UnicodetoUTF8(ws);
		return Ret.c_str();
	}

	const wchar_t* __fastcall LoadCSFString(const char* pLabel, const char** pOutExtraData, 
		const char* pSourceCodeFileName, int nSourceCodeFileLine)
	{
		
		auto PreprocessedStr = LoadCSFString_PreProcess(pLabel, pOutExtraData);
		if (PreprocessedStr)return PreprocessedStr;

		//Debug::Log("[IH] Load String %s , Ext %p, SRC %s, Line %d\n", pLabel, pOutExtraData, pSourceCodeFileName, nSourceCodeFileLine);
		bool Missing = false;
		auto psd = Ra2MD.GetStr_DefaultOpt(pLabel,Missing);
		if (Missing)Debug::Log("***NO_STRING*** '%s' -> file:%s line:%d\n", pLabel, pSourceCodeFileName, nSourceCodeFileLine);
		else if (pOutExtraData)*pOutExtraData = psd->ext.c_str();
		return psd->str.c_str();
	}

	bool __fastcall ReadFile(const char* FileName)
	{
		//Debug::Log("IHCore :  Reading CSF File \"%s\"\n", FileName);
		//IHExtCSF Ano;
		//Ano.Load<ExtCCFile>(FileName);
		//Ra2MD.Merge(std::move(Ano));
		return true;
	}

	void Apply()
	{
		Patch::Apply_LJMP(0x7346A0, LoadFile);
		Patch::Apply_LJMP(0x734990, ReadFile);
		Patch::Apply_LJMP(0x734D30, Unload);
		Patch::Apply_LJMP(0x734E60, LoadCSFString);

		IH::SetTextDrawRouter("CSF", TextDrawRouter_CSF);
	}

	void MergeStringTable(const char* Name)
	{
		Debug::Log("IHCore : Merging String Table \"%s\"\n", Name);
		std::string_view sv = Name;
		if (sv.ends_with(".csf"))
		{
			IHExtCSF Ext;
			Ext.Load<ExtCCFile>(Name);
			if(!Ext.Available())Debug::Log("IHCore : CANNOT Open CSF File  \"%s\" \n", Name);
			Ra2MD.Merge(std::move(Ext));
		}
		else if (sv.ends_with(".json"))
		{
			JsonFile Ext;
			auto Str = Ext.ParseChecked(GetStringFromFile<ExtCCFile>(Name), "<SYNTAX ERROR>");
			if (!Str.empty())
			{
				Debug::Log("IHCore : CANNOT Open Json File \"%s\" Info:\n%s\n", Name, Str.c_str());
			}
			else
			{
				auto Obj = Ext.GetObj();
				auto Sec = Obj.GetObjectItem("CSF\xa3\xba");
				if (Sec.Available())
				{
					for (auto& p : Sec.GetMapString())
					{
						auto ws = ANSItoUnicode(p.second);
						ReanalyzeStr(ws);
						Ra2MD.Merge(p.first, std::move(ws));
					}
				}

				Sec = Obj.GetObjectItem((const char*)u8"CSF：");
				if (Sec.Available())
				{
					for (auto& p : Sec.GetMapString())
					{
						auto ws = ANSItoUnicode(p.second);
						ReanalyzeStr(ws);
						Ra2MD.Merge(p.first, std::move(ws));
					}
				}
			}
		}
		else //As INI
		{
			Ini::IniFile Ext;
			Ext.Load_ForCCINI<ExtCCFile>(Name);
			if (Ext.Available())
			{
				auto Sec = Ext.GetSection("CSF\xa3\xba");
				if (Ext.IsFound(Sec))
				{
					for (auto& p : Sec->second.SecStr)
					{
						auto ws = ANSItoUnicode(p.second.Value.str);
						ReanalyzeStr(ws);
						Ra2MD.Merge(p.second.Key.str, std::move(ws));
					}
				}

				Sec = Ext.GetSection((const char*)u8"CSF：");
				if (Ext.IsFound(Sec))
				{
					for (auto& p : Sec->second.SecStr)
					{
						auto ws = UTF8toUnicode(p.second.Value.str);
						ReanalyzeStr(ws);
						Ra2MD.Merge(p.second.Key.str, std::move(ws));
					}
				}
			}
			else
			{
				Debug::Log("IHCore : CANNOT Open INI File  \"%s\" \n", Name);
			}
		}
		
	}

	void MergeFromIni(CCINIClass* pIni)
	{
		auto pSec = pIni->GetSection("StringTable");
		static char StrBuf[600];
		if (pSec)
		{
			auto cnt = pIni->GetKeyCount("StringTable");
			const char* pKey;
			for (int i = 0; i < cnt; i++)
			{
				pKey = pIni->GetKeyName("StringTable", i);
				if (pKey && pIni->GetString("StringTable", pKey, StrBuf))
				{
					MergeStringTable(StrBuf);
				}
			}
		}
	}
};



void CSFFile::Counstructor(CSFFile* This)
{
	This->Language = CSFLanguages::US;
	This->Version = This->LabelCount = This->StringCount = This->Reserved = 0;
	This->Handle = (CSFHandle)new IHExtCSF;
}
void CSFFile::Destructor(CSFFile* This)
{
	This->Language = CSFLanguages::US;
	This->Version = This->LabelCount = This->StringCount = This->Reserved = 0;
	delete ((IHExtCSF*)This->Handle);
	This->Handle = nullptr;
}
void CSFFile::MoveAssign(CSFFile* This, CSFFile* Another)
{
	delete ((IHExtCSF*)This->Handle);
	This->Handle = Another->Handle;
	Another->Handle = nullptr;
	This->Language = Another->Language;
	This->Version = Another->Version;
	This->LabelCount = Another->LabelCount;
	This->StringCount = Another->StringCount;
	This->Reserved = Another->Reserved;
}
void CSFFile::Assign(CSFFile* This, CSFFile* Another)
{
	if (Another->Handle)
	{
		if (!This->Handle)This->Handle = (CSFHandle)new IHExtCSF;
		if (!This->Handle)return;
		((IHExtCSF*)This->Handle)->Assign(*((IHExtCSF*)Another->Handle));
	}
	else if (This->Handle)
	{
		delete ((IHExtCSF*)This->Handle);
		This->Handle = nullptr;
	}
	This->Language = Another->Language;
	This->Version = Another->Version;
	This->LabelCount = Another->LabelCount;
	This->StringCount = Another->StringCount;
	This->Reserved = Another->Reserved;
}
bool CSFFile::LoadAsCCFile_A(CSFFile* This, const char* name)
{
	CSFFile::Destructor(This);
	IHExtCSF* HD= new IHExtCSF;
	auto ret = HD->Load<ExtCCFile>(name);
	This->Handle = HD;
	This->Language = (CSFLanguages)HD->Language;
	This->Version = HD->Version;
	This->LabelCount = HD->LblN;
	This->StringCount = HD->StrN;
	This->Reserved = HD->Reserved;
	return ret;
}
bool CSFFile::LoadAsCCFile_B(CSFFile* This, CCFileClass* pCC)
{
	CSFFile::Destructor(This);
	if (!pCC)return false;
	IHExtCSF* HD = new IHExtCSF;
	auto CC = ExtCCFile{ pCC };
	auto ret = HD->Load(CC);
	This->Handle = HD;
	This->Language = (CSFLanguages)HD->Language;
	This->Version = HD->Version;
	This->LabelCount = HD->LblN;
	This->StringCount = HD->StrN;
	This->Reserved = HD->Reserved;
	return ret;
}
bool CSFFile::LoadAsExternalFile(CSFFile* This, const char* name)
{
	CSFFile::Destructor(This);
	IHExtCSF* HD = new IHExtCSF;
	auto ret = HD->Load<ExtFileClass>(name);
	This->Handle = HD;
	This->Language = (CSFLanguages)HD->Language;
	This->Version = HD->Version;
	This->LabelCount = HD->LblN;
	This->StringCount = HD->StrN;
	This->Reserved = HD->Reserved;
	return ret;
}
CSFEntry CSFFile::GetString(CSFFile* This, const char* Key)//merge操作之后请重新获取
{
	if (!This->Handle)return CSFEntry{ L"","" };
	auto psd = ((IHExtCSF*)This->Handle)->GetStr_Opt(Key);
	if (!psd)return CSFEntry{ L"","" };
	else return CSFEntry{ psd->str.c_str(),psd->ext.c_str() };
}
CSFEntry CSFFile::GetStringDefault(CSFFile* This, const char* Key)//merge操作之后请重新获取
{
	if (!This->Handle)return CSFEntry{ L"","" };
	bool Missing;
	auto psd = ((IHExtCSF*)This->Handle)->GetStr_DefaultOpt(Key, Missing);
	return CSFEntry{ psd->str.c_str(),psd->ext.c_str() };
}
void CSFFile::Merge_A(CSFFile* This, CSFFile* Another)
{
	if (!This->Handle)return;
	((IHExtCSF*)This->Handle)->Merge(*((IHExtCSF*)Another->Handle));
}
void CSFFile::Merge_B(CSFFile* This, const char* Key, const wchar_t* Value, const char* Extra)
{
	if (!This->Handle)return;
	((IHExtCSF*)This->Handle)->Merge(Key, Value, Extra);
}
bool CSFFile::Available(CSFFile* This)
{
	if (!This->Handle)return false;
	else return ((IHExtCSF*)This->Handle)->Available();
}
void CSFFile::Clear(CSFFile* This)
{
	if (This->Handle)((IHExtCSF*)This->Handle)->Clear();
	This->Language = CSFLanguages::US;
	This->Version = This->LabelCount = This->StringCount = This->Reserved = 0;
}

CSFFile_ITable CSFFile_InstTable
{
	CSFFile::Counstructor,
	CSFFile::Destructor,
	CSFFile::MoveAssign,
	CSFFile::Assign,
	CSFFile::LoadAsCCFile_A,
	CSFFile::LoadAsCCFile_B,
	CSFFile::LoadAsExternalFile,
	CSFFile::GetString,
	CSFFile::GetStringDefault,
	CSFFile::Merge_A,
	CSFFile::Merge_B,
	CSFFile::Available,
	CSFFile::Clear,
};

