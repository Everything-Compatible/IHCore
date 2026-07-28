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

bool OGGManager::DecodeAndCache(const std::string& key, const std::string& oggPath)
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

	// Decode OGG -> PCM via stb_vorbis (memory -> memory)
	int error = 0;
	stb_vorbis* vorbis = stb_vorbis_open_memory(oggData.data(), (int)oggSize, &error, nullptr);
	if (!vorbis)
	{
		Debug::Log("[OGG] stb_vorbis error %d decoding \"%s\"\n", error, oggPath.c_str());
		return false;
	}

	stb_vorbis_info info = stb_vorbis_get_info(vorbis);
	unsigned int totalSamples = stb_vorbis_stream_length_in_samples(vorbis);

	uint16_t channels = (uint16_t)info.channels;
	uint32_t srcSampleRate = info.sample_rate;
	uint16_t bitsPerSample = 16;
	uint32_t bytesPerFrame = channels * (bitsPerSample / 8);
	uint32_t headerSize = 44;

	if (totalSamples == 0)
	{
		stb_vorbis_close(vorbis);
		Debug::Log("[OGG] Zero-length audio in \"%s\"\n", oggPath.c_str());
		return false;
	}

	// Decode all PCM frames into a temporary buffer first
	uint32_t srcPcmBytes = totalSamples * bytesPerFrame;
	std::vector<uint8_t> tempPcm(srcPcmBytes);

	int samplesDecoded = stb_vorbis_get_samples_short_interleaved(
		vorbis, channels,
		reinterpret_cast<short*>(tempPcm.data()),
		(int)(totalSamples * channels));

	stb_vorbis_close(vorbis);

	if (samplesDecoded <= 0)
	{
		Debug::Log("[OGG] No samples decoded from \"%s\"\n", oggPath.c_str());
		return false;
	}

	// RA2 requires 22050 Hz PCM. Resample if necessary.
	uint32_t finalSampleRate = srcSampleRate;
	uint32_t finalDataSize;
	std::vector<uint8_t> resampledPcm;	// only used when resampling

	if (srcSampleRate != 22050)
	{
		finalSampleRate = 22050;

		// outputFrames = inputFrames * 22050 / srcSampleRate
		int srcFrames = samplesDecoded;
		int dstFrames = (int)((double)srcFrames * 22050.0 / (double)srcSampleRate);

		if (dstFrames <= 0)
		{
			Debug::Log("[OGG] Resample produced 0 frames from \"%s\"\n", oggPath.c_str());
			return false;
		}

		finalDataSize = dstFrames * bytesPerFrame;
		resampledPcm.resize(finalDataSize);

		const short* src = reinterpret_cast<const short*>(tempPcm.data());
		short* dst = reinterpret_cast<short*>(resampledPcm.data());

		// Linear interpolation: each output frame maps to srcSampleRate/22050 input frames
		double step = (double)srcSampleRate / 22050.0;

		for (int i = 0; i < dstFrames; i++)
		{
			double srcPos = i * step;
			int srcIndex = (int)srcPos;
			double frac = srcPos - srcIndex;

			if (srcIndex >= srcFrames - 1)
			{
				for (int c = 0; c < channels; c++)
					dst[i * channels + c] = src[(srcFrames - 1) * channels + c];
			}
			else
			{
				for (int c = 0; c < channels; c++)
				{
					double s0 = src[srcIndex * channels + c];
					double s1 = src[(srcIndex + 1) * channels + c];
					double v = s0 + (s1 - s0) * frac;
					if (v > 32767.0) v = 32767.0;
					if (v < -32768.0) v = -32768.0;
					dst[i * channels + c] = (short)v;
				}
			}
		}

		Debug::Log("[OGG] Resampled \"%s\" %u Hz -> 22050 Hz (%d -> %d frames)\n",
			oggPath.c_str(), srcSampleRate, srcFrames, dstFrames);
	}
	else
	{
		finalDataSize = (uint32_t)samplesDecoded * bytesPerFrame;
	}

	// Build the complete WAV file in memory
	Entry entry;
	entry.data.resize(headerSize + finalDataSize);
	uint8_t* wav = entry.data.data();

	// RIFF header
	memcpy(wav, "RIFF", 4);
	*(uint32_t*)(wav + 4) = headerSize + finalDataSize - 8;
	memcpy(wav + 8, "WAVE", 4);

	// fmt sub-chunk
	memcpy(wav + 12, "fmt ", 4);
	*(uint32_t*)(wav + 16) = 16;
	*(uint16_t*)(wav + 20) = 1;			// PCM
	*(uint16_t*)(wav + 22) = channels;
	*(uint32_t*)(wav + 24) = finalSampleRate;
	*(uint32_t*)(wav + 28) = finalSampleRate * bytesPerFrame;
	*(uint16_t*)(wav + 32) = bytesPerFrame;
	*(uint16_t*)(wav + 34) = bitsPerSample;

	// data sub-chunk
	memcpy(wav + 36, "data", 4);
	*(uint32_t*)(wav + 40) = finalDataSize;

	// Copy PCM data (resampled or original)
	const uint8_t* pcmSrc = (srcSampleRate != 22050) ? resampledPcm.data() : tempPcm.data();
	memcpy(wav + headerSize, pcmSrc, finalDataSize);

	m_cache[key] = std::move(entry);
	Debug::Log("[OGG] Cached \"%s\" (%zu bytes, %u Hz, %u ch)\n",
		key.c_str(), m_cache[key].data.size(), finalSampleRate, channels);
	return true;
}

bool OGGManager::Has(const std::string& key) const
{
	return m_cache.find(key) != m_cache.end();
}

const uint8_t* OGGManager::GetData(const std::string& key) const
{
	auto it = m_cache.find(key);
	return (it != m_cache.end()) ? it->second.data.data() : nullptr;
}

size_t OGGManager::GetSize(const std::string& key) const
{
	auto it = m_cache.find(key);
	return (it != m_cache.end()) ? it->second.data.size() : 0;
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
	m_position = 0;
	m_open = false;
	m_initialized = false;
}

const char* OGGMemoryFileClass::GetFileName() const
{
	return m_fileName.c_str();
}

const char* OGGMemoryFileClass::SetFileName(const char* pFileName)
{
	m_fileName = pFileName ? pFileName : "";
	m_cacheKey = m_fileName;
	m_initialized = true;
	return m_fileName.c_str();
}

bool OGGMemoryFileClass::Exists(bool writeShared)
{
	return OGGManager::Instance().Has(m_cacheKey);
}

bool OGGMemoryFileClass::HasHandle()
{
	return m_open;
}

bool OGGMemoryFileClass::Open(FileAccessMode access)
{
	if (!OGGManager::Instance().Has(m_cacheKey))
		return false;
	m_position = 0;
	m_open = true;
	return true;
}

int OGGMemoryFileClass::ReadBytes(void* pBuffer, int nNumBytes)
{
	if (!m_open || !pBuffer || nNumBytes <= 0)
		return 0;

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
	if (m_cacheKey.empty())
		return 0;
	const size_t sz = OGGManager::Instance().GetSize(m_cacheKey);
	return (sz <= 0x7FFFFFFF) ? (int)sz : 0x7FFFFFFF;
}

void OGGMemoryFileClass::Close()
{
	// Keep the decoded PCM buffer alive in OGGManager.
	// This allows re-opening the same file without re-decoding.
	m_position = 0;
	m_open = false;
}

vptr_t OGGMemoryFileClass::GetVTable()
{
	return GetIHFileRegisterKey<OGGMemoryFileClass>();
}
