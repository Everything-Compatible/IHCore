#pragma once

#include<cstdio>
#include<string>
#include<vector>
#include<cstdint>
#include<functional>
#include"../Common/IHLoader/IH.Loader.h"
#include"../Common/IHLoader/PArray.h"


class CCFileClass;
enum class FileAccessMode;
enum class FileSeekMode;

class ExtFileClass
{
	FILE* fp;
	bool IsOpen{ false };
public:
	using AccessMode = const char*;
	using AccessModeW = const wchar_t*;
	using SeekMode = int;

    FILE* GetRaw()const;
	bool Open(const char* path, AccessMode mode, void* Reserved = nullptr);
    bool Open(const wchar_t* path, AccessModeW mode, void* Reserved = nullptr);
    size_t Read(void* Buf, size_t Size, size_t Count)const;
    size_t Write(const void* Buf, size_t Size, size_t Count)const;
	char GetChr()const;
	bool PutChr(const char Byte)const;
	bool Ln()const;
	char* GetStr(char* Buf, size_t Size)const;
	size_t PutStr(const char* Buf)const;
	int Close();
	int Seek(int Offset, SeekMode Base)const;
	int Position()const;
	bool Eof()const;
	bool Available()const;
	void Rewind()const;
	int Flush()const;
	size_t GetSize();
	PArray<BYTE> ReadWholeFile(size_t ReservedBytes = 0);

	bool ReadData(std::string& Str)const;
	bool WriteData(const std::string& Str)const;
	bool WriteLabel(const std::string& Str)const;
    bool ReadLabel(std::string& Str)const;
    template<typename T>
    bool WriteData(const T& Data)const;
    template<typename T>
    bool ReadData(T& Data)const;
    template<typename T>
    bool WriteVector(const std::vector<T>& Data)const;
    template<typename T>
    bool ReadVector(std::vector<T>& Data)const;
    int WriteVector(const std::vector<std::string>& Data)const;
    int ReadVector(std::vector<std::string>& Data)const;
    template<typename T>
    int WriteVector(const std::vector<T>& Data, const std::function<bool(const ExtFileClass&, const T&)>& Proc)const;
    template<typename T>
    int WriteVector(std::vector<T>& Data, const std::function<bool(const ExtFileClass&, T&)>& Proc)const;
	template<typename T>
	int ReadVector(std::vector<T>& Data, const std::function<bool(const ExtFileClass&, T&)>& Proc)const;

    ~ExtFileClass();

    static AccessMode GetReadSign() { return "r"; }
	static AccessMode GetBinaryReadSign() { return "rb"; }
    static AccessModeW GetReadSignW() { return L"r"; }
	static SeekMode GetSeekSet(){return SEEK_SET; }
	static SeekMode GetSeekCur(){return SEEK_CUR; }
	static SeekMode GetSeekEND() { return SEEK_END; }
};

class ExtCCFile
{
private:
	CCFileClass* pCC;
	bool IsOpen{ false };
	size_t Pos;
public:
	using AccessMode = FileAccessMode;
	using SeekMode = FileSeekMode;

	ExtCCFile() = default;
	ExtCCFile(CCFileClass* pp);
	CCFileClass* GetRaw()const;

	bool Open(const char* path, AccessMode mode, void* Reserved = nullptr);
	size_t Read(void* Buf, size_t Size, size_t Count);
	//size_t Write(const void* Buf, size_t Size, size_t Count)const;
	char GetChr();
	//bool PutChr(const char Byte)const;
	//bool Ln()const;
	char* GetStr(char* Buf, size_t Size);
	//size_t PutStr(const char* Buf)const;
	int Close();
	int Seek(int Offset, SeekMode Base);
	int Position()const;
	bool Eof()const;
	bool Available()const;
	void Rewind();
	bool Exists()const;
	size_t GetSize();
	PArray<BYTE> ReadWholeFile(size_t ReservedBytes = 0) const;
	CCFileClass* Release();

	~ExtCCFile();

	static AccessMode GetReadSign();
	static AccessMode GetBinaryReadSign();
	static SeekMode GetSeekSet();
	static SeekMode GetSeekCur();
	static SeekMode GetSeekEND();
};

template<typename FileLoader>
std::string GetStringFromFile(const char* FileName)
{
	FileLoader File;
	if (!File.Open(FileName, FileLoader::GetReadSign()))return "";
	PArray<BYTE> Arr = File.ReadWholeFile(16);
	std::string LoadStr{ (char*)Arr.Data };
	Arr.Delete();
	return LoadStr;
}