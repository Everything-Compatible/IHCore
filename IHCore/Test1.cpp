
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

bool PrintWindow(HWND hd);

//0FE45EC0
void CB1_Impl(int DlgItemID)
{
	static TCHAR stp[2000], stq[2000];

	/*
	HWND HN = nullptr;
	HWND hd = GetWindow(Game::hWnd, GW_CHILD);
	//HWND hd = GetWindow(hd0, GW_CHILD);
	while (hd != NULL)                    //循环得到所有的子窗口
	{
		ZeroMemory(stp, 2000);
		//ZeroMemory(stq, 2000);
		GetClassName(hd, stp, 2000);
		if (StrCmpW(stp, L"#32770"))
		{
			HN = hd;
			MessageBoxA(NULL, "HNHN", "FUCK", MB_OK);
		}
		//GetWindowText(hd, stq, 2000);
		//swprintf_s(stq, L"hWnd = %d, DlgCtrlID = %d, CurDlgID = %d", hd, GetDlgCtrlID(hd), DlgItemID);
		//MessageBoxW(NULL, (wchar_t*)DlgItemID, stp, MB_OK);
		hd = GetNextWindow(hd, GW_HWNDNEXT);
	}//*/
	/*
	HWND HNew = GetDlgItem(HN, 1448);
	if (HNew != NULL)
	{
		PrintWindow(HNew);
	}
	else
	{
		PrintWindow(Game::hWnd);
	}*/

	/*
	HWND hd = GetDesktopWindow();        //得到桌面窗口
	hd = GetWindow(hd, GW_CHILD);        //得到屏幕上第一个子窗口
	DWORD dwid;
	while (hd != NULL)                    //循环得到所有的子窗口
	{
		GetWindowThreadProcessId(hd, &dwid);
		GetClassName(hd, BW, 1000);
		if (dwid == pi.dwProcessId && !wcscmp(BW, L"Notepad2U"))
		{
			tgWnd = hd;
			//MessageBoxW(nullptr, L"DETECT!", L"DetectWindow", MB_OK);
			break;
		}
		hd = GetNextWindow(hd, GW_HWNDNEXT);
	}
	if (tgWnd != NULL)break;
	else Sleep(RetryWaitMsec);//重试
	*/
	//HWND HNew = GetDlgItem(Game::hWnd, DlgItemID);
	//if (HNew != NULL)
	//{
	//	MessageBoxW(NULL, L"kala", L"dkfd", MB_OK);
	//	ZeroMemory(stp, 2000);
	//	GetDlgItemText(Game::hWnd, DlgItemID, stp, 2000);
	//	MessageBoxW(NULL, std::to_wstring(DlgItemID).c_str(), stp, MB_OK);
	//}
}
/*
DEFINE_HOOK(604116, Game_DialogFunc_CB1, 5)
{
	GET(int, DlgItemID, EAX);

	if (DlgItemID == -1) {
		return 0x604120;
	}
	CB1_Impl(DlgItemID);
	return 0x6040FC;
}//*/

DEFINE_HOOK(0x4A60E4, PrintText01, 6)
{
	GET(wchar_t*, Text, EAX);
	R->ESP(R->ESP() - 0x408);//sub esp,408h

	if (Text)
	{
		//MessageBoxW(NULL, Text, L"dkfd", MB_OK);
		if (!StrCmpW(GameData::MPSelectedMapName(), Text))
		{
			//GameData::OpenWebsite("https://www.baidu.com", true);
			//MessageBoxW(NULL, Text, L"dkfd", MB_OK);
		}
	}
	return 0;
}
DEFINE_HOOK(0x4A61C4, PrintText02, 6)
{
	GET(wchar_t*, Text, EAX);
	R->ESP(R->ESP() - 0x40C);//sub esp,40Ch

	if (Text)
	{
		//MessageBoxW(NULL, Text, L"dkfd", MB_OK);
		if (!StrCmpW(GameData::MPSelectedMapName(), Text))
		{
			//MessageBoxW(NULL, Text, L"dkfd", MB_OK);
			
		}
	}
	return 0;
}
//Seems to be unused 
DEFINE_HOOK(0x5BD3F4, OutText01, 5)
{
	GET(int, IStr, ESI);
	GET(int, V7, EBP);

	//MessageBoxA(NULL, (LPCSTR)&IStr, "dkfd", MB_OK);
	
	return (V7 == 1) ? 0x5BD3F9 : 0x5BD419;
}


//我不知道这东西是否真的会被调用
extern "C"
{
	int GetWindowTextA_Alt(HWND hWnd, LPSTR lpString, int nMaxCount)
	{
		MessageBoxW(NULL, L"KKSK", L"kokosuki", MB_OK);
		return GetWindowTextA(hWnd, lpString, nMaxCount);
	}
}
DEFINE_HOOK(0x5D4F31, CatchGetWindowTextA, 6)
{
	R->EBX(GetWindowTextA_Alt);
	MessageBoxW(NULL, L"CatchGetWindowTextA", L"kokosuki", MB_OK);
	return 0x5D4F37;
}

//在弹窗时会被调用
DEFINE_HOOK(0x4C841E, CatchSetDlgItemTextA, 5)
{
	GET(HWND, hDlg, ESI);
	const int nIDDlgItem = 0x484;
	const char* pBuffer = reinterpret_cast<const char*>(0x8A3A08);
	//MessageBoxW(NULL, L"CatchSetDlgItemTextA", L"kokosuki", MB_OK);
	//SetDlgItemTextA(hDlg, nIDDlgItem, pBuffer);
	SetDlgItemTextA(hDlg, nIDDlgItem, "Fuck You!!");
	return 0x4C8424;
}


char StringBuf[0x1000];

/*
DEFINE_HOOK(0x73952116, OriginalDebugLog, 5)
{
	return 0x408530;
}*/

/*
DEFINE_HOOK(0x4068E0, OriginalDebugLog_PossibleFix, 5)
{
	return 0x4068E5;
}
DEFINE_HOOK(0x4068E5, OriginalDebugLog, 5)//0x4A4AF9
{
	GET_STACK(const char*, LogStr, 0x4);
	//va_list args;
	//args = (va_list)R->StackPtr<int>(0x4) + _INTSIZEOF(char*);
	//vsprintf_s(StringBuf, LogStr, args);
	//DbgLog.AddLog_CurTime(false);
	//DbgLog.AddLog(StringBuf);
	//va_end(args);
	DbgLog.AddLog_CurTime(false);
	DbgLog.AddLog(StringBuf);
	return 0;
	//return 0x408530;
}*/
/*
DEFINE_HOOK(0x4A60EA,PrintText01, 4)
{
	//REF_STACK(wchar_t*, Text, 0x8);
	if (Text)return 0x4A615D;
	else return 0x4A60EE;
}*/

/*
DEFINE_HOOK(60411B, Game_DialogFunc_Subtext_Load, 5)
{
	GET(int, DlgItemID, EAX);

	Dialogs::StatusString = nullptr;
	if(DlgItemID == -1) {
		return 0x604120;
	}
	if(DlgItemID >= ARES_GUI_START) {
		switch(DlgItemID) {
			case ARES_CHK_RMG_URBAN_AREAS:
				Dialogs::StatusString = "STT:RMGUrbanAreas";
				break;
			case ARES_CHK_MULTIENGINEER:
				Dialogs::StatusString = "STT:MultiEngineer";
				break;
			default:
				Dialogs::StatusString = "GUI:Debug";
		}
		return 0x604135;
	}
	return 0x604126;
}


DEFINE_HOOK(604136, Game_DialogFunc_Subtext_Propagate, 5)
{
	if(Dialogs::StatusString) {
		R->EAX(Dialogs::StatusString);
		return 0x60413B;
	}
	return 0;
}
*/
bool PrintWindow(HWND hd)
{
	bool Ret = true;
	static char  AW[5000], BW[5000],CW[100000];
	std::string ss;
	DWORD dwid;
	GetWindowThreadProcessId(hd, &dwid);
	GetWindowTextA(hd, AW, 5000);
	GetClassNameA(hd, BW, 5000);
	//SELECTOR

	ZeroMemory(CW, 100000);
	sprintf(CW,"Name=\"%s\" Class=\"%s\" HWND=%X PID=%X \n", AW, BW, (int)hd, dwid);
	ss += CW;
	LRESULT L = (int)SendMessageA(hd, WM_GETTEXTLENGTH, 0, 0);
	LPCSTR Buf = new CHAR[L + 2];
	if (SendMessageTimeoutA(hd, WM_GETTEXT, (WPARAM)(L + 2), (LPARAM)Buf, SMTO_NORMAL, 1000, NULL))
	{
		if (strlen(Buf) != 0)
		{
			ZeroMemory(CW, 100000);
			sprintf(CW, "Text=\"%s\"\n", Buf);
			ss += CW;
		}
	}
	delete[]Buf;
	MessageBoxA(NULL, ss.c_str(), "FUCK", MB_OK);
	return Ret;
}
/*
DEFINE_HOOK(604136, Game_DialogFunc_CB, 5)
{
	GET(int, DlgItemID, EAX);

	//DSurface::Tile->DrawText(L"Tile", 10, 10, COLOR_RED);// | COLOR_GREEN//Tile会落在战场的绘制层上 几乎不刷新
	//DSurface::Sidebar->DrawText(L"Sidebar", 10, 10, COLOR_RED);
	//DSurface::Primary->DrawText(L"Primary", 10, 10, COLOR_RED);//Primary是战场，但是图层在上面 会刷新
	//DSurface::Hidden->DrawText(L"Hidden", 10, 10, COLOR_RED);
	//DSurface::Alternate->DrawText(L"Alternate", 10, 10, COLOR_RED);
	//DSurface::Temp->DrawText(L"Temp", 10, 10, COLOR_RED);
	//DSurface::Composite->DrawText(L"Composite", 10, 10, COLOR_RED);
	//UI::StandardWndProc
	/*
	HWND HNew = GetDlgItem(Game::hWnd, DlgItemID);
	if (HNew != NULL)
	{
		MessageBoxW(NULL, L"kala", L"dkfd", MB_OK);
	}*/
	/*
	if (Dialogs::StatusString) {
		R->EAX(Dialogs::StatusString);
		return 0x60413B;
	}
	return 0;
}*/

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
		
		//ExtIni Rules;
		//Rules.Load<ExtCCFile>("Rulesmd.ini");

		//SessionClass::Instance->Config.MapDescription[0] = L'囸';
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
		if (CurrentFrame() < LastFrame)
		{
			GameStart();
		}0x4D0;
		LastFrame = CurrentFrame();
		DrawGlobalTimer();
		Local::ExecutorRunLoop();
		Local::LocalFrameUpdate();

		ECDebug::FlushCommands();
	}
}
/*
	DSurface::Tile->DrawText(L"Tile", 10, 10, COLOR_RED);// | COLOR_GREEN//Tile会落在战场的绘制层上 几乎不刷新
		DSurface::Sidebar->DrawText(L"Sidebar", 10, 10, COLOR_RED);
		DSurface::Primary->DrawText(L"Primary", 10, 10, COLOR_RED);//Primary是战场在前缓冲，但是图层在上面 会刷新
		DSurface::Hidden->DrawText(L"Hidden", 10, 10, COLOR_RED);
		DSurface::Alternate->DrawText(L"Alternate", 10, 10, COLOR_RED);
		DSurface::Temp->DrawText(L"Temp", 10, 10, COLOR_RED);
		DSurface::Composite->DrawText(L"Composite", 10, 10, COLOR_RED);
	
	static constexpr reference<DSurface*, 0x8872FCu> const Tile {};
	static constexpr reference<DSurface*, 0x887300u> const Sidebar {};
	static constexpr reference<DSurface*, 0x887308u> const Primary {};
	static constexpr reference<DSurface*, 0x88730Cu> const Hidden {};
	static constexpr reference<DSurface*, 0x887310u> const Alternate {};
	static constexpr reference<DSurface*, 0x887314u> const Temp {};
	static constexpr reference<DSurface*, 0x88731Cu> const Composite {};

	static constexpr reference<RectangleStruct, 0x886F90u> const SidebarBounds {};
	static constexpr reference<RectangleStruct, 0x886FA0u> const ViewBounds {};
	static constexpr reference<RectangleStruct, 0x886FB0u> const WindowBounds {};
*/
/* 这个INJ里面删了
DEFINE_HOOK(0x6F9E50, IHTechnoClassUpdate, 5)
{
	using namespace Test;
	//::MessageBoxA(NULL, "CAONIMA21", "CAONIMABI21", MB_OK);
	//GET(TechnoClass* const, pThis, ECX);
	if (CurrentFrame() != LastFrame)
	{
		FrameUpdate();
	}
	return 0;
}*/

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

	//IH_ENTER_FN_LOG(UpdateFrame, nullptr);
	/*
	if (!strncmp(pThis->GetType()->ID, "GAPOWR", 24))
	{
		::MessageBoxA(NULL, UTF8toANSI(pThis->GetType()->UINameLabel).c_str(), "tch", MB_OK);
		char cch[10000];
		CCINIClass::INI_Rules()->GetString("GAPOWR", "UIName", cch);
		::MessageBoxA(NULL, UTF8toANSI(cch).c_str(), "tch2", MB_OK);

	}//*/
	/*
	if (pThis->IsSelected)
	{
		GameData::ShowInternalErrorDialog();
		pThis->Deselect();
	}//*/
	//IH_EXIT_FN_LOG(UpdateFrame, nullptr);
	return 0;
}

DEFINE_HOOK(0x52B390, IHHasSec, 5)
{
	//GET(INIClass::IndexType* const, pThis, ECX);
	/*
	GET_STACK(const char*, pName, 0x4);
	GET_STACK(int, i1, 0x0);
	GET_STACK(int, i2, 0x4);
	GET_STACK(int, i3, 0x8);
	GET_STACK(int, i4, 0xC);
	GET_STACK(int, i5, 0x10);
	GET_STACK(int, i6, 0x14);
	GET_STACK(int, i7, 0x18);
	GET_STACK(int, i8, 0x1C);
	GET_STACK(int, i9, 0x20);
	GET_STACK(int, i10, -4);
	GET_STACK(int, i11, -8);
	Debug::Log("Get Sec %d %d %d %d %d %d %d %d %d %d %d",i11,i10, i1, i2, i3, i4, i5,i6,i7,i8,i9);
	*/
	//Debug::Log("Getting Section ID %d\n", pName);
	return 0;
}



