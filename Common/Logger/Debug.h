#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <string_view>

namespace Debug
{
	void Log(const char* pFormat, ...);
	void LogAndMessage(const char* pFormat, ...);
	void LogString(const char* pStr);
	void LogString(const std::string& Str);
	void MessageString(const char* pStr);
	void MessageString(const std::string& Str);
	void LogAndMessageString(const char* pStr);
	void LogAndMessageString(const std::string& Str);

	template <class... _Types>
	inline void LogFormat(const std::format_string<_Types...> fmt, _Types&&... args) {
		std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
		LogString(msg);
	}
	inline void LogVFormat(std::string_view fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		LogString(msg);
	}
	template <class... _Types>
	inline void MessageFormat(const std::format_string<_Types...> fmt, _Types&&... args) {
		std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
		MessageString(msg);
	}
	inline void MessageVFormat(const std::string& fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		MessageString(msg);
	}
	template <class... _Types>
	inline void LogFormatAndMessage(const std::format_string<_Types...> fmt, _Types&&... args) {
		std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
		LogAndMessageString(msg);
	}
	inline void LogVFormatAndMessage(const std::string& fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		LogAndMessageString(msg);
	}
}

#define IH_ENTER_FN_LOG(fn,ext) //(Debug::Log("IHCore : Enter hook " #fn " at 0x%08X, Ext call " #ext " at 0x%08X \n",fn,ext)) 

#define IH_EXIT_FN_LOG(fn,ext) //(Debug::Log("IHCore : Exit hook " #fn " at 0x%08X, Ext call " #ext " at 0x%08X \n",fn,ext)) 