#pragma once
#include <YRPP.h>
#include <CD.h>
#include <CRT.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "ToolFunc.h"
#include <IH.Loader.h>

class IHFileClass;

struct CDPath : public ForwardNode<CDPath>
{
	char* Path;

	~CDPath()
	{
		CRT::free(Path);
	}
};

class CDDrives
{
public:
	static constexpr reference<CDDrives, 0x89E410u> const Instance;

	ForwardList<CDPath> Paths;
private:
	CDDriveManagerClass** Current;
	CDDriveManagerClass** Prev;
public:
	char PathStr[516];

	inline CDDriveManagerClass* GetManager()
	{
		return *Current;
	}

	inline CDDriveManagerClass* GetPreviousManager()
	{
		return *Prev;
	}

	inline void PushPathToListHead(const char* Str)
	{
		CDPath* pPath = GameCreate<CDPath>();
		pPath->Path = CRT::strdup(Str);
		Paths.Push(pPath);
	}

	inline char* PushPathToListTail(const char* Str)
	{
		JMP_STD(0x47AD50);
	}

	static BOOL RemakeCDPath()
	{
		JMP_STD(0x47AAC0);
	}

	static BOOL __fastcall MakeCDPath(const char* PathStr)//以;分隔路径
	{
		JMP_STD(0x47AB10);
	}
};

class CDExt
{
	CDDrives* pDrives;
	std::unordered_map<std::string, std::string, UpperHash, UpperEqualPred> Redirect;
	std::unordered_set<std::string, UpperHash, UpperEqualPred> LoadedMixFileNames;
public:
	std::vector<std::string> PathHead;
	std::vector<std::string> PathTail;
	std::vector<std::string> PathFirst;
	

	CDExt() = delete;
	CDExt(const CDExt&) = delete;
	CDExt(CDExt&&) = delete;
	CDExt(CDDrives* p);

	void PushCustomPathToHead(const char* Path);
	void PushCustomPathToHead(const std::string& Path);
	void PushCustomPathToTail(const char* Path);
	void PushCustomPathToTail(const std::string& Path);
	void PushCustomPathToFirst(const char* Path);
	void PushCustomPathToFirst(const std::string& Path);
	void AddRedirect(const char* Old, const char* New);
	const char* TryRedirect(const char* Name);
	void RegisterLoadedMix(const char* Name);
	bool IsLoadedMix(const char* Name);
};
extern CDExt CDExt_Instance;
extern std::unordered_set<std::string, UpperHash, UpperEqualPred> BlackList;
extern std::unordered_set<std::string, UpperHash, UpperEqualPred> WhiteList;
void ExtCD_InitBeforeEverything();
std::string GetBindingIHFile(const char* pFileName);
bool HasBindingIHFile(const char* pFileName);

struct BiasedFileEntry
{
	bool Biased;
	bool NeedsCachedAccess;
	bool CachedAccess;
	bool CachedAccessClosed;
	int BiasedOffset;
	int BiasedLength;
	char* BiasedFileName;
	size_t CachedAccessThreshold;
	size_t CachedAccessToken;
	int CachedAccessPosition;
	FileIterationType CachedAccessIterType;
private:
	DWORD FileStart;//PlaceHolder
public:
	IHFileClass* GetFile()
	{
		return reinterpret_cast<IHFileClass*>(&FileStart);
	}

	static consteval size_t HeaderSize()
	{
		return sizeof(BiasedFileEntry) - sizeof(DWORD);
	}

	int Position();

	void CloseCache();

	int Seek(int Offset, FileSeekMode Mode);

	int ReadBytes(void* Buffer, int Size);
};
static_assert(BiasedFileEntry::HeaderSize() % 16 == 0);

class FileClassExt
{
public:
	const char* CDFileClass_SetFileName(char* pOriginalFileName);
	const char* RawFileClass_GetFileName();
	bool BufferIOFileClass_Exists(bool WriteShared);
	CCFileClass* CCFileClass_Constructor_pFileName_InMixFile(const char* Source);
	//same as CCFileClass::CTOR but an ordinary member function here
	CCFileClass* CCFileClass_Constructor_pFileName_Orig(const char* Source) { JMP_THIS(0x4739F0); }
	BOOL CCFileClass_Open(FileAccessMode Mode);
	bool CCFileClass_Exists(bool WriteShared);
};

struct IHExtPtrType
{
	BiasedFileEntry* GetEntry();
	IHFileClass* GetIHExt();
	void Destroy();
};

struct IHExtPtrTypeDeleter
{
	void operator()(IHExtPtrType* p);
};

using IHExtUniquePtr = std::unique_ptr<IHExtPtrType, IHExtPtrTypeDeleter>;