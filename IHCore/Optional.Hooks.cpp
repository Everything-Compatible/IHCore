
#include <YRPP.h>
#include <Helpers/Macro.h>
#include "Debug.h"
#include "Patch.h"
#include "SomeData.h"
#include "ToolFunc.h"


DEFINE_HOOK(0x7C946E, IHMalloc, 5)
{
	GET_STACK(size_t, Size, 4);
	GET_STACK(size_t, Caller, 0);
	Debug::Log("%X Allocate %d(0x%X) bytes\n", Caller, Size, Size);
	return 0;
}
DEFINE_HOOK(0x7C93E8, IHFree, 5)
{
	GET_STACK(void*, Block, 4);
	GET_STACK(size_t, Caller, 0);
	Debug::Log("%X Free Block %X\n", Caller, Block);
	return 0;
}

void __cdecl IHFree(void* Block)
{
	free(Block);
}


bool WWSBShouldCatch = false;
wchar_t IHExceptionHandlerBuf[2000];
DEFINE_HOOK(0x6BB996, IHExceptionHandler, 5)
{
	if (WWSBShouldCatch)return 0x4C8FE0;
	else
	{
		GET_STACK(PEXCEPTION_POINTERS, pExcept, 0x4);
		swprintf_s(IHExceptionHandlerBuf, L"Õ¨ÁËß÷ EIP = %08X Code = %08X",
			pExcept->ContextRecord->Eip, pExcept->ExceptionRecord->ExceptionCode);
		MessageListClass::Instance->PrintMessage(IHExceptionHandlerBuf);
		Debug::Log(UnicodetoANSI(IHExceptionHandlerBuf).c_str());
		R->EAX(-1);
		return 0x4C9144;
	}
}

//6BB996 = IHExceptionHandler, 5