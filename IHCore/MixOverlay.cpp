#include "MixOverlay.h"

#include <windows.h>
#include <shlwapi.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>

#include <YRPP.h>
#include <CRC.h>
#include <Helpers/Macro.h>

#include "Debug.h"
#include "../Common/LocalData.h"
#include "../Common/IHLoader/SyringeEx.h"

#pragma comment(lib, "Shlwapi.lib")

JsonObject GetIHCoreJson();

//=============================================================================
// MixOverlayManager
//=============================================================================

MixOverlayManager& MixOverlayManager::Instance()
{
	static MixOverlayManager instance;
	return instance;
}

bool MixOverlayManager::Add(const char* dir, const char* mix, bool first)
{
	if (!dir || !mix || !*dir || !*mix)
		return false;

	std::string mixName = mix;
	std::transform(mixName.begin(), mixName.end(), mixName.begin(), ::toupper);

	std::string dirPath = dir;
	if (dirPath.find(':') == std::string::npos
		&& !(dirPath.size() >= 2 && dirPath[0] == '\\' && dirPath[1] == '\\'))
	{
		std::string relative = dir;
		while (!relative.empty() && (relative.front() == '\\' || relative.front() == '/'))
			relative.erase(relative.begin());

		dirPath = SyringeData::ExecutableDirectoryPath();
		if (!dirPath.empty() && dirPath.back() != '\\' && dirPath.back() != '/')
			dirPath += "\\";
		dirPath += relative;
	}

	std::string search = dirPath;
	if (!search.empty() && search.back() != '\\' && search.back() != '/')
		search += "\\";
	search += "*";

	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(search.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		Debug::Log("IHCore : MixOverlay directory \"%s\" not found or unreadable.\n", dirPath.c_str());
		return false;
	}

	struct FileInfo
	{
		std::string name;
		std::string path;
		uint32_t size;
	};

	std::vector<FileInfo> files;
	do
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		std::string name = fd.cFileName;
		if (name == "." || name == "..")
			continue;

		std::string path = dirPath;
		if (!path.empty() && path.back() != '\\' && path.back() != '/')
			path += "\\";
		path += name;

		files.push_back({ name, path, fd.nFileSizeLow });
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);

	if (files.empty())
	{
		Debug::Log("IHCore : MixOverlay directory \"%s\" is empty; skipped.\n", dirPath.c_str());
		return false;
	}

	struct MixEntry
	{
		int32_t crc;
		uint32_t offset;
		uint32_t size;
		std::string name;
		std::string path;
	};

	std::vector<MixEntry> entries;
	uint32_t dataSize = 0;
	for (const auto& f : files)
	{
		std::string upper = f.name;
		std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
		int crc = CRCEngine::Memory(upper.c_str(), (int)upper.size(), 0);
		entries.push_back({ crc, dataSize, f.size, f.name, f.path });
		dataSize += f.size;
	}

	std::sort(entries.begin(), entries.end(), [](const MixEntry& a, const MixEntry& b)
	{
		return a.crc < b.crc;
	});

	uint16_t count = (uint16_t)entries.size();
	std::vector<uint8_t> image(6u + (size_t)count * 12u + dataSize, 0);
	memcpy(image.data(), &count, 2);
	memcpy(image.data() + 2, &dataSize, 4);

	size_t pos = 6;
	for (const auto& e : entries)
	{
		memcpy(image.data() + pos, &e.crc, 4);
		memcpy(image.data() + pos + 4, &e.offset, 4);
		memcpy(image.data() + pos + 8, &e.size, 4);
		pos += 12;
	}

	for (const auto& e : entries)
	{
		FILE* fp = fopen(e.path.c_str(), "rb");
		if (!fp)
		{
			Debug::Log("IHCore : MixOverlay failed to open \"%s\"; skipped mapping %s.\n", e.path.c_str(), mixName.c_str());
			return false;
		}

		size_t read = fread(image.data() + 6u + (size_t)count * 12u + e.offset, 1, e.size, fp);
		fclose(fp);

		if (read != e.size)
		{
			Debug::Log("IHCore : MixOverlay failed to read \"%s\"; skipped mapping %s.\n", e.path.c_str(), mixName.c_str());
			return false;
		}
	}

	std::string internalName = "$IH" + mixName;

	auto it = _entries.find(mixName);
	if (it != _entries.end())
	{
		Debug::Log("IHCore : MixOverlay duplicate mapping for %s; replacing.\n", mixName.c_str());
		_internalToEntry.erase(it->second.InternalName);
		_entries.erase(it);
	}

	MixOverlayEntry entry;
	entry.MixName = mixName;
	entry.DirectoryPath = dirPath;
	entry.First = first;
	entry.InternalName = internalName;
	entry.Image = std::move(image);

	for (const auto& e : entries)
	{
		std::string upper = e.name;
		std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
		entry.Files[upper] = { 6u + (uint32_t)count * 12u + e.offset, e.size };
	}

	auto res = _entries.emplace(mixName, std::move(entry));
	_internalToEntry[internalName] = &res.first->second;

	return true;
}

bool MixOverlayManager::IsInternalName(const char* name) const
{
	if (!name)
		return false;
	return _internalToEntry.find(name) != _internalToEntry.end();
}

const std::vector<uint8_t>* MixOverlayManager::GetImage(const char* internalName) const
{
	if (!internalName)
		return nullptr;

	auto it = _internalToEntry.find(internalName);
	if (it == _internalToEntry.end())
		return nullptr;

	return &it->second->Image;
}

bool MixOverlayManager::IsRealMixLoaded(const char* mixName) const
{
	if (!mixName || !*mixName)
		return false;

	GenericList& list = static_cast<GenericList&>(MixFileClass::MIXes.get());
	for (GenericNode* n = list.First(); n->IsValid(); n = n->Next())
	{
		auto* mix = static_cast<MixFileClass*>(n);
		if (!mix || !mix->FileName)
			continue;

		const char* base = PathFindFileNameA(mix->FileName);
		if (base && _stricmp(base, mixName) == 0)
			return true;
	}

	return false;
}

bool MixOverlayManager::TryGetOverlayFile(const char* filename, bool first, const uint8_t*& outData, int& outSize)
{
	if (!filename || !*filename)
		return false;

	std::string upper = filename;
	std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

	for (auto& kv : _entries)
	{
		auto& entry = kv.second;
		if (entry.First != first)
			continue;

		auto it = entry.Files.find(upper);
		if (it == entry.Files.end())
			continue;

		if (!IsRealMixLoaded(entry.MixName.c_str()))
			continue;

		const auto& info = it->second;
		if (info.Offset >= entry.Image.size() || info.Size == 0)
			continue;

		outData = entry.Image.data() + info.Offset;
		outSize = (int)info.Size;
		return true;
	}

	return false;
}

MixOverlayEntry* MixOverlayManager::FindByMixName(const char* mixName)
{
	if (!mixName)
		return nullptr;

	auto it = _entries.find(mixName);
	if (it == _entries.end())
		return nullptr;

	return &it->second;
}

MixOverlayEntry* MixOverlayManager::FindByRealMix(MixFileClass* real)
{
	if (!real)
		return nullptr;

	auto it = _realToEntry.find(real);
	if (it == _realToEntry.end())
		return nullptr;

	return it->second;
}

MixOverlayEntry* MixOverlayManager::FindByOverlay(MixFileClass* overlay)
{
	if (!overlay)
		return nullptr;

	auto it = _overlayToEntry.find(overlay);
	if (it == _overlayToEntry.end())
		return nullptr;

	return it->second;
}

void MixOverlayManager::Associate(MixOverlayEntry& entry, MixFileClass* real, MixFileClass* overlay)
{
	entry.Real = real;
	entry.Overlay = overlay;
	_realToEntry[real] = &entry;
	_overlayToEntry[overlay] = &entry;
}

void MixOverlayManager::Dissociate(MixOverlayEntry& entry)
{
	if (entry.Real)
		_realToEntry.erase(entry.Real);
	if (entry.Overlay)
		_overlayToEntry.erase(entry.Overlay);
	entry.Real = nullptr;
	entry.Overlay = nullptr;
}

void MixOverlayManager::DissociateOverlay(MixOverlayEntry& entry)
{
	if (entry.Overlay)
		_overlayToEntry.erase(entry.Overlay);
	entry.Overlay = nullptr;
}

//=============================================================================
// VirtualMixFileClass
//=============================================================================

void VirtualMixFileClass::Initialize()
{
	m_position = 0;
	m_open = false;
	m_initialized = false;
	new (&m_fileName) std::string();
}

const char* VirtualMixFileClass::GetFileName() const
{
	return m_fileName.c_str();
}

const char* VirtualMixFileClass::SetFileName(const char* pFileName)
{
	m_fileName = pFileName ? pFileName : "";
	m_initialized = true;
	return m_fileName.c_str();
}

bool VirtualMixFileClass::Exists(bool writeShared)
{
	return MixOverlayManager::Instance().GetImage(m_fileName.c_str()) != nullptr;
}

bool VirtualMixFileClass::HasHandle()
{
	return m_open;
}

bool VirtualMixFileClass::Open(FileAccessMode access)
{
	if (!MixOverlayManager::Instance().GetImage(m_fileName.c_str()))
		return false;

	m_position = 0;
	m_open = true;
	return true;
}

int VirtualMixFileClass::ReadBytes(void* pBuffer, int nNumBytes)
{
	if (!m_open || !pBuffer || nNumBytes <= 0)
		return 0;

	const auto* image = MixOverlayManager::Instance().GetImage(m_fileName.c_str());
	if (!image)
		return 0;

	if (m_position >= image->size())
		return 0;

	const size_t available = image->size() - m_position;
	const int toRead = (nNumBytes < (int)available) ? nNumBytes : (int)available;
	memcpy(pBuffer, image->data() + m_position, toRead);
	m_position += toRead;
	return toRead;
}

int VirtualMixFileClass::Seek(int offset, FileSeekMode seek)
{
	if (!m_initialized)
		return 0;

	const auto* image = MixOverlayManager::Instance().GetImage(m_fileName.c_str());
	const size_t fileSize = image ? image->size() : 0;

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

int VirtualMixFileClass::GetFileSize()
{
	const auto* image = MixOverlayManager::Instance().GetImage(m_fileName.c_str());
	if (!image)
		return 0;

	const size_t sz = image->size();
	return (sz <= 0x7FFFFFFF) ? (int)sz : 0x7FFFFFFF;
}

void VirtualMixFileClass::Close()
{
	m_position = 0;
	m_open = false;
}

vptr_t VirtualMixFileClass::GetVTable()
{
	return GetIHFileRegisterKey<VirtualMixFileClass>();
}

//=============================================================================
// Init
//=============================================================================

void MixOverlay_InitBeforeEverything()
{
	Local::RegisterIHFileStream(pVirtualMixFileClassName,
		{ VirtualMixFileClass::GetVTable(), sizeof(VirtualMixFileClass) });

	auto cfg = GetIHCoreJson();
	if (cfg)
	{
		auto ParseList = [&cfg](const char* key, bool first)
		{
			auto arr = cfg.GetObjectItem(key);
			if (!arr.Available() || !arr.IsNotEmptyArray())
				return;

			for (auto& item : arr.GetArrayObject())
			{
				auto dirObj = item.GetObjectItem("Directory");
				auto mixObj = item.GetObjectItem("Mix");
				if (!dirObj.Available() || !dirObj.IsTypeString())
				{
					Debug::Log("IHCore : MixOverlay config %s entry missing Directory.\n", key);
					continue;
				}
				if (!mixObj.Available() || !mixObj.IsTypeString())
				{
					Debug::Log("IHCore : MixOverlay config %s entry missing Mix.\n", key);
					continue;
				}

				MixOverlayManager::Instance().Add(dirObj.GetCString(), mixObj.GetCString(), first);
			}
		};

		ParseList("MixDirectory_First", true);
		ParseList("MixDirectory_Last", false);
	}

}
