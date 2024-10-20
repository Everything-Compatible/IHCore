
#include "ToolFunc.h"
#include <Windows.h>
#include <ctime>

// ANSI�ַ���ת����Unicode
std::wstring ANSItoUnicode(const std::string& MBCS)
{
    int nLength = MultiByteToWideChar(CP_ACP, 0, MBCS.c_str(), -1, NULL, NULL);   // ��ȡ���������ȣ��ٷ����ڴ�
    WCHAR* tch = new WCHAR[nLength + 4]();
    nLength = MultiByteToWideChar(CP_ACP, 0, MBCS.c_str(), -1, tch, nLength);     // ��MBCSת����Unicode
    std::wstring ret = tch;
    delete[] tch;
    return ret;
}
// UTF-8�ַ���ת����Unicode
std::wstring UTF8toUnicode(const std::string& UTF8)
{
    int nLength = MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, NULL, NULL);   // ��ȡ���������ȣ��ٷ����ڴ�
    WCHAR* tch = new WCHAR[nLength + 4]{};
    MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, tch, nLength);     // ��UTF-8ת����Unicode
    std::wstring ret = tch;
    delete[] tch;
    return ret;
}
// Unicode�ַ���ת����UTF-8
std::string UnicodetoUTF8(const std::wstring& Unicode)
{
    int UTF8len = WideCharToMultiByte(CP_UTF8, 0, Unicode.c_str(), -1, 0, 0, 0, 0);// ��ȡUTF-8���볤��
    char* UTF8 = new CHAR[UTF8len + 4]{};
    WideCharToMultiByte(CP_UTF8, 0, Unicode.c_str(), -1, UTF8, UTF8len, 0, 0); //ת����UTF-8����
    std::string ret = UTF8;
    delete[] UTF8;
    return ret;
}
// Unicode�ַ���ת����ANSI
std::string UnicodetoANSI(const std::wstring& Unicode)
{
    int ANSIlen = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, 0, 0, 0, 0);// ��ȡUTF-8���볤��
    char* ANSI = new CHAR[ANSIlen + 4]{};
    WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, ANSI, ANSIlen, 0, 0); //ת����UTF-8����
    std::string ret = ANSI;
    delete[] ANSI;
    return ret;
}
// ANSI�ַ���ת����UTF-8
std::string ANSItoUTF8(const std::string& MBCS)
{
    return UnicodetoUTF8(ANSItoUnicode(MBCS));
}
// UTF-8�ַ���ת����ANSI
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
    // ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
    tt = (li.QuadPart - EpochFIleTime) / 10;
    return tt;
}




//const static char �·�[12][20] = 
//{ "һ��", "����", "����", "����", "����", "����", "����", "����", "����", "ʮ��", "ʮһ��", "ʮ����", };
//{ "1��", "2��", "3��", "4��", "5��", "6��", "7��", "8��", "9��", "10��", "11��", "12��", };
std::string TimeNow()
{
    const static char ����[7][20] =
    { "������", "����һ", "���ڶ�", "������", "������", "������", "������" };
    char* TBuf = new char[5000]();
    std::tm stm;
    std::time_t tt = std::time(0);
    localtime_s(&stm, &tt);
    sprintf(TBuf, "%04d��%02d��%02d�� %s %02d:%02d:%02d", stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, ����[stm.tm_wday], stm.tm_hour, stm.tm_min, stm.tm_sec);
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
