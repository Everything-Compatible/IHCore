#pragma once
#include "IH.Loader.h"
#include <SwizzleManagerClass.h>

//��MyInit�������е���
//������رմ����
//����Դ˿����ô��������
void DisableLoadSave();



//��ʱ����
//���ƶ�����ʱ���Ƿ���
//����������ڳ����쳣�����ʱ����־��¼�������ô��������������SetStreamReadErrorHandler�Ļ���
//Ĭ�Ͽ���
void StreamReaderLoadCheck(bool bCheck);
//�����Ƿ����״̬
bool StreamReaderLoadCheck();

//��ʱ����
//���ƴ浵��ʱ���Ƿ���
//����������ڳ����쳣�����ʱ����־��¼�������ô��������������SetStreamReadErrorHandler�Ļ���
//Ĭ�Ͽ���
void StreamWriterSaveCheck(bool bCheck);
//�����Ƿ����״̬
bool StreamWriterSaveCheck();



//��������д����Ĵ���ص�
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
//��������SIInterface_ExtendDataָ��
// ��������FinalSwizzle�������е��ã���
//SIInterface_ExtendDataָ����ԭʼָ��+4������Ĭ�ϵ�Swizzle�޷�����SIInterface_ExtendDataָ�����
//��FinalSwizzle�׶��ֶ���SIFinalSwizzle�����������������е�SIInterface_ExtendData*
template<typename T>
bool SIFinalSwizzle(T* pOld, T*& pNew)
{
	return SIFinalSwizzleImpl(pOld, pNew);
}