#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <YRPP.h>
#include "../Common/IHLoader/IH.File.h"
#include "ToolFunc.h"

class MixFileClass;

struct OverlayFileInfo
{
	uint32_t Offset{ 0 }; // offset into MixOverlayEntry::Image data section
	uint32_t Size{ 0 };
};

struct MixOverlayEntry
{
	std::string MixName;          // normalized, e.g. "SIDEC01.MIX"
	std::string DirectoryPath;    // absolute path
	bool First{ false };
	std::string InternalName;     // e.g. "$IHSIDEC01.MIX"
	std::vector<uint8_t> Image;   // generated plain MIX image
	std::unordered_map<std::string, OverlayFileInfo, UpperHash, UpperEqualPred> Files;
	MixFileClass* Overlay{ nullptr };
	MixFileClass* Real{ nullptr };
};

class MixOverlayManager
{
public:
	static MixOverlayManager& Instance();

	bool Add(const char* dir, const char* mix, bool first);
	bool IsInternalName(const char* name) const;
	const std::vector<uint8_t>* GetImage(const char* internalName) const;
	bool TryGetOverlayFile(const char* filename, bool first, const uint8_t*& outData, int& outSize);
	bool IsRealMixLoaded(const char* mixName) const;
	MixOverlayEntry* FindByMixName(const char* mixName);
	MixOverlayEntry* FindByRealMix(MixFileClass* real);
	MixOverlayEntry* FindByOverlay(MixFileClass* overlay);
	void Associate(MixOverlayEntry& entry, MixFileClass* real, MixFileClass* overlay);
	void Dissociate(MixOverlayEntry& entry);
	void DissociateOverlay(MixOverlayEntry& entry);

private:
	MixOverlayManager() = default;
	~MixOverlayManager() = default;
	MixOverlayManager(const MixOverlayManager&) = delete;
	MixOverlayManager& operator=(const MixOverlayManager&) = delete;

	std::unordered_map<std::string, MixOverlayEntry, UpperHash, UpperEqualPred> _entries;
	std::unordered_map<std::string, MixOverlayEntry*, UpperHash, UpperEqualPred> _internalToEntry;
	std::unordered_map<MixFileClass*, MixOverlayEntry*> _realToEntry;
	std::unordered_map<MixFileClass*, MixOverlayEntry*> _overlayToEntry;
};

class VirtualMixFileClass : public IHReadOnlyFileClass
{
public:
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

	static vptr_t GetVTable();

	explicit VirtualMixFileClass(noinit_t _)
		: IHReadOnlyFileClass(_)
	{ }

private:
	std::string m_fileName;
	size_t m_position{ 0 };
	bool m_open{ false };
	bool m_initialized{ false };
};

static const char* pVirtualMixFileClassName = "VirtualMixFileClass";

void MixOverlay_InitBeforeEverything();
