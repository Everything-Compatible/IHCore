#pragma once

#include <climits>
#include <string>
#include <vector>
#include <Windows.h>
#include <ArrayClasses.h>

#define UTF8 //Used in code to mark a utf8 string

using Version_t = unsigned;
using LoadID_t = unsigned;
using HPHandle = void*;
using Name_t = const char*;
using U8Name_t = const char8_t*;
using WName_t = const wchar_t*;
using ObjHandle = HPHandle;
using FuncHandle = HPHandle;

struct CStringAW;
struct StdStringAW;
struct CStringAWU;
struct StdStringAWU;
template<typename T> struct PointerArray;
//template<typename T> struct PointerBufferArray;//for POD or something alike
//template<typename T> struct PointerRemoteArray;

struct HPVersionInfo;
struct HPDLLHeader;
struct HPDLLInput;
struct HPDLLOutput;
struct HPDLLData;
struct HPFuncPackHeader;
struct HPFuncPack;
struct HPDependency;

using HPLoadI_t = const HPDLLOutput* (__cdecl*)(const HPDLLInput*);
using HPLoadII_t = bool(__cdecl*)(void);

const Version_t NoVersionLimit = UINT_MAX;

struct CStringAW
{
	Name_t A;
	WName_t W;

	CStringAW() = default;
	CStringAW(const StdStringAW& Std);
	operator StdStringAW();
};

struct StdStringAW
{
	std::string A;
	std::wstring W;

	StdStringAW() = default;
	StdStringAW(const CStringAW& CS);
	operator CStringAW();
};

struct CStringAWU
{
	Name_t A;
	WName_t W;
	U8Name_t U;

	CStringAWU() = default;
	CStringAWU(const StdStringAWU& Std);
	operator StdStringAWU();
};

struct StdStringAWU
{
	std::string A;
	std::wstring W;
	std::u8string U;

	StdStringAWU() = default;
	StdStringAWU(const CStringAWU& CS);
	operator CStringAWU();
};

#define MakeCstringAW_Impl(str) CStringAW{Str ,L ## Str}
#define MakeCstringAW(str) MakeCstringAW_Impl(Str)
#define MakeCstringAWU_Impl(str) CStringAWU{Str ,L ## Str ,u8 ## Str}
#define MakeCstringAWU(str) MakeCstringAWU_Impl(Str)

template<typename T>
struct PointerArray
{
	size_t N;
	T* Data;
	void Delete() { N = 0; delete[]Data; }
	void Alloc(size_t n) { if (N)Delete(); N = n; Data = new T[n]; }
};

using BytePointerArray = PointerArray<std::byte>;
/*
template<typename T>
struct PointerBufferArray : public PointerArray
{
	PointerBufferArray() = delete;
	PointerBufferArray(const PointerBufferArray<T>&);
	PointerBufferArray(PointerBufferArray<T>&&);
	PointerBufferArray(PointerBufferArray<T>&&);
	PointerBufferArray(size_t NObj);
	PointerBufferArray(const std::vector<T>&);
	~PointerBufferArray();
};
template<typename T>
struct PointerRemoteArray
{

};*/

template<typename T>
inline PointerArray<T> GetNullPointerArray() { return PointerArray<T>{ 0, nullptr }; }
template<typename T>
inline PointerArray<T> GetArrayView(const std::vector<T>& Vec) { return PointerArray<T>{ Vec.size(), (T*)Vec.data() }; }

struct HPVersionInfo
{
	Name_t Name;
	Version_t Version;
	Version_t LowestSupportedVersion;
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//you MUST check its size when you receive a struct with a Size
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

struct HPDLLInput
{
	const size_t Size{ sizeof(HPDLLInput) };

	HPDLLHeader* Header;
	LoadID_t LoadID;
	CStringAW AbsPath;
	const HPFuncPackHeader* (__cdecl* GetFuncPack)(
		Name_t Name, Version_t RequiredDLLVersion, Version_t RequiredFuncPackVersion);//Available since LoadII; 
	const VectorClass<FuncHandle>(__cdecl* GetCallback)(Name_t FuncName);//TODO: CAN IT REALLY WORK?
};

struct HPDLLHeader
{
	const size_t Size{ sizeof(HPDLLHeader) };

	HPVersionInfo Info;
	bool AllowMultipleVersions;
};

struct HPFuncPackHeader
{
	const size_t Size{ sizeof(HPFuncPackHeader) };

	HPVersionInfo Info;
	size_t PackSize;
	ObjHandle PackData;//Pointer to the actual FuncPack
};

struct HPFuncPackData
{
	const size_t Size{ sizeof(HPFuncPackData) };

	HPFuncPackHeader* Header;
	PointerArray<HPDependency> Dependency;
	bool (__cdecl* InitFuncPack)(void);
};

struct HPDLLOutput
{
	const size_t Size{ sizeof(HPDLLOutput) };

	HPDLLHeader* Header;
	HPLoadI_t HPLoadI;
	HPLoadII_t HPLoadII;
	CStringAW AbsPath;
	CStringAW AbsDir;
	HMODULE Handle;
	PointerArray<HPFuncPackData> FuncPacks;
	const FuncHandle(__cdecl* GetCallback)(Name_t FuncName);
};

struct HPDependency
{
	enum _Type : int8_t
	{
		RuntimeOptional = 0,
		RuntimeRequired = 1,
		LoadOptional = 2,
		LoadRequired = 3
	};

	Name_t Name;
	Version_t RequiresVersion;
	_Type Type;
};

struct HPDLLData//At Core
{
	HPLoadI_t HPLoadI;
	HPDLLInput Input;
	HPDLLOutput* Output;
};


#define HPExportFunc extern "C" __declspec(dllexport)
#define HPImportFunc extern "C" __declspec(dllimport)