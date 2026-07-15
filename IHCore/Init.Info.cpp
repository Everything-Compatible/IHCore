#include "Version.h"
#include <YRPP.h>

DEFINE_HOOK(0x531413, IHGameStart, 5)
{
	DSurface::Hidden->DrawText(PRODUCT_FULLNAME_STR L" is active!", 10, 480, COLOR_RED | COLOR_GREEN);
	return 0;
}
