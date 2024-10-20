#include"ExtIni.h"
#include"ExtFile.h"
#include <YRPP.h>
#include <Straws.h>
#include <CRT.h>
#include <set>
#include <Helpers/Macro.h>
#include "Debug.h"
#include "SyringeEx.h"
#include "ExtCsf.h"
#include "ToolFunc.h"

hash_t StrHash(const char* str)
{
	return StrHash(std::string(str));
}
hash_t StrHash(const std::string& str)
{
	std::hash<std::string> hash_str;
	return hash_str(str);
}
void CStringTrimFront(char*& str)
{
	
	if (!str)return;
	while (*str == ' ' || *str == '\t')
	{
		if (!(*str))break;
		++str;
	}
}
void CStringTrimBack(char* str)
{
	if (!str)return;
	char* res = str;
	--res; while (*(++res)); --res;
	while (res >= str &&( *res == ' ' || *res == '\t' || *res == '\r' || *res == '\n'))--res;
	*(res + 1) = 0;
}
void CStringTrim(char*& str)
{
	CStringTrimFront(str);
	CStringTrimBack(str);
}
const char* BoolCStr(bool a)
{
	return a ? "true" : "false";
}
const std::string& BoolStr(bool a)
{
	static const std::string T{ "true" }, F{ "false" };
	return a ? T : F;
}

namespace Ini
{
	void StringData::Set(const char* ext_str)
	{
		str = ext_str;
		Rehash();
	}
	void StringData::Rehash()
	{
		HashVal = StrHash(str);
	}
	StringData::StringData(const char* ext_str)
	{
		Set(ext_str);
	}
	StringData& StringData::operator=(const StringData& rhs)
	{
		str = rhs.str;
		HashVal = rhs.HashVal;
		return *this;
	}

	const std::string& IniFile::GetValue(const std::string& Sec, const std::string& Key, const std::string& Default)
	{
		auto it = GetSection(Sec);
		if (!IsFound(it))return Default;
		return it->second.GetStrRef(Key, Default);
	}


	std::string_view TrimView(std::string_view Line)
	{
		while (!Line.empty())
		{
			if (isspace(Line.front()))Line = Line.substr(1, Line.size() - 1);
			else break;
		}
		while (!Line.empty())
		{
			if (isspace(Line.back()))
			{
				if (Line.size() >= 2 && Line[Line.size() - 2] < 0)break;
				else Line = Line.substr(0, Line.size() - 1);
			}
			else break;
		}
		return Line;
	}

	void IniToken::Tokenize(std::string_view Line)
	{
		Line = TrimView(Line);
		if (Line.empty() || Line.front() == ';')
		{
			Empty = true;
			return;
		}
		auto p = Line.find_first_of(";");
		if (p != std::string_view::npos)
		{
			Line = Line.substr(0, p);
		}
		if (Line.front() == '[')
		{
			p = Line.find_first_of("]");
			if (p != std::string_view::npos)
			{
				auto Sub = Line.substr(1, p - 1);
				Empty = false;
				IsSection = true;
				Key = TrimView(Sub);
				auto Suc = TrimView(Line.substr(p + 1, Line.size() - p - 1));
				if (!Suc.empty())
				{
					if (Suc.front() == ':')
					{
						Suc = TrimView(Suc.substr(1, Suc.size() - 1));
						if (!Suc.empty())
						{
							if (Suc.front() == '[')
							{
								auto q = Suc.find_first_of("]");
								if (q != std::string_view::npos)
								{
									Suc= Suc.substr(1, q - 1);
									Value = TrimView(Suc);
								}
							}
							
						}
					}
				}
				return;
			}
		}
		if (Line.front() != '=')
		{
			p = Line.find_first_of("=");
			if (p != std::string_view::npos && Line.size() > p + 1)
			{
				auto Sub = TrimView(Line.substr(0, p));
				auto Val = TrimView(Line.substr(p + 1, Line.size() - p - 1));
				if (!Sub.empty() && !Val.empty())
				{
					Empty = false;
					IsSection = false;
					Key = Sub; 
					Value = Val;
					return;
				}
			}
		}
		Empty = true;
		return;
	}
	std::vector<std::string_view> GetLines(BytePointerArray Text, size_t ExtBytes)
	{
		std::vector<std::string_view> Res;
		auto Data = (char*)Text.Data;
		memset(Data + Text.N - ExtBytes, 0, ExtBytes);
		for (size_t i = 0; i < Text.N; ++i)
		{
			if (Data[i]=='\r'|| Data[i] == '\n')Data[i] = 0;
		}
		for (size_t i = 0; i < Text.N;)
		{
			auto Dt = Data + i;
			i++;
			if (!*Dt)continue;
			Res.push_back(Dt);
			i += strlen(Dt);
		}
		return Res;
	}
	std::vector<IniToken> GetTokens(const std::vector<std::string_view>& Lines)
	{
		std::vector<IniToken> Result;
		Result.reserve(Lines.size());
		for (auto view : Lines)
		{
			Result.emplace_back(view);
		}
		return Result;
	}

	void PrintToken(const IniToken& tk)
	{
		static BufString ss;
		if (tk.Empty)
		{
			sprintf(ss, "EMPTY");
		}
		else if (tk.IsSection)
		{
			sprintf(ss, "SECTION Key=\"%s\" Value=\"%s\"", tk.Key.c_str(), tk.Value.c_str());
		}
		else
		{
			sprintf(ss, "LINE Key=\"%s\" Value=\"%s\"", tk.Key.c_str(), tk.Value.c_str());
		}

		//if (!tk.Empty)::MessageBoxA(NULL, ss, "Writing", MB_OK);
	}

	bool IniFile::Load(const std::vector<IniToken>& Tokens, const std::string& Name)
	{
		//for (auto& tk : Tokens)PrintToken(tk);
		if (Tokens.empty())return Loaded = false;
		IniName.Set(Name.c_str());
		IniSection* Cur = &FileSec[StrHash("")];
		FileSec[StrHash("")] = IniSection("");
		for (auto& tok : Tokens)
		{
			if (tok.Empty)continue;
			if (tok.IsSection)
			{
				auto it = FileSec.find(StrHash(tok.Key));
				Cur = &FileSec[StrHash(tok.Key)];
				if (it == NotFound())
				{
					Cur->SecName.Set(tok.Key.c_str());
					Cur->Parent = tok.Value;
					SectionSequence.push_back(Cur->SecName.HashVal);
				}
			}
			else
			{
				auto H = StrHash(tok.Key);
				auto& K = Cur->SecStr[H];
				K.Key.Set(tok.Key.c_str());
				K.Value.Set(tok.Value.c_str());
				Cur->KeySequence.push_back(H);
			}
		}
		for (auto& p : FileSec)
		{
			if (!p.second.Parent.empty())
			{
				auto it = GetSection(p.second.Parent);
				if (IsFound(it))
				{
					for (const auto& L : it->second.SecStr)
					{
						p.second.SetStrIfNotExist(L.second.Key.str, L.second.Value.str);
					}
				}
			}
		}
		return Loaded = true;
	}
}


BOOL INIClass_ReadFile(INIClass* pINI, CCFileClass* pFile)
{
	if (!pINI)return FALSE;
	if (!pFile)return FALSE;
	//::MessageBoxA(NULL, "1SS01", "tdfh2", MB_OK);
	ExtCCFile sf(pFile);
	ExtIni INI;
	//::MessageBoxA(NULL, "1SS02", "tdfh2", MB_OK);
	INI.Load(sf, pFile->GetFileName());
	sf.Release();
	//::MessageBoxA(NULL, "1SS03", "tdfh2", MB_OK);
	return INIClass_FromExtIni(pINI, INI);
}
BOOL INIClass_FromExtIni(INIClass* pINI, ExtIni& Ext)
{
	//::MessageBoxA(NULL, "1SS04", "tdfh2", MB_OK);
	if (!pINI)return FALSE;
	if (!Ext.Available())return FALSE;
	//::MessageBoxA(NULL, "1SS05", "tdfh2", MB_OK);
	//Don't care about digest let CCINIClass Generate it
	pINI->CurrentSectionName = nullptr;
	pINI->CurrentSection = nullptr;
	pINI->LineComments = nullptr;
	for (auto& pSec : Ext.FileSec)
	{
		for (auto& pLine : pSec.second.SecStr)
		{
			pINI->WriteString(
				pSec.second.SecName.str.c_str(),
				pLine.second.Key.str.c_str(),
				pLine.second.Value.str.c_str());
		}
	}
	//::MessageBoxA(NULL, "1SS06", "tdfh2", MB_OK);
	return TRUE;
}


size_t __fastcall CacheStraw_GetLine(CacheStraw* Straw, char* pBuffer, size_t size, BYTE* a4)
{
	JMP_STD(0x65D5C0);
}

//std::unordered_map<std::string, std::string> GlbKeyMap;
std::unordered_map<std::string, std::string> GlbValueMap;

char BBuf[200010];


size_t __fastcall CacheStraw_GetLineEx(CacheStraw* Straw, char* pBuffer, size_t size, BYTE* pEndOfFile)
{
	auto len = CacheStraw_GetLine(Straw, BBuf, 200000, pEndOfFile);
	BBuf[len] = 0;
	if (pEndOfFile && *pEndOfFile && len)*pEndOfFile = 0;
	if (!len)
	{
		memset(pBuffer, 0, size);
		return 0;
	}
	Ini::IniToken tok(BBuf);
	if (!tok.Empty && !tok.IsSection && !tok.Value.empty() && !tok.Key.empty())
	{
		auto HVal = SyringeData::SyringeHash(tok.Value.c_str());
		auto SVal = std::to_string(HVal);
		auto ws = AnalyzeMyString(tok.Value.c_str());
		if (ws)
		{
			auto hs = StrHash(tok.Value);
			auto hh = std::to_string(hs);
			GlbValueMap[SVal] = hh;
			AddRegStr(std::move(hh), ws);
		}
		else GlbValueMap[SVal] = tok.Value;
		auto len = sprintf_s(pBuffer, size, "%s=%s", tok.Key.c_str(), SVal.c_str());
		return len;
	}
	else
	{
		BBuf[size - 1] = 0;
		strncpy(pBuffer, BBuf, size);
		return size - 1;
	}
}

char* __cdecl StrDupValue(const char* Str)
{
	if (!Str)return nullptr;
	auto Ret = CRT::strdup(GlbValueMap[Str].c_str());
	return Ret;
}

char* __cdecl StrDupKey(const char* Str)
{
	if (!Str)return nullptr;
	return CRT::strdup(Str);
}

char* __cdecl StrDupSection(const char* Str)
{
	if (!Str)return nullptr;
	auto Ret = CRT::strdup(Str);
	return Ret;
}


/*
class AppendStraw : public Straw
{
	bool Appended{ false };
public:
	virtual int Get(void* pBuffer, int slen)
	{
		Debug::Log("GERTIHIHIH!\n");
		if (this->ChainTo)
		{
			auto len = this->ChainTo->Get(pBuffer, slen);
			if (pBuffer && slen && !len && !Appended)
			{
				Appended = true;
				Debug::Log("APPEND!\n");
				*((char*)pBuffer) = 32;
				return 1;
			}
		}

		return 0;
	}
};

struct CCFakeINI
{
	BOOL INIClass_OriginalReadFile(Straw* pFile, bool bLoadComments)
	{
		JMP_THIS(0x525A60);
	}
	
	BOOL INIClass_ReadFile(FileStraw* pFile, bool bLoadComments)
	{
		//AppendStraw Dummy;
		//Dummy.ChainTo = pFile;
		//auto Orig = pFile->ChainFrom;
		//pFile->ChainFrom = &Dummy;
		if (pFile)
		{
			AppendStraw* s = GameCreate<AppendStraw>();

			//DWORD FakeVTbl[4];
			//const DWORD* VTbl_0 = (const DWORD*)0x7E61EC;
			//memcpy(FakeVTbl, VTbl_0, sizeof(FakeVTbl));
			//Debug::Log("%08X %08X %08X %08X %08X \n",s, FakeVTbl[0], FakeVTbl[1], FakeVTbl[2], FakeVTbl[3]);
			//FakeVTbl[3] = union_cast<DWORD>(&AppendStraw::Get);
			//*((DWORD**)&s) = FakeVTbl + 1;
			VTABLE_SET(s, 0x7E61F0);
			DWORD* VTbl = (DWORD*)0x7E61F0;
			DWORD OldProt;
			//VirtualProtect(VTbl, 12, PAGE_EXECUTE_READWRITE, &OldProt);
			auto d = VTbl[2];
			//VTbl[2] = union_cast<DWORD>(&AppendStraw::Get);

			s->Get_From(*pFile);
			Debug::Log("IHIHIHIHIH\n");
			auto result = INIClass_OriginalReadFile(s, bLoadComments);

			//VTbl[2] = d;
			//VirtualProtect(VTbl, 12, OldProt, nullptr);
			GameDelete(s);
			//pFile->ChainFrom = Orig;
			return result;
		}
		else return INIClass_OriginalReadFile(pFile, bLoadComments);
	}


};
*/


#include "Patch.h"

void ExtIni_InitBeforeEverything()
{
	Patch::Apply_CALL(0x526070, StrDupKey);
	Patch::Apply_CALL(0x526062, StrDupValue);
	Patch::Apply_CALL(0x525E07, StrDupSection);
	//Patch::Apply_CALL(0x525C6E, CacheStraw_GetLineEx);
	Patch::Apply_CALL(0x525E70, CacheStraw_GetLineEx);
	//Patch::Apply_CALL(0x47422B, union_cast<void*>(&CCFakeINI::INIClass_ReadFile));
}


