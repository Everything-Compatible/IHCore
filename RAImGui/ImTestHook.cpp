
#include <YRPP.h>
#include "Version.h"
#include "ImTest.h"
#include <Helpers/Macro.h>


int rnmtq = 0;
int rd2 = 0;
int rd3 = 0;

/*
void TestLibList()//UNUSED
{
	HMODULE Lib = LoadLibraryW(L"Patches\\HPLibList.dll");
	if (Lib == NULL)rnmtq += 1;
	else
	{
		int(__cdecl * GetNLibs)(void);
		int(__cdecl * GetMaxLibs)(void);
		bool(__cdecl * RegisterEntry)(DWORD);
		const DWORD* (__cdecl * GetEntries)(void);

#define GetFunc(x) {x=(decltype(x))GetProcAddress(Lib, #x);\
			if(x==nullptr)rnmtq+=10;}

		GetFunc(GetNLibs);
		GetFunc(GetMaxLibs);
		GetFunc(RegisterEntry);
		GetFunc(GetEntries);

		if (rnmtq != 2)
		{
			if (GetMaxLibs() != 20000)rnmtq += 100;
			for (int i = 0; i <= 1000; i++)
			{
				if (!RegisterEntry(i))rnmtq += 1000;
			}
			if (GetNLibs() != 1000)rnmtq += 100000000;
			rd3 = GetNLibs();
			rd2 = *(GetEntries() + 443);
		}
	}
}*/

DEFINE_HOOK(0x531413, IHGameStart, 5)
{
	DSurface::Hidden->DrawText(PRODUCT_FULLNAME_STR L" is active!", 10, 460, COLOR_RED | COLOR_GREEN);
	//Sleep(1000);
	//DSurface::Hidden->DrawText((std::to_wstring(rnmtq)+L" & "+std::to_wstring(rd2) + L" & " + std::to_wstring(rd3)).c_str(), 10, 440, COLOR_RED | COLOR_GREEN);
	return 0;
}

/*
DEFINE_HOOKEX(0x6BC0CD, IHLoadRA2MD, 5, 114514, "hhhaaaaaaa")
{
	//ImTest_Full(0, nullptr);
	//ImTest_Full(0, nullptr);
	//ImTest_Full(0, nullptr);
	return 0;
}*/

extern int Temp_Update;

/*
DEFINE_HOOK(0x6BC0CD, IHStartA, 5)
{
	Temp_Update = 0;
	return 0;
}
//* /

DEFINE_HOOK(0x48CCC0, Main_Game, 8)
{
	//::MessageBoxA(NULL, "CAONIMA2", "CAONIMABI2", MB_OK);
	/*
	if (Ares::bTestingRun)
	{
		//		TestScaffold::FloatToIntTest();
		return 0x48CFCB;
	}// * /
	Temp_Update = 0;
	return 0;
}*/

void ImTest_UpdateCheck()
{
	static bool first{ true };
	if (first)
	{
		ImTest_SetUp();
		first = false;
	}


	//DSurface::Composite->DrawText(L"ImTest_UpdateCheck", 10, 30, COLOR_RED);
	//DSurface::Primary->DrawText(L"AVERYLONGTEXTAVERYLONGTEXTAVERYLONGTEXTAVERYLONGTEXTAVERYLONGTEXT", 300, 50, COLOR_RED);
	//::MessageBoxA(NULL, "CAONIMA1", "CAONIMABI1", MB_OK);
	static bool Loaded{ false };
	//if (Temp_Update == 0)
	{
		Loaded = true;
		/*
		if (Loaded)
		{
			ImTest_CleanUp();
			Loaded = false;
		}
		if (ImTest_SetUp())
		{
			Loaded = true;
		}*/
	}
	//Temp_Update++;
	//if (Loaded)
	{
		if (!ImTest_MainLoop() || ImTest_ShouldClose())
		{
			ImTest_CleanUp();
			ImTest_SetUp();
			//ImTest_CleanUp();
			//Loaded = false;
		}
	}
}

/*

DEFINE_HOOK(0x6C7921, SendStatisticsPacket_AddField_MyId, 0x6)
{
	::MessageBoxA(NULL, "CAONIMA", "CAONIMABI", MB_OK);
	return 0;
}*/
//DEFINE_HOOK(0x6C856C, SendStatisticsPacket_WriteStatisticsDump, 0x5)

namespace Test
{
	int CurrentFrame()
	{
		return Unsorted::CurrentFrame();
	}
	int LastFrame = 0;

	void FrameUpdate()
	{
		ImTest_UpdateCheck();
		ImTest_DrawOnGameUI();
	}
}

DEFINE_HOOK(0x6F9E50, IHTechnoClassUpdate, 5)
{
	//using namespace Test;
	//::MessageBoxA(NULL, "CAONIMA21", "CAONIMABI21", MB_OK);
	//GET(TechnoClass* const, pThis, ECX);

	/*
	auto CurFrame = CurrentFrame();
	if (CurFrame != LastFrame)
	{
		if (LastFrame > CurFrame)
		{
			Temp_Update = 0;
		}
		LastFrame = CurFrame;
		FrameUpdate();
	}*/
	return 0;
}


DEFINE_HOOK(0x4F4583, GScreenClass_DrawText, 0x6)
{
	/*
auto wanted = Drawing::GetTextDimensions(Phobos::VersionDescription, { 0,0 }, 0, 2, 0);

RectangleStruct rect = {
	DSurface::Composite->GetWidth() - wanted.Width - 10,
	0,
	wanted.Width + 10,
	wanted.Height + 10
};

Point2D location{ rect.X + 5,5 };

DSurface::Composite->FillRect(&rect, COLOR_BLACK);*/
//DSurface::Composite->DrawText(Phobos::VersionDescription, &location, COLOR_RED);
	//DSurface::Composite->DrawText(L"ImTest_IHTestLogic001", 10, 50, COLOR_RED);
	//Test::FrameUpdate();
	return 0;
}

DEFINE_HOOK(0x6D4684, TacticalClass_Draw_FlyingStrings, 6)
{
	//FlyingStrings::UpdateAll();
	//Test::FrameUpdate();
	return 0;
}
