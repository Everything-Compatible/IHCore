#pragma once
#include "../Common/IHLoader/IH.File.h"
#include <string>
#include <vector>
#include <unordered_map>

//=============================================================================
// OGGManager - global singleton that caches decoded OGG audio.
// Decoded data is kept in memory for the entire game session.
//=============================================================================
class OGGManager
{
public:
	static OGGManager& Instance();

	// Decode an OGG file, convert to WAV (header + PCM), and cache it.
	// key: the original WAV filename (e.g. "SOME.WAV")
	// oggPath: full path to the .ogg file on disk
	bool DecodeAndCache(const std::string& key, const std::string& oggPath);

	bool DecodeAndCache(const std::string& key, const std::string& oggPath, void* pBuffer, int Size);

	// Query cache
	bool Has(const std::string& key) const;
	const uint8_t* GetData(const std::string& key) const;
	size_t GetSize(const std::string& key) const;

	// Free all cached data (game shutdown)
	void ClearAll();

private:
	OGGManager() = default;
	~OGGManager();
	OGGManager(const OGGManager&) = delete;
	OGGManager& operator=(const OGGManager&) = delete;

	struct Entry
	{
		std::vector<uint8_t> data;	// Complete WAV file: RIFF header + PCM
	};

	std::unordered_map<std::string, Entry> m_cache;
};

//=============================================================================
// OGGMemoryFileClass - in-memory file that presents decoded OGG as a WAV.
// All I/O reads from the buffer cached by OGGManager.
// Close() only resets position; the buffer is never freed (cached forever).
//=============================================================================
class OGGMemoryFileClass : public IHReadOnlyFileClass
{
public:
	// IHFileClass interface
	virtual void Initialize() override;
	virtual const char* GetFileName() const override;
	virtual const char* SetFileName(const char* pFileName) override;
	virtual bool Exists(bool writeShared = false) override;
	virtual bool HasHandle() override;
	virtual bool Open(FileAccessMode access) override;
	virtual int ReadBytes(void* pBuffer, int nNumBytes) override;
	virtual int Seek(int offset, FileSeekMode seek) override;
	virtual int GetFileSize() override;
	virtual void Close() override;

	// VTable helpers
	static vptr_t GetVTable();

	// noinit_t constructor is public so GetIHFileRegisterKey can create
	// a static vtable-probe instance.
	explicit OGGMemoryFileClass(noinit_t _)
		: IHReadOnlyFileClass(_)
	{ }

private:
	std::string m_fileName;   // Original requested name (e.g. "SOME.WAV")
	std::string m_cacheKey;   // Key used to look up in OGGManager
	size_t      m_position;
	bool        m_open;
	bool        m_initialized;
};

//=============================================================================
// OGG fallback entry point. Called from CDFileClass_SetFileName.
//=============================================================================
void TryOggFallback(CDFileClass* pThis, const char* pFileName);


static const char* pOGGMemoryFileClassName = "OGGMemoryFileClass";