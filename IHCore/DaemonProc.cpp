#include "ECDbgConsole.h"
#include "ToolFunc.h"
#include "ECInterprocess.h"
#include "StringManagerExt.h"

extern std::atomic_bool SyringeDaemonMonitor_Connecting;
extern std::unique_ptr<std::thread> SyringeDaemonMonitorThread;

void DaemonProcessAddress()
{
	Debug::Log("Daemon Report : \n%s\n", UnicodetoUTF8(SyringeData::GetDaemonReport()).c_str());
}

void SyringeDaemonMonitorThreadFunc()
{
	SyringeData::DaemonConnect();
	if (SyringeData::IsDaemonConnected())
	{
		printf(conv u8"\033[33m已连接 \033[1;34m%s \033[33m。\033[0m\n", SyringeData::GetExeData().SyringeVersionStr);
	}
	else
	{
		puts(conv u8"\033[1;31m无法连接到Syringe。\033[0m");
		puts(conv u8"\033[33m已退出调试模式。\033[0m");
		return;
	}
	SyringeDaemonMonitor_Connecting = false;

	Game::IsActive = false;

	while (1)
	{
		if (!SyringeData::IsDaemonConnected())return;
		if (SyringeData::ShouldCloseDaemonPipe())break;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	SyringeData::DaemonDisconnect();
}

void DaemonStartWork()
{
	SyringeDaemonMonitor_Connecting = true;
	SyringeDaemonMonitorThread = std::make_unique<std::thread>(SyringeDaemonMonitorThreadFunc);
	SyringeDaemonMonitorThread->detach();
	ECDebug::CommandStack.Clear();
	printf(conv u8"\033[33m进入调试模式……");
	while (SyringeDaemonMonitor_Connecting)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		putchar('.');
	}
	printf("\033[0m");

	DaemonProcessAddress();
}

