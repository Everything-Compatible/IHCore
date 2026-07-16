#include "CachedFile.h"
#include <unordered_map>
#include <Debug.h>
#include <chrono>
#include "ExtCD.h"

struct CacheEntry;
struct CachePage;

struct CachePageLRUEntry
{
	CacheEntry* Entry;
	size_t PageIndex;
	void ReleaseHoldingPage();
	bool operator==(const CachePageLRUEntry& Other) const;
};

struct CachePageLRU
{
	const size_t LRUSize = 4;//4个够了，初始化阶段mix的局部性非常高，会反复随机读写同一个文件
	std::vector<CachePageLRUEntry> OpeningPages;
	void RequestOpenPage(CachePageLRUEntry Entry);
	void RemoveClosedPage(CachePageLRUEntry Entry);
};

CachePageLRU GlobalLRU;
 
struct CachePage
{
	bool Filled{ false };
	bool Holding{ false };
	LPVOID PagePtr{ nullptr };
};

struct CacheEntry
{
private:
	HANDLE hMap;
	size_t Size;
	std::vector<CachePage> Pages;
	bool Valid;
	uint64_t totaltime;
public:
	CacheEntry() = delete;
	CacheEntry(IHFileClass* File);
	~CacheEntry();
	LPVOID GetPage(size_t Index);//返回映射的页指针，保证是Holding的，可能有或者没有被填充，不应被持久化
	void FillPage(size_t Index, FileIterationType IterType, IHFileClass* File);//将页填充到缓存中
	bool IsPageFilled(size_t Index) const;
	LPVOID GetFilledPage(size_t Index, FileIterationType IterType, IHFileClass* File);
	void ReleaseHoldingPage(size_t Index);
	bool IsValid() const { return Valid; }
	size_t GetSize() const { return Size; }
	size_t GetPageCount() const { return Pages.size(); }
};

bool CachePageLRUEntry::operator==(const CachePageLRUEntry& Other) const
{
	return Entry == Other.Entry && PageIndex == Other.PageIndex;
}

void CachePageLRUEntry::ReleaseHoldingPage()
{
	Entry->ReleaseHoldingPage(PageIndex);
}

void CachePageLRU::RequestOpenPage(CachePageLRUEntry Entry)
{
	auto it = std::find(OpeningPages.begin(), OpeningPages.end(), Entry);
	if (it != OpeningPages.end())
	{
		OpeningPages.erase(it);
	}
	else if(OpeningPages.size() >= LRUSize)
	{
		auto& Oldest = OpeningPages.front();
		Oldest.ReleaseHoldingPage();
		OpeningPages.erase(OpeningPages.begin());
	}
	OpeningPages.push_back(Entry);
}

void CachePageLRU::RemoveClosedPage(CachePageLRUEntry Entry)
{
	auto it = std::find(OpeningPages.begin(), OpeningPages.end(), Entry);
	if (it != OpeningPages.end())
	{
		OpeningPages.erase(it);
	}
}

CacheEntry::CacheEntry(IHFileClass* File)
{
	CFM_Log("CacheEntry {} for file {} created\n", (void*)this, File->GetFileName());
	if(!File)
	{
		Valid = false;
		return;
	}
	Size = File->GetFileSize();
	if (Size < 0)
	{
		Valid = false;
		return;
	}
	auto NumPages = (Size + CachedFile::CachePageSize - 1) / CachedFile::CachePageSize;
	Pages.resize(NumPages);
	auto TotalSize = NumPages * CachedFile::CachePageSize;
	hMap = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, (DWORD)TotalSize, nullptr);
	if (!hMap)
	{
		Valid = false;
		return;
	}
	CFM_Log("CacheEntry {} Created with {} pages, total size {}\n", (void*)this, NumPages, TotalSize);
	Valid = true;
	totaltime = 0;
}

CacheEntry::~CacheEntry()
{
	CFM_Log("CacheEntry {} Destroyed\n", (void*)this);
	if (Valid)
	{
		for(size_t i = 0; i < Pages.size(); i++)
		{
			auto& Page = Pages[i];
			if (Page.Holding)
			{
				UnmapViewOfFile(Page.PagePtr);
				Page.PagePtr = nullptr;
				GlobalLRU.RemoveClosedPage({ this, i });
			}
		}
		CloseHandle(hMap);
	}
}

LPVOID CacheEntry::GetPage(size_t Index)
{
	if (Index >= Pages.size())return nullptr;
	auto& Page = Pages[Index];
	if (Page.Holding)
	{
		return Page.PagePtr;
	}
	else
	{
		CFM_Log("CacheEntry {} Requested Page {}\n", (void*)this, Index);
		GlobalLRU.RequestOpenPage({ this, Index });
		Page.PagePtr = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, (DWORD)(Index * CachedFile::CachePageSize), CachedFile::CachePageSize);
		Page.Holding = true;
		return Page.PagePtr;
	}
}

void CacheEntry::ReleaseHoldingPage(size_t Index)
{
	if (Index >= Pages.size())return;
	auto& Page = Pages[Index];
	if (Page.Holding)
	{
		CFM_Log("CacheEntry {} Released Page {}\n", (void*)this, Index);
		UnmapViewOfFile(Page.PagePtr);
		Page.PagePtr = nullptr;
		Page.Holding = false;
	}
}

bool CacheEntry::IsPageFilled(size_t Index) const
{
	if (Index >= Pages.size())return false;
	auto& Page = Pages[Index];
	return Page.Filled;
}

LPVOID CacheEntry::GetFilledPage(size_t Index, FileIterationType IterType, IHFileClass* File)
{
	FillPage(Index, IterType, File);
	return GetPage(Index);
}

void CacheEntry::FillPage(size_t Index, FileIterationType IterType, IHFileClass* File)
{
	if(Index >= Pages.size())return;
	if (IsPageFilled(Index))return;

	int Position = Index * CachedFile::CachePageSize;
	auto ReadPosition = (int)Position;
	auto RealPosition = File->Position();
	auto FillPageMethod = [&](size_t Idx)
	{
		CFM_Log("CacheEntry {} Filled Page {}\n", (void*)this, Idx);
		auto PagePtr = GetPage(Idx);
		auto ReadSize = std::min(CachedFile::CachePageSize, Size - ReadPosition);
		int nFilled = 0;
		for (auto p : Pages)if (p.Filled)nFilled++;
		auto start = std::chrono::high_resolution_clock::now();
		auto BytesRead = File->ReadBytes(PagePtr, ReadSize);
		auto finish = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
		totaltime += duration;
		Pages[Idx].Filled = (BytesRead == ReadSize);
		RealPosition = ReadPosition + BytesRead;
	};
	auto SeekReadPos = [&]()
	{
		//File->Seek(ReadPosition, FileSeekMode::Set);
		auto initial = File->Position();
		auto start = std::chrono::high_resolution_clock::now();
		auto result = File->Seek(ReadPosition, FileSeekMode::Set);
		auto finish = std::chrono::high_resolution_clock::now();
		uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
		totaltime += duration;
	};
	auto SeekAndFillPage = [&](size_t Idx)
	{
		SeekReadPos();
		FillPageMethod(Idx);
	};
	auto GoForwardAndFillPage = [&]()
	{
		auto ReadFromIdx = (RealPosition + CachedFile::CachePageSize - 1) / CachedFile::CachePageSize;
		//从ReadFromIdx开始读，直到Index
		for (size_t Idx = ReadFromIdx; Idx <= Index; Idx++)
		{
			if (!Pages[Idx].Filled)
			{
				ReadPosition = Idx * CachedFile::CachePageSize;
				if (ReadPosition > RealPosition)
					SeekReadPos();
				FillPageMethod(Idx);
			}
		}
	};
	auto GoBackwardAndFillPage = [&]()
	{
		auto ReadFromIdx = RealPosition / CachedFile::CachePageSize;
		//从ReadFromIdx开始读，直到Index
		for (size_t Idx = ReadFromIdx; Idx >= Index && Idx != MAXSIZE_T; Idx--)
		{
			if (!Pages[Idx].Filled)
			{
				ReadPosition = Idx * CachedFile::CachePageSize;
				if (ReadPosition < RealPosition)
					SeekReadPos();
				FillPageMethod(Idx);
			}
		}
	};

	//就在这一页，直接读
	if(ReadPosition == RealPosition)
	{
		FillPageMethod(Index);
		return;
	}
	
	switch (IterType)
	{
		//前向：如果RealPosition>Position，则seek回去读这一页；否则，从RealPosition开始读，填充中间路过的所有完整页面
	case FileIterationType::Forward:
		if (RealPosition > Position)
			SeekAndFillPage(Index);
		else
			GoForwardAndFillPage();
		break;
		//后向：如果RealPosition<Position，则seek回去读这一页；否则，从RealPosition开始读，填充中间路过的所有完整页面
	case FileIterationType::Backward:
		if (RealPosition < Position)
			SeekAndFillPage(Index);
		else
			GoBackwardAndFillPage();
		break;
		//双向：总是从RealPosition开始读，填充中间路过的所有完整页面
	case FileIterationType::Bidirectional:
		if (RealPosition < Position)
			GoForwardAndFillPage();
		else
			GoBackwardAndFillPage();
		break;
	case FileIterationType::RandomAccess:
	default:
		SeekAndFillPage(Index);
		break;
	}
}

namespace CachedFile
{
	size_t GenerateToken(const std::string& Class, const char* pFileName)
	{
		size_t hash1 = std::hash<std::string>{}(Class);
		size_t hash2 = std::hash<std::string>{}(pFileName);
		const size_t magic = 0x9e3779b9;
		size_t combined = hash1 ^ (hash2 + magic + (hash1 << 6) + (hash1 >> 2));
		return combined;
	}

	std::unordered_map<size_t, CacheEntry> CacheMap;//Key : Token, Value: CacheEntry
	std::unordered_map<size_t, IHExtUniquePtr> CachedIHExtEntries;

	// CRT 静态析构顺序不确定: CachedIHExtEntries 可能先于 CCFileClass 析构钩子被销毁
	static bool g_IsShuttingDown = false;

	void SetShuttingDown()
	{
		g_IsShuttingDown = true;
	}

	CacheEntry& GetOrCreateCacheEntry(size_t Token, IHFileClass* File)
	{
		auto it = CacheMap.find(Token);
		if (it == CacheMap.end())
		{
			CacheMap.emplace(Token, File);
			it = CacheMap.find(Token);
		}
		return it->second;
	}

	int Seek(size_t Token, IHFileClass* File, int& Position, int Offset)
	{
		auto& Entry = GetOrCreateCacheEntry(Token, File);
		if (!Entry.IsValid())
		{
			auto result = File->Seek(Offset, FileSeekMode::Set);
			Position = result;
		}
		else
		{
			auto dif = abs(Position - Offset);
			Position = Offset;
		}
		return Position;
	}

	int ReadBytes(size_t Token, IHFileClass* File, FileIterationType IterType, int& Position, void* Buffer, int Size)
	{
		auto& Entry = GetOrCreateCacheEntry(Token, File);
		if (!Entry.IsValid())
		{
			auto result = File->ReadBytes(Buffer, Size);
			Position += result;
			return result;
		}
		else
		{
			//Debug::LogFormat("[IH] Cached ReadBytes Position {} Size {} Entry Size {}\n", Position, Size, Entry.GetSize());
			if(Position < 0 || Size <= 0 || Position >= (int)Entry.GetSize())
			{
				return 0;
			}

			auto start = std::chrono::high_resolution_clock::now();

			if (Position + Size > (int)Entry.GetSize())
			{
				Size = Entry.GetSize() - Position;
			}

			auto StartPageIndex = Position / CachePageSize;
			auto EndPageIndex = (Position + Size - 1) / CachePageSize;

			int TotalBytesRead = 0;
			for (size_t Idx = StartPageIndex; Idx <= EndPageIndex; Idx++)
			{
				auto PagePtr = Entry.GetFilledPage(Idx, IterType, File);
				auto PageOffset = (Idx == StartPageIndex) ? ((size_t)Position % CachePageSize) : 0;
				auto ReadSize = std::min(CachePageSize - PageOffset, size_t(Size - TotalBytesRead));
				auto SrcPtr = static_cast<BYTE*>(PagePtr) + PageOffset;
				auto DstPtr = static_cast<BYTE*>(Buffer) + TotalBytesRead;
				memcpy(DstPtr, SrcPtr, ReadSize);
				TotalBytesRead += ReadSize;
			}
			Position += TotalBytesRead;

			auto finish = std::chrono::high_resolution_clock::now();
			uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
			return TotalBytesRead;
		}
	}

	void ClearCache(size_t Token)
	{
		CacheMap.erase(Token);
	}

	void TakeOverIHFile(IHExtPtrType* pExt)
	{
		if(CanAcceptIHExtPtr(pExt))
		{
			auto Token = pExt->GetEntry()->CachedAccessToken;
			CachedIHExtEntries[Token] = IHExtUniquePtr(pExt);
			CFM_Log("IHExtPtr {} File {} Position {} Cached Pos {} Taken Over with Token {}\n", (void*)pExt, pExt->GetEntry()->GetFile()->GetFileName(), pExt->GetEntry()->GetFile()->Position(), pExt->GetEntry()->Position(), Token);
		}
		else
		{
			Debug::LogFormat("CachedFile::TakeOverIHFile failed for pExt {}\n", (void*)pExt);
		}
	}
	
	IHExtPtrType* ReleaseIHFile(size_t Token)
	{
		auto it = CachedIHExtEntries.find(Token);
		if (it != CachedIHExtEntries.end())
		{
			auto pExt = it->second.release();
			CachedIHExtEntries.erase(it);
			return pExt;
		}
		return nullptr;
	}
	bool CanAcceptIHFile(size_t Token)
	{
		if (g_IsShuttingDown) return false;
		return CachedIHExtEntries.find(Token) == CachedIHExtEntries.end();
	}
	bool CanReleaseIHFile(size_t Token)
	{
		if (g_IsShuttingDown) return false;
		return CachedIHExtEntries.find(Token) != CachedIHExtEntries.end();
	}
	bool CanAcceptIHExtPtr(IHExtPtrType* pExt)
	{
		if (!pExt) return false;
		auto Entry = pExt->GetEntry();
		if (!Entry->CachedAccess) return false;
		auto Token = Entry->CachedAccessToken;
		return CanAcceptIHFile(Token);
	}
}