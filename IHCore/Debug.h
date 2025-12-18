#pragma once
#include <Windows.h>
#include <Helpers/Macro.h>

namespace Debug
{
	inline void Log(const char* pFormat, ...)
	{
		JMP_STD(0x4068E0);
	}
};


#define IH_ENTER_FN_LOG(fn,ext) //(Debug::Log("IHCore : Enter hook " #fn " at 0x%08X, Ext call " #ext " at 0x%08X \n",fn,ext)) 

#define IH_EXIT_FN_LOG(fn,ext) //(Debug::Log("IHCore : Exit hook " #fn " at 0x%08X, Ext call " #ext " at 0x%08X \n",fn,ext)) 