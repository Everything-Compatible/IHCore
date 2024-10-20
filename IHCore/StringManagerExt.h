#pragma once
#include <YRPP.h>
#include <MapSeedClass.h>
#include "Patch.h"
#include "Debug.h"
#include "ExtCsf.h"
#include "LoaderLib.h"



namespace StringManagerExt
{
	bool __fastcall LoadFile(char* FileName);
	CSFString* Unload();
	const wchar_t* __fastcall LoadCSFString(const char* pLabel, const char** pOutExtraData,
		const char* pSourceCodeFileName, int nSourceCodeFileLine);

	void Apply();
	void MergeFromIni(CCINIClass* pINI);
	void MergeStringTable(const char* Name);
};




class CSFFile
{
	CSFHandle Handle;
	uint32_t Version, LabelCount, StringCount, Reserved;
	CSFLanguages Language;
public: 

	static void Counstructor(CSFFile* This);
	static void Destructor(CSFFile* This);
	static void MoveAssign(CSFFile* This, CSFFile* Another);
	static void Assign(CSFFile* This, CSFFile* Another);
	static bool LoadAsCCFile_A(CSFFile* This, const char* name);
	static bool LoadAsCCFile_B(CSFFile* This, CCFileClass* pCC);
	static bool LoadAsExternalFile(CSFFile* This, const char* name);
	static CSFEntry GetString(CSFFile* This, const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
	static CSFEntry GetStringDefault(CSFFile* This, const char* Key);//merge操作之后请重新获取，clear操作之后全部失效
	static void Merge_A(CSFFile* This, CSFFile* Another);
	static void Merge_B(CSFFile* This, const char* Key, const wchar_t* value, const char* Extra);
	static bool Available(CSFFile* This);
	static void Clear(CSFFile* This);
};

struct CSFFile_ITable
{
	decltype(&CSFFile::Counstructor) Constructor;
	decltype(&CSFFile::Destructor) Destructor;
	decltype(&CSFFile::MoveAssign) MoveAssign;
	decltype(&CSFFile::Assign) Assign;
	decltype(&CSFFile::LoadAsCCFile_A) LoadAsCCFile_A;
	decltype(&CSFFile::LoadAsCCFile_B) LoadAsCCFile_B;
	decltype(&CSFFile::LoadAsExternalFile) LoadAsExternalFile;
	decltype(&CSFFile::GetString) GetString;//merge操作之后请重新获取
	decltype(&CSFFile::GetStringDefault) GetStringDefault;//merge操作之后请重新获取
	decltype(&CSFFile::Merge_A) Merge_A;
	decltype(&CSFFile::Merge_B) Merge_B;
	decltype(&CSFFile::Available) Available;
	decltype(&CSFFile::Clear) Clear;
};

extern CSFFile_ITable CSFFile_InstTable;