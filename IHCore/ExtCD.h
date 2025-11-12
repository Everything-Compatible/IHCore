#pragma once
#include <YRPP.h>
#include <CD.h>
#include <CRT.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "ToolFunc.h"

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
};
extern CDExt CDExt_Instance;
extern std::unordered_set<std::string, UpperHash, UpperEqualPred> BlackList;
extern std::unordered_set<std::string, UpperHash, UpperEqualPred> WhiteList;
void ExtCD_InitBeforeEverything();

class FileClassExt
{
public:
	const char* CDFileClass_SetFileName(char* pOriginalFileName);
	const char* RawFileClass_GetFileName();
	bool BufferIOFileClass_Exists(bool WriteShared);
	
	/*
	bool CDFileClass_Open(FileAccessMode Mode)
	{
		static std::unordered_set<std::string> Set;
		auto This = reinterpret_cast<CDFileClass*>(this);
		if (Set.insert(This->GetFileName()).second)
			Debug::Log("Opening File \"%s\"\n", This->GetFileName());
		return This->BufferIOFileClass::Open(Mode);
	}
	bool CDFileClass_Open(FileAccessMode Mode)
	{
		auto This = reinterpret_cast<CDFileClass*>(this);
		Debug::Log("Opening File \"%s\"\n", This->GetFileName());
		std::string Name = This->GetFileName();
		for (auto& c : Name)c = (char)toupper(c);
		if (Name == "RA2MD.CSF")return false;
		return This->BufferIOFileClass::Open(Mode);
	}
	*/
	
};