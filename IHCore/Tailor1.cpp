

#include <Windows.h>
#include <capi.h>
#include <YRPP.h>
#include <cstdio>
#include <Drawing.h>
#include <fstream>

/*
53534C = HangweiMahjong, 7
4F4583 = T1_GScreenClass_DrawText, 6
*/

static const size_t readLength = 128;
char readBuffer[readLength];
constexpr auto NONE_STR = "<none>";
constexpr auto VERSION_INFO = "VersionInfo";
wchar_t VersionDescription[2048];
bool ShowModVersionInfo = true;

DEFINE_HOOK(0x53534C, HangweiMahjong, 0x7)
{
    ShowModVersionInfo = CCINIClass::INI_RA2MD->ReadBool("Users", "ShowModVersionInfo", true);

    if (ShowModVersionInfo) {

        CCINIClass::INI_UIMD->ReadString(VERSION_INFO, "Version", NONE_STR, readBuffer);
        char NOSTR[128];
        strcpy(NOSTR, "NOSTR:");
        strcat(NOSTR, readBuffer);
        const wchar_t* Version = (readBuffer != nullptr && strlen(readBuffer) != 0 && !INIClass::IsBlank(readBuffer)) ? StringTable::LoadString(NOSTR) : L"N/A";

        // const wchar_t* VersionDescription = L"命令与征服：曙光之战vβ0.7.3稳定版，mod交流1群：559496825，作者：wen_rui1245、云游四海";
        const wchar_t* txt_before_ver = wcsstr(StringTable::LoadString("TXT_BEFORE_VERSION"), L"MISSING:") ? L"感谢游玩「命令与征服：落日之光 v" : StringTable::LoadString("TXT_BEFORE_VERSION");
        const wchar_t* txt_after_ver = wcsstr(StringTable::LoadString("TXT_AFTER_VERSION"), L"MISSING:") ? L"」。本MOD完全免费，不得倒卖。祝您游戏愉快！" : StringTable::LoadString("TXT_AFTER_VERSION");

        wcscpy(VersionDescription, txt_before_ver);
        wcscat(VersionDescription, Version);
        wcscat(VersionDescription, txt_after_ver);
    }
    return 0;
}

DEFINE_HOOK(0x4F4583, T1_GScreenClass_DrawText, 0x6)
{
    if (ShowModVersionInfo)
    {
        auto wanted = Drawing::GetTextDimensions(VersionDescription, { 0,0 }, 0, 0, 0);

        RectangleStruct rect = {
            DSurface::Composite->GetWidth() - wanted.Width - 10,
            0,
            wanted.Width + 10,
            wanted.Height + 10
        };

        Point2D location{ rect.X + 5,5 };

        DSurface::Composite->FillRect(&rect, COLOR_BLACK);
        DSurface::Composite->DrawRect(&rect, COLOR_WHITE);
        DSurface::Composite->DrawText(VersionDescription, &location, COLOR_WHITE);
    }
    return 0;
}
