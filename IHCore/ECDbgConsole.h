#pragma once
#include <functional>
#include "InfoStack.h"
#include "ExtJson.h"

using DbgCommand = std::function<std::u8string(void)>;

namespace ECExec
{
	bool IsConsoleLocked();

	bool LockConsole();

	bool UnlockConsole();

	bool TryLockConsole();
}

namespace ECCommand
{
	extern std::unordered_map<std::u8string, std::u8string> GlobalVariables;

	void ReturnString(const std::u8string& Str);

	void ReturnError(const std::u8string& Str, int Code);

	void DoNotEcho();

	void ReturnStdError(long Code);

	void SetGlobalVar(const std::u8string& Key, const std::u8string& Value);

	void SetErrorCode(int Code);

	int GetErrorCode();

	void GetLastResult(std::u8string& Ret, std::u8string& ErrorStr, int& ErrorCode);

	const std::u8string& GetVar(const std::u8string& Key);
}

namespace ECDebug
{
	extern InfoStack<DbgCommand> CommandStack;
	extern InfoStack<std::u8string> CommandOutput;

	void FlushCommands();

	bool HasConsole();

	bool IsConsoleOpen();

	bool OpenDebugConsole();

	bool CloseDebugConsole();
}