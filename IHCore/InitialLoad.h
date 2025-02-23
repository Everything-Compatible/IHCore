#pragma once
#include <IH.h>

extern InitialLoad::Service<InitialLoadParam_CustomPath>
	Service_CustomPathListHead,
	Service_CustomPathListTail,
	Service_CustomPathListFirst;

extern InitialLoad::Service<InitialLoadParam_CustomFile>
	Service_StringTable,
	Service_MixFile;

extern InitialLoad::Service<InitialLoadParam_StringTablePair>
	Service_StringTableAddPair;

extern InitialLoad::Service<InitialLoadParam_RedirectFile>
	Service_BindToStream,
	Service_RedirectFile;

extern InitialLoad::Service<InitialLoadParam_RegisterVTable>
	Service_RegisterIHFile;

extern InitialLoad::Service<InitialLoadParam_RegisterFunction>
	Service_RegisterIHFileFilter,// bool(__cdecl*)(const char*)
	Service_RegisterListenerAccess;// FuncHandle (__cdecl*)(const char*) FOR INTERNAL USE