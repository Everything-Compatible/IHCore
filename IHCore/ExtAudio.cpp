// stb_vorbis is compiled separately in stb_vorbis_impl.cpp.
// Only the thin wrapper header is included here to avoid
// conflicts with Windows SDK headers.
#include "stb_vorbis_wrapper.h"

#include "ExtAudio.h"
#include "Debug.h"
#include <cstdio>
#include <cstring>

//=============================================================================
// OGGManager
//=============================================================================

OGGManager& OGGManager::Instance()
{
	static OGGManager instance;
	return instance;
}

OGGManager::~OGGManager()
{
	ClearAll();
}

bool OGGManager::CacheOggHeader(const std::string& key, const std::string& oggPath)
{
	if (m_cache.find(key) != m_cache.end())
		return true;

	// Read the .ogg file into memory
	FILE* fp = fopen(oggPath.c_str(), "rb");
	if (!fp)
	{
		Debug::Log("[OGG] Failed to open \"%s\"\n", oggPath.c_str());
		return false;
	}

	fseek(fp, 0, SEEK_END);
	long oggSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (oggSize <= 0)
	{
		fclose(fp);
		Debug::Log("[OGG] Empty file \"%s\"\n", oggPath.c_str());
		return false;
	}

	std::vector<uint8_t> oggData(oggSize);
	if (fread(oggData.data(), 1, oggSize, fp) != (size_t)oggSize)
	{
		fclose(fp);
		Debug::Log("[OGG] Failed to read \"%s\"\n", oggPath.c_str());
		return false;
	}
	fclose(fp);

	return CacheOggHeader(key, oggPath, oggData.data(), (int)oggSize);
}


bool OGGManager::CacheOggHeader(const std::string& key, const std::string& oggPath, void* pBuffer, int Size)
{
	if (m_cache.find(key) != m_cache.end())
		return true;

	// Copy raw OGG to DLL heap so the caller can free pBuffer immediately.
	// stb_vorbis reads from this copy in both Phase 1 (header) and Phase 2 (decode).
	Entry entry;
	const uint8_t* src = static_cast<const uint8_t*>(pBuffer);
	entry.rawOgg.assign(src, src + Size);

	// Phase 1: open OGG, read header info only, then close — no decode.
	int error = 0;
	stb_vorbis* vorbis = stb_vorbis_open_memory(entry.rawOgg.data(), (int)entry.rawOgg.size(), &error, nullptr);
	if (!vorbis)
	{
		Debug::Log("[OGG] stb_vorbis header parse error %d for \"%s\"\n", error, oggPath.c_str());
		return false;
	}

	stb_vorbis_info info = stb_vorbis_get_info(vorbis);
	unsigned int totalSamples = stb_vorbis_stream_length_in_samples(vorbis);
	stb_vorbis_close(vorbis);  // Close immediately — no PCM decoding yet

	if (totalSamples == 0)
	{
		Debug::Log("[OGG] Zero-length audio in \"%s\"\n", oggPath.c_str());
		return false;
	}

	// Fixed output format: 22050 Hz, 16-bit, PCM
	const uint32_t TARGET_RATE   = 22050;
	const uint16_t BITS          = 16;
	const uint16_t channels      = (uint16_t)info.channels;
	const uint32_t bytesPerFrame = channels * (BITS / 8);
	const uint32_t headerSize    = 44;

	// Compute resampled PCM size
	uint32_t dstFrames = (uint32_t)((uint64_t)totalSamples * TARGET_RATE / info.sample_rate);
	if (dstFrames == 0) dstFrames = 1;  // guard against degenerate files
	entry.finalDataSize = dstFrames * bytesPerFrame;

	// Build the 44-byte WAV header (sample rate fixed to 22050 Hz)
	entry.wavHeader.resize(headerSize);
	uint8_t* wav = entry.wavHeader.data();

	// RIFF chunk
	memcpy(wav,      "RIFF", 4);
	memcpy(wav + 8,  "WAVE", 4);
	*(uint32_t*)(wav + 4) = 36 + entry.finalDataSize;  // ChunkSize

	// fmt sub-chunk
	memcpy(wav + 12, "fmt ", 4);
	*(uint32_t*)(wav + 16) = 16;                        // Subchunk1Size (PCM)
	*(uint16_t*)(wav + 20) = 1;                         // AudioFormat (PCM)
	*(uint16_t*)(wav + 22) = channels;
	*(uint32_t*)(wav + 24) = TARGET_RATE;                // SampleRate  → fixed 22050
	*(uint32_t*)(wav + 28) = TARGET_RATE * bytesPerFrame; // ByteRate
	*(uint16_t*)(wav + 32) = bytesPerFrame;              // BlockAlign
	*(uint16_t*)(wav + 34) = BITS;                       // BitsPerSample

	// data sub-chunk
	memcpy(wav + 36, "data", 4);
	*(uint32_t*)(wav + 40) = entry.finalDataSize;        // Subchunk2Size

	entry.pcmReady = false;

	Debug::Log("[OGG] Header cached \"%s\" (%u Hz→22050 Hz, %u ch, %u→%u frames, raw=%zu bytes)\n",
		key.c_str(), info.sample_rate, channels, totalSamples, dstFrames, entry.rawOgg.size());

	m_cache[key] = std::move(entry);
	return true;
}

bool OGGManager::EnsurePcmDecoded(const std::string& key)
{
	Debug::Log("[OGG] Ensuring %s Loaded...\n", key.c_str());
	auto it = m_cache.find(key);
	if (it == m_cache.end())
		return false;

	Entry& entry = it->second;
	if (entry.pcmReady)
		return true;  // Already decoded — idempotent

	// Open from DLL-heap raw OGG copy and decode all PCM
	int error = 0;
	stb_vorbis* vorbis = stb_vorbis_open_memory(entry.rawOgg.data(), (int)entry.rawOgg.size(), &error, nullptr);
	if (!vorbis)
	{
		Debug::Log("[OGG] EnsurePcmDecoded: stb_vorbis error %d for \"%s\"\n", error, key.c_str());
		return false;
	}
	Debug::Log("[OGG] stb_vorbis opened %s Successfully.\n", key.c_str());

	stb_vorbis_info info = stb_vorbis_get_info(vorbis);
	const uint16_t channels      = (uint16_t)info.channels;
	const uint32_t bytesPerFrame = channels * 2;  // 16-bit

	unsigned int totalSamples = stb_vorbis_stream_length_in_samples(vorbis);

	// Decode all PCM frames into temporary buffer
	uint32_t srcPcmBytes = totalSamples * bytesPerFrame;
	Debug::Log("[OGG] channels = %u, bytesPerFrame = %u, totalSamples = %u, srcPcmBytes = %u\n",
		(unsigned)channels, bytesPerFrame, totalSamples, srcPcmBytes);
	std::vector<uint8_t> tempPcm(srcPcmBytes);
	Debug::Log("[OGG] Generating Samples\n");
	int samplesDecoded = stb_vorbis_get_samples_short_interleaved(
		vorbis, channels,
		reinterpret_cast<short*>(tempPcm.data()),
		(int)(totalSamples * channels));
	stb_vorbis_close(vorbis);

	if (samplesDecoded <= 0)
	{
		Debug::Log("[OGG] EnsurePcmDecoded: no samples from \"%s\"\n", key.c_str());
		return false;
	}
	Debug::Log("[OGG] Decoded %d Samples\n", samplesDecoded);

	// Resample to 22050 Hz if needed (linear interpolation)
	std::vector<uint8_t> finalPcm;
	if (info.sample_rate != 22050)
	{
		int srcFrames = samplesDecoded / channels;  // samplesDecoded is per-short, convert to frames
		int dstFrames = (int)((double)srcFrames * 22050.0 / (double)info.sample_rate);
		Debug::Log("[OGG] Adjusting to 22050Hz...\n[OGG] srcFrames = %d, dstFrames = %d\n[OGG] Allocating %u temp bytes\n",
			srcFrames, dstFrames, dstFrames * bytesPerFrame);
		finalPcm.resize(dstFrames * bytesPerFrame);

		const short* src = reinterpret_cast<const short*>(tempPcm.data());
		short*       dst = reinterpret_cast<short*>(finalPcm.data());
		double step = (double)info.sample_rate / 22050.0;
		Debug::Log("[OGG] src = %p dst = %p step = %f\n", src, dst, step);
		for (int i = 0; i < dstFrames; i++)
		{
			double srcPos = i * step;
			int    srcIdx = (int)srcPos;
			double frac   = srcPos - srcIdx;

			if (srcIdx >= srcFrames - 1)
			{
				for (int c = 0; c < channels; c++)
					dst[i * channels + c] = src[(srcFrames - 1) * channels + c];
			}
			else
			{
				for (int c = 0; c < channels; c++)
				{
					double s0 = src[srcIdx * channels + c];
					double s1 = src[(srcIdx + 1) * channels + c];
					double v  = s0 + (s1 - s0) * frac;
					if (v > 32767.0)  v = 32767.0;
					if (v < -32768.0) v = -32768.0;
					dst[i * channels + c] = (short)v;
				}
			}
		}
		Debug::Log("[OGG] EnsurePcmDecoded resampled \"%s\" %u→22050 Hz (%d→%d frames)\n",
			key.c_str(), info.sample_rate, srcFrames, dstFrames);
	}
	else
	{
		finalPcm = std::move(tempPcm);
	}
	Debug::Log("[OGG] Inserting %u bytes after header...\n", finalPcm.size());

	// Append PCM to wavHeader → complete WAV in memory
	entry.wavHeader.insert(entry.wavHeader.end(), finalPcm.begin(), finalPcm.end());

	// Release raw OGG copy (no longer needed after decode)
	entry.rawOgg.clear();
	entry.rawOgg.shrink_to_fit();
	entry.pcmReady = true;

	Debug::Log("[OGG] PCM decoded & cached \"%s\" (%zu bytes total)\n",
		key.c_str(), entry.wavHeader.size());
	return true;
}

bool OGGManager::Has(const std::string& key) const
{
	return m_cache.find(key) != m_cache.end();
}

const uint8_t* OGGManager::GetData(const std::string& key) const
{
	auto it = m_cache.find(key);
	// Phase 1: wavHeader is 44 bytes; Phase 2: wavHeader holds the complete WAV
	return (it != m_cache.end()) ? it->second.wavHeader.data() : nullptr;
}

size_t OGGManager::GetSize(const std::string& key) const
{
	auto it = m_cache.find(key);
	if (it == m_cache.end()) return 0;
	// Phase 1: virtual size (header + expected PCM); Phase 2: actual size
	if (it->second.pcmReady)
		return it->second.wavHeader.size();
	else
		return 44 + it->second.finalDataSize;
}

void OGGManager::ClearAll()
{
	m_cache.clear();
}

//=============================================================================
// OGGMemoryFileClass
//=============================================================================

void OGGMemoryFileClass::Initialize()
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> Initialize()\n", (LPCVOID)this);
	m_position = 0;
	m_open = false;
	m_initialized = false;
	new (&m_fileName) std::string();
	new (&m_cacheKey) std::string();
}

const char* OGGMemoryFileClass::GetFileName() const
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> GetFileName() = \"{}\"\n", (LPCVOID)this, m_fileName);
	return m_fileName.c_str();
}

const char* OGGMemoryFileClass::SetFileName(const char* pFileName)
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> SetFileName(\"{}\")\n", (LPCVOID)this, pFileName ? pFileName : "");
	m_fileName = pFileName ? pFileName : "";
	m_cacheKey = m_fileName;
	m_initialized = true;
	return m_fileName.c_str();
}

bool OGGMemoryFileClass::Exists(bool writeShared)
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> Exists({})\n", (LPCVOID)this, writeShared);
	return OGGManager::Instance().Has(m_cacheKey);
}

bool OGGMemoryFileClass::HasHandle()
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> HasHandle() = {}\n", (LPCVOID)this, m_open);
	return m_open;
}

bool OGGMemoryFileClass::Open(FileAccessMode access)
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> Open({})\n", (LPCVOID)this, (int)access);
	if (!OGGManager::Instance().Has(m_cacheKey))
		return false;
	m_position = 0;
	m_open = true;
	return true;
}

int OGGMemoryFileClass::ReadBytes(void* pBuffer, int nNumBytes)
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> ReadBytes({}, {})\n", (LPCVOID)this, (LPCVOID)pBuffer, nNumBytes);
	if (!m_open || !pBuffer || nNumBytes <= 0)
		return 0;

	// Deferred decode: only trigger when the read range touches PCM data (byte 44+)
	const bool needsPcm = (m_position < 44 && m_position + nNumBytes > 44)
	                    || (m_position >= 44);
	if (needsPcm)
		OGGManager::Instance().EnsurePcmDecoded(m_cacheKey);

	const size_t fileSize = OGGManager::Instance().GetSize(m_cacheKey);
	if (m_position >= fileSize)
		return 0;

	const size_t available = fileSize - m_position;
	const int toRead = (nNumBytes < (int)available) ? nNumBytes : (int)available;

	const uint8_t* data = OGGManager::Instance().GetData(m_cacheKey);
	if (!data)
		return 0;

	memcpy(pBuffer, data + m_position, toRead);
	m_position += toRead;
	return toRead;
}

int OGGMemoryFileClass::Seek(int offset, FileSeekMode seek)
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> Seek({}, {})\n", (LPCVOID)this, offset, (int)seek);
	if (!m_initialized)
		return 0;

	size_t fileSize = 0;
	if (!m_cacheKey.empty())
		fileSize = OGGManager::Instance().GetSize(m_cacheKey);

	size_t newPos = m_position;
	switch (seek)
	{
	case FileSeekMode::Set:
		newPos = (offset >= 0) ? (size_t)offset : 0;
		break;
	case FileSeekMode::Current:
		if (offset >= 0)
			newPos = m_position + offset;
		else
			newPos = ((size_t)(-offset) >= m_position) ? 0 : m_position + offset;
		break;
	case FileSeekMode::End:
		if (offset >= 0)
			newPos = fileSize;
		else
			newPos = ((size_t)(-offset) >= fileSize) ? 0 : fileSize + offset;
		break;
	}

	m_position = (newPos > fileSize) ? fileSize : newPos;
	return (int)m_position;
}

int OGGMemoryFileClass::GetFileSize()
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> GetFileSize()\n", (LPCVOID)this);
	if (m_cacheKey.empty())
		return 0;
	const size_t sz = OGGManager::Instance().GetSize(m_cacheKey);
	return (sz <= 0x7FFFFFFF) ? (int)sz : 0x7FFFFFFF;
}

void OGGMemoryFileClass::Close()
{
	Debug::LogFormat("[OGGMemoryFileClass] This {} -> Close()\n", (LPCVOID)this);
	// Keep the decoded PCM buffer alive in OGGManager.
	// This allows re-opening the same file without re-decoding.
	m_position = 0;
	m_open = false;
}

vptr_t OGGMemoryFileClass::GetVTable()
{
	return GetIHFileRegisterKey<OGGMemoryFileClass>();
}
