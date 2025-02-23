#pragma once
/*
	辅助头文件，不被 EC.h 包含
*/
#include <YRPP.h>
#include "EC.Stream.h"

typedef void (CALLBACK* InternalService_PointerGotInvalid)(AbstractClass* const pObject, bool bRemoved);
typedef void (CALLBACK* InternalService_LoadGame)(IStream* pStream);
typedef bool (CALLBACK* SIFinalSwizzleFn)(void* pOld, void*& pNew);
typedef void (CALLBACK* InternalService_FinalSwizzle)(IStream* pStream, SIFinalSwizzleFn SIFinalSwizzle);
typedef void (CALLBACK* InternalService_SaveGame)(IStream* pStream);

struct LoadSaveInterface
{
	InternalService_LoadGame LoadGame;
	InternalService_FinalSwizzle FinalSwizzle;
	InternalService_SaveGame SaveGame;
	InternalService_PointerGotInvalid PointerGotInvalid;

	int Reserved[4];
};

static_assert(sizeof(LoadSaveInterface) == 32);

const char* Internal_GetLibRegName();
int Internal_GetLibVersion();
void Internal_DebugLog(const char* pFormat, ...);


void ECGameClass_PointerGotInvalid(AbstractClass* const pObject, bool bRemoved);
void ECGameClass_FinalSwizzle();
void ECGameClass_LoadGame(IStream* pStream);
void ECGameClass_SaveGame(IStream* pStream);