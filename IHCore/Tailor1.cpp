

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

        // const wchar_t* VersionDescription = L"���������������֮սv��0.7.3�ȶ��棬mod����1Ⱥ��559496825�����ߣ�wen_rui1245�������ĺ�";
        const wchar_t* txt_before_ver = wcsstr(StringTable::LoadString("TXT_BEFORE_VERSION"), L"MISSING:") ? L"��л���桸����������������֮�� v" : StringTable::LoadString("TXT_BEFORE_VERSION");
        const wchar_t* txt_after_ver = wcsstr(StringTable::LoadString("TXT_AFTER_VERSION"), L"MISSING:") ? L"������MOD��ȫ��ѣ����õ�����ף����Ϸ��죡" : StringTable::LoadString("TXT_AFTER_VERSION");

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
