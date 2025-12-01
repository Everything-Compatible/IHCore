#include <string>
#include <YRPP.h>
#include <CRT.h>
#include <Helpers/Macro.h>
#include <CommandClass.h>
#include <charconv>
#include "Version.h"
#include <ToolFunc.h>
#include "SomeData.h"
#include "Debug.h"
#include "Global.h"
#include "Patch.h"
#include "ExtCD.h"

char SimpleMBToWC_Old(wchar_t* Target, const char* Src, int nChars)
{
	char TmpChr; // al

	for (TmpChr = *Src; *Src; TmpChr = *Src)
	{
		if (!nChars)
			break;
		*Target++ = (unsigned __int8)TmpChr;
		++Src;
		if (nChars != -1)
			--nChars;
	}
	*Target = 0;
	return TmpChr;
}

char SimpleMBToWC_New(wchar_t* Target, const char* Src, int nChars)
{
	char TmpChr;
	auto ws = UTF8toUnicode(Src);
	int i = 0;
	for (TmpChr = *Src; *Src; TmpChr = *Src, i++)
	{
		if (!nChars)
			break;
		*Target++ = ws[i];
		++Src;
		if (nChars != -1)
			--nChars;
	}
	*Target = 0;
	return TmpChr;
}

std::wstring IH_MBToWC_Ex(const char* Src)
{
	//::MessageBoxA(NULL, Src, "IH01", MB_OK);
	if (strlen(Src) <= 2)return UTF8toUnicode(Src);
	bool IsNew = false, IsUTF8 = false;
	//if (Src[0] == '#')
	{
		//char sc[1000];
		//Debug::Log("SimpleMBToWC_Ex : # %X %X %X %X", (unsigned)(unsigned char)Src[1], (unsigned)(unsigned char)Src[2], (unsigned)(unsigned char)Src[3], (unsigned)(unsigned char)Src[4]);
		//sprintf_s(sc, "SimpleMBToWC_Ex : # %X %X %X %X", (unsigned)(unsigned char)Src[1], (unsigned)(unsigned char)Src[2], (unsigned)(unsigned char)Src[3], (unsigned)(unsigned char)Src[4]);
		//::MessageBoxA(NULL, sc, "IH0Ex", MB_OK);
	}
	if (!strncmp(Src, "：", 2))
	{
		IsNew = true;
		IsUTF8 = false;
	}
	else if (strlen(Src) > 2 && !strncmp(Src, (const char*)u8"：", 3))
	{
		IsNew = true;
		IsUTF8 = true;
	}
	//::MessageBoxA(NULL, std::string(2, UC1).c_str(), "IH02", MB_OK);
	if(!IsNew)return UTF8toUnicode(Src);
	return (IsUTF8 ? UTF8toUnicode : ANSItoUnicode)(Src + (IsUTF8 ? 3 : 2));
}

std::wstring IH_MBToWC_Ex(const std::string& Src)
{
	return IH_MBToWC_Ex(Src.c_str());
}

char SimpleMBToWC_Ex(wchar_t* Target, const char* Src, int nChars)
{ 
	//::MessageBoxA(NULL, Src, "x01", MB_OK); 
	if (strlen(Src) <= 2)return SimpleMBToWC_Old(Target, Src, nChars);

	bool IsNew = false, IsUTF8 = false;
	if (Src[0] == '#')
	{
		//char sc[1000];
		//Debug::Log("SimpleMBToWC_Ex : # %02X %02X %02X %02X", (int)sc[1], (int)sc[2], (int)sc[3], (int)sc[4]);

	}
	if (!strncmp(Src, "：", 2))
	{
		IsNew = true;
		IsUTF8 = false;
	}
	else if (strlen(Src) > 2 && !strncmp(Src, (const char*)u8"：", 3))
	{
		IsNew = true;
		IsUTF8 = true;
	}

	if(!IsNew)return SimpleMBToWC_New(Target, Src, nChars);
	//::MessageBoxA(NULL, BoolCStr(IsUTF8), "01", MB_OK);
	char TmpChr; // al
	std::wstring ws;
	if (IsUTF8)ws = UTF8toUnicode(Src + 3);
	else ws = ANSItoUnicode(Src + 2);
	auto Tg = Target;
	//::MessageBoxW(NULL, ws.c_str(), L"03", MB_OK);
	int i = 0;
	Src += 2; nChars -= 2; 
	for (TmpChr = *Src; *Src; TmpChr = *Src,i++)
	{
		if (!nChars)
			break;
		*Target++ = ws[i];
		++Src;
		if (nChars != -1)
			--nChars;
	}
	*Target = 0;
	//::MessageBoxW(NULL, Tg, L"04", MB_OK);
	return TmpChr;
}


DEFINE_HOOK(0x735060, SimpleANSItoUTF16, 5)
{
	GET(wchar_t*, Dest, ECX);
	GET(char*, Src, EDX);
	GET(int, Len, ESI);
	//IH_ENTER_FN_LOG(SimpleANSItoUTF16, SimpleMBToWC_Ex);
	//Debug::Log(__FUNCTION__": Src \"%s\"\n", Src);
	R->AL(SimpleMBToWC_Ex(Dest,Src,Len));
	//IH_EXIT_FN_LOG(SimpleANSItoUTF16, SimpleMBToWC_Ex);
	return 0x73508C;
}

reference<char, 0x889F64> DefaultStr;
//829284 :  No Name

int safe_string_to_int(std::string_view str, int default_value) noexcept {
	int value;
	auto result = std::from_chars(str.data(), str.data() + str.size(), value);
	if (result.ec != std::errc{} || result.ptr != str.data() + str.size()) {
		return default_value;
	}
	return value;
}

char* WWSB_Trim_New(char* Str);

static void ReadYRMData_Rewritten(_In_ const char* FileName,
	_Out_ wchar_t* Desc,
	_Out_ char* Digest,
	_Out_ char* GameModes,
	_Out_ bool& IsOfficialMap,
	_Out_ int& MinPlayers,
	_Out_ int& MaxPlayers)
{
	//rewrite version of ReadYRMData
	//to boost performance 
	IsOfficialMap = true;
	MinPlayers = 2;
	MaxPlayers = 4;
	strcpy(GameModes, &DefaultStr);

	//streaming the whole file until [Basic] 
	//discard [Digest] section since no digest is also acceptable

	ExtCCFile File;
	File.Open(FileName, ExtCCFile::GetReadSign());
	if (!File.Available())return;

	std::vector<char> CurrentLine;
	CurrentLine.resize(512);
	bool InBasicSection = false;
	BYTE c;
	auto pcc = File.GetRaw();
	std::wstring ws1;

	//start streaming
	while (pcc->ReadBytes(&c, 1))
	{
		if (c == '\r' || c == '\n')
		{
			//analyze the current line
			

			//end of line
			CurrentLine.push_back('\0');
			//remove comment
			for (auto& c : CurrentLine)
				if (c == ';')c = '\0';
			//trim spaces
			auto LineStr = CurrentLine.data();
			LineStr = WWSB_Trim_New(LineStr);
			//check section
			size_t Len = strlen(LineStr);
			if (Len)
			{
				if (
					LineStr[0] == '[' ||
					LineStr[Len - 1] == ']'
					)
				{
					if (strcmp(LineStr, "[Basic]") == 0)
					{
						InBasicSection = true;
					}
					else
					{
						if (InBasicSection)
						{
							//we are leaving Basic section
							//generate Desc
							wchar_t dst[100];
							if (ws1.empty())ws1 = L"No Name";
							if (MinPlayers == MaxPlayers)swprintf_s(dst, 88, L"%ls (%d)", ws1.c_str(), MinPlayers);
							else swprintf_s(dst, 88, L"%ls (%d-%d)", ws1.c_str(), MinPlayers, MaxPlayers);
							StrCpyNW(Desc, dst, 44);
							//done
							break;
						}
						InBasicSection = false;
					}
				}
				else if (InBasicSection)
				{
					//key=value
					auto EqualPos = strchr(LineStr, '=');
					if (EqualPos)
					{
						*EqualPos = '\0';
						auto Key = WWSB_Trim_New(LineStr);
						auto Value = WWSB_Trim_New(EqualPos + 1);
						
						//Keys:
						//Name, GameMode, Official, MinPlayer, MaxPlayer
						if (strcmp(Key, "Name") == 0)
						{
							ws1 = IH_MBToWC_Ex(Value);
						}
						else if (strcmp(Key, "GameMode") == 0)
						{
							strcpy(GameModes, Value);
						}
						else if (strcmp(Key, "Official") == 0)
						{
							IsOfficialMap = IsTrueString(Value);
						}
						else if (strcmp(Key, "MinPlayer") == 0)
						{
							MinPlayers = safe_string_to_int(Value, 2);
						}
						else if (strcmp(Key, "MaxPlayer") == 0)
						{
							MaxPlayers = safe_string_to_int(Value, 4);
						}
					}
				}
			}



			//then clear and continue
			CurrentLine.clear();
		}
		else
		{
			CurrentLine.push_back(c);
		}
	}

	

	//digest
	strcpy(Digest, "No Digest");
}

static void ReadYRMData_EXT(_In_ const char* FileName,
	_Out_ wchar_t* Desc,
	_Out_ char* Digest,
	_Out_ char* GameModes,
	_Out_ bool* IsOfficialMap,
	_Out_ int* MinPlayers,
	_Out_ int* MaxPlayers)
{
	//::MessageBoxA(NULL, FileName, "CTX", MB_OK);
	ReadYRMData_Rewritten(FileName, Desc, Digest, GameModes, *IsOfficialMap, *MinPlayers, *MaxPlayers);
	Debug::Log("[IH] Read YRM Data : \n    File \"%s\"\n    Desc \"%s\"\n    Digest \"%s\"\n    GameModes \"%s\"\n    IsOfficialMap %d\n    MinPlayers %d\n    MaxPlayers %d\n",
		FileName, UnicodetoUTF8(Desc).c_str(), Digest, GameModes, *IsOfficialMap, *MinPlayers, *MaxPlayers);
}


DEFINE_HOOK(0x6994F0, ReadYRMData, 5)
{
	GET(char*, FileName, ECX);
	GET(wchar_t*, Desc, EDX);
	REF_STACK(char*, Digest, 0x4);
	REF_STACK(char*, GameModes, 0x8);
	REF_STACK(bool*, IsOfficialMap, 0xC);
	REF_STACK(int*, MinPlayers, 0x10);
	REF_STACK(int*, MaxPlayers, 0x14);
	IH_ENTER_FN_LOG(ReadYRMData, ReadYRMData_EXT);
	ReadYRMData_EXT(FileName, Desc, Digest, GameModes, IsOfficialMap, MinPlayers, MaxPlayers);
	IH_EXIT_FN_LOG(ReadYRMData, ReadYRMData_EXT);
	return 0x69997A;
}

bool IsSpaceEx(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

char* WWSB_Trim_New(char* Str)
{
	bool Ex = false;
	if (strstr(Str, (const char*)u8"："))Ex = true;
	//if(Ex)::MessageBoxA(NULL, UTF8toANSI(Str).c_str(), "IHTr1", MB_OK);
	
	char* _Str;
	char Cur;
	char* StrCursor;
	signed int i;

	_Str = Str;
	if (Str)
	{
		Cur = *Str;
		StrCursor = Str;
		if (*Str)
		{
			do
			{
				if (!IsSpaceEx(Cur))break;
				//if (Cur > 0x20 || Cur < 0x0)break;
				Cur = *++StrCursor;
			} while (Cur);
			if (StrCursor != Str)
				strcpy(Str, StrCursor);
		}
		for (i = strlen(Str) - 1; i >= 0; --i)
		{
			if (!*StrCursor)
				break;
			if (!IsSpaceEx(_Str[i]))break;
			//if (_Str[i] > 0x20 || _Str[i] < 0x0)break;
			_Str[i] = 0;
		}
	}
	if (Ex)
	{
		//::MessageBoxA(NULL, UTF8toANSI(_Str).c_str(), "IHTr2", MB_OK);
		//strcpy(_Str, UTF8toANSI(_Str).c_str());
		//::MessageBoxA(NULL, _Str, "IHTr3", MB_OK);
	}
	return _Str;
}

DEFINE_HOOK(0x727CF0, WWSB_Trim, 5)
{
	GET(char*, Str, ECX);
	//IH_ENTER_FN_LOG(WWSB_Trim, WWSB_Trim_New);
	R->EAX(WWSB_Trim_New(Str));
	//IH_EXIT_FN_LOG(WWSB_Trim, WWSB_Trim_New);
	return 0x727D56;
}



