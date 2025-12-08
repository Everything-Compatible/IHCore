#pragma once
#include <EC.h>
#include <string_view>
#include <LibIProj.h>

class INIWeaverProjFileClass final : public IHInputStreamClass
{
public:
	//Destructor
	virtual	~INIWeaverProjFileClass();
	//FileClass
	virtual const char* GetFileName() const;
	virtual const char* SetFileName(const char* pFileName);
	virtual bool Exists(bool writeShared = false);
	virtual bool HasHandle();
	virtual bool Open(FileAccessMode access);
	virtual int ReadBytes(void* pBuffer, int nNumBytes); //Returns number of bytes read.
	virtual void Close();
	virtual void Initialize();
public:
	explicit __forceinline INIWeaverProjFileClass(noinit_t _) :IHInputStreamClass(_)
	{ }
private:
	INIWeaverProjectStreamer Stream;
	CCFileClass* UnderlyingFile{ nullptr };
	std::optional<decltype(Stream.StreamLines().begin())> Iter{ std::nullopt };
	int BytesReadInLine{ 0 };
};

bool __cdecl INIWeaverProjFileFilter(const char* Name);