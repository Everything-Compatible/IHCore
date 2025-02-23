#pragma once
#include "IH.Loader.h"
#include <SwizzleManagerClass.h>

//在MyInit函数当中调用
//调用则关闭存读档
//否则对此库启用存读档功能
void DisableLoadSave();



//随时调用
//控制读档的时候是否检查
//如果检查则会在出现异常情况的时候日志记录，并调用错误处理（如果设置了SetStreamReadErrorHandler的话）
//默认开启
void StreamReaderLoadCheck(bool bCheck);
//返回是否检查的状态
bool StreamReaderLoadCheck();

//随时调用
//控制存档的时候是否检查
//如果检查则会在出现异常情况的时候日志记录，并调用错误处理（如果设置了SetStreamReadErrorHandler的话）
//默认开启
void StreamWriterSaveCheck(bool bCheck);
//返回是否检查的状态
bool StreamWriterSaveCheck();



//设置流读写错误的处理回调
enum class ECStreamErrorType
{
	//Read
	ReadBlockError,
	EmitExpectEndOfBlockWarning,
	EmitLoadWarning,
	EmitExpectWarning,
	EmitSwizzleWarning,

	//Write
	WriteBlockError
};
typedef void (*ECStreamErrorHandler)(ECStreamErrorType Error);
void SetStreamReadErrorHandler(ECStreamErrorHandler Handler);
void SetStreamWriteErrorHandler(ECStreamErrorHandler Handler);
ECStreamErrorHandler GetStreamReadErrorHandler();
ECStreamErrorHandler GetStreamWriteErrorHandler();



bool SIFinalSwizzleImpl(void* pOld, void*& pNew);
//用于重排SIInterface_ExtendData指针
// ！！仅在FinalSwizzle函数当中调用！！
//SIInterface_ExtendData指针是原始指针+4，所以默认的Swizzle无法更换SIInterface_ExtendData指针变量
//在FinalSwizzle阶段手动用SIFinalSwizzle这个函数来处理对象当中的SIInterface_ExtendData*
template<typename T>
bool SIFinalSwizzle(T* pOld, T*& pNew)
{
	return SIFinalSwizzleImpl(pOld, pNew);
}