#pragma once
#include<cstdio>
#include<cstring>
#include<string>
#include<string_view>
#include<unordered_map>
#include<vector>
#include <Windows.h>
#include "ToolFunc.h"
#include "../Common/IHLoader/PArray.h"

namespace Ini
{
	struct IniToken
	{
		bool IsSection{ false }, Empty{ false };
		std::string Key, Value;

		void Tokenize(std::string_view);
		IniToken() = default;
		IniToken(std::string_view Line)
		{
			Tokenize(Line);
		}
	};

	std::vector<std::string_view> GetLines(PArray<BYTE> Text,size_t ExtBytes);//ExtBytes > 0
	std::vector<IniToken> GetTokens(const std::vector<std::string_view>& Lines);
	template<typename FileLoader>
	std::vector<IniToken> GetTokensFromFile(FileLoader& Loader)
	{
		PArray<BYTE> Arr = Loader.ReadWholeFile(16);
		auto Ret = GetTokens(GetLines(Arr, 16));
		Arr.Delete();
		return Ret;
	}

	inline std::vector<IniToken>&& CCINIRegenTokens(std::vector<IniToken>&& Tok)
	{
		int i = 0;
		for (auto& p : Tok)
		{
			if (!p.IsSection && !p.Empty && !p.Key.empty() && p.Key == "+")
			{
				p.Key = ";;" + std::to_string(i);
			}
			++i;
		}
		return std::move(Tok);
	}
	
	
	struct StringData
	{
		std::string str;
		hash_t HashVal;
		void Set(const char* ext_str);
		void Rehash();
		StringData() = default;
		StringData(const char* ext_str);
		StringData& operator=(const StringData& rhs);
	};

	struct IniLine
	{
		StringData Key;
		StringData Value;
		IniLine() = default;
		hash_t GetHash()
		{
			return Key.HashVal;
		}
	};

	typedef std::unordered_map<hash_t, IniLine>::iterator IniLineIter;

	struct IniSection
	{
	public:
		std::string Parent;
		StringData SecName;
		std::unordered_map<hash_t, IniLine> SecStr;
		std::vector<hash_t>KeySequence;

		void SetName(const char* ext_str)
		{
			SecName.Set(ext_str);
		}
		void SetName(const StringData& ext_str)
		{
			SecName = ext_str;
		}
		IniSection() = default;
		IniSection(const char* ext_str)
		{
			SetName(ext_str);
		}
		IniSection(const std::string& ext_str)
		{
			SetName(ext_str.c_str());
		}
		IniSection(const StringData& ext_str)
		{
			SetName(ext_str);
		}
		hash_t GetNameHash()
		{
			return SecName.HashVal;
		}
		inline IniLineIter GetLine(hash_t KeyHash)
		{
			return SecStr.find(KeyHash);
		}
		inline bool HasKey(hash_t KeyHash)
		{
			return GetLine(KeyHash) == SecStr.end();
		}
		inline IniLineIter GetLine(const char* KeyHash)
		{
			return SecStr.find(StrHash(KeyHash));
		}
		inline bool HasKey(const char* KeyHash)
		{
			return GetLine(KeyHash) == SecStr.end();
		}
		inline IniLineIter GetLine(const std::string& KeyHash)
		{
			return SecStr.find(StrHash(KeyHash));
		}
		inline bool HasKey(const std::string& KeyHash)
		{
			return GetLine(KeyHash) == SecStr.end();
		}
		inline IniLineIter NotFound()
		{
			return SecStr.end();
		}
		inline bool IsFound(IniLineIter it)
		{
			return it != SecStr.end();
		}
		inline const std::string& GetStrRef(const std::string& Key)
		{
			static std::string EmptyStr{ "" };
			auto it = GetLine(Key);
			if (IsFound(it))return it->second.Value.str;
			else return EmptyStr;
		}
		inline const std::string& GetStrRef(const std::string& Key,const std::string& Default)
		{
			auto it = GetLine(Key);
			if (IsFound(it))return it->second.Value.str;
			else return Default;
		}
		inline std::string GetStrCopy(const std::string& Key)
		{
			static std::string EmptyStr{ "" };
			auto it = GetLine(Key);
			if (IsFound(it))return it->second.Value.str;
			else return EmptyStr;
		}
		inline std::string GetStrCopy(const std::string& Key, const std::string& Default)
		{
			auto it = GetLine(Key);
			if (IsFound(it))return it->second.Value.str;
			else return Default;
		}
		inline void GetIfExists(const std::string& Key, std::string& Val)
		{
			Val = GetStrRef(Key);
		}
		inline void GetIfExists(const std::string& Key, std::string& Val, const std::string& Default)
		{
			Val = GetStrRef(Key, Default);
		}
		inline void GetIfExists(const std::string& Key, char* Val)
		{
			strcpy(Val, GetStrRef(Key).c_str());
		}
		inline void GetIfExists(const std::string& Key, char* Val, const std::string& Default)
		{
			strcpy(Val, GetStrRef(Key, Default).c_str());
		}
		inline void SetStr(const std::string& Key, const std::string& Val)
		{
			auto it = GetLine(Key);
			if (IsFound(it))it->second.Value.Set(Val.c_str());
			else
			{
				auto& p = SecStr[StrHash(Key)];
				p.Key.Set(Key.c_str());
				p.Value.Set(Val.c_str());
				KeySequence.push_back(StrHash(Key));
			}
		}
		inline void SetStrIfNotExist(const std::string& Key, const std::string& Val)
		{
			auto it = GetLine(Key);
			if (!IsFound(it))
			{
				auto& p = SecStr[StrHash(Key)];
				p.Key.Set(Key.c_str());
				p.Value.Set(Val.c_str());
				KeySequence.push_back(StrHash(Key));
			}
		}
	};

	typedef std::unordered_map<hash_t, IniSection>::iterator IniSectionIter;

	struct IniFile
	{
		StringData IniName;
		std::unordered_map<hash_t, IniSection> FileSec;
		std::vector<hash_t>SectionSequence;
		bool Loaded{ false };
		IniFile() = default;
		bool Available()
		{
			return Loaded;
		}
		bool Load(const std::vector<IniToken>& Tokens,const std::string& Name);
		template<typename FileLoader>
		bool Load(const char* fname)
		{
			FileLoader sf;
			sf.Open(fname, FileLoader::GetReadSign());
			if (!sf.Available() || sf.Eof()) return Loaded = false;
			return Load(GetTokensFromFile(sf), fname);
		}
		template<typename FileLoader>
		inline bool Load_ForCCINI(const char* fname)
		{
			FileLoader sf;
			sf.Open(fname, FileLoader::GetReadSign());
			if (!sf.Available() || sf.Eof()) return Loaded = false;
			return Load(CCINIRegenTokens(std::move(GetTokensFromFile(sf))), fname);
		}
		template<typename FileLoader>
		inline bool Load(const FileLoader& sf, const std::string& Name)
		{
			return Load(GetTokensFromFile(sf), Name);
		}
		inline IniSectionIter GetSection(hash_t SecHash)
		{
			return FileSec.find(SecHash);
		}
		inline bool HasSection(hash_t SecHash)
		{
			return GetSection(SecHash) == FileSec.end();
		}
		inline IniSectionIter GetSection(const char* Sec)
		{
			return FileSec.find(StrHash(Sec));
		}
		inline bool HasSection(const char* SecHash)
		{
			return GetSection(SecHash) == FileSec.end();
		}
		inline IniSectionIter GetSection(const std::string& Sec)
		{
			return FileSec.find(StrHash(Sec));
		}
		inline bool HasSection(const std::string& SecHash)
		{
			return GetSection(SecHash) == FileSec.end();
		}
		inline IniSectionIter NotFound()
		{
			return FileSec.end();
		}
		inline bool IsFound(IniSectionIter it)
		{
			return it != FileSec.end();
		}
		const std::string& GetValue(const std::string& Sec, const std::string& Key, const std::string& Default);
	};
}

using ExtIni = Ini::IniFile;

class INIClass;
class CCFileClass;

BOOL INIClass_ReadFile(INIClass* pINI, CCFileClass* pFile);
BOOL INIClass_FromExtIni(INIClass* pINI,ExtIni& Ext);
