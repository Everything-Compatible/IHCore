#pragma once

#include <string>
#include <cstdint>
#include <vector>

//Although it may cause UB
namespace std
{
    template<typename T>
    _NODISCARD inline string to_string(const T* _Val)
    {
        const auto _Len = static_cast<size_t>(_CSTD _scprintf("%p", _Val));
        string _Str(_Len, '\0');
        _CSTD sprintf_s(&_Str[0], _Len + 1, "%p", _Val);
        return _Str;
    }

    template<typename T>
    _NODISCARD inline wstring to_wstring(const T* _Val)
    {
        const auto _Len = static_cast<size_t>(_CSTD _scwprintf(L"%p", _Val));
        wstring _Str(_Len, L'\0');
        _CSTD swprintf_s(&_Str[0], _Len + 1, L"%p", _Val);
        return _Str;
    }
}

// ANSI字符集转换成Unicode
std::wstring ANSItoUnicode(const std::string& MBCS);
// UTF-8字符集转换成Unicode
std::wstring UTF8toUnicode(const std::string& UTF8);
// Unicode字符集转换成UTF-8
std::string UnicodetoUTF8(const std::wstring& Unicode);
// Unicode字符集转换成ANSI
std::string UnicodetoANSI(const std::wstring& Unicode);
// ANSI字符集转换成UTF-8
std::string ANSItoUTF8(const std::string& MBCS);
// UTF-8字符集转换成ANSI
std::string UTF8toANSI(const std::string& MBCS);

uint64_t GetSysTimeMicros();
std::string TimeNow();

bool IsTrueString(const std::string& s);

std::u8string& operator~(std::string& str);
std::string& operator~(std::u8string& str);
const std::u8string& operator~(const std::string& str);
const std::string& operator~(const std::u8string& str);
std::u8string&& operator~(std::string&& str);
std::string&& operator~(std::u8string&& str);
const std::u8string&& operator~(const std::string&& str);
const std::string&& operator~(const std::u8string&& str);
struct conv_t final {};
inline const conv_t conv_x;
#define conv conv_x-
const char* operator-(const conv_t&, const char8_t* str);
const char8_t* operator-(const conv_t&, const char* str);


//定义在ExtIni.cpp
const size_t MAX_LINE_LENGTH = 6000;
using hash_t = size_t;
using BufString = char[MAX_LINE_LENGTH];

hash_t StrHash(const char*);
hash_t StrHash(const std::string&);

std::string Trim(const std::string& str);
std::u8string Trim(const std::u8string& str);
std::wstring Trim(const std::wstring& str);
std::vector<std::string> SplitParam(const std::string_view Text);
std::vector<std::u8string> SplitParam(const std::u8string_view Text);
std::vector<std::u8string> SplitParamEx(const std::u8string& Text);
const char* BoolCStr(bool);
const std::string& BoolStr(bool);

template<typename dst_type, typename src_type>
dst_type constexpr union_cast(src_type src)
{
    union {
        src_type s;
        dst_type d;
    }u;
    u.s = src;
    return u.d;
}

size_t UpperHashStr(const char* str);

struct UpperHash
{
    inline size_t operator()(const std::string& s) const
    {
        return UpperHashStr(s.c_str());
    }
};

struct UpperEqualPred
{
    inline bool operator ()(const std::string& s1, const std::string& s2) const
    {
        return UpperHashStr(s1.c_str()) == UpperHashStr(s2.c_str());
    }
};