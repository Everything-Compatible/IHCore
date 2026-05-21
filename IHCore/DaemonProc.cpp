#include "ECDbgConsole.h"
#include "ToolFunc.h"
#include "ECInterprocess.h"
#include "StringManagerExt.h"
#include "InitialLoad.h"
#include <IH.h>

extern std::atomic_bool SyringeDaemonMonitor_Connecting;
extern std::unique_ptr<std::thread> SyringeDaemonMonitorThread;
bool IsReadable(HANDLE hProc, LPCVOID Ptr);
bool IsExecutable(HANDLE hProc, LPCVOID Ptr);

struct AddressCommentSource
{
	const char* Name{ nullptr };
	AddressCommentProvider Handle{ 0 };
};

std::vector<AddressCommentSource> AddressCommentProviders;
std::vector<AddressCommentInfo> AddressList;

std::vector<std::string> SplitLines(const std::string& Str)
{
	std::vector<std::string> Lines;
	size_t Start = 0;
	while (true)
	{
		size_t End = Str.find('\n', Start);
		if (End == std::string::npos)
		{
			Lines.push_back(Trim(Str.substr(Start)));
			break;
		}
		else
		{
			Lines.push_back(Trim(Str.substr(Start, End - Start)));
			Start = End + 1;
		}
	}
	return Lines;
}

std::string MakeAddressComment(const AddressCommentInfo& AddrInfo)
{
	std::string Result;
	for (auto& Provider : AddressCommentProviders)
	{
		auto Str = Provider.Handle(AddrInfo);

		if (strlen(Str))
		{
			std::string s;
			s += Provider.Name;
			s += " : ";
			s += Str;

			Result += s;
			Result.push_back('\n');
		}

		free((void*)Str);
	}
	if(!Result.empty())
		Result.pop_back();
	return Result;
}

void ProcessAndReportAddressList()
{
	std::vector<std::string> StringList;
	std::vector<const char*> L;

	AddressCommentProviders.clear();
	Service_RegisterAddressCommentProvider.RefreshAndProcess([](const auto& Func)
		{
			AddressCommentProviders.push_back(AddressCommentSource{
					Func.Name, (AddressCommentProvider)Func.Handle
				});
		});

	StringList.reserve(AddressList.size());
	for (auto& AddrInfo : AddressList)
	{
		StringList.push_back(MakeAddressComment(AddrInfo));
	}


	L.reserve(AddressList.size());
	for (auto& S : StringList)
		L.push_back(S.c_str());
	JsonFile J(cJSON_CreateStringArray(L.data(), (int)L.size()));
	ProcessSyringeRequestToJSON("FlushDumpInfo", std::move(J));
}

void DaemonProcessAddress()
{
	//Debug::Log("Daemon Report : \n%s\n", UnicodetoUTF8(SyringeData::GetDaemonReport()).c_str());
	AddressList.clear();

	auto Report = UnicodetoUTF8(SyringeData::GetDaemonReport());
	if (!Report.empty())
	{
		auto Lines = SplitLines(Report);
		
		if (Lines.size() >= 1)
		{
			//Line 1 : Address List
			JsonFile J;
			J.Parse(Lines[0]);
			if (J.Available())
			{
				auto Arr = J.GetObj().GetArrayInt();
				auto hProc = GetCurrentProcess();
				for (auto Addr : Arr)
				{
					AddressCommentInfo Item;
					Item.Addr = Addr;
					Item.CanRead = IsReadable(hProc, (LPCVOID)Addr);
					Item.CanExecute = IsExecutable(hProc, (LPCVOID)Addr);
					AddressList.push_back(Item);
				}
			}
		}
	}
	
	ProcessAndReportAddressList();
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

void DaemonMinimalWork()
{
	SyringeData::DaemonConnect();
	if (SyringeData::IsDaemonConnected())
	{
		Debug::Log("[EC] Syringe Daemon Connected.\n");
	}
	else
	{
		Debug::Log("[EC] Failed to Connect Syringe Daemon.\n");
		return;
	}

	Game::IsActive = false;

	DaemonProcessAddress();

	SyringeData::DaemonDisconnect();
}