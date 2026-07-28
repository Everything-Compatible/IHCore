// Separate compilation unit for stb_vorbis implementation.
// The original stb_vorbis.h is NEVER included in files that also pull in
// Windows SDK headers, avoiding type conflicts.

#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.h"
