#pragma once

#include <ExtJson.h>

struct FontConfig
{
	const char* ss;
};

class IHConfigClass
{
	JsonObject Font;
};
extern IHConfigClass IHInstConfig;