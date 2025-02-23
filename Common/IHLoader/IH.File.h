#pragma once
#include <CCFileClass.h>
#include "SyringeEx.h"
#include "IH.Initial.h"

//���е�������Ӧ������ IHFileClass �� IHReadOnlyFileClass 
//�����н���noinit_t�Ĺ��캯��
//ʵ�������麯�������ؼ���

class IHFileClass :public FileClass
{
public:
	//Destructor
	virtual	~IHFileClass() {};//OK
	//FileClass
	virtual const char* GetFileName() const = 0;//OK
	virtual const char* SetFileName(const char* pFileName) = 0;//OK
	virtual BOOL CreateFile() = 0;//OK
	virtual BOOL DeleteFile() = 0;//OK
	virtual bool Exists(bool writeShared = false) = 0;
	virtual bool HasHandle() = 0;
	virtual bool Open(FileAccessMode access) = 0;
	virtual bool OpenEx(const char* pFileName, FileAccessMode access);
	virtual int ReadBytes(void* pBuffer, int nNumBytes) = 0; //Returns number of bytes read.
	virtual int Seek(int offset, FileSeekMode seek) = 0;
	virtual int GetFileSize() = 0;
	virtual int WriteBytes(void* pBuffer, int nNumBytes) = 0; //Returns number of bytes written.
	virtual void Close() = 0;//OK
	virtual void CDCheck(DWORD errorCode, bool bUnk, const char* pFilename);
	virtual void Initialize() = 0;

protected:
	explicit __forceinline IHFileClass(noinit_t _):FileClass(_)
	{ }

	//Properties
public:
};

class IHReadOnlyFileClass :public IHFileClass
{
public:
	//Destructor
	virtual	~IHReadOnlyFileClass() {};
	//FileClass
	virtual const char* GetFileName() const = 0;
	virtual const char* SetFileName(const char* pFileName) = 0;
	virtual BOOL CreateFile();
	virtual BOOL DeleteFile();
	virtual bool Exists(bool writeShared = false) = 0;
	virtual bool HasHandle() = 0;
	virtual bool Open(FileAccessMode access) = 0;
	virtual int ReadBytes(void* pBuffer, int nNumBytes) = 0; //Returns number of bytes read.
	virtual int Seek(int offset, FileSeekMode seek) = 0;
	virtual int GetFileSize() = 0;
	virtual int WriteBytes(void* pBuffer, int nNumBytes); //Returns number of bytes written.
	virtual void Close() = 0;
	virtual void Initialize() = 0;
protected:
	explicit __forceinline IHReadOnlyFileClass(noinit_t _) :IHFileClass(_)
	{ }

	//Properties
public:
};

template<typename ParamType>
vptr_t GetIHFileRegisterKey()
{
	static_assert(std::is_base_of<IHFileClass, ParamType>::value, "ParamType ���������� IHFileClass ��");
	ParamType p{ noinit_t() };
	return *reinterpret_cast<vptr_t*>(&p);
}