
#include <YRPP.h>
#include <Helpers/Macro.h>
#include "Debug.h"
#include "Patch.h"



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

