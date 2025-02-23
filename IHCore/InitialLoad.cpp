#include <windows.h>
#include "InitialLoad.h"
#include "..\Common\IHLoader\EC.GameClass.h"

ECRTTI_ExportRTTI(ECGameClass)
ECRTTI_ExportRTTI(ECGameTypeClass, ECGameClass)

InitialLoad::Service<InitialLoadParam_CustomPath>
	Service_CustomPathListHead("IHFile::CustomPathListHead"),//不分大小写
	Service_CustomPathListTail("IHFile::CustomPathListTail"),//不分大小写
	Service_CustomPathListFirst("IHFile::CustomPathListFirst");//不分大小写

InitialLoad::Service<InitialLoadParam_CustomFile>
	Service_StringTable("IHFile::AddStringTable"),//不分大小写
	Service_MixFile("IHFile::AddMixFile");//不分大小写

InitialLoad::Service<InitialLoadParam_StringTablePair>
	Service_StringTableAddPair("StringTable::AddPair");//Key不分大小写

InitialLoad::Service<InitialLoadParam_RedirectFile>
	Service_BindToStream("IHFile::BindToStream"),//文件不分大小写 流名字分大小写 OriginalName填文件 TargetName填流
	Service_RedirectFile("IHFile::RedirectFileName");//不分大小写

InitialLoad::Service<InitialLoadParam_RegisterVTable>
	Service_RegisterIHFile("IHFile::RegisterIHFile");//流名字分大小写

InitialLoad::Service<InitialLoadParam_RegisterFunction>
	Service_RegisterIHFileFilter("IHFile::RegisterIHFileFilter"),//名字分大小写
	Service_RegisterListenerAccess("EC::Internal::ListenerAccess");//名字分大小写
