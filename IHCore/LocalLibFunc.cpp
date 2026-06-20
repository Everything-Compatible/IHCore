
#include "..\Common\LocalData.h"
#include <Windows.h>
#include "Debug.h"
#include "SomeData.h"
#include <Version.h>
#include <YRPP.h>
#include "StringManagerExt.h"
#include "ECInterprocess.h"
#include "ECDbgConsole.h"
#include <iostream>
#include <shellapi.h>

const char8_t* GetTextDrawVariable(const std::u8string_view Key);

void __cdecl ShowIEDialog(void)
{
	GameData::ShowInternalErrorDialog();
}

bool __cdecl OpenWebsite(const char* Site)
{
	return GameData::OpenWebsite(Site, TRUE);
}


std::u8string FormatMessageU8(DWORD ErrorValue)
{
	wchar_t Buffer[1024];
	DWORD Result = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ErrorValue, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Buffer, 1024, NULL);
	if (Result == 0)
	{
		return u8"Unknown error";
	}
	return ~UnicodetoUTF8(Buffer);
}

void __cdecl AddMoney(JsonObject obj)
{
	auto Args = obj.ItemArrayString("Args");
	if (Args.empty())return;
	auto money = strtol(Args[0].c_str(), nullptr, 10);
	HouseClass::CurrentPlayer->GiveMoney(money);
	printf("Added Money : %ld\n", money);
}

//跳帧指令的现存问题：
//只能影响超武和触发计数器
/*
void __cdecl TickFrame(JsonObject obj)
{
	//ticking some frames forward
	auto Args = obj.ItemArrayString("Args");
	if (Args.empty())return;
	auto frames = strtol(Args[0].c_str(), nullptr, 10);
	if (frames <= 0)return;
	int& frame = Unsorted::CurrentFrame;
	frame += frames;
	printf("Ticked Frames : %ld\n", frames);
}
*/

//整活指令之让你看广告
void __cdecl WatchAD(JsonObject obj)
{
	//puts(obj.GetText().c_str());
	auto Args = obj.ItemArrayString("Args");
	if (Args.empty())return;
	//puts(Args[0].c_str());
	ShellExecuteW(NULL, L"open", UTF8toUnicode(Args[0]).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void __cdecl AutoWrapTextEx(
	class BitFont* _In_ Font,
	const wchar_t* _In_ Str,
	int _In_ MaxPixels,
	int _In_ MaxChars,
	bool _In_ WordWrap,
	int& _Out_ Len,
	const wchar_t*& _Out_ StrBefore,
	const wchar_t*& _Out_ StrAfter
);

std::unordered_map<std::string, FuncInfo>IHCore_Funcs
{
	{"IEDialog",FuncInfo(ShowIEDialog ,FuncType::Procedure)},
	{"WatchAD",FuncInfo(WatchAD ,FuncType::Action, true, true)},
	{"AddMoney",FuncInfo(AddMoney ,FuncType::Action, true, true)},

	{"OpenWebsite",FuncInfo(OpenWebsite ,FuncType::Default)},//bool (__cdecl*)(const char*)
	{"GetCSFString",FuncInfo(StringManagerExt::GetCSFString ,FuncType::Default)},
	{"AutoWrapTextEx",FuncInfo(AutoWrapTextEx, FuncType::Default) }
};

FuncInfo* IHCore_GetFunc(const char* Name, int Version)
{
	if (Version != DoNotCheckVersion && PRODUCT_VERSION < Version)return nullptr;
	auto it = IHCore_Funcs.find(Name);
	if (it == IHCore_Funcs.end())return nullptr;
	else return &it->second;
}

namespace Internal
{
	void __cdecl SetValue(JsonObject Context)
	{
		for (auto&& [Key, Value] : Context.GetMapString())
		{
			ECCommand::SetGlobalVar(~Key, ~Value);
		}
	}

	void __cdecl ListGlobalVars(void)
	{
		for (const auto& [Key, Value] : ECCommand::GlobalVariables)
		{
			std::cout << ~Key << " = " << ~Value << std::endl;
		}
	}

	void __cdecl GetDrawTextValue(JsonObject Context)
	{
		auto s = Context.GetObjectItem("Name");
		if (s.Available() && s.IsTypeString())
		{
			const char8_t* Str = GetTextDrawVariable(conv s.GetCString());
			if (Str)
			{
				ECCommand::ReturnString(Str);
			}
			else
			{
				ECCommand::ReturnStdError(ERROR_INVALID_INDEX);
			}
		}
		else
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
		}
	}

	void __cdecl GetDrawTextValueII(JsonObject Context)
	{
		auto Args = Context.ItemArrayString("Args");
		if (Args.empty())
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
			return;
		}

		if (Args.size() == 1)
		{
			const char8_t* Str = GetTextDrawVariable(~Args[0]);
			if (Str)
			{
				ECCommand::ReturnString(Str);
			}
			else
			{
				ECCommand::ReturnStdError(ERROR_INVALID_INDEX);
			}
			return;
		}

		bool AllAvailable = true;
		for(auto& Arg : Args)
		{
			const char8_t* Str = GetTextDrawVariable(~Arg);
			printf("%s = %s\n", Arg.c_str(), Str ? conv Str : "(Not Found)");
			AllAvailable = AllAvailable && Str;
		}

		if (AllAvailable)
		{
			ECCommand::DoNotEcho();
			ECCommand::ReturnString(u8"");
		}
		else
		{
			ECCommand::DoNotEcho();
			ECCommand::ReturnStdError(ERROR_INVALID_INDEX);
		}
	}

	void __cdecl GetCSFString(JsonObject Context)
	{
		auto Args = Context.ItemArrayString("Args");
		if (Args.size() != 1)
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
			return;
		}

		auto& Arg = Args[0];
		const wchar_t* Str = StringManagerExt::GetCSFString(Arg.c_str());
		if (Str)
		{
			ECCommand::ReturnString(~UnicodetoUTF8(Str));
		}
		else
		{
			ECCommand::ReturnStdError(ERROR_INVALID_INDEX);
		}
	}

	void __cdecl PrintArg(JsonObject Context)
	{
		auto Text = Context.GetText();
		auto TextEx = Context.GetTextEx();
		std::cout << TextEx;
		ECCommand::DoNotEcho();
		ECCommand::ReturnString(~Text);
	}

	void __cdecl RepeatArg(JsonObject Context)
	{
		auto Args = Context.ItemArrayString("Args");
		std::string Result;
		for (auto& Arg : Args)
		{
			Result += Arg;
			Result += " ";
		}
		if (!Result.empty())
		{
			Result.pop_back();
		}
		std::cout << "\033[1;33m" << Result << "\033[0m";
		ECCommand::DoNotEcho();
		ECCommand::ReturnString(~Result);
	}

	void __cdecl Print(JsonObject Context)
	{
		auto s = Context.GetObjectItem("Text");
		if (s.Available() && s.IsTypeString())
		{
			ECCommand::ReturnString(~s.GetString());
		}
		else
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
		}
	}

	void __cdecl Exit(JsonObject Context)
	{
		auto s = Context.GetObjectItem("Code");
		if (s.Available() && s.IsTypeNumber())
		{
			int Code = s.GetInt();
			ExitProcess(Code);
		}
		else
		{
			ExitProcess(0);
		}
	}

	void __cdecl Resume()
	{
		SyringeData::DaemonDisconnect();
	}

	void __cdecl KABOOM()
	{
		//This function is used to test the crash handler
		//It will cause a crash
		int* p = nullptr;
		*p = 0; //This will cause an access violation
	}

	void __cdecl Clear()
	{
		//cls
		//printf("\033[2J\033[H");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole == INVALID_HANDLE_VALUE) return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

		DWORD dwSize = csbi.dwSize.X * csbi.dwSize.Y;
		COORD coordScreen = { 0, 0 };
		DWORD dwWritten;

		// 用空格覆盖整个缓冲区
		FillConsoleOutputCharacter(hConsole, L' ', dwSize, coordScreen, &dwWritten);
		// 恢复默认文本属性
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwSize, coordScreen, &dwWritten);
		// 将光标移到左上角
		SetConsoleCursorPosition(hConsole, coordScreen);
	}

	bool __cdecl Ping(JsonObject Context)
	{
		//ping a remote component to check if it's alive
		//-Name  
		//[-TimeOut] default to 5000ms

		auto oName = Context.GetObjectItem("Name");
		if (!oName)
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
			return false;
		}
		if (!oName.IsTypeString())
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
			return false;
		}
		auto Name = oName.GetString();
		auto oTimeOut = Context.GetObjectItem("TimeOut");
		int TimeOut = -1;
		if (oTimeOut)
		{
			if (!oTimeOut.IsTypeNumber())
			{
				ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			TimeOut = oTimeOut.GetInt();
		}

		auto pCom = RemoteComponentManager::GetComponentByName(~Name);
		if (!pCom)
		{
			ECCommand::ReturnStdError(ERROR_INVALID_NAME);
			return false;
		}

		auto [Result, Delay] = pCom->Ping(TimeOut);

		if (Result)
		{
			std::cout << "Pinged Component " << Name << " successfully, Delay: " << Delay << "ms";
		}
		else
		{
			std::cout << "Pinged Component " << Name << " failed, Time Out";
		}

		ECCommand::DoNotEcho();

		return Result;
	}

	bool WildcardMatch(const char* pattern, const char* str)
	{
		// 递归终止：两个指针都到末尾
		if (*pattern == '\0' && *str == '\0') return true;
		// pattern 到末尾但 str 没到 → 不匹配
		if (*pattern == '\0') return false;

		if (*pattern == '*') {
			// 跳过连续的 *
			while (*(pattern + 1) == '*') pattern++;
			// * 匹配零个字符 → pattern+1 vs str
			if (WildcardMatch(pattern + 1, str)) return true;
			// * 匹配一个或多个字符 → pattern vs str+1（str 非空时）
			return (*str != '\0') && WildcardMatch(pattern, str + 1);
		}

		// ? 匹配任意单个字符，普通字符精确匹配
		if (*pattern == '?') {
			return (*str != '\0') && WildcardMatch(pattern + 1, str + 1);
		}

		return (*pattern == *str) && WildcardMatch(pattern + 1, str + 1);
	}

	bool ParseVersionCmp(const std::string& expr, int& outOp, int& outVer)
	{
		// 解析 >5, >=10, <3, <=8, =7, !=2 等
		// outOp: 0='>', 1='>=', 2='<', 3='<=', 4='=', 5='!='
		if (expr.empty()) return false;
		const char* s = expr.c_str();
		if (*s == '>') {
			s++;
			if (*s == '=') { outOp = 1; s++; }
			else outOp = 0;
		} else if (*s == '<') {
			s++;
			if (*s == '=') { outOp = 3; s++; }
			else outOp = 2;
		} else if (*s == '=') {
			s++;
			outOp = 4;
		} else if (*s == '!') {
			s++;
			if (*s != '=') return false;
			s++;
			outOp = 5;
		} else {
			return false;
		}
		if (*s == '\0') return false;
		try {
			outVer = std::stoi(s);
		} catch (...) {
			return false;
		}
		return true;
	}

	bool VersionCmpPass(int op, int targetVer, int actualVer)
	{
		switch (op) {
		case 0: return actualVer >  targetVer;
		case 1: return actualVer >= targetVer;
		case 2: return actualVer <  targetVer;
		case 3: return actualVer <= targetVer;
		case 4: return actualVer == targetVer;
		case 5: return actualVer != targetVer;
		default: return false;
		}
	}

	bool __cdecl FindLib(JsonObject Context)
	{
		// FindLib -Name <string> 支持 *? 通配符
		//         [-Version >/</>=/<=/=/!=<int>]
		//         [-LSV     >/</>=/<=/=/!=<int>]

		auto oName = Context.GetObjectItem("Name");
		if (!oName || !oName.IsTypeString())
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
			return false;
		}
		auto NamePattern = oName.GetString();

		bool hasVerCmp = false;
		int  verOp = 0, verVal = 0;
		auto oVer = Context.GetObjectItem("Version");
		if (oVer && oVer.IsTypeString())
		{
			if (!ParseVersionCmp(oVer.GetString(), verOp, verVal))
			{
				ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			hasVerCmp = true;
		}

		bool hasLSVCmp = false;
		int  lsvOp = 0, lsvVal = 0;
		auto oLSV = Context.GetObjectItem("LSV");
		if (oLSV && oLSV.IsTypeString())
		{
			if (!ParseVersionCmp(oLSV.GetString(), lsvOp, lsvVal))
			{
				ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
				return false;
			}
			hasLSVCmp = true;
		}

		bool foundAny = false;

		for (auto& [libName, pLib] : Local::LibMap)
		{
			if (!pLib || !pLib->Available) continue;

			if (!WildcardMatch(NamePattern.c_str(), libName.c_str()))
				continue;

			auto* info = pLib->Out->Info;
			if (!info) continue;

			if (hasVerCmp && !VersionCmpPass(verOp, verVal, info->Version))
				continue;
			if (hasLSVCmp && !VersionCmpPass(lsvOp, lsvVal, info->LowestSupportedVersion))
				continue;

			auto& var = Local::Var_Lib;
			auto Name = info->LibName;
			//.Ver .LSV .Desc .Dep
			using namespace std::string_literals;
			auto& Var_Ver = var[Name + ".Ver"s];
			auto& Var_LSV = var[Name + ".LSV"s];
			auto& Var_Desc = var[Name + ".Desc"s];
			auto& Var_Dep = var[Name + ".Dep"s];

			std::cout << "----------------------------------------" << std::endl;
			std::cout << "Library: " << Name << std::endl;
			std::cout << "  Version: " << ~Var_Ver << std::endl;
			std::cout << "  Lowest Supported Version: " << ~Var_LSV << std::endl;
			std::cout << "  Description: " << ~Var_Desc << std::endl;
			std::cout << "  Dependencies: " << ~Var_Dep << std::endl;

			foundAny = true;
		}

		if (!foundAny)
		{
			std::cout << "No matching libraries found." << std::endl;
		}

		ECCommand::DoNotEcho();
		return foundAny;
	}

	void __cdecl DebugLogLine(JsonObject Context)
	{
		auto s = Context.GetObjectItem("Line");
		if (s.Available() && s.IsTypeString())
		{
			Debug::Log("%s\n", ~s.GetString());
			ECCommand::DoNotEcho();
		}
		else
		{
			ECCommand::ReturnStdError(ERROR_BAD_ARGUMENTS);
		}
	}
}

std::unordered_map<std::string, FuncInfo>Internal_Funcs
{
	{"SetValue",FuncInfo(Internal::SetValue ,FuncType::Action)},
	{"ListGlobalVars",FuncInfo(Internal::ListGlobalVars ,FuncType::Procedure)},
	{"GetCSFString",FuncInfo(Internal::GetCSFString ,FuncType::Action, true, true)},
	{"GetDrawTextValue",FuncInfo(Internal::GetDrawTextValue ,FuncType::Action)},
	{"TextVar",FuncInfo(Internal::GetDrawTextValueII ,FuncType::Action, true, true)},
	{"Clear", FuncInfo(Internal::Clear ,FuncType::Action)},
	{"PrintArg",FuncInfo(Internal::PrintArg ,FuncType::Action)},
	{"RepeatArg",FuncInfo(Internal::RepeatArg ,FuncType::Action, true, true)},
	{"Exit",FuncInfo(Internal::Exit ,FuncType::Action)},
	{"Print",FuncInfo(Internal::Print ,FuncType::Action)},
	{"Resume",FuncInfo(Internal::Resume ,FuncType::Procedure)},
	{"KABOOM",FuncInfo(Internal::KABOOM ,FuncType::Procedure)},
	{"Ping",FuncInfo(Internal::Ping ,FuncType::Condition)},
	{"FindLib",FuncInfo(Internal::FindLib ,FuncType::Condition)},
	{"DebugLog",FuncInfo(Internal::DebugLogLine ,FuncType::Action)},
};

std::vector<std::string> GetInternalSupportedFunctions()
{
	std::vector<std::string> Ret;
	for (const auto& [Name, Info] : Internal_Funcs)
	{
		Ret.push_back(Name);
	}
	return Ret;
}

FuncInfo* Internal_GetFunc(const char* Name, int Version)
{
	if (Version != DoNotCheckVersion && PRODUCT_VERSION < Version)return nullptr;
	auto it = Internal_Funcs.find(Name);
	if (it == Internal_Funcs.end())return nullptr;
	else return &it->second;
}


std::variant<JsonFile, std::string, std::monostate> ProcessSyringeRequestToJSON(const std::string& Method, JsonFile&& Arguments)
{
	/*
	Request:
	{
		"Method" : string,
		"Arguments" : Object
	}
	Result:
	{
		"Response" : string,
		"Error" : int
	}
	*/
	JsonFile ArgJson;
	auto Obj = ArgJson.GetObj();
	Obj.AddString("Method", Method);
	Obj.AddObjectItem("Arguments", std::move(Arguments));

	auto Message = ArgJson.GetObj().GetText();

	//printf(conv u8"\033[1;31mSending Message \033[1;34m\"%s\" \033[1;31mto Syringe...\033[0m\n", Message.c_str());

	auto Result = SyringeData::SendRequestMessage(Message);

	if (Result.empty())
		return std::monostate{};

	JsonFile ResultJson;
	auto ErrorStr = ResultJson.ParseTmpChecked(std::move(Result), conv u8"\033[33m【错误位置】\033[31m");

	if (!ErrorStr.empty())
		return std::move(ErrorStr);

	else 
		return std::move(ResultJson);
}

GenCallRetType ProcessSyringeRequest(const std::string& Method, JsonFile&& Arguments)
{
	auto ResultVariant = ProcessSyringeRequestToJSON(Method, std::move(Arguments));

	if (std::holds_alternative<std::monostate>(ResultVariant))
	{
		ECCommand::ReturnStdError(ERROR_INVALID_FUNCTION);
		return GenCallRetType::Void;
	}
	else if (std::holds_alternative<std::string>(ResultVariant))
	{
		auto& ErrorStr = std::get<std::string>(ResultVariant);
		ECCommand::ReturnError(~ErrorStr, ERROR_INVALID_DATA);
		return GenCallRetType::Void;
	}

	auto& ResultJson = std::get<JsonFile>(ResultVariant);

	auto ResponseObj = ResultJson.GetObj().GetObjectItem("Response");
	auto ErrorObj = ResultJson.GetObj().GetObjectItem("Error");

	if (!ResponseObj || !ResponseObj.IsTypeString() || !ErrorObj || !ErrorObj.IsTypeNumber())
	{
		ECCommand::ReturnStdError(ERROR_INVALID_DATA);
		return GenCallRetType::Void;
	}

	auto Response = ResponseObj.GetString();

	if (ErrorObj.GetInt() != 0)
	{
		ECCommand::ReturnError(~Response, ErrorObj.GetInt());
		return GenCallRetType::Void;
	}

	if (!_strcmpi(Response.c_str(), "true"))
		return GenCallRetType::True;
	if (!_strcmpi(Response.c_str(), "false"))
		return GenCallRetType::False;
	ECCommand::ReturnString(~Response);
	return GenCallRetType::Void;
}
GenCallRetType ProcessSyringeRequestAlt(const std::string& Method, GeneratorParam Arg)
{
	JsonFile ArgJson;
	auto Obj = ArgJson.GetObj();
	Obj.CreateObject();
	Obj.AddInt("Version", Arg.ExecutorVersion);
	Obj.AddString("Pointer", std::to_string(Arg.Ptr));
	Obj.AddString("AbsTypeStr", AbstractClass::GetClassName(Arg.Type));
	Obj.AddInt("AbsType", (int)Arg.Type);
	return ProcessSyringeRequest(Method, std::move(ArgJson));
}