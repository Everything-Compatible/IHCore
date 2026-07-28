// Thin wrapper header for stb_vorbis.
// Declares only the types and functions needed by ExtAudio.cpp.
// The actual stb_vorbis.h is included only in stb_vorbis_impl.cpp,
// keeping its type definitions isolated from Windows SDK headers.

#pragma once
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// --- Types matching stb_vorbis.h declarations ---

typedef struct stb_vorbis stb_vorbis;

typedef struct
{
	unsigned int sample_rate;
	int channels;
	unsigned int setup_memory_required;
	unsigned int setup_temp_memory_required;
	unsigned int temp_memory_required;
	int max_frame_size;
} stb_vorbis_info;

typedef struct
{
	char* alloc_buffer;
	int alloc_buffer_length_in_bytes;
} stb_vorbis_alloc;

// --- Functions used by ExtAudio.cpp ---

extern stb_vorbis_info stb_vorbis_get_info(stb_vorbis* f);
extern stb_vorbis* stb_vorbis_open_memory(const unsigned char* data, int len, int* error, const stb_vorbis_alloc* alloc);
extern unsigned int stb_vorbis_stream_length_in_samples(stb_vorbis* f);
extern int stb_vorbis_get_samples_short_interleaved(stb_vorbis* f, int channels, short* buffer, int num_shorts);
extern void stb_vorbis_close(stb_vorbis* f);

#ifdef __cplusplus
}
#endif
