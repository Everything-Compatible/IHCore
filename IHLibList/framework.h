#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#define EXPORT __declspec(dllexport)

using HPHandle = void*;
using FuncHandle = HPHandle;

struct InitialLoadParam 
{
	size_t size;
};

struct UniPArray
{
	size_t N;
	void* Ptr;
};

struct IHInitialLoadService
{
	const char* Service;
	const InitialLoadParam* Param;
	int Reserved[6];
};

extern "C"
{
	EXPORT int __cdecl GetNLibs(void);
	EXPORT int __cdecl GetMaxLibs(void);
	EXPORT bool __cdecl RegisterEntry(FuncHandle);
	EXPORT const FuncHandle* __cdecl GetEntries(void);
	EXPORT void __cdecl ServiceRequest(IHInitialLoadService);
	EXPORT void __cdecl QueryServiceRequest(const char* Service, UniPArray* Ret);
}
