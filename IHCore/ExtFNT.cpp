#include <YRPP.h>
#include <BitFont.h>
#include <Helpers/Macro.h>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <optional>
#include "ToolFunc.h"
#include "Debug.h"
#include "../Common/IHLoader/IH.h"


const wchar_t Magic_StyleAction = 0xFFFF;// L'唐';

const char8_t* GetTextDrawVariable(const std::u8string_view Key)
{
    auto v = IH::GetTextDrawVariable(conv Key.data());
    if (!v)
    {
        auto RouteNamePos = Key.find_first_of(u8'.');
        if (RouteNamePos != std::u8string_view::npos)
        {
            auto router = Key.substr(0, RouteNamePos);
            auto key = Key.substr(RouteNamePos + 1);
            auto routerfn = IH::GetTextDrawRouter(conv std::u8string(router).c_str());
            if (routerfn)
            {
                v = routerfn(conv std::u8string(key).c_str());
                if (v)return v;
                else return nullptr;
            }
            else return nullptr;
        }
        else return nullptr;
    }
    return strlen(conv v) ? v : u8"";
}

std::wstring GetTextDrawVariable(const wchar_t* Key)
{
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
    auto k = UnicodetoUTF8(Key);

    auto v = GetTextDrawVariable(~k);
	return v ? UTF8toUnicode(conv v) : L"MISSING:" + std::wstring(Key);
}

void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        out_r = out_g = out_b = v;
        return;
    }

    h = fmodf(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

void ColorConvertHSVtoRGB(ColorStruct c, ColorStruct& o)
{
    float r, g, b;
    ColorConvertHSVtoRGB(c.R / 255.0F, c.G / 255.0F, c.B / 255.0F, r, g, b);
    o.R = (int)(r * 255.0F); o.G = (int)(g * 255.0F); o.B = (int)(b * 255.0F);
}

short ColorConvertHSVtoCol16(ColorStruct c)
{
    float r, g, b;
    ColorConvertHSVtoRGB(c.R / 255.0F, c.G / 255.0F, c.B / 255.0F, r, g, b);
    return ((((short)(r * 255.0F)) >> 3 << 11) | (((short)(g * 255.0F)) >> 2 << 5) | ((short)(b * 255.0F)) >> 3);
}

struct CurrentDrawStyle
{
    bool HasStyle{ false };
    bool Bold{ false };
    bool StrikeThrough{ false };
    bool Underline{ false };
    bool HasColor{ false };
    bool Random{ false };
    short Col16{ 0 };

    //测试用的函数
    inline std::wstring GetWT() const
    {
        wchar_t s[20];
        std::wstring w{ L"[" };
        w.push_back(HasStyle ? L'T' : L'F');
        w.push_back(Bold ? L'T' : L'F');
        w.push_back(StrikeThrough ? L'T' : L'F');
        w.push_back(Underline ? L'T' : L'F');
        w.push_back(HasColor ? L'T' : L'F');
        w.push_back(HasColor ? L'T' : L'F');
        w.push_back(Random ? L'T' : L'F');
        swprintf_s(s, L"%04X]", ((int)Col16) & 0x0000FFFF);
        w += s;
        return w;
    }
};


wchar_t GetNextChar(const wchar_t*& ws, CurrentDrawStyle& cd)
{
    const static short ColorSet[16] =
    {
        (short)0x0000, (short)0x0015, (short)0x0540, (short)0x0555,
        (short)0xA800, (short)0xA815, (short)0xFD40, (short)0xAD55,
        (short)0x52AA, (short)0x52BF, (short)0x57EA, (short)0x57FF,
        (short)0xFAAA, (short)0xFABF, (short)0xFFEA, (short)0xFFFF,
    };
    bool Styled{ false };
    if (ws[0] == L'§' && ws[1] == L'§')//OK
    {
        ws += 2;
        return L'§';
    }
    if (ws[0] == L'#' && ws[1] == L'#')//OK
    {
        ws += 2;
        return L'#';
    }
    while (*ws == L'§' || *ws == L'#')
    {
        if (*ws == L'§')
        {
            if (ws[1] == L'x' || ws[1] == L'X')
            {
                auto sz = wcsnlen(ws, 6);
#define u(i)  && iswxdigit(ws[i])
                if (sz == 6 u(2)u(3)u(4)u(5))
#undef u
                {
                    int as;
                    swscanf_s(ws + 2, L"%4X", &as);
                    cd.HasColor = true;
                    cd.Bold = cd.StrikeThrough = cd.Underline = false;
                    cd.Col16 = as;
                    ws += 6;
                    Styled = true;
                }
                else break;
            }
            else if (iswxdigit(ws[1]))
            {
                int idx = 0;
                if (iswdigit(ws[1]))idx = ws[1] - L'0';
                else if (iswlower(ws[1]))idx = ws[1] - L'a' + 10;
                else if (iswupper(ws[1]))idx = ws[1] - L'A' + 10;
                cd.HasColor = true;
                cd.Bold = cd.StrikeThrough = cd.Underline = false;
                cd.Col16 = ColorSet[idx];
                ws += 2;
                Styled = true;
            }
            else if (ws[1] == L'y' || ws[1] == L'Y')
            {
                auto sz = wcsnlen(ws, 4);
#define u(i)  && iswdigit(ws[i])
                if (sz == 4 u(2)u(3))
#undef u
                {
                    int as;
                    swscanf_s(ws + 2, L"%2d", &as);
                    cd.HasColor = true;
                    cd.Bold = cd.StrikeThrough = cd.Underline = false;
                    if (as >= ColorScheme::Array->Count)cd.Col16 = 0x0000;
                    else
                    {
                        auto c = (*ColorScheme::Array)[as]->BaseColor;
                        cd.Col16 = ColorConvertHSVtoCol16(c);
                    }
                    ws += 4;
                    Styled = true;
                }
                else break;
            }
            else if (ws[1] == L'l' || ws[1] == L'L')
            {
                ws += 2;
                cd.HasStyle = true;
                cd.Bold = true;
                Styled = true;
            }
            else if (ws[1] == L'm' || ws[1] == L'M')
            {
                ws += 2;
                cd.HasStyle = true;
                cd.StrikeThrough = true;
                Styled = true;
            }
            else if (ws[1] == L'n' || ws[1] == L'N')
            {
                ws += 2;
                cd.HasStyle = true;
                cd.Underline = true;
                Styled = true;
            }
            else if (ws[1] == L'k' || ws[1] == L'K')
            {
                ws += 2;
                cd.HasStyle = true;
                cd.Random = true;
                Styled = true;
            }
            else if (ws[1] == L'r' || ws[1] == L'R')
            {
                ws += 2;
                *((int64_t*)&cd) = 0;
                Styled = true;
                break;
            }
            else break;
        }
        else if (*ws == L'#')
        {
            auto sz = wcsnlen(ws, 7);
#define u(i)  && iswxdigit(ws[i])
            if (sz == 7 u(1)u(2)u(3)u(4)u(5)u(6))
#undef u
            {
                int r, g, b;
                swscanf_s(ws + 1, L"%2X%2X%2X", &r, &g, &b);
                short as = ((r >> 3 << 11) | (g >> 2 << 5) | b >> 3);
                cd.HasColor = true;
                cd.Bold = cd.StrikeThrough = cd.Underline = false;
                cd.Col16 = as;
                ws += 7;
                Styled = true;
            }
            else break;
        }
    }
    if (Styled)
    {
        return Magic_StyleAction;
    }
    else
    {
        ++ws;
        return *(ws - 1);
    }
}

std::wstring GetReplacedDrawStr(const wchar_t* s)
{
    //TextDrawVariable
	//Looks like #{var}
	std::wstring res;
	for (auto p = s; *p;)
	{
		if (p[0] == L'#' && p[1] == L'{')
		{
			auto q = wcschr(p + 2, L'}');
			if (q)
			{
				auto varname = std::wstring(p + 2, q - (p + 2));
				auto varvalue = GetTextDrawVariable(varname.c_str());
				res += varvalue;
				p = q + 1;
			}
			else
			{
				res.push_back(*p);
				++p;
			}
		}
		else
		{
			res.push_back(*p);
			++p;
		}
	}
	return res;
}

struct DrawStyleList
{
    std::vector<CurrentDrawStyle> List;
    std::wstring NewStr;
    int Cur{ 0 };
    size_t CurCharIdx{ 0 };
    wchar_t CurrentChar{ 0 };
    bool HasBitmap{ false };
    BYTE GlyphBuffer[80];
    BYTE* TargetBuf{ nullptr };
    void ChangeStyle()
    {
        ++Cur;
    }
    CurrentDrawStyle GetCur()
    {
        return List[Cur];
    }
    const wchar_t* MakeString(const wchar_t* ws)
    {
        //if (MessageListClass::Instance->GetEditBuffer() == ws)return L"我测你的马";
        CurrentDrawStyle ds{};
        NewStr.clear();
        Cur = 0;
        List.emplace_back();
        auto VarReplaced = GetReplacedDrawStr(ws);
        for (auto p = VarReplaced.c_str(); *p;)
        {
            NewStr.push_back(GetNextChar(p, ds));
            if (NewStr.back() == Magic_StyleAction)
            {
                List.push_back(ds);
                //NewStr += ds.GetWT();
            }
        }
        return NewStr.c_str();
    }
    const wchar_t* MakeStringAlt(const wchar_t* ws)
    {
        //if (MessageListClass::Instance->GetEditBuffer() == ws)return L"我测你的马";
        CurrentDrawStyle ds{};
        NewStr.clear();
        for (auto p = ws; *p;)
        {
            NewStr.push_back(GetNextChar(p, ds));
            if (NewStr.back() == Magic_StyleAction)
            {
                NewStr.pop_back();
                //NewStr += ds.GetWT();
            }
            //if (NewStr.back() == '\r' || NewStr.back() == '\n')
            //    NewStr.push_back(Magic_StyleAction);
        }
        // NewStr.push_back(Magic_StyleAction);
        return NewStr.c_str();
    }
};

namespace DrawStyle
{
    int GlyphLineDelta;
    BYTE MagicGlyphWidth;
    BYTE* MagicGlyphBuf;
    std::vector<BYTE*>GlyphByWidth[16];
    std::map<DWORD, std::unique_ptr<DrawStyleList>> TMap;
    std::mutex TMtx;
    DrawStyleList& GetList()
    {
        TMtx.lock();
        auto& p = TMap[GetCurrentThreadId()];
        if (!p)p.reset(new DrawStyleList);
        TMtx.unlock();
        return *p;
    }
    CurrentDrawStyle GetCurStyle()
    {
        return GetList().GetCur();
    }
    void ChangeStyle()
    {
        GetList().ChangeStyle();
    }
    void ClearStyle()
    {
        TMtx.lock();
        auto& p = TMap[GetCurrentThreadId()];
        p.reset(nullptr);
        TMtx.unlock();
    }
    const wchar_t* MakeString(const wchar_t* ws)
    {
        return GetList().MakeString(ws);
    }
    const wchar_t* MakeStringAlt(const wchar_t* ws)
    {
        return GetList().MakeStringAlt(ws);
    }
    int RedrawGlyph(wchar_t wc, bool ExtendWidth)
    {
        auto& List = GetList();
        List.CurrentChar = wc;
        List.HasBitmap = false;
        auto Cur = List.GetCur();
        if (Cur.HasStyle)
        {
            auto p = BitFont::Instance->GetCharacterBitmap(wc);
            //Debug::Log("[%s]", UnicodetoUTF8(std::wstring(wc, 1)).c_str());
            if (p)
            {
                List.HasBitmap = true;
                List.TargetBuf = p;
                //Debug::Log("[IH] %X 1 TargetBuf = %X\n", GetCurrentThreadId(),  List.TargetBuf);
                memcpy(List.GlyphBuffer, List.TargetBuf, BitFont::Instance->InternalPTR->SymbolDataSize);
                if (Cur.HasStyle && ExtendWidth)
                    List.GlyphBuffer[0]++;
                if (Cur.Bold)
                {
                    for (int i = 0; i < BitFont::Instance->InternalPTR->FontHeight; i++)
                    {
                        int& at = *(int*)(&List.GlyphBuffer[1 + i * DrawStyle::GlyphLineDelta]);
                        auto u1 = at & 0x00FFFFFF;
                        at = (~((u1 << 1) & (u1 >> 1))) & (u1 << 1) | u1 | (at & 0xFF000000);
                    }
                }
                if (Cur.StrikeThrough)
                    *(int*)(&List.GlyphBuffer[1 + 7 * DrawStyle::GlyphLineDelta]) |= 0x00FFFFFF;
                if (Cur.Underline)
                    *(int*)(&List.GlyphBuffer[1 + 15 * DrawStyle::GlyphLineDelta]) |= 0x00FFFFFF;
            }
            else List.HasBitmap = false;
        }
        return Cur.HasColor ? ((int)Cur.Col16 & 0x0000FFFF) : -1;
    }
};

char Magic_Bitmap[80] = { (char)0 };
char Magic_Bitmap_Alt[80] = { (char)0 };

DEFINE_HOOK(0x4346C0, BitFont_GetCharBitmap, 5)
{
    GET_STACK(wchar_t, Char, 0x4);
    if (Char == Magic_StyleAction)
    {
        R->EAX(Magic_Bitmap);
        return 0x4346EE;
    }
    else return 0;
}

DEFINE_HOOK(0x4338DF, InitExtFont, 6)
{
    GET(BitFont*, pFont, ESI);
    for (int i = 0; i < 0x10000; i++)
    {
        auto pBitMap = pFont->GetCharacterBitmap(i);
        if (pBitMap)DrawStyle::GlyphByWidth[pBitMap[0] & 0xF].push_back(pBitMap);
    }
    DrawStyle::GlyphLineDelta = (pFont->InternalPTR->SymbolDataSize - 1) / pFont->InternalPTR->FontHeight;
    DrawStyle::MagicGlyphBuf = pFont->GetCharacterBitmap(Magic_StyleAction);
    if (DrawStyle::MagicGlyphBuf)
    {
        DrawStyle::MagicGlyphWidth = DrawStyle::MagicGlyphBuf[0];
    }
    else
    {
        DrawStyle::MagicGlyphBuf = (BYTE*)pFont->AtlasBuffer;
        DrawStyle::MagicGlyphWidth = pFont->GetCharacterBitmap(L'X')[0];
    }
    return 0;
}

//设置格式
DEFINE_HOOK(0x434120, BitFont_Blit_A, 5)
{
    GET_STACK(wchar_t, Char, 0x4);
    REF_STACK(int, Color16, 0x10);
    auto& List = DrawStyle::GetList();
    List.CurCharIdx++;
    if (Char == Magic_StyleAction)
    {
        List.CurrentChar = Magic_StyleAction;
        DrawStyle::ChangeStyle();
        return 0;// 0x4344FA;
    }
    else
    {
        Color16 = DrawStyle::RedrawGlyph(Char, List.CurCharIdx < List.NewStr.size());
        return 0;
    }
}

DEFINE_HOOK(0x43417A, BitFont_Blit_B, 6)
{
    auto& List = DrawStyle::GetList();
    if (List.CurrentChar == Magic_StyleAction)
    {
        R->EAX(Magic_Bitmap);
        return 0x434194;
    }
    else if (List.GetCur().Random)
    {
        auto pBitMap = BitFont::Instance->GetCharacterBitmap(List.CurrentChar);
        if (pBitMap)
        {
            auto& v = DrawStyle::GlyphByWidth[pBitMap[0] & 0xF];
            if (!v.empty())
            {
                R->EAX(v[Randomizer::Global()(0, v.size() - 1)]);
                return 0x434194;
            }
        }
    }
    else if (List.GetCur().HasStyle)
    {
        R->EAX(List.GlyphBuffer);
        return 0x434194;
    }
    return 0;
}


//恢复格式
DEFINE_HOOK(0x4344E4, BitFont_Blit_C, 7)
{
    auto& List = DrawStyle::GetList();
    List.HasBitmap = false;
    if (DrawStyle::GetCurStyle().HasStyle && List.CurCharIdx < List.NewStr.size())
        R->EAX(R->EAX() - 1);
    return 0;
}

DEFINE_HOOK(0x433CF0, BitFont_GetTextDimension_A, 6)
{
    REF_STACK(const wchar_t*, Text, 0x4);
    //Debug::Log("0 Reading \"%s\"\n", UnicodetoUTF8(Text).c_str());
    Text = DrawStyle::MakeStringAlt(Text);
    //Debug::Log("0 Rendering \"%s\"\n", UnicodetoUTF8(Text).c_str());
    return 0;
};

DEFINE_HOOK(0x433DCE, BitFont_GetTextDimension_B, 7)
{
    GET(int, _Char, EDI);
    ((BYTE*)(BitFont::Instance->AtlasBuffer))[0] = ((wchar_t)_Char == Magic_StyleAction) ? (BYTE)0 : DrawStyle::MagicGlyphWidth;
    return 0;
}



DEFINE_HOOK(0x434500, BitFont_Blit1, 7)
{
    REF_STACK(const wchar_t*, Text, 0x4);
    //Debug::Log("1 Reading \"%s\"\n", UnicodetoUTF8(Text).c_str());
    Text = DrawStyle::MakeString(Text);
    //Debug::Log("1 Rendering \"%s\"\n", UnicodetoUTF8(Text).c_str());
    return 0;
};

DEFINE_HOOK(0x43467F, BitFont_Blit1Ret1, 5)
{
    //Debug::Log("1 CompleteA!\n");
    DrawStyle::ClearStyle();
    return 0;
};

DEFINE_HOOK(0x4346A6, BitFont_Blit1Ret2, 5)
{
    //Debug::Log("1 CompleteB!\n");
    DrawStyle::ClearStyle();
    return 0;
};

DEFINE_HOOK(0x434CD0, BitFont_Blit2, 5)
{
    REF_STACK(const wchar_t*, Text, 0xC);
    //Debug::Log("2 Reading \"%s\"\n", UnicodetoUTF8(Text).c_str());
    Text = DrawStyle::MakeString(Text);
    //Debug::Log("2 Rendering \"%s\"\n", UnicodetoUTF8(Text).c_str());
    return 0;
}

DEFINE_HOOK(0x435157, BitFont_Blit2Ret1, 5)
{
    //Debug::Log("2 CompleteA!\n");
    DrawStyle::ClearStyle();
    return 0;
};

DEFINE_HOOK(0x4352FB, BitFont_Blit2Ret2, 5)
{
    //Debug::Log("2 CompleteB!\n");
    DrawStyle::ClearStyle();
    return 0;
};


#if 0
DEFINE_HOOK(0x4338DF, ResetFontTest, 6)
{
    GET(BitFont*, pFont, ESI);

    Debug::Log("[IH] Resetting Font\n");

    std::chrono::high_resolution_clock cl;
    auto start = cl.now();

    BYTE GlyphBuffer[80];
    GLYPHMETRICS glyph;
    MAT2 mat2 = { {0,1},{0, 0},{0, 0},{0, 1} };
    HDC hdc = CreateCompatibleDC(NULL);
    auto sz = pFont->InternalPTR->SymbolDataSize;
    auto ByteWidth = (sz - 1) / 16;

    HFONT hFont1 = CreateFontW(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"consolas");
    HFONT hFontOriginal = (HFONT)SelectObject(hdc, hFont1);


    //for (int i = 0; i < 0x10000; i++)
    for (int i = 0; i < 0x80; i++)
    {
        auto pBitMap = pFont->GetCharacterBitmap((wchar_t)i);
        if (pBitMap)
        {
            //DWORD szBuffer = GetGlyphOutlineW(hdc, (UINT)i, GGO_BITMAP, &glyph, 0, nullptr, &mat2);
            //GetGlyphOutlineW(hdc, (UINT)i, GGO_BITMAP, &glyph, szBuffer, GlyphBuffer, &mat2);
            GetGlyphOutlineW(hdc, (UINT)i, GGO_BITMAP, &glyph, sizeof(GlyphBuffer), GlyphBuffer, &mat2);
            memset(pBitMap, 0, sz);
            pBitMap[0] = glyph.gmCellIncX;
            glyph.gmptGlyphOrigin.y += 2;
            for (int j = 0; j < glyph.gmBlackBoxY; j++)
            {
                //((UINT*)GlyphBuffer)[j] >>= glyph.gmptGlyphOrigin.x;
            }
            int mm = 16 - std::max(glyph.gmptGlyphOrigin.y, (long)glyph.gmBlackBoxY);
            for (int j = 0; j < glyph.gmBlackBoxY; j++)
            {
                memcpy(pBitMap + 1 + (j + mm) * ByteWidth, GlyphBuffer + j * 4, ByteWidth);
            }
            memset(GlyphBuffer, 0, sizeof(GlyphBuffer));
        }
    }

    SelectObject(hdc, hFontOriginal);
    DeleteObject(hFont1);
    DeleteDC(hdc);

    auto end = cl.now();
    auto nano = (end - start).count();
    auto ns = nano % 1000; nano /= 1000;
    auto μs = nano % 1000; nano /= 1000;
    auto ms = nano % 1000; nano /= 1000;
    auto sec = nano;
    Debug::Log("[IH] Font redirection in %lld s %lld ms %lld μs %lld ns.", sec, ms, μs, ns);

    for (int i = 0; i < 0x10000; i++)
    {
        auto pBitMap = pFont->GetCharacterBitmap((wchar_t)i);
        if (pBitMap)
        {
            //if (*pBitMap > 7);//*pBitMap = 15;
            //else *pBitMap = 7;
        }
    }

    return 0;
}

#endif