
#include "EC.h"

bool HasSyringeIH()
{
	return SyringeData::HasSyringeIH();
}
bool IHAvailable();
bool HasIH()
{
	return IHAvailable();
}
#ifndef SIWIC
bool HasWIC()
{
	return SITool::Available();
}
#endif

std::unordered_map<std::string, LibFuncHandle> ListenerMap;

LibFuncHandle __cdecl ListenerAccess(const char* Name)
{
	auto it = ListenerMap.find(Name);
	if (it != ListenerMap.end())return it->second;
	else return nullptr;
}
namespace ECListener
{
	void Set(const char* ListenerType, LibFuncHandle Function)
	{
		ListenerMap[ListenerType] = Function;
	}
	LibFuncHandle Get(const char* ListenerType)
	{
		return ListenerAccess(ListenerType);
	}
	void Remove(const char* ListenerType)
	{
		if (ListenerMap.find(ListenerType) != ListenerMap.end())ListenerMap.erase(ListenerType);
	}

	InitialLoad::Service<InitialLoadParam_RegisterFunction>InternalService_ListenerAccess("EC::Internal::ListenerAccess");

	std::vector<LibFuncHandle> GetAll(const char* ListenerType)
	{
		std::vector<LibFuncHandle> Result;
		InternalService_ListenerAccess.RefreshAndProcess([&](const InitialLoadParam_RegisterFunction& Request)
			{
				if (Request.Handle)
				{
					auto hd = AsType<decltype(ListenerAccess)>(Request.Handle)(ListenerType);
					if (hd)Result.push_back(hd);
				}
			});
		return Result;
	}

	void Listen_LoadBeforeTypeData(Listener_OnLoadGame Func)
	{
		Set("EC::OnReadingRules", Func);
	}
	void Listen_LoadAfterTypeData(Listener_OnLoadGame Func)
	{
		Set("EC::LoadAfterTypeData", Func);
	}

	void Listen_BeginWritingExceptIH(Listener_BeginWritingExceptIH Func)
	{
		Set("EC::BeginWritingExceptIH", Func);
	}

	void Listen_LoadCSFString(Listener_LoadCSFString Func)
	{
		Set("EC::OnLoadCSFString", Func);
	}

	void Listen_LoadScenario(Listener_LoadScenario Func)
	{
		Set("EC::OnLoadScenario", Func);
	}

	void Listen_AfterLoadGame(Listener_AfterLoadGame Func)
	{
		Set("EC::AfterLoadGame", Func);
	}

	void Listen_AfterLoadINI(Listener_AfterLoadINI Func)
	{
		Set("EC::AfterLoadINI", Func);
	}

	void Listen_ClearScenario(Listener_ClearScenario Func)
	{
		Set("EC::ClearScenario", Func);
	}
}
