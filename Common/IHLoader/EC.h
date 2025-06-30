#pragma once

/*
���ﻥͨ SDK

2024.12

����SDK�������й�IHCore SyringeIH SIWIC������Դ�Ľӿ�
ͬʱ������һ��������cJson��Ϊ���ݽ�����ʽ
*/

/*
cJSON��Dave Gamble������һ��Json�����⡣
ExtJson.h��cJson����C++�ļ򵥰�װ��
https://github.com/DaveGamble/cJSON
*/
#include "ExtJson.h"

/*
SyringeIH �� ����֮�� ��Syringe��������
SyringeEx.h�����������֧�ֵĽӿڡ�
https://github.com/IronHammer-Std/Syringe-IH
������Դ����ÿ�һ�����ﻥͨQQȺ�������¿��������еģ�
����Ŀ��Ares-Developers �� Syringe0.7.2.0 ��������
https://github.com/Ares-Developers/Syringe
*/
#include "SyringeEx.h"

/*
IH �� ����֮�� ������YR������չ��
*/
#include "IH.h"

/*
SIWinterIsComing�����WIC����Sidoupiar����ζ�齴 ������YR������չ��
*/
#ifndef SIWIC
#include "WIC.h"
#endif

/*
EC��һЩ�ۺϽӿڡ�
*/
#include "EC.Misc.h"
#include "EC.Listener.h"
#include "EC.LoadSave.h"
#include "EC.ObjBase.h"

/*
һ����ĳЩ�ض�������Ƿ��ڹ�����API��
�������true��������صĹ����ܹ�����������

cJSON.h��ExtJson.h��������
SyringeEx.h�Ľӿ�����SyringeIH
IH.xxx.h�Ľӿ�����IH���
WIC.xxx.h�Ľӿ�����WIC���

��DllMain��Init::Initialize()�� HasSyringeIH �� HasIH ���Թ���
��EC��ʼ���� HasWIC ���Թ���
*/
bool HasSyringeIH();
bool HasIH();
#ifndef SIWIC
bool HasWIC();
#endif


/*
��ȡEC������״̬
����ʼ�տ���
������ֵΪExt::ECLoadStage::InitCompleteʱ
EC������ʽ���
*/
inline Ext::ECLoadStage GetECLoadStage()
{
    return Ext::GetLoadStage();
}


/*

��ʼ������ ��DllMain��DLL_PROCESS_ATTACH�׶ε���

*/
bool ECInitLibrary(
    const char* LibraryName, //����
    int Version,
    int LowestSupportedVersion,
    UTF8_CString Description,
    const std::function<void()>& OnFirstInit,
    const std::function<void()>& OnOrderedInit,
    std::initializer_list<InitDependency> Dependencies = {}
);
bool ECInitLibrary(
    const char* LibraryName, //����
    int Version,
    int LowestSupportedVersion,
    UTF8_CString Description,
    const std::function<void()>& OnFirstInit,
    const std::function<void()>& OnOrderedInit,
    GetFunc_t GetFunc,
	std::initializer_list<InitDependency> Dependencies = {}
);
bool ECInitLibrary(
    const char* LibraryName, //����
    int Version,
    int LowestSupportedVersion,
    UTF8_CString Description,
    const std::function<void()>& OnFirstInit,
    const std::function<void()>& OnOrderedInit,
    const std::unordered_map<std::string, LibFuncHandle>& ExportFuncs,
    std::initializer_list<InitDependency> Dependencies = {}
);

/*
�ṹ��
���е�ͷ�ļ�
cJSON.h->ExtJson.h->SyringeEx.h->
                IH.Initial.h->IH.File.h  ->IH.Ext.h
                              IH.Loader.h->IH.InitialService.h                        ->IH.h  
                                              WIC.Define.h->WIC.Interface.h              ->WIC.h
                                            WIC.Template.h->WIC.Buff.h->WIC.ExampleBuff.h
*/