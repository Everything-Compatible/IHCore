
#include "..\Common\LocalData.h"
#include <Windows.h>
#include "Debug.h"
#include "SomeData.h"
#include <Version.h>
#include <YRPP.h>
#include "StringManagerExt.h"
#include "ECDbgConsole.h"
#include <iostream>

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


std::unordered_map<std::string, FuncInfo>IHCore_Funcs
{
	{"ShowIEDialog",FuncInfo(ShowIEDialog ,FuncType::Procedure)},
	{"OpenWebsite",FuncInfo(OpenWebsite ,FuncType::Default)},//bool (__cdecl*)(const char*)
	{"GetCSFString",FuncInfo(StringManagerExt::GetCSFString ,FuncType::Default)}
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

	void __cdecl GetCSFString(JsonObject Context)
	{
		auto s = Context.GetObjectItem("Name");
		if (s.Available() && s.IsTypeString())
		{
			const wchar_t* Str = StringManagerExt::GetCSFString(s.GetCString());
			if (Str)
			{
				ECCommand::ReturnString(~UnicodetoUTF8(Str));
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

	void __cdecl PrintArg(JsonObject Context)
	{
		auto Text = Context.GetText();
		auto TextEx = Context.GetTextEx();
		std::cout << TextEx;
		ECCommand::DoNotEcho();
		ECCommand::ReturnString(~Text);
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
}


std::unordered_map<std::string, FuncInfo>Internal_Funcs
{
	{"SetValue",FuncInfo(Internal::SetValue ,FuncType::Action)},
	{"ListGlobalVars",FuncInfo(Internal::ListGlobalVars ,FuncType::Procedure)},
	{"GetCSFString",FuncInfo(Internal::GetCSFString ,FuncType::Action)},
	{"GetDrawTextValue",FuncInfo(Internal::GetDrawTextValue ,FuncType::Action)},
	{"PrintArg",FuncInfo(Internal::PrintArg ,FuncType::Action)},
	{"Exit",FuncInfo(Internal::Exit ,FuncType::Procedure)},
	{"Print",FuncInfo(Internal::Print ,FuncType::Action)},
	{"Resume",FuncInfo(Internal::Resume ,FuncType::Procedure)},
	{"KABOOM",FuncInfo(Internal::KABOOM ,FuncType::Procedure)}
};

FuncInfo* Internal_GetFunc(const char* Name, int Version)
{
	if (Version != DoNotCheckVersion && PRODUCT_VERSION < Version)return nullptr;
	auto it = Internal_Funcs.find(Name);
	if (it == Internal_Funcs.end())return nullptr;
	else return &it->second;
}


GenCallRetType ProcessSyringeRequest(const std::string& Method, JsonFile&& Arguments)
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

	printf(conv u8"\033[1;31mSending Message \033[1;34m\"%s\" \033[1;31mto Syringe...\033[0m\n", Message.c_str());

	auto Result = SyringeData::SendRequestMessage(Message);

	if (Result.empty())
	{
		ECCommand::ReturnStdError(ERROR_INVALID_FUNCTION);
		return GenCallRetType::Void;
	}

	JsonFile ResultJson;
	auto ErrorStr = ResultJson.ParseTmpChecked(std::move(Result), conv u8"\033[33m【错误位置】\033[31m");

	if (!ErrorStr.empty())
	{
		ECCommand::ReturnError(~ErrorStr, ERROR_INVALID_DATA);
		return GenCallRetType::Void;
	}

	auto ResponseObj = ResultJson.GetObj().GetObjectItem("Response");
	auto ErrorObj = ResultJson.GetObj().GetObjectItem("Error");

	if (!ResponseObj.Available() || !ResponseObj.IsTypeString() || !ErrorObj.Available() || !ErrorObj.IsTypeNumber())
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