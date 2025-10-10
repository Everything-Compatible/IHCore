#include"ExtFile.h"
#include <cstring>
#include <YRPP.h>

char* strnchr(const char* s, int c, size_t n) 
{ 
    for (size_t i = 0; i < n; i++) 
    { 
        if (s[i] == c) { return (char*)&s[i]; } 
    } 
    return NULL; 
}

FILE* ExtFileClass::GetRaw()const
{
    return fp;
}
bool ExtFileClass::Open(const char* path, ExtFileClass::AccessMode mode, void* Reserved)
{
    (void)Reserved;
    fopen_s(&fp, path, mode);
    return IsOpen = (fp != NULL);
}
bool ExtFileClass::Open(const wchar_t* path, ExtFileClass::AccessModeW mode, void* Reserved)
{
    (void)Reserved;
    IsOpen = (_wfopen_s(&fp, path, mode) == 0);
    return IsOpen;
}
size_t ExtFileClass::Read(void* Buf, size_t Size, size_t Count)const
{
    if (!IsOpen)return 0;
    return fread(Buf, Size, Count, fp);
}
size_t ExtFileClass::Write(const void* Buf, size_t Size, size_t Count)const
{
    if (!IsOpen)return 0;
    return fwrite(Buf, Size, Count, fp);
}
char ExtFileClass::GetChr()const
{
    if (!IsOpen)return 0;
    return (char)fgetc(fp);
}
bool ExtFileClass::PutChr(const char Byte)const
{
    if (!IsOpen)return 0;
    return fputc(Byte, fp) != EOF;
}
bool ExtFileClass::Ln()const
{
    return PutChr('\n');
}
char* ExtFileClass::GetStr(char* Buf, size_t Size)const
{
    if (!IsOpen)return 0;
    return fgets(Buf, Size, fp);
}
size_t ExtFileClass::PutStr(const char* Buf)const
{
    if (!IsOpen)return 0;
    return fputs(Buf, fp);
}
int ExtFileClass::Close()
{
    if (!IsOpen)return 0;
    IsOpen = false;
    return fclose(fp);
}
int ExtFileClass::Seek(int Offset, ExtFileClass::SeekMode Base)const
{
    return fseek(fp, Offset, Base);
}
int ExtFileClass::Position()const
{
    return ftell(fp);
}
bool ExtFileClass::Eof()const
{
    if (!IsOpen)return true;
    return feof(fp);
}
bool ExtFileClass::Available()const
{
    return IsOpen;
}
void ExtFileClass::Rewind()const
{
    rewind(fp);
}
int ExtFileClass::Flush()const
{
    return fflush(fp);
}

bool ExtFileClass::ReadData(std::string& Str)const
{
    const size_t MaxSize = 65536;
    static char Buf[MaxSize];
    int64_t Size;
    if (!Read(&Size, sizeof(Size), 1))return false;
    char* _Buf;
    if (Size > MaxSize - 1)_Buf = new char[(size_t)Size + 2];
    else _Buf = Buf;
    _Buf[Read(_Buf, 1, (size_t)Size)] = 0;
    Str = _Buf;
    if (Size > MaxSize - 1)delete[] _Buf;
    return true;
}
bool ExtFileClass::WriteData(const std::string& Str)const
{
    int64_t Size = Str.size();
    if (!Write(&Size, sizeof(Size), 1))return false;
    Write(Str.c_str(), 1, Str.size());
    return true;
}
bool ExtFileClass::WriteLabel(const std::string& Str)const
{
    if (!Write(Str.c_str(), 1, Str.size()))return false;
    WriteData('\0');
    return true;
}
bool ExtFileClass::ReadLabel(std::string& Str)const//待测试！！
{
    const size_t MaxSize = 65536;
    static char Buf[MaxSize];
    size_t Cur = 0;
    while (ReadData(Buf[Cur]))if (Buf[Cur++] == '\0')break;
    Str = Buf;
    return true;
}
template<typename T>
bool ExtFileClass::WriteData(const T& Data)const
{
    return Write(&Data, sizeof(T), 1);
}
template<typename T>
bool ExtFileClass::ReadData(T& Data)const
{
    return Read(&Data, sizeof(T), 1);
}
template<typename T>
bool ExtFileClass::WriteVector(const std::vector<T>& Data)const//待测试！！
{
    if (!WriteData((int64_t)Data.size()))return false;
    return Write(Data.data(), sizeof(T), Data.size());
}
template<typename T>
bool ExtFileClass::ReadVector(std::vector<T>& Data)const//待测试！！
{
    int64_t Size;
    if (!ReadData(Size))return false;
    Data.resize((size_t)Size);
    return Read(Data.data(), sizeof(T), Size);
}
int ExtFileClass::WriteVector(const std::vector<std::string>& Data)const//待测试！！
{
    if (!WriteData((int64_t)Data.size()))return 0;
    int Ret = 0;
    for (const auto& d : Data)
    {
        if (!WriteData(d)) break;
        else ++Ret;
    }
    return Ret;
}
int ExtFileClass::ReadVector(std::vector<std::string>& Data)const//待测试！！
{
    int64_t Size;
    if (!ReadData(Size))return 0;
    Data.resize((size_t)Size);
    int Ret = 0;
    for (auto& d : Data)
    {
        if (!ReadData(d)) break;
        else ++Ret;
    }
    return Ret;
}
template<typename T>
int ExtFileClass::WriteVector(const std::vector<T>& Data, const std::function<bool(const ExtFileClass&, const T&)>& Proc)const//待测试！！
{
    if (!WriteData((int64_t)Data.size()))return 0;
    int Ret = 0;
    for (const auto& d : Data)
    {
        if (!Proc(*this, d)) break;
        else ++Ret;
    }
    return Ret;
}
template<typename T>
int ExtFileClass::WriteVector(std::vector<T>& Data, const std::function<bool(const ExtFileClass&, T&)>& Proc)const//待测试！！
{
    if (!WriteData((int64_t)Data.size()))return 0;
    int Ret = 0;
    for (auto& d : Data)
    {
        if (!Proc(*this, d)) break;
        else ++Ret;
    }
    return Ret;
}
template<typename T>
int ExtFileClass::ReadVector(std::vector<T>& Data, const std::function<bool(const ExtFileClass&, T&)>& Proc)const//待测试！！
{
    int64_t Size;
    if (!ReadData(Size))return 0;
    Data.resize((size_t)Size);
    int Ret = 0;
    for (auto& d : Data)
    {
        if (!Proc(*this, d)) break;
        else ++Ret;
    }
    return Ret;
}

ExtFileClass::~ExtFileClass()
{
    if (IsOpen)Close();
}

size_t ExtFileClass::GetSize()
{
    int Cur = Position();
    Seek(0, SEEK_END);
    int Res = Position();
    Seek(Cur,SEEK_SET);
    return Res;
}

PArray<BYTE> ExtFileClass::ReadWholeFile(size_t ReservedBytes)
{
    PArray<BYTE> Result;
    Result.Alloc(GetSize() + ReservedBytes);
    Read((void*)const_cast<BYTE*>(Result.Data), 1, Result.N);
    return Result;
}






ExtCCFile::ExtCCFile(CCFileClass* pp):pCC(pp),IsOpen(true),Pos(0)
{
    Seek(0, ExtCCFile::GetSeekSet());
}

CCFileClass* ExtCCFile::GetRaw()const
{
    return pCC;
}

bool ExtCCFile::Open(const char* path, ExtCCFile::AccessMode mode, void* Reserved)
{
    pCC = GameCreate<CCFileClass>(path);
    if (!pCC->Exists())return IsOpen = false;
    if (!pCC->Open(mode))return IsOpen = false;
    IsOpen = true;
    Pos = 0;
    return IsOpen;
}
size_t ExtCCFile::Read(void* Buf, size_t Size, size_t Count)
{
    std::byte* pBuf = (std::byte*)Buf;
    if (!Available())return 0;
    for (size_t i = 0; i < Count; i++)
    {
        if (Eof())return i;
        pCC->ReadBytes(pBuf + i * Size, Size);
        Pos += Size;
    }
    return Count;
}
//size_t ExtCCFile::Write(const void* Buf, size_t Size, size_t Count)const;
char ExtCCFile::GetChr()
{
    if (!Available())return 0;
    if (Eof())return 0;
    char bf;
    pCC->ReadBytes(&bf, 1);
    ++Pos;
    return bf;
}
//bool ExtCCFile::PutChr(const char Byte)const;
//bool ExtCCFile::Ln()const;

bool GetStr_Cut_Impl(char* pBuf, size_t Size)
{
    char* Ln1 = strnchr(pBuf, '\r', Size);
    char* Ln2 = strnchr(pBuf, '\n', Size);
    auto [pMin, pMax] = std::minmax(Ln1, Ln2);
    if (!pMin)
    {
        if (pMax)
        {
            *pMax = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        *pMin = 0;
        return true;
    }
}

char* ExtCCFile::GetStr(char* Buf, size_t Size)
{
    if (!Available())return nullptr;
    if (Eof())return nullptr;
    size_t ToRead_Lst = 0;
    size_t ToRead = 1;
    for (ToRead = 1; ToRead < Size; ToRead <<= 1)
    {
        ToRead_Lst = ToRead >> 1;//1 >> 1 = 0
        auto pBuf = Buf + ToRead_Lst;
        pCC->ReadBytes(pBuf, ToRead - ToRead_Lst);
        Pos += ToRead - ToRead_Lst;
        if (GetStr_Cut_Impl(pBuf, ToRead - ToRead_Lst))return Buf;
        if (Eof())
        {
            Buf[ToRead] = '\0';
            return Buf;
        }
    }
    ToRead_Lst = ToRead >> 1;
    auto pBuf = Buf + ToRead_Lst;
    pCC->ReadBytes(pBuf, Size - 1 - ToRead_Lst);
    Pos += Size - 1 - ToRead_Lst;
    if (GetStr_Cut_Impl(pBuf, Size - 1 - ToRead_Lst))return Buf;
    Buf[Size - 1] = '\0';
    return Buf;
}
//size_t ExtCCFile::PutStr(const char* Buf)const;
int ExtCCFile::Close()
{
    if (!Available())return FALSE;
    pCC->Close();
    GameDelete(pCC);
    pCC = nullptr;
    Pos = 0;
    IsOpen = false;
    return TRUE;
}
int ExtCCFile::Seek(int Offset, ExtCCFile::SeekMode Base)
{
    if (!Available())return -1;
    size_t sz = pCC->GetFileSize();
    switch (Base)
    {
    case ExtCCFile::SeekMode::Set:Pos = Offset; break;
    case ExtCCFile::SeekMode::Current:Pos += Offset; break;
    case ExtCCFile::SeekMode::End:Pos = (size_t)((int)sz +Offset); break;
    default:break;
    }
    if (Pos > sz)Pos = sz;
    return pCC->Seek(Offset, Base);
}
int ExtCCFile::Position()const
{
    if (!Available())return -1;
    return Pos;
}
bool ExtCCFile::Eof()const
{
    const char* BoolCStr(bool);
    if (!Available())return true;
    return Pos >= (size_t)pCC->GetFileSize();
}
bool ExtCCFile::Available()const
{
    if (!pCC)return false;
    return IsOpen;
}
void ExtCCFile::Rewind()
{
    if (!Available())return;
    pCC->Seek(0, FileSeekMode::Set);
    Pos = 0;
}
bool ExtCCFile::Exists()const
{
    if (!IsOpen)return false;
    if (!pCC)return false;
    return pCC->Exists();
}

ExtCCFile::~ExtCCFile()
{
    if (Available())Close();
}

ExtCCFile::AccessMode ExtCCFile::GetReadSign()
{
    return FileAccessMode::Read;
}
ExtCCFile::AccessMode ExtCCFile::GetBinaryReadSign()
{
    return FileAccessMode::Read;
}
ExtCCFile::SeekMode ExtCCFile::GetSeekSet()
{
    return SeekMode::Set;
}
ExtCCFile::SeekMode ExtCCFile::GetSeekCur()
{
    return SeekMode::Current;
}
ExtCCFile::SeekMode ExtCCFile::GetSeekEND()
{
    return SeekMode::End;
}

size_t ExtCCFile::GetSize()
{
    if (!Available())return 0u;
    return pCC->GetFileSize();
}
PArray<BYTE> ExtCCFile::ReadWholeFile(size_t ReservedBytes) const
{
    PArray<BYTE> cc; cc.N = 0;
    if (!Available())return cc;
    cc.Alloc(pCC->GetFileSize() + ReservedBytes);
    pCC->ReadBytes((void*)cc.Data, cc.N);
    return cc;
}

CCFileClass* ExtCCFile::Release()
{
    IsOpen = false;
    Pos = 0;
    CCFileClass* Ret = nullptr;
    std::swap(Ret, pCC);
    return Ret;
}