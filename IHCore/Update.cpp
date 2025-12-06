
#include <string>
#include <YRPP.h>
#include <Helpers/Macro.h>
#include <CommandClass.h>

#include "Version.h"
#include <ToolFunc.h>
#include "SomeData.h"
#include "Debug.h"
#include "Global.h"
#include "ExtIni.h"
#include "ECDbgConsole.h"

namespace Local
{
	void ExecutorRunLoop();
	void LocalFrameUpdate();
	void ResetAll();
}

namespace Test
{
	//reference<wchar_t, 0xA8B34F> RA;

	//DEFINE_HOOK(0x6BC0CD, IHStartA, 5)
	//DEFINE_HOOK(0x48CCC0, Main_Game, 8)
	void GameStart()
	{
		Local::ResetAll();
	}

	int CurrentFrame()
	{
		return Unsorted::CurrentFrame();
	}
	int LastFrame = INT_MAX;

	void DrawGlobalTimer()
	{
		//DSurface::Temp->DrawText(std::to_wstring(CurrentFrame()).c_str(), 10, 10, COLOR_RED);
		//DSurface::Temp->DrawText(PRODUCT_FULLNAME_WSTR, 10, 10, COLOR_RED);
		//DSurface::Temp->DrawText(SessionClass::Instance->Config.MapDescription, 10, 30, COLOR_RED);
		//DSurface::Temp->DrawText(GameData::MPMapSelPage_SelectedMapName, 10, 30, COLOR_RED);
		//DSurface::Temp->DrawText(std::to_wstring(SessionClass::Instance->Config.MapDescription).c_str(), 10, 50, COLOR_RED);
		//DSurface::Temp->DrawText(GameData::MPLatestRamdomMap(), 10, 50, COLOR_RED);
	}

	void FrameUpdate()
	{
		void RemoteCommFrameUpdate();

		if (CurrentFrame() < LastFrame)
		{
			GameStart();
		}
		LastFrame = CurrentFrame();
		DrawGlobalTimer();
		Local::ExecutorRunLoop();
		Local::LocalFrameUpdate();

#ifdef CalcEx_UnitTest
		if (CurrentFrame() == 10)
		{
			MessageListClass::Instance->PrintMessage(StringTable::LoadString("MessageList_UnitTest_String"), 0x100);
		}
#endif // CalcEx_UnitTest



		RemoteCommFrameUpdate();
	}
}

DEFINE_HOOK(0x6D4684, UpdateFrame, 6)
{
	//IH_ENTER_FN_LOG(UpdateFrame, Test::FrameUpdate);
	Test::FrameUpdate();
	//IH_EXIT_FN_LOG(UpdateFrame, Test::FrameUpdate);
	return 0;
}

DEFINE_HOOK(0x6F9E50, IHTechnoClassUpdate, 5)
{
	GET(TechnoClass* const, pThis, ECX);
	return 0;
}

/*
DEFINE_HOOK(0x52B390, IHHasSec, 5)
{
	return 0;
}
*/



