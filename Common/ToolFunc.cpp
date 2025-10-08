
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

std::u8string& operator~(std::string& str)
{
	return *(std::u8string*)&str;
}
std::string& operator~(std::u8string& str)
{
	return *(std::string*)&str;
}
const std::u8string& operator~(const std::string& str)
{
	return *(const std::u8string*)&str;
}
const std::string& operator~(const std::u8string& str)
{
	return *(const std::string*)&str;
}
std::u8string&& operator~(std::string&& str)
{
	return reinterpret_cast<std::u8string&&>(str);
}
std::string&& operator~(std::u8string&& str)
{
    return reinterpret_cast<std::string&&>(str);
}
const std::u8string&& operator~(const std::string&& str)
{
    return reinterpret_cast<const std::u8string&&>(str);
}
const std::string&& operator~(const std::u8string&& str)
{
    return reinterpret_cast<const std::string&&>(str);
}
const char* operator-(const conv_t&, const char8_t* str)
{
	return reinterpret_cast<const char*>(str);
}
const char8_t* operator-(const conv_t&, const char* str)
{
	return reinterpret_cast<const char8_t*>(str);
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

std::string Trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::u8string Trim(const std::u8string& str) {
    auto start = str.find_first_not_of(u8" \t\r\n");
    auto end = str.find_last_not_of(u8" \t\r\n");
    return (start == std::string::npos || end == std::string::npos) ? u8"" : str.substr(start, end - start + 1);
}

std::wstring Trim(const std::wstring& str) {
    auto start = str.find_first_not_of(L" \t\r\n");
    auto end = str.find_last_not_of(L" \t\r\n");
    return (start == std::wstring::npos || end == std::wstring::npos) ? L"" : str.substr(start, end - start + 1);
}

std::vector<std::string> SplitParam(const std::string_view Text)//ORIG
{
    if (Text.empty())return {};
    size_t cur = 0, crl;
    std::vector<std::string> ret;
    while ((crl = Text.find_first_of(' ', cur)) != Text.npos)
    {
        ret.push_back(Trim(std::string(Text.begin() + cur, Text.begin() + crl)));
        cur = crl + 1;
    }
    ret.push_back(Trim(std::string(Text.begin() + cur, Text.end())));
    return ret;
}

std::vector<std::u8string> SplitParam(const std::u8string_view Text)//ORIG
{
    if (Text.empty())return {};
    size_t cur = 0, crl;
    std::vector<std::u8string> ret;
    while ((crl = Text.find_first_of(' ', cur)) != Text.npos)
    {
        auto Str = Trim(std::u8string(Text.begin() + cur, Text.begin() + crl));
        if(!Str.empty())ret.push_back(std::move(Str));
        cur = crl + 1;
    }
    {
        auto Str = Trim(std::u8string(Text.begin() + cur, Text.end()));
        if (!Str.empty())ret.push_back(std::move(Str));
    }
    return ret;
}

using std::vector;
using std::u8string;

// ��������������Ƿ���ʮ����������
bool IsHexDigit(char8_t c) {
    return (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F');
}

// ������������ʮ�������ַ�ת��Ϊ��ֵ
int HexCharToValue(char8_t c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return 0;
}

// ��������������˽���ת������
void ProcessOctalEscape(const u8string& input, size_t& i, u8string& current) {
    int value = 0;
    int digits_processed = 0;

    while (i < input.size() &&
        (input[i] >= '0' && input[i] <= '7') &&
        digits_processed < 3) {
        value = value * 8 + (input[i] - '0');
        digits_processed++;
        i++;
    }

    current += static_cast<char8_t>(value);
}

// ��������������ʮ������ת������ (\xXX)
void ProcessHexEscape(const u8string& input, size_t& i, u8string& current, int max_digits) {
    int value = 0;
    int digits_processed = 0;

    while (i < input.size() &&
        IsHexDigit(input[i]) &&
        digits_processed < max_digits) {
        value = value * 16 + HexCharToValue(input[i]);
        digits_processed++;
        i++;
    }

    current += static_cast<char8_t>(value);
}

// ��������������Unicodeת������ (\u+XXXX)
void ProcessUnicodeEscape(const u8string& input, size_t& i, u8string& current) {
    if (i >= input.size()) return;

    int value = 0;
    int digits_processed = 0;

    while (i < input.size() &&
        IsHexDigit(input[i]) &&
        digits_processed < 4) {
        value = value * 16 + HexCharToValue(input[i]);
        digits_processed++;
        i++;
    }

	printf("Processing Unicode escape: value = %X\n", value);

    // ��Unicode���ת��ΪUTF-8
    if (value <= 0x7F) {
        current += static_cast<char8_t>(value);
    }
    else if (value <= 0x7FF) {
        current += static_cast<char8_t>(0xC0 | (value >> 6));
        current += static_cast<char8_t>(0x80 | (value & 0x3F));
    }
    else if (value <= 0xFFFF) {
        current += static_cast<char8_t>(0xE0 | (value >> 12));
        current += static_cast<char8_t>(0x80 | ((value >> 6) & 0x3F));
        current += static_cast<char8_t>(0x80 | (value & 0x3F));
    }
    else if (value <= 0x10FFFF) {
        current += static_cast<char8_t>(0xF0 | (value >> 18));
        current += static_cast<char8_t>(0x80 | ((value >> 12) & 0x3F));
        current += static_cast<char8_t>(0x80 | ((value >> 6) & 0x3F));
        current += static_cast<char8_t>(0x80 | (value & 0x3F));
    }
}

vector<u8string> SplitParamEx(const u8string& input) {
    vector<u8string> result;
    u8string current;
    bool in_quotes = false;
    bool escape_next = false;

    for (size_t i = 0; i < input.size(); i++) {
        auto c = input[i];

        // ����ת���ַ�
        if (escape_next) {
            switch (c) {
            case 'a': current += '\a'; break;
            case 'b': current += '\b'; break;
            case 'f': current += '\f'; break;
            case 'n': current += '\n'; break;
            case 'r': current += '\r'; break;
            case 't': current += '\t'; break;
            case 'v': current += '\v'; break;
            case '\\': current += '\\'; break;
            case '\'': current += '\''; break;
            case '\"': current += '\"'; break;
            case '\?': current += '\?'; break;
            case '0': 
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7':
                i++;
                ProcessOctalEscape(input, i, current);
                i--;
                break;

            case 'x':
                if (i + 1 < input.size() && IsHexDigit(input[i + 1])) 
                {
					i++; // ���� 'x'
                    ProcessHexEscape(input, i, current, 2);
                    i--;
                    break;
                }
                break;

            case 'u':
                if (i + 1 < input.size()) 
                {
					i++; // ���� 'u'
                    ProcessUnicodeEscape(input, i, current);
                    i--;
                    break;
                }
                break;
            }
            escape_next = false;
            continue;
        }

        // ����Ƿ���������
        if (c == '"') {
            if (in_quotes) {
                // �����ǰ���������Ҳ���ת������ţ���������
                result.push_back(current);
                current.clear();
                in_quotes = false;
            }
            else {
                // ��ʼһ���µ����Ŷ�
                in_quotes = true;
            }
            continue;
        }

        if (in_quotes) {
            // �������ڣ�ֱ��׷�ӣ���������ת���ַ���
            if (c == '\\') {
                escape_next = true;
            }
            else {
                current += c;
            }
        }
        else {
            // ���������ڣ�����Ƿ��Ƿָ���
            if (std::isspace(c)) {
                if (!current.empty()) {
                    result.push_back(current);
                    current.clear();
                }
            }
            else {
                current += c;
            }
        }
    }

    // �������һ��Ԫ��
    if (!current.empty() && !in_quotes) {
        result.push_back(current);
    }

    return result;
}