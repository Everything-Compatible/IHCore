#include "IProjFile.h"

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
	if (!UnderlyingFile) return false;
	Close();
	if (!UnderlyingFile->Open(access)) return false;
	size_t Size = UnderlyingFile->GetFileSize();
	auto Buffer = UnderlyingFile->ReadWholeFile();
	bool Ret = Stream.Load(Buffer, Size, false);
	GameDelete(Buffer);
	Iter = Stream.StreamLines().begin();
}

int INIWeaverProjFileClass::ReadBytes(void* pBuffer, int nNumBytes) //Returns number of bytes read.
{
	if (!Iter) return 0;
	if (*Iter == Stream.StreamLines().end())
		return 0;
	int BytesRead = 0;
	while (BytesRead < nNumBytes && Iter != Stream.StreamLines().end())
	{
		const auto& Line = **Iter;
		int BytesLeftInLine = static_cast<int>(Line.size()) - BytesReadInLine;
		int BytesToRead = std::min(nNumBytes - BytesRead, BytesLeftInLine);
		memcpy(static_cast<char*>(pBuffer) + BytesRead, Line.data() + BytesReadInLine, BytesToRead);
		BytesRead += BytesToRead;
		BytesReadInLine += BytesToRead;
		if (BytesReadInLine >= static_cast<int>(Line.size()))
		{
			//Move to next line
			++(*Iter);
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

void INIWeaverProjFileClass::Close()
{
	if (UnderlyingFile)
	{
		UnderlyingFile->Close();
		GameDelete(UnderlyingFile);
		UnderlyingFile = nullptr;
	}

	Iter = std::nullopt;
}
void INIWeaverProjFileClass::Initialize()
{
	//受构造方式影响，此类型的构造函数不会真正执行
	//这个函数实际上是构造函数的替代品
	new(this) INIWeaverProjFileClass(noinit_t{});
}