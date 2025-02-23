#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>


//�����Redirectϵ���Ǹ�����ԭ������λ�õ�����Ŀ��
//ԭλ���ַ�����δ���ģ�ֱ�ӷ�����Ϊԭֵ
//����ȷ����Դ�����Ե�ʱ������Ĳ���ȫ����Щ����
//����˵�Ƿ���Ҫ��Ӧ�޸�����ֹ�޷���ʼ��ȷ��

//Ҫ�����const char*�Ƿ���ʱֵ��������һֱ����

//Original: EXPANDMD%02d.MIX at 0x82668C
void RedirectExpandMD(const char* NewTarget);
//Original: ELOCAL*.MIX at 0x826614
void RedirectELocal(const char* NewTarget);
//Original: ECACHE*.MIX at 0x826620
void RedirectECache(const char* NewTarget);
//Original: MOVIES%02d.MIX at 0x81C200
void RedirectMovies(const char* NewTarget);
//Original: MOVMD%02d.MIX at 0x81C210
void RedirectMovMD(const char* NewTarget);
//Original: MAPS%02d.MIX at 0x81C2C4
void RedirectMaps(const char* NewTarget);
//Original: MAPSMD%02d.MIX at 0x81C2EC
void RedirectMapsMD(const char* NewTarget);

//Original: EXPANDMD%02d.MIX at 0x82668C
void RedirectExpandMD();
//Original: ELOCAL*.MIX at 0x826614
void RedirectELocal();
//Original: ECACHE*.MIX at 0x826620
void RedirectECache();
//Original: MOVIES%02d.MIX at 0x81C200
void RedirectMovies();
//Original: MOVMD%02d.MIX at 0x81C210
void RedirectMovMD();
//Original: MAPS%02d.MIX at 0x81C2C4
void RedirectMaps();
//Original: MAPSMD%02d.MIX at 0x81C2EC
void RedirectMapsMD();



//һ�����������ʱ���Է���
//�Ծֵ��в�Ҫ��

uint32_t Random_uint32();
int32_t Random_int32();
uint16_t Random_uint16();
int16_t Random_int16();
uint64_t Random_uint64();
int64_t Random_int64();

uint32_t Random_uint32(uint32_t Min, uint32_t Max);
int32_t Random_int32(int32_t Min, int32_t Max);
uint16_t Random_uint16(uint16_t Min, uint16_t Max);
int16_t Random_int16(int16_t Min, int16_t Max);
uint64_t Random_uint64(uint64_t Min, uint64_t Max);
int64_t Random_int64(int64_t Min, int64_t Max);
float Random_float(float Min, float Max);
double Random_double(double Min, double Max);

std::string RandStr(int i);
std::wstring RandWStr(int i);

//һ�����ת������ ���� GBK UTF8 UTF16
std::wstring GBKToUTF16(const std::string& GBK);
std::wstring UTF8ToUTF16(const std::string& UTF8);
std::string UTF16ToUTF8(const std::wstring& UTF16);
std::string UTF16ToGBK(const std::wstring& UTF16);
std::string GBKToUTF8(const std::string& GBK);
std::string UTF8ToGBK(const std::string& GBK);


//��Сд����
struct ECUpperHash
{
    inline size_t operator()(const std::string& s) const;
};
struct ECUpperEqualPred
{
    inline bool operator ()(const std::string& s1, const std::string& s2) const;
};
//���Դ�Сд��Map
template<typename T>
using IgnoreCaseUnorderedMap = std::unordered_map<std::string, T, ECUpperHash, ECUpperEqualPred>;
//���Դ�Сд��Set
using IgnoreCaseUnorderedSet = std::unordered_set<std::string, ECUpperHash, ECUpperEqualPred>;


//��NB��ǿת������˵�����ѳ�Ա����ת����һ��ָ��
template<typename dst_type, typename src_type>
dst_type constexpr UnionCast(src_type src)
{
    union {
        src_type s;
        dst_type d;
    }u;
    u.s = src;
    return u.d;
}




//����Ȩ�޵��ڴ�д�루���Ǵ��ڵ��ڴ棩
void ForcedMemcpy(void* Target, const void* Source, size_t Size);
void ForcedMemcpy(size_t TargetAddr, size_t SourceAddr, size_t Size);

//�Ƚϰ�ȫ���ڴ���루Source�����������ַ������ǲ��ɶ��᷵��false��
bool ReadMemory(const void* Source, void* Buffer, size_t Size);