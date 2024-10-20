
#include "ToolFunc.h"
#include <Windows.h>
#include <ctime>

// ANSI字符集转换成Unicode
std::wstring ANSItoUnicode(const std::string& MBCS)
{
    int nLength = MultiByteToWideChar(CP_ACP, 0, MBCS.c_str(), -1, NULL, NULL);   // 获取缓冲区长度，再分配内存
    WCHAR* tch = new WCHAR[nLength + 4]();
    nLength = MultiByteToWideChar(CP_ACP, 0, MBCS.c_str(), -1, tch, nLength);     // 将MBCS转换成Unicode
    std::wstring ret = tch;
    delete[] tch;
    return ret;
}
// UTF-8字符集转换成Unicode
std::wstring UTF8toUnicode(const std::string& UTF8)
{
    int nLength = MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, NULL, NULL);   // 获取缓冲区长度，再分配内存
    WCHAR* tch = new WCHAR[nLength + 4]{};
    MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, tch, nLength);     // 将UTF-8转换成Unicode
    std::wstring ret = tch;
    delete[] tch;
    return ret;
}
// Unicode字符集转换成UTF-8
std::string UnicodetoUTF8(const std::wstring& Unicode)
{
    int UTF8len = WideCharToMultiByte(CP_UTF8, 0, Unicode.c_str(), -1, 0, 0, 0, 0);// 获取UTF-8编码长度
    char* UTF8 = new CHAR[UTF8len + 4]{};
    WideCharToMultiByte(CP_UTF8, 0, Unicode.c_str(), -1, UTF8, UTF8len, 0, 0); //转换成UTF-8编码
    std::string ret = UTF8;
    delete[] UTF8;
    return ret;
}
// Unicode字符集转换成ANSI
std::string UnicodetoANSI(const std::wstring& Unicode)
{
    int ANSIlen = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, 0, 0, 0, 0);// 获取UTF-8编码长度
    char* ANSI = new CHAR[ANSIlen + 4]{};
    WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, ANSI, ANSIlen, 0, 0); //转换成UTF-8编码
    std::string ret = ANSI;
    delete[] ANSI;
    return ret;
}
// ANSI字符集转换成UTF-8
std::string ANSItoUTF8(const std::string& MBCS)
{
    return UnicodetoUTF8(ANSItoUnicode(MBCS));
}
// UTF-8字符集转换成ANSI
std::string UTF8toANSI(const std::string& MBCS)
{
    return UnicodetoANSI(UTF8toUnicode(MBCS));
}

uint64_t GetSysTimeMicros()
{
    const uint64_t EpochFIleTime = (116444736000000000ULL);
    FILETIME ft;
    LARGE_INTEGER li;
    uint64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
    tt = (li.QuadPart - EpochFIleTime) / 10;
    return tt;
}




//const static char 月份[12][20] = 
//{ "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月", };
//{ "1月", "2月", "3月", "4月", "5月", "6月", "7月", "8月", "9月", "10月", "11月", "12月", };
std::string TimeNow()
{
    const static char 星期[7][20] =
    { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };
    char* TBuf = new char[5000]();
    std::tm stm;
    std::time_t tt = std::time(0);
    localtime_s(&stm, &tt);
    sprintf(TBuf, "%04d年%02d月%02d日 %s %02d:%02d:%02d", stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, 星期[stm.tm_wday], stm.tm_hour, stm.tm_min, stm.tm_sec);
    std::string rt = TBuf;
    delete[]TBuf;
    return rt;
}


bool IsTrueString(const std::string& s1)
{
    return s1 == "true" || s1 == "yes" || s1 == "1" || s1 == "T" || s1 == "True" || s1 == "Yes" || s1 == "t" || s1 == "Y" || s1 == "y";
}

size_t UpperHashStr(const char* str)
{
    size_t Result = 0;
    size_t Mod = 19260817;
    for (const char* ps = str; *ps; ++ps)
    {
        Result *= Mod;
        Result += (size_t)toupper(*ps);
    }
    return Result + strlen(str);
}
