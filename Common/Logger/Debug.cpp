#include "Debug.h"
#include <CRT.h>
#include <MessageListClass.h>

namespace Debug
{
	void Log(const char* pFormat, ...)
	{
		JMP_STD(0x4068E0);
	}
	void LogAndMessage(const char* pFormat, ...)
	{
		va_list args;
		va_start(args, pFormat);
		char buffer[0x1000];
		vsnprintf(buffer, 0x1000, pFormat,args);
		va_end(args);
		LogString(buffer);
		MessageString(buffer);
	}
	void LogString(const char* pStr)
	{
		Log("%s", pStr);
	}
	void LogString(const std::string& Str)
	{
		Log("%s", Str.c_str());
	}
	void MessageString(const char* pStr)
	{
		wchar_t buffer[0x1000];
		CRT::mbstowcs(buffer, pStr, 0x1000);
		static_cast<MessageListClass&>(MessageListClass::Instance).PrintMessage(buffer);
	}
	void MessageString(const std::string& Str)
	{
		wchar_t buffer[0x1000];
		CRT::mbstowcs(buffer, Str.c_str(), 0x1000);
		static_cast<MessageListClass&>(MessageListClass::Instance).PrintMessage(buffer);
	}
	void LogAndMessageString(const char* pStr)
	{
		LogString(pStr);
		MessageString(pStr);
	}
	void LogAndMessageString(const std::string& Str)
	{
		LogString(Str);
		MessageString(Str);
	}
}