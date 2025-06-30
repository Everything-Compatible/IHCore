#pragma once
#include "IH.Loader.h"


struct ExceptionAnalyzer
{
	//��Ϊ��SyringeIH�Ƿ����Ӱ�죬����ͨ�� HasSyringeIH() ȷ���Ƿ���ڡ�
	void (CALLBACK* AnalyzeAddr)(DWORD /*In*/ Addr, size_t /*In*/ BaseDescBufSize, DWORD& /*Out*/ RelAddr, char* /*Out GBK*/ BaseDesc);
	void (CALLBACK* GetAccessStr)(LPVOID /*In*/ Addr, size_t /*In*/ BaseDescBufSize, char* /*Out GBK*/ AccessStr);
	bool (CALLBACK* IsAddrExecutable)(LPVOID /*In*/ Addr);
	void (CALLBACK* WriteToExceptIH)(const char* /*In GBK*/ Format, ...);

	int Reserved[4];
};

/*
ǰ�ţ�
EC��������ʵ�ֻ��ǳ�ʼ����ӿ�
������һ������EC::Internal::ListenerAccess�ķ���
�Ժ�һЩ���߰���Ĺ���Ҳ���÷���ӿڸ�
������������ʼ���������᲻��ô�����
*/

namespace ECListener
{
	void Set(const char* ListenerType, LibFuncHandle Function);

	LibFuncHandle Get(const char* ListenerType);

	void Remove(const char* ListenerType);

	std::vector<LibFuncHandle> GetAll(const char* ListenerType);

	//!!һ����ÿ����ֻ����һ��������!!

	//��EXE���ʼ��ȫ�ֶ����ʼ����ɣ��ڴ�ɷ��䣬Winmain��ͷλ��
	//���һЩ��Ҫ�ĳ�ʼ������
	typedef void (CALLBACK* Listener_InitBeforeEverything)();
	void Listen_InitBeforeEverything(Listener_InitBeforeEverything Func);

	//�ڶ���rules����rulesmd.ini����ͼ�ļ�����ʱ����
	//Listen_LoadBeforeTypeData��ԭ������Ͷ�ȡǰ����
	//Listen_LoadAfterTypeData��ԭ������Ͷ�ȡ�󴥷�
	//������������
	typedef void (CALLBACK* Listener_OnLoadGame)(const CCINIClass* pIni);
	void Listen_LoadBeforeTypeData(Listener_OnLoadGame Func);
	void Listen_LoadAfterTypeData(Listener_OnLoadGame Func);

	//�ڴ���FEʱ���ã����Ϊһ�����쳣״̬�¿��԰�ȫ�����ڴ�ĺ���
	//��¼��Ҫ�Ĵ�����Ϣ��ת����except_ih.txt
	typedef void (CALLBACK* Listener_BeginWritingExceptIH)(const ExceptionAnalyzer& Anal);
	void Listen_BeginWritingExceptIH(Listener_BeginWritingExceptIH Func);

	//������CSF��Ŀʱ���ã����Խ���CSF�Ľ���
	//�����Ҫ�޸Ľ����Ľ�������Է��ط���ֵ��Ϊ�µĽ��������Ҫ�󷵻ص�const wchar_t*�Ƿ���ʱֵ��������һֱ����
	//������������nullptr���򷵻�ԭ�в��Ҳ���
	typedef const wchar_t* (CALLBACK* Listener_LoadCSFString)(const char* pLabel);
	void Listen_LoadCSFString(Listener_LoadCSFString Func);

	//�Ծִ�INI����ʱ��������̵��д���
	typedef void (CALLBACK* Listener_LoadScenario)();
	void Listen_LoadScenario(Listener_LoadScenario Func);

	//�Ӵ浵����Ծ�ʱ��������ɺ󽫻����
	// HasWIC() == true ʱ���Թ���
	// ʲô����Ҫ����Ϊʲô��������ĺ���ִ�е�ʱ��Swizzle��ô��û��Ч
	typedef void (CALLBACK* Listener_AfterLoadGame)();
	void Listen_AfterLoadGame(Listener_AfterLoadGame Func);

	//��INI����ͼ����Ծ�ʱ��������ɺ󽫻����
	// HasWIC() == true ʱ���Թ���
	typedef void (CALLBACK* Listener_AfterLoadINI)();
	void Listen_AfterLoadINI(Listener_AfterLoadINI Func);

	//�Ծֽ�������������ʱ�������
	// HasWIC() == true ʱ���Թ���
	typedef void (CALLBACK* Listener_ClearScenario)();
	void Listen_ClearScenario(Listener_ClearScenario Func);
}