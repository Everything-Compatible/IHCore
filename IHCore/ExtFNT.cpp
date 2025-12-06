#include "ExtFNT.h"
#include <Helpers/Macro.h>
#include <vector>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <optional>
#include "ToolFunc.h"
#include "Debug.h"
#include "../Common/IHLoader/IH.h"
#include <EC.Misc.h>

const wchar_t Magic_StyleAction = 0xFFFF;// L'唐';

const wchar_t Magic_ValueClip = 0xFEFF;// L'宋';

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

//take : if k[-2] = Magic_ValueClip, take k[-1] as length (signed short)
//take_len > 0 : [0 ..take_len)
//take_len < 0 : [-take_len ..length)
//take : if k[-3] = Magic_ValueClip, take [k[-2] .. k[-1]) as range
std::wstring GetTextDrawVariable(const wchar_t* Key)
{
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
    std::wstring key = Key;
    //take : if k[-2] = Magic_ValueClip, take k[-1] as length (signed short)
#ifdef CalcEx_UnitTest
    //Debug::Log("[IH] Orig.Key = %s\n", UnicodetoUTF8(key).c_str());
#endif

	bool take = false;
    bool take_range = false;
    short take_begin = 0;
    short take_end = 0;
    short take_len = 0;
    if (key.length() >= 3 && key[key.length() - 3] == Magic_ValueClip)
    {
        take_range = true;
        take_begin = (short)key[key.length() - 2];
		take_end = (short)key[key.length() - 1];
		//remove the magic chars
		key.pop_back();
		key.pop_back();
		key.pop_back();
    }
	else if (key.length() >= 2 && key[key.length() - 2] == Magic_ValueClip)
	{
		take = true;
		take_len = (short)key[key.length() - 1];
		//remove the magic chars
        key.pop_back();
		key.pop_back();
	}

    auto k = UnicodetoUTF8(key);
#ifdef CalcEx_UnitTest
    //Debug::Log("[IH] Key = %s\n", k.c_str());
#endif

    auto v = GetTextDrawVariable(~k);
	auto Value = v ? UTF8toUnicode(conv v) : L"MISSING:" + std::wstring(key);
    if (take_range)
    {
		if (Value.length() > (size_t)take_begin)
		{
			if (Value.length() > (size_t)take_end)
				Value = Value.substr(take_begin, take_end - take_begin);
			else Value = Value.substr(take_begin);
		}
		else Value = L"";
    }
    else if (take)
    {
#ifdef CalcEx_UnitTest
        //Debug::Log("[IH] take %d Orig.Value = %s\n", int(take_len), UnicodetoUTF8(Value).c_str());
#endif
        /*
		take_len > 0 : [0 .. take_len)
		take_len < 0 : [-take_len .. length)
        */
		if (take_len < 0)
		{
			if (Value.length() > (size_t)(-take_len))
				Value = Value.substr(-take_len);
            else Value = L"";
		}
		else
		{
            if (Value.length() > (size_t)(take_len))
                Value = Value.substr(0, take_len);
		}
    }
#ifdef CalcEx_UnitTest
    if (take_range)
    {
		Debug::Log("[IH] take range [%d .. %d) Value = %s\n", int(take_begin), int(take_end), UnicodetoUTF8(Value).c_str());
	}
    else if (take)
    {
        Debug::Log("[IH] take %d Value = %s\n", int(take_len), UnicodetoUTF8(Value).c_str());
    }
    //Debug::Log("[IH] Value = %s\n", UnicodetoUTF8(Value).c_str());
#endif
	return Value;
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

struct LookupVarIndex
{
	static const constexpr size_t None = 0xFFFFFFFF;
    size_t VarIdx;// 0xFFFFFFFF for none
    size_t StartIdx;// relative to the var start
	ptrdiff_t TotalIdx;// relative to the whole string start
};

struct LookupVarInfo
{
    std::wstring VarName;
    int Offset;
    bool UseClipRange;
    int ClipStart;
    int ClipEnd;
    int ExtLen;
};

std::tuple<std::wstring, std::vector<LookupVarIndex>, std::vector<LookupVarInfo>> GetReplacedDrawStrIndexed(const wchar_t* s)
{
    std::vector<LookupVarIndex> VarIdx;
    std::vector<LookupVarInfo> Vars;
    std::wstring res;
    //TextDrawVariable
    //Looks like #{var}
    for (auto p = s; *p;)
    {
        if (p[0] == L'#' && p[1] == L'{')
        {
            auto q = wcschr(p + 2, L'}');
            if (q)
            {
                auto varname = std::wstring(p + 2, q - (p + 2));
                auto varvalue = GetTextDrawVariable(varname.c_str());
                if (varname.length() > 3 && varname[varname.length() - 3] == Magic_ValueClip)
                {
                    auto Begin = (short)varname[varname.length() - 2];
                    auto End = (short)varname[varname.length() - 1];
                    Vars.push_back({ std::move(varname), p - s, true, Begin, End, 6 });
                    varname.pop_back(); varname.pop_back(); varname.pop_back(); 
                }
                else if (varname.length() > 2 && varname[varname.length() - 2] == Magic_ValueClip)
                {
					auto Len = (short)varname[varname.length() - 1];
                    varname.pop_back(); varname.pop_back();
                    Vars.push_back({ std::move(varname), p - s, false, Len < 0 ? -Len : 0 , 0, 5});
                }
                else Vars.push_back({ std::move(varname), p - s, false, 0 , 0, 3});
				//记录变量名和偏移
				
				for (size_t i = 0; i < varvalue.length(); ++i)
					//记录每个字符的变量索引和相对位置
					//p + 2 - s + (int)i : 相对整个字符串的位置
                    VarIdx.push_back({ Vars.size() - 1, i, p + 2 - s + (int)i });
                res += varvalue;
                p = q + 1;
            }
            else
            {
                VarIdx.push_back({ LookupVarIndex::None, 0, p - s });
                res.push_back(*p);
                ++p;
            }
        }
        else
        {
			VarIdx.push_back({ LookupVarIndex::None, 0, p - s });
            res.push_back(*p);
            ++p;
        }
    }
    return { res, VarIdx, Vars };
}

struct LookupDrawStyle 
{
    CurrentDrawStyle Style;
    LookupVarIndex Index;
};

struct DrawStyleLookupList
{
	std::vector<LookupDrawStyle> List;
    std::vector<LookupVarInfo> VarList;
    const wchar_t* OldStr;
    std::wstring NewStr;
    const wchar_t* MakeString(const wchar_t* ws)
    {
        OldStr = ws;
		LookupDrawStyle ds{};
        NewStr.clear();
        List.emplace_back();
#ifdef CalcEx_UnitTest
		//Debug::Log("[IH] MakeString Input: \"%s\"\n", UnicodetoUTF8(ws).c_str());
#endif
		auto [VarReplaced, VarIdx, Vars] = GetReplacedDrawStrIndexed(ws); 
#ifdef CalcEx_UnitTest
        //Debug::Log("[IH] GetReplacedDrawStrIndexed: \"%s\"\n", UnicodetoUTF8(VarReplaced).c_str());
#endif
        auto pinit = VarReplaced.c_str();
        for (auto p = pinit; *p;)
        {
            NewStr.push_back(GetNextChar(p, ds.Style));
            if (NewStr.back() == Magic_StyleAction)
                NewStr.pop_back();
            else
            {
				//实际上是个字符
				//向List添加一个元素
                //记录这个字符的style和相对位置
				// p - 1是刚刚读入的字符
				size_t idx = (size_t)(p - 1 - pinit);
				ds.Index = VarIdx[idx];
				List.push_back(ds);
            }
        }
        VarList = std::move(Vars);
		return NewStr.c_str();
    }
};

struct DrawStyleList
{
    std::vector<CurrentDrawStyle> List;
    std::wstring NewStr;
    std::wstring NewStrAlt;
    int Cur{ 0 };
    size_t CurCharIdx{ 0 };
    wchar_t CurrentChar{ 0 };
    bool HasBitmap{ false };
    bool NoReblit{ false };
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
        CurrentDrawStyle ds{};
        NewStrAlt.clear();
        auto VarReplaced = GetReplacedDrawStr(ws);
        for (auto p = VarReplaced.c_str(); *p;)
        {
            NewStrAlt.push_back(GetNextChar(p, ds));
            if (NewStrAlt.back() == Magic_StyleAction)
            {
                NewStrAlt.pop_back();
                //NewStr += ds.GetWT();
            }
            //if (NewStr.back() == '\r' || NewStr.back() == '\n')
            //    NewStr.push_back(Magic_StyleAction);
        }
        return NewStrAlt.c_str();
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

//缺少测试
#ifdef USE_PREPROCESSED_STRINGS

    std::unordered_map<DWORD, std::unique_ptr<DrawStyleList>> Preprocessed;
    std::shared_mutex PMtx;
    static const size_t PreprocessedHeaderSize = 3;
    static const wchar_t PreprocessedHeaderMagic = (wchar_t)0xFEFF;

    //preprocessed format : 
    //FEFF IdLow IdHigh
    bool IsPreprocessedWString(const wchar_t* str)
    {
		if (!str)return false;
		if (wcsnlen_s(str, PreprocessedHeaderSize) < PreprocessedHeaderSize) return false;
        if (str[0] != PreprocessedHeaderMagic) return false;
		DWORD idlow = (DWORD)str[1], idhigh = (DWORD)str[2];
		DWORD ID = idlow | (idhigh << 16);
		std::shared_lock<std::shared_mutex> lr(PMtx);
		return Preprocessed.contains(ID);
    }

    const wchar_t* PreprocessBlitString(const wchar_t* str)
    {
        std::unique_lock<std::shared_mutex> lw(PMtx);
        auto ID = (DWORD)ECUniqueID();
        auto& prep = Preprocessed[ID];
        prep.reset(new DrawStyleList());
		prep->MakeString(str);
        prep->MakeStringAlt(str);

        prep->NewStr.insert(0, 3, L'\0');
        prep->NewStr[0] = PreprocessedHeaderMagic;
        prep->NewStr[1] = (wchar_t)(ID & 0x0000FFFF);
        prep->NewStr[2] = (wchar_t)((ID >> 16) & 0x0000FFFF);

		prep->NewStrAlt.insert(0, 3, L'\0');
		prep->NewStrAlt[0] = PreprocessedHeaderMagic;
		prep->NewStrAlt[1] = (wchar_t)(ID & 0x0000FFFF);
		prep->NewStrAlt[2] = (wchar_t)((ID >> 16) & 0x0000FFFF);

        return prep->NewStrAlt.c_str();
    }

    void ReleasePreprocessedString(const wchar_t* str)
    {
        if (!IsPreprocessedWString(str))return;
        DWORD idlow = (DWORD)str[1], idhigh = (DWORD)str[2];
        DWORD ID = idlow | (idhigh << 16);
		std::unique_lock<std::shared_mutex> lw(PMtx);
		Preprocessed.erase(ID);
    }

    DrawStyleList* GetStyleFromPreprocessedString(const wchar_t* str)
    {
        if (!IsPreprocessedWString(str))return nullptr;
        DWORD idlow = (DWORD)str[1], idhigh = (DWORD)str[2];
        DWORD ID = idlow | (idhigh << 16);
		std::shared_lock<std::shared_mutex> lr(PMtx);
		auto it = Preprocessed.find(ID);
		if (it != Preprocessed.end())return it->second.get();
		else return nullptr;
    }

    const wchar_t* MakeString(const wchar_t* ws)
    {
        auto pStyle = GetStyleFromPreprocessedString(ws);
        if (pStyle)
        {
            GetList() = *pStyle;
            return GetList().NewStr.c_str() + PreprocessedHeaderSize;
        }
        else
        {
            return GetList().MakeString(ws);
        }
    }
    const wchar_t* MakeStringAlt(const wchar_t* ws)
    {
        auto pStyle = GetStyleFromPreprocessedString(ws);
        if (pStyle)
        {
            GetList() = *pStyle;
            return GetList().NewStrAlt.c_str() + PreprocessedHeaderSize;
        }
        else
        {
            return GetList().MakeStringAlt(ws);
        }
    }
#else
    DrawStyleList& GetList();
    const wchar_t* MakeString(const wchar_t* ws)
    {
        return GetList().MakeString(ws);
    }
    const wchar_t* MakeStringAlt(const wchar_t* ws)
    {
        return GetList().MakeStringAlt(ws);
    }

#endif
    bool NoReblit()
    {
		std::scoped_lock<std::mutex> lock(TMtx);
        auto& p = TMap[GetCurrentThreadId()];
        if (!p)return false;
		return p->NoReblit;
    }
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
                if (Cur.HasStyle && ExtendWidth && (List.GlyphBuffer[0] & 0xF) != 0xF )
                    List.GlyphBuffer[0]++;
                if (Cur.Bold)
                {
                    for (int i = 0; i < BitFont::Instance->InternalPTR->FontHeight; i++)
                    {
                        int& at = *(int*)(&List.GlyphBuffer[1 + i * DrawStyle::GlyphLineDelta]);
                        //extract
                        auto u1 = at & 0x00FFFFFF;
						//reverse bytes : low 3 bytes of u1 : B0 B1 B2 -> B2 B1 B0
						u1 = ((u1 & 0x0000FF) << 16) | (u1 & 0x00FF00) | ((u1 & 0xFF0000) >> 16);
						//bold effect
                        auto u2 = (~((u1 << 1) & (u1 >> 1))) & (u1 << 1) | u1;
						//reverse back
						u2 = ((u2 & 0x0000FF) << 16) | (u2 & 0x00FF00) | ((u2 & 0xFF0000) >> 16);
                        //write back
						at = u2 | (at & 0xFF000000);
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

struct ResetWidth
{
    wchar_t Begin;
    wchar_t End;
    uint8_t width;
    enum _Align{
		AlignLeft,
		AlignCenter,
		AlignRight
	}Align;
};

std::vector<ResetWidth> ResetWidthList;
std::vector<uint8_t> ActualWidthList(0x10000, 0);

#ifdef CalcEx_UnitTest
void CalcMaxLengthEx_UnitTest(BitFont* pFont);
#endif

wchar_t LoadWch(JsonObject o)
{
	if (!o)return 0;
	//support : 0xXXXX & 字
	wchar_t wch = 0;
    auto Str = o.GetString();
	if (!Str.empty() && Str[0])
	{
        if (Str.starts_with("0x"))
		{
			int val = 0;
			sscanf_s(Str.c_str() + 2, "%X", &val);
			wch = (wchar_t)(val & 0x0000FFFF);
		}
        else
        {
            wch = Str[0];
        }
	}
	return wch;
}

void InitResetWidthList()
{
    JsonObject GetIHCoreJson();
    if (auto cfg = GetIHCoreJson(); cfg)
    {
		auto oReset = cfg.GetObjectItem("AdjustFont");
        if (oReset.IsTypeArray())
        {
            for (auto&& p : oReset.GetArrayObject())
            {
                ResetWidth rw;
				auto oWidth = p.GetObjectItem("Width");
                if(oWidth && oWidth.IsTypeNumber())
					rw.width = (uint8_t)std::min(std::max(oWidth.GetInt(), 0), 255);
                rw.Begin = LoadWch(p.GetObjectItem("Begin"));
				rw.End = LoadWch(p.GetObjectItem("End"));
                auto oAlign = p.GetObjectItem("Align");
                if (oAlign && oAlign.IsTypeString())
                {
                    auto Str = oAlign.GetString();
					if (!_stricmp(Str.c_str(), "left"))rw.Align = ResetWidth::AlignLeft;
					else if (!_stricmp(Str.c_str(), "center"))rw.Align = ResetWidth::AlignCenter;
					else if (!_stricmp(Str.c_str(), "right"))rw.Align = ResetWidth::AlignRight;
					else rw.Align = ResetWidth::AlignLeft;
                }
                else rw.Align = ResetWidth::AlignLeft;
				ResetWidthList.push_back(rw);
            }
        }
    }
}

#ifdef BitMap_UnitTest
void PrintBitMap(size_t sz, const unsigned char* Data)
{
    char ss[120];
    Debug::Log("Width=%d\n", (int)Data[0]);
    for (size_t i = 0; i < sz; i++)
    {
        _itoa(Data[i], ss, 2);
        Debug::Log("%08s", ss);
        if (i == 0)Debug::Log("\n");
        if (i % 3 == 0)Debug::Log("\n");
    }
    Debug::Log("\n");
}
#endif

void ShiftCharBitmap(BitFont* pFont, BYTE* pBitmap, int Shift)
{
	if (Shift == 0)return;
#ifdef BitMap_UnitTest
    Debug::Log("Shift = %d\n", Shift);
#endif
    auto Size = pFont->InternalPTR->SymbolDataSize;
    auto Height = pFont->InternalPTR->FontHeight;
    auto Delta = (Size - 1) / Height;
    for (int i = 0; i < pFont->InternalPTR->FontHeight; i++)
    {
        BYTE b0 = pBitmap[1 + i * Delta];
        BYTE b1 = pBitmap[2 + i * Delta];
        int u = ((b1 << 8) | b0);
        u = u | u << 16;
        if (Shift > 0) u = (u >> Shift);
        else u = (u << (-Shift)) >> 16;
        pBitmap[1 + i * Delta] = (BYTE)(u & 0xFF);
        pBitmap[2 + i * Delta] = (BYTE)((u >> 8) & 0xFF);
		
    }
#ifdef BitMap_UnitTest
    PrintBitMap(Size, pBitmap);
#endif
}

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
    InitResetWidthList();
    for (int i = 0; i < 0x10000; i++)
    {
        auto pBitMap = pFont->GetCharacterBitmap(i);
        if (pBitMap)
        {
            auto Width = pBitMap[0] & 0xF;
			for (auto& rw : ResetWidthList)
                if ((uint16_t)i >= (uint16_t)rw.Begin && (uint16_t)i < (uint16_t)rw.End)
                {
                    switch (rw.Align)
                    {
					case ResetWidth::AlignLeft:
						break;
					case ResetWidth::AlignCenter:
						ShiftCharBitmap(pFont, pBitMap, (int)(rw.width - Width) / 2);
						break;
					case ResetWidth::AlignRight:
                        ShiftCharBitmap(pFont, pBitMap, (int)(rw.width - Width));
                        break;
                    }
                    Width = rw.width;
                    break;
                }
			pBitMap[0] = (BYTE)((pBitMap[0] & 0xF0) | (std::min(Width, 15) & 0x0F));

			ActualWidthList[i] = (uint8_t)Width;

            DrawStyle::GlyphByWidth[Width].push_back(pBitMap);
        }
    }
    DrawStyle::GlyphLineDelta = (pFont->InternalPTR->SymbolDataSize - 1) / pFont->InternalPTR->FontHeight;
    DrawStyle::MagicGlyphBuf = new BYTE[pFont->InternalPTR->SymbolDataSize];
    memset(DrawStyle::MagicGlyphBuf, 0, pFont->InternalPTR->SymbolDataSize);

#ifdef CalcEx_UnitTest
	//CalcMaxLengthEx_UnitTest(pFont);
#endif
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
        return 0;
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
    Text = DrawStyle::MakeStringAlt(Text);
    return 0;
};

DEFINE_HOOK(0x433DCE, BitFont_GetTextDimension_B, 7)
{
    GET(int, _Char, EDI);
    ((BYTE*)(BitFont::Instance->AtlasBuffer))[0] = ((wchar_t)_Char == Magic_StyleAction) ? (BYTE)0 : DrawStyle::MagicGlyphWidth;
    return 0;
}


//BitFont::BlitInBounds
DEFINE_HOOK(0x434500, BitFont_Blit1, 7)
{
    REF_STACK(const wchar_t*, Text, 0x4);
    if(!DrawStyle::NoReblit())
        Text = DrawStyle::MakeString(Text);
    return 0;
};

DEFINE_HOOK(0x43467F, BitFont_Blit1Ret1, 5)
{
    if (!DrawStyle::NoReblit())
        DrawStyle::ClearStyle();
    return 0;
};

DEFINE_HOOK(0x4346A6, BitFont_Blit1Ret2, 5)
{
    if (!DrawStyle::NoReblit())
        DrawStyle::ClearStyle();
    return 0;
};

//BitFont::DrawText_MultipleLines
DEFINE_HOOK(0x434CD0, BitFont_Blit2, 5)
{
    REF_STACK(const wchar_t*, Text, 0xC);
    Text = DrawStyle::MakeString(Text);
    return 0;
}

DEFINE_HOOK(0x435157, BitFont_Blit2Ret1, 5)
{
    DrawStyle::ClearStyle();
    return 0;
};

DEFINE_HOOK(0x4352FB, BitFont_Blit2Ret2, 5)
{
    DrawStyle::ClearStyle();
    return 0;
};

//BlitTextAnimated
DEFINE_HOOK(0x623880, BitFont_Blit3, 5)
{
    REF_STACK(const wchar_t*, Text, 0x4);
	Text = DrawStyle::MakeString(Text);
	//这个函数里面会调用BlitInBounds，所以不需要里面重新计算格式
    DrawStyle::GetList().NoReblit = true;
    return 0;
}

DEFINE_HOOK(0x623C9D, BitFont_Blit3Ret1, 0xA)
{
    DrawStyle::ClearStyle();
    return 0;
}

DEFINE_HOOK(0x623CB7, BitFont_Blit3Ret2, 5)
{
    DrawStyle::ClearStyle();
    return 0;
}

//记录原始函数以备参考
#if 0
struct BitFontEx
{
    BitFont* This() { return reinterpret_cast<BitFont*>(this); }

    int CalcMaxLength_Original(wchar_t* Str, int MaxPixels, int MaxChars, bool WordWrap)
    {
        BitFont* _this; // edx
        int UsedPixels; // ebp
        wchar_t* v8; // ecx
        wchar_t CurCh; // di
        char* v10; // eax
        int v11; // edx
        int v12; // edx
        int v14; // edx
        wchar_t* v15; // ecx
        int v16; // eax
        int Len; // [esp+8h] [ebp-8h]

        _this = This();
        UsedPixels = 0;
        auto pInternal = This()->InternalPTR;
        Len = 0;
        if (!pInternal)
            return 0;
        v8 = Str;
        if (!Str)
            return 0;
        CurCh = *Str;
        if (*Str)
        {
            while (1)
            {
                if (CurCh == '\t')
                {
                    UsedPixels = _this->TabWidthPixels + UsedPixels - (_this->TabWidthPixels + UsedPixels) % _this->TabWidthPixels;
                }
                else if (CurCh != '\n' && CurCh != '\r')
                {
                    v10 = 0;
                    v11 = pInternal->SymbolTable[CurCh];
                    if (v11)
                        v10 = &pInternal->Bitmaps[(v11 - 1) * pInternal->SymbolDataSize];
                    v8 = Str;
                    if (v10 || (v10 = (char*)This()->AtlasBuffer) != 0)
                        UsedPixels += *v10 + This()->Spacing;
                }
                v12 = ++Len;
                if (UsedPixels > MaxPixels)
                    break;
                if (MaxChars && v12 == MaxChars || WordWrap && (CurCh == '\r' || CurCh == '\n'))
                    return Len;
                CurCh = v8[1];
                Str = ++v8;
                if (!CurCh)
                    return Len;
                _this = This();
            }
            v14 = v12 - 1;
            Len = v14;
            if (WordWrap)
            {
                if (CurCh > 0x20u)
                {
                    v15 = v8 - 1;
                    v16 = v14;
                    if (v14)
                    {
                        while (*v15 > 0x20u)
                        {
                            --v16;
                            --v15;
                            if (!v16)
                                return v14;
                        }
                        if (v16)
                            Len = v16;
                    }
                }
            }
        }
        return Len;
    }
};
#endif

bool CanSeparateAfter(wchar_t ch)
{
	//决定是否可以在此字符后断行
	//WW的默认逻辑是只有<=0x20的字符可以断行
    
    /*
    重新设计：
	暂且先加入针对CJK字符&各种空格的支持
    */

	if (ch <= 0x20u)return true;

	if ((ch >= 0x4E00 && ch <= 0x9FFF) ||//CJK统一汉字
		(ch >= 0x3400 && ch <= 0x4DBF) ||//CJK统一汉字扩展A
		(ch >= 0x3040 && ch <= 0x309F) ||//平假名
		(ch >= 0x30A0 && ch <= 0x30FF) ||//片假名
		(ch >= 0xAC00 && ch <= 0xD7AF) ||//韩文音节
		(ch >= 0x1100 && ch <= 0x11FF) ||//韩文字母
		(ch >= 0x3130 && ch <= 0x318F)   //韩文字母兼容区
        )
		return true;
    if ((ch == 0x3000) ||//全角空格
        (ch == 0x00A0) ||//不间断空格
        (ch == 0x2002) ||//EN SPACE
        (ch == 0x2003) ||//EM SPACE
        (ch == 0x200A)   //HAIR SPACE
        )
        return true; 
	if (ch == L',' || ch == L'.' || ch == L':' ||
        ch == L'!' || ch == L'?' || ch == L';' ||
        ch == L'/' || ch == L'\\' || ch == L'|' )
		return true;

	return false;
}

void SplitRegenString(
    const wchar_t* _In_ Str, 
	const DrawStyleLookupList& _In_ Lookup,
    int& _Inout_ Len, 
    const wchar_t*& _Out_ StrBefore,
    const wchar_t*& _Out_ StrAfter
)
{
	//调整Len：从Str[Len-1]起向后第一个非控制字符的位置
    //防止\t \r \n等打头
    while(Str[Len - 1] && Str[Len - 1] < 0x20u)
	{
		Len++;
	}

	//重新生成剩余字符串
	//Str[Len] 指向剩余字符串的起始
	//StrBefore和StrAfter使用GameCreateArray重新分配，请使用后用GameDeleteArray释放
    
    if (!Str[Len])
    {
		//剩余字符串为空
        Len = wcslen(Lookup.OldStr);
        StrBefore = Lookup.OldStr;
        StrAfter = nullptr;
		return;
    }

	//取出前半部分最后一个字符的style信息
	auto& LastStyle = Lookup.List[Len - 1];

    //最终的StrAfter : 补充的格式符 + OldStr[OldIdx + 1 .. end]
    //后半部分的格式在前半截的最后一个字符的格式的基础上变化而来
    //所以需要补充描述前半截的最后一个字符的格式符
    auto& LastCharStyle = LastStyle.Style;
	std::wstring afterPrefix;
    /*
    bool HasStyle{ false };
    bool Bold{ false };             §l
    bool StrikeThrough{ false };    §m
    bool Underline{ false };        §n
    bool HasColor{ false };         §xHHHH
    bool Random{ false };           §k
    short Col16{ 0 };
    */
	if (LastCharStyle.HasStyle)
	{
        if (LastCharStyle.Bold)afterPrefix += L"§l";
		if (LastCharStyle.StrikeThrough)afterPrefix += L"§m";
		if (LastCharStyle.Underline)afterPrefix += L"§n";
		if (LastCharStyle.Random)afterPrefix += L"§k";
		if (LastCharStyle.HasColor)
		{
			wchar_t colorbuf[12];
			swprintf_s(colorbuf, L"§x%04X", (unsigned short)LastCharStyle.Col16);
			afterPrefix += colorbuf;
		}
#ifdef CalcEx_UnitTest
        Debug::Log("[IH] Prefix Str : \"%s\"\n", UnicodetoUTF8(afterPrefix).c_str());
#endif
	}


	//重新生成StrBefore StrAfter
	if (LastStyle.Index.VarIdx != LookupVarIndex::None)
	{
		//最后一个字符是变量的一部分
		auto VarIdx = LastStyle.Index.VarIdx;
        auto& VarInfo = Lookup.VarList[VarIdx];
        auto& VarName = VarInfo.VarName;
        auto VarStartIdx = VarInfo.Offset;
        auto VarClipIdx = VarInfo.ClipStart;
        auto VarClipEnd = VarInfo.ClipEnd;
        auto UseVarClipRange = VarInfo.UseClipRange;
        auto VarExtLen = VarInfo.ExtLen;
        auto ReserveLen = VarClipIdx + LastStyle.Index.TotalIdx - VarStartIdx;

		//StrBefore 需要重新生成
		//最终的StrBefore : OldStr[0 .. VarStartIdx- 1] + VarName引用
        //take : if k[-2] = Magic_ValueClip, take k[-1] as length (signed short)
        //VarName引用 : # { VarName Magic_ValueClip (length) }
		//StrBefore 的长度 : VarStartIdx + VarName.length() + 5
        size_t beforeLen;
        if (VarClipIdx == 0)
        {
            beforeLen = VarStartIdx + VarName.length() + 5;
            auto before = GameCreateArray<wchar_t>(beforeLen + 1);
            wcsncpy(before, Lookup.OldStr, VarStartIdx);
            before[VarStartIdx] = L'#';
            before[VarStartIdx + 1] = L'{';
            wcsncpy(&before[VarStartIdx + 2], VarName.c_str(), VarName.length());
            before[beforeLen - 3] = Magic_ValueClip;
            before[beforeLen - 2] = (wchar_t)((short)(ReserveLen));
            before[beforeLen - 1] = L'}';
            before[beforeLen] = L'\0';
            StrBefore = before;
        }
        else
        {
            beforeLen = VarStartIdx + VarName.length() + 6;
            auto before = GameCreateArray<wchar_t>(beforeLen + 1);
            wcsncpy(before, Lookup.OldStr, VarStartIdx);
            before[VarStartIdx] = L'#';
            before[VarStartIdx + 1] = L'{';
            wcsncpy(&before[VarStartIdx + 2], VarName.c_str(), VarName.length());
            before[beforeLen - 4] = Magic_ValueClip;
            before[beforeLen - 3] = (wchar_t)((short)(VarClipIdx));
            before[beforeLen - 2] = (wchar_t)((short)(ReserveLen));
            before[beforeLen - 1] = L'}';
            before[beforeLen] = L'\0';
            StrBefore = before;
        }
		

#ifdef CalcEx_UnitTest
        Debug::Log("[IH] Before VarName = %s VarClipIdx = %d\n", UnicodetoUTF8(VarName).c_str(), VarClipIdx);
        Debug::Log("[IH] StrBefore Idx = %d \n", (int)((short)(ReserveLen)));
#endif

        //StrAfter 需要重新生成
        //最终的StrAfter : afterPrefix + VarName引用 + OldStr[VarEndIdx + 1 .. end]
        //take : if k[-2] = Magic_ValueClip, take k[-1] as length (signed short)
        //VarName引用 : # { VarName Magic_ValueClip (length) }
		//StrAfter 的长度 : afterPrefix.length() + VarName.length() + 5 + (OldStrLen - (VarEndIdx + 1))
        //VarOriginalLen : # { VarName }
		auto VarOriginalLen = VarName.length() + VarExtLen;
		auto VarEndIdx = VarStartIdx + VarOriginalLen - 1;
        auto OldStrLen = wcslen(Lookup.OldStr);
		auto afterLen = afterPrefix.length() + VarName.length() + 5 + (OldStrLen - (VarEndIdx + 1));
		auto after = GameCreateArray<wchar_t>(afterLen + 1);
        auto afterPre = afterPrefix.length();
		auto afterVar = afterPre + VarName.length() + 5;
        short VarLeftLen = -((short)(ReserveLen));
		wcsncpy(after, afterPrefix.c_str(), afterPrefix.length());
		after[afterPre] = L'#';
		after[afterPre + 1] = L'{';
		wcsncpy(&after[afterPre + 2], VarName.c_str(), VarName.length());
		after[afterVar - 3] = Magic_ValueClip;
		after[afterVar - 2] = (wchar_t)(VarLeftLen);
		after[afterVar - 1] = L'}';
		wcsncpy(&after[afterVar], &Lookup.OldStr[VarEndIdx + 1], OldStrLen - (VarEndIdx + 1));
		after[afterLen] = L'\0';
		StrAfter = after;

#ifdef CalcEx_UnitTest
        Debug::Log("[IH] StrAfter Idx = %d \n", (int)(VarLeftLen));
#endif

		//Len调整为StrBefore的长度
		Len = beforeLen;
	}
	else
	{
#ifdef CalcEx_UnitTest
        Debug::Log("Before Range: %s", UnicodetoUTF8(std::wstring(Lookup.OldStr, LastStyle.Index.TotalIdx + 1)).c_str());
#endif
        //StrBefore 需要重新生成
        //最后一个字符不是变量的一部分
		//直接复制OldStr的值
        auto OldIdx = LastStyle.Index.TotalIdx;
		auto before = GameCreateArray<wchar_t>(OldIdx + 2);
		wcsncpy(before, Lookup.OldStr, OldIdx + 1);
		before[OldIdx + 1] = L'\0';
		StrBefore = before;

		//StrAfter 需要重新生成
		//最终的StrAfter : afterPrefix + OldStr[OldIdx + 1 .. end]
		auto OldStrLen = wcslen(Lookup.OldStr);
		auto afterLen = afterPrefix.length() + (OldStrLen - (OldIdx + 1));
		auto after = GameCreateArray<wchar_t>(afterLen + 1);
		wcsncpy(after, afterPrefix.c_str(), afterPrefix.length());
		wcsncpy(&after[afterPrefix.length()], &Lookup.OldStr[OldIdx + 1], OldStrLen - (OldIdx + 1));
		after[afterLen] = L'\0';
		StrAfter = after;

        //Len调整为StrBefore的长度
		Len = OldIdx + 1;
	}

}

/*
BitFont::AutoWrapTextEx

在计算字符串最大长度时忽略格式控制字符
同时重新生成字符串剩余部分

输出：
Len = 0 ,StrBefore = nullptr ,StrAfter = nullptr : 
        存在错误（如字符串是空串或字体不可用）
Len != 0 ,StrBefore != nullptr ,StrAfter = nullptr : 
		完全输出，返回字符串长度， StrBefore就是Str，不需要重新分配
Len != 0 ,StrBefore != nullptr ,StrAfter != nullptr : 
        部分输出，返回输出长度，StrBefore指向重整后的字符串，StrAfter指向剩余字符串;
        StrBefore和StrAfter使用GameCreateArray重新分配，请使用后用GameDeleteArray释放

*/
void __cdecl AutoWrapTextEx(
    BitFont* _In_ Font,
    const wchar_t* _In_ Str,
    int _In_ MaxPixels,
    int _In_ MaxChars,
    bool _In_ WordWrap,
    int& _Out_ Len, 
    const wchar_t*& _Out_ StrBefore,
    const wchar_t*& _Out_ StrAfter
)
{
    DrawStyleLookupList Lookup;
	Str = Lookup.MakeString(Str);

#ifdef CalcEx_UnitTest
	Debug::Log("[IH] AutoWrapTextEx Input Str : \n\t\"%s\"\n", UnicodetoUTF8(std::wstring(Str)).c_str());
#endif

    int UsedPixels = 0;
    Len = 0;
    StrAfter = nullptr;

    //Len = 0 ,StrBefore = nullptr ,StrAfter = nullptr
    if (!Str)return;
    if (!Font->InternalPTR)return;

    auto str = Str;
    wchar_t wch;
    while (*str)
    {
        wch = *str;
		if (wch == L'\t')
		{
			//tab : move to next tab stop
			UsedPixels = Font->TabWidthPixels + UsedPixels - (Font->TabWidthPixels + UsedPixels) % Font->TabWidthPixels;
		}
        else if (wch != '\n' && wch != '\r')
        {
			auto pChBuf = Font->GetCharacterBitmap(wch);
			if (!pChBuf) pChBuf = (BYTE*)Font->AtlasBuffer;
			if (pChBuf)UsedPixels += pChBuf[0] + Font->Spacing;
        }

        ++Len;

        if (UsedPixels > MaxPixels)
        {
            //Len != 0 ,StrBefore != nullptr ,StrAfter != nullptr
            //计算Word Wrap & 返回

			//Str[Len - 1] & *str 指向最后一个输出字符的下一个
			//如果当前的字符是空白，则不执行Word Wrap
			//否则尝试回溯
			//假定Len > 1，即超像素的至少是第二个字符
            if (WordWrap && !CanSeparateAfter(wch))
            {
#ifdef CalcEx_UnitTest
				Debug::Log("[IH] Try Wrap \"%s\"\n", UnicodetoUTF8(Str).c_str());
#endif

                auto Prev = str - 1;
                auto PrevLen = Len - 1;
                //Str + Len = str + 1
                //Str + PrevLen = Prev + 1
                while (!CanSeparateAfter(*Prev))
                {
                    --Prev;
                    --PrevLen;
                    if (!PrevLen)
                    {
#ifdef CalcEx_UnitTest
                        Debug::Log("[IH] Wrapped A \"%s\"\n", UnicodetoUTF8(std::wstring(Str, Len)).c_str());
#endif
                        return SplitRegenString(Str, Lookup, Len, StrBefore, StrAfter);
                    }
                }
				Len = PrevLen;
#ifdef CalcEx_UnitTest
                Debug::Log("[IH] Wrapped B \"%s\"\n", UnicodetoUTF8(std::wstring(Str, Len)).c_str());
#endif
                return SplitRegenString(Str, Lookup, Len, StrBefore, StrAfter);
            }
            else
            {
#ifdef CalcEx_UnitTest
                Debug::Log("[IH] Wrapped C Len=%d \"%s\"\n", Len, UnicodetoUTF8(std::wstring(Str, Len)).c_str());
#endif
				//当前字符可断行 或者 不计算Word Wrap
				return SplitRegenString(Str, Lookup, Len, StrBefore, StrAfter);
            }
        }

        if (MaxChars && Len == MaxChars || WordWrap && (wch == '\r' || wch == '\n'))
        {
			//Len != 0 ,StrBefore != nullptr ,StrAfter != nullptr
			//不计算Word Wrap & 返回
            return SplitRegenString(Str, Lookup, Len, StrBefore, StrAfter);
        }

        ++str;

        if (!(*str))
        {
            //Len != 0 ,StrBefore != nullptr ,StrAfter = nullptr
            StrBefore = Lookup.OldStr;
			StrAfter = nullptr;
			return;
        }
    }
}

std::wstring ParseStringForTest(const wchar_t* Str)
{
    DrawStyleList List;
    return List.MakeStringAlt(Str);
}

#ifdef CalcEx_UnitTest

void CalcMaxLengthEx_UnitTest(BitFont* pFont)
{
    auto Str = StringTable::LoadString("CalcMaxLengthEx_UnitTest_String");

	Debug::Log("[IH] AutoWrapTextEx Unit Test Input Str : \n\t\"%s\"\n", UnicodetoUTF8(std::wstring(Str)).c_str());

    //reinterpret_cast<int(__thiscall*)(BitFont*, const wchar_t*, int, int, int)>(0x433F50)
    //(pBit, message, availableWidth, 111, 1);

    //一个乱给的测试值
    int MaxPixels = 200;
    //实际情况的值：
    int MaxChars = 111;
	bool WordWrap = true;
	
	int Len = 0;
	const wchar_t* StrBefore = nullptr;
	const wchar_t* StrAfter = nullptr;
    DrawStyleList List;
    std::vector<std::wstring> OrigOutput;
    std::vector<std::wstring> ParsedOutput;
    while (1)
    {
		//模拟切分 - 循环调用直到StrAfter为空
        //前面每次：StrBefore StrAfter 非空
		//输出 StrBefore 并释放，用 StrAfter 作为下一次的输入
		//最后一次：StrAfter 为空
        //输出 StrBefore 并释放，退出循环
        AutoWrapTextEx(pFont, Str, MaxPixels, MaxChars, WordWrap, Len, StrBefore, StrAfter);
        /*
        输出：
        Len = 0 ,StrBefore = nullptr ,StrAfter = nullptr :
                存在错误（如字符串是空串或字体不可用）
        Len != 0 ,StrBefore != nullptr ,StrAfter = nullptr :
                完全输出，返回字符串长度， StrBefore就是Str，不需要重新分配
        Len != 0 ,StrBefore != nullptr ,StrAfter != nullptr :
                部分输出，返回输出长度，StrBefore指向重整后的字符串，StrAfter指向剩余字符串;
                StrBefore和StrAfter使用GameCreateArray重新分配，请使用后用GameDeleteArray释放
        */
        if (Len == 0)
        {
            Debug::Log("[IH] Unit Test Failed.");
            return;
        }
        else
        {
            //输出与释放StrBefore
            //Debug::Log("[IH] Len = %u\n", Len);
            List.MakeStringAlt(StrBefore);
			Debug::Log("[IH] StrBefore : \n%s\n", UnicodetoUTF8(std::wstring(StrBefore)).c_str());
            Debug::Log("[IH] Parsed : \n%s\n", UnicodetoUTF8(List.NewStrAlt).c_str());
            OrigOutput.push_back(StrBefore);
			ParsedOutput.push_back(List.NewStrAlt);
			if (StrAfter)
			{
				Debug::Log("[IH] StrAfter : \n%s\n", UnicodetoUTF8(std::wstring(StrAfter)).c_str());
			}
			GameDeleteArray(StrBefore, Len + 1);
			StrBefore = nullptr;
			//准备下一次输入
			if (StrAfter)
			{
				Str = StrAfter;
				StrAfter = nullptr;
			}
			else
			{
				
				break;
			}
        }
    }

    Debug::Log("ORIGINAL OUTPUT:\n");
    for (auto& s : OrigOutput)
    {
        Debug::Log("%s\n", UnicodetoUTF8(s).c_str());
    }
	Debug::Log("PARSED OUTPUT:\n");
    for (auto& s : ParsedOutput)
    {
		Debug::Log("%s\n", UnicodetoUTF8(s).c_str());
    }
    Debug::Log("[IH] Unit Test Succeeded.\n");
}

/*
INPUT:
YajuSenpai YajuSenpai Yaju Senpai Yaj YajuSenpai uSenp ai  YajuSenpai YajuSenpai YajuSenpai
PARSED:
YajuSenpai YajuSenpai Yaju Senpai Yaj YajuSenpai uSenp ai  YajuSenpai YajuSenpai YajuSenpai
PARSED OUTPUT:
YajuSenpai YajuSenpai Yaju Senpai
Yaj YajuSenpai uSenp ai
YajuSenpai YajuSenpai YajuSenpai

INPUT:
MISSING:Key.DeployObject.Namdfrgfd:MISSING:Key.DeployObject.Ndfrgfd:ameMISSING:Key.DeployObject.Name
PARSED:
MISSING:Key.DeployObject.Namdfrgfd:MISSING:Key.DeployObject.Ndfrgfd:ameMISSING:Key.DeployObject.Name
PARSED OUTPUT:
MISSING:Key.DeployObject.Namdfrgfd:
MISSING:Key.DeployObject.Ndfrgfd:
ameMISSING:Key.DeployObject.Name

INPUT:
#{Key.DeployObject.Name} : #{CSF.TXT_OK}yaju#{Key.DeployObject.Name} : #{CSF.TXT_OK}senpai#{Key.DeployObject.Name} : #{CSF.TXT_OK}114514#{Key.DeployObject.Name} : #{CSF.TXT_OK}
PARSED:
MISSING:Key.DeployObject.Name : 确定yajuMISSING:Key.DeployObject.Name : 确定senpaiMISSING:Key.DeployObject.Name : 确定114514MISSING:Key.DeployObject.Name : 确定
ORIGINAL OUTPUT:
#{Key.DeployObject.Name} : #{CSF.TXT_OK﻿}
#{CSF.TXT_OK﻿ }yaju#{Key.DeployObject.Name} :
 #{CSF.TXT_OK}senpai#{Key.DeployObject.Name﻿}
#{Key.DeployObject.Name﻿￴} : #{CSF.TXT_OK﻿}
#{CSF.TXT_OK﻿ }114514#{Key.DeployObject.Name﻿}
#{Key.DeployObject.Name﻿￢} : #{CSF.TXT_OK}
PARSED OUTPUT:
MISSING:Key.DeployObject.Name : 确定
yajuMISSING:Key.DeployObject.Name :
 确定senpaiMISSING:Key.
DeployObject.Name : 确定
114514MISSING:Key.DeployObject.Name
 : 确定

INPUT:
#{Key.DeployObject.Name} : #{CSF.Name:MTNK}yaju#{Key.DeployObject.Name} : #{CSF.Name:MTNK}senpai#{Key.DeployObject.Name} : #{CSF.TXT_OK}
PARSED:
MISSING:Key.DeployObject.Name : @mimi[鳐鱼空中要塞]mimiyajuMISSING:Key.DeployObject.Name : @mimi[鳐鱼空中要塞]mimisenpaiMISSING:Key.DeployObject.Name : 确定
ORIGINAL OUTPUT:
#{Key.DeployObject.Name} :
 #{CSF.Name:MTNK﻿}
§l§x57FF#{CSF.Name:MTNK﻿￢}yaju#{Key.DeployObject.Name﻿}
#{Key.DeployObject.Name﻿￧} : #{CSF.Name:MTNK﻿}
§l§x57FF#{CSF.Name:MTNK﻿￢}senpai#{Key.DeployObject.Name﻿}
#{Key.DeployObject.Name﻿￧} : #{CSF.TXT_OK}
PARSED OUTPUT:
MISSING:Key.DeployObject.Name :
 @mimi[鳐鱼空中要塞
]mimiyajuMISSING:Key.DeployObject.
Name : @mimi[鳐鱼空中要塞
]mimisenpaiMISSING:Key.DeployObject.
Name : 确定

INPUT:
CalcMaxLengthEx_UnitTest_String=#{Key.DeployObject.Name} : #{CSF.VeryLong}yaju#{Key.DeployObject.Name}
PARSED:
MISSING:Key.DeployObject.Name : 本人也是经过了深思熟虑，在每个日日夜夜思考这个问题。从这个角度来看，这是不可避免的。 每个人都不得不面对这些问题。在面对这种问题时，这种事实对本人来说意义重大，相信对这个世界也是有一定意义的。yajuMISSING:Key.DeployObject.Name
ORIGINAL OUTPUT:
#{Key.DeployObject.Name} : #{CSF.VeryLong﻿}
#{CSF.VeryLong﻿}
#{CSF.VeryLong﻿}
#{CSF.VeryLong﻿-}
#{CSF.VeryLong﻿-;}
#{CSF.VeryLong﻿;I}
#{CSF.VeryLong﻿IW}
#{CSF.VeryLong﻿ﾩ}yaju#{Key.DeployObject.Name﻿}
#{Key.DeployObject.Name﻿￸}
PARSED OUTPUT:
MISSING:Key.DeployObject.Name : 本人
也是经过了深思熟虑，在每个日
日夜夜思考这个问题。从这个角
度来看，这是不可避免的。 每个
人都不得不面对这些问题。在面
对这种问题时，这种事实对本人
来说意义重大，相信对这个世界
也是有一定意义的。yajuMISSING:
Key.DeployObject.Name
*/

#endif

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