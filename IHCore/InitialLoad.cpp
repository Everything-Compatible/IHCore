#include <windows.h>
#include "InitialLoad.h"
#include "..\Common\IHLoader\EC.GameClass.h"

ECRTTI_ExportRTTI(ECGameClass)
ECRTTI_ExportRTTI(ECGameTypeClass, ECGameClass)

InitialLoad::Service<InitialLoadParam_CustomPath>
	Service_CustomPathListHead("IHFile::CustomPathListHead"),//���ִ�Сд
	Service_CustomPathListTail("IHFile::CustomPathListTail"),//���ִ�Сд
	Service_CustomPathListFirst("IHFile::CustomPathListFirst");//���ִ�Сд

InitialLoad::Service<InitialLoadParam_CustomFile>
	Service_StringTable("IHFile::AddStringTable"),//���ִ�Сд
	Service_MixFile("IHFile::AddMixFile");//���ִ�Сд

InitialLoad::Service<InitialLoadParam_StringTablePair>
	Service_StringTableAddPair("StringTable::AddPair");//Key���ִ�Сд

InitialLoad::Service<InitialLoadParam_RedirectFile>
	Service_BindToStream("IHFile::BindToStream"),//�ļ����ִ�Сд �����ִַ�Сд OriginalName���ļ� TargetName����
	Service_RedirectFile("IHFile::RedirectFileName");//���ִ�Сд

InitialLoad::Service<InitialLoadParam_RegisterVTable>
	Service_RegisterIHFile("IHFile::RegisterIHFile");//�����ִַ�Сд

InitialLoad::Service<InitialLoadParam_RegisterFunction>
	Service_RegisterIHFileFilter("IHFile::RegisterIHFileFilter"),//���ִַ�Сд
	Service_RegisterListenerAccess("EC::Internal::ListenerAccess");//���ִַ�Сд
