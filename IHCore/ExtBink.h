#pragma once
#include <YRPP.h>

#define BINKIOPROCESSOR       0x02000000L // Set an io processor (call BinkIO first)


struct BinkIOUserData
{
    CCFileClass* pFile;
    CRITICAL_SECTION* cs;
    bool ownsFile;   // true=堆BINKIO拥有文件, close时释放; false=栈BINKIO/错误路径, close时不释放
};

struct BINKIO {
    int(__stdcall* seek_read)(BINKIO* io, int pos, void* buf, int size);
    int(__stdcall* read_async)(BINKIO* io, int frame, int offset, void* buf, int size);
    int(__stdcall* io_size)(BINKIO* io, int param);
    void(__stdcall* prepare)(BINKIO* io, void* buf, int size, int maxSize, int flags);
    int(__stdcall* pending)(BINKIO* io);
    void(__stdcall* close)(BINKIO* io);
    DWORD UserDataKey;
    
    BinkIOUserData& GetUserData();
	void DestroyUserData();

    CCFileClass* GetFile()
    {
        return GetUserData().pFile;
    }
    LPCRITICAL_SECTION GetCS()
    {
        return GetUserData().cs;
	}


};

using BinkIOInitFn = int  (__stdcall *) (BINKIO* io, DWORD Handle, int Flags);



void __stdcall ExtBink_SetIO(BinkIOInitFn callback);

int __stdcall ExtBink_Open(DWORD Handle, DWORD Flags);

void ExtBink_InitBeforeEverything();