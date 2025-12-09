#include "IProjFile.h"

void Internal_DebugLog(const char*, ...);

bool __cdecl INIWeaverProjFileFilter(const char* Name)
{
	//return true if Name has a .iproj extension
	auto len = strlen(Name);
	if (len < 6)return false;
	return _stricmp(Name + len - 6, ".iproj") == 0;
}

//Destructor
INIWeaverProjFileClass::~INIWeaverProjFileClass()
{
	Close();
}

//FileClass
const char* INIWeaverProjFileClass::GetFileName() const
{
	if (UnderlyingFile)
		return UnderlyingFile->GetFileName();
	else
		return nullptr;
}

const char* INIWeaverProjFileClass::SetFileName(const char* pFileName)
{
	Internal_DebugLog("INIWeaverProjFileClass::SetFileName called with %s\n", pFileName);
	UseOriginalFileClass_Scoped _;
	if (!UnderlyingFile)
	{
		UnderlyingFile = GameCreate<CCFileClass>(pFileName);
		return UnderlyingFile->GetFileName();
	}
	else
	{
		return UnderlyingFile->SetFileName(pFileName);
	}

}

bool INIWeaverProjFileClass::Exists(bool writeShared)
{
	if (UnderlyingFile)
		return UnderlyingFile->Exists(writeShared);
	else
		return false;
}

bool INIWeaverProjFileClass::HasHandle()
{
	if (UnderlyingFile)
		return UnderlyingFile->HasHandle();
	else
		return false;
}


bool INIWeaverProjFileClass::Open(FileAccessMode access)
{
	Internal_DebugLog("INIWeaverProjFileClass::Open called with access %d\n", access);
	if (!UnderlyingFile) return false;
	Clear();
	if (!UnderlyingFile->Open(access)) return false;
	size_t Size = UnderlyingFile->GetFileSize();
	auto Buffer = UnderlyingFile->ReadWholeFile();
	bool Ret = Stream.Load(Buffer, Size, false);
	auto& sproj = Stream.GetSProj();
	Internal_DebugLog("Project Info : Create Version %s\n",
		GetVersionStr(sproj.CreateVersionMajor * 10000 + sproj.CreateVersionMinor * 100 + sproj.CreateVersionRelease).c_str());
	GameDelete(Buffer);

	Gen = Stream.StreamLines();
	Iter = std::make_unique<decltype(Gen->begin())>(Gen->begin());
	CurLine = **Iter;
	BytesReadInLine = 0;
	Internal_DebugLog("INIWeaverProjFileClass::Open %s\n", Ret ? "succeeded" : "failed");
	return Ret;
}

int INIWeaverProjFileClass::ReadBytes(void* pBuffer, int nNumBytes) //Returns number of bytes read.
{
	Internal_DebugLog("INIWeaverProjFileClass::ReadBytes called with nNumBytes %d\n", nNumBytes);
	if (!Iter) return 0;
	if (*Iter == Gen->end())
		return 0;
	int BytesRead = 0;
	while (BytesRead < nNumBytes && *Iter != Gen->end())
	{
		const auto& Line = CurLine;
		Internal_DebugLog("Reading line: %s\n", Line.c_str());
		int BytesLeftInLine = static_cast<int>(Line.size()) - BytesReadInLine;
		int BytesToRead = std::min(nNumBytes - BytesRead, BytesLeftInLine);
		memcpy(static_cast<char*>(pBuffer) + BytesRead, Line.data() + BytesReadInLine, BytesToRead);
		BytesRead += BytesToRead;
		BytesReadInLine += BytesToRead;
		if (BytesReadInLine >= static_cast<int>(Line.size()))
		{
			//Move to next line
			++(*Iter);
			CurLine = (**Iter);
			BytesReadInLine = 0;
			//Add newline character
			if (BytesRead < nNumBytes)
			{
				static const char NewlineChar = '\n';
				memcpy(static_cast<char*>(pBuffer) + BytesRead, &NewlineChar, 1);
				BytesRead += 1;
			}
		}
	}
	return BytesRead;
}

void INIWeaverProjFileClass::Clear()
{
	if (UnderlyingFile)
	{
		UnderlyingFile->Close();
	}

	Gen = std::nullopt;
	Iter.reset();
	CurLine.clear();
}

void INIWeaverProjFileClass::Close()
{
	Clear();

	if (UnderlyingFile)
	{
		GameDelete(UnderlyingFile);
		UnderlyingFile = nullptr;
	}
}
void INIWeaverProjFileClass::Initialize()
{
	//受构造方式影响，此类型的构造函数不会真正执行
	//这个函数实际上是构造函数的替代品
	new(this) INIWeaverProjFileClass(noinit_t{});
}