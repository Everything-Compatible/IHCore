/*

IH V0.4 SDK
引擎作者：钢铁之锤  QQ：2482911962
SDK作者：钢铁之锤  QQ：2482911962

IH 是 钢铁之锤 开发的YR引擎扩展,目前包括IHCore IHLibList 2个组件。
这是IH的配套SDK，提供了调用IH内部功能的接口。
如果您想要调用IH的接口，请在项目当中包含此SDK。
在把IH组件（IHCore.dll IHLibList.dll等）放置于游戏目录，
并确保Syringe识别了它们后，且正确地将您的DLL通过IHLoader注册到IH组件后，
此SDK当中的接口即为可用的。
详细的接口说明请查阅IH.Loader.h IH.File.h等头文件内接口处的注释。
如有问题或反馈请联系作者。

2024年12月

*/
#pragma once

#include "IH.Loader.h"

#include "IH.Ext.h"

#include "IH.InitialService.h"

#include "IH.File.h"

#include "IH.Config.h"


namespace IH
{
	void* IHCore_Malloc(size_t Size);
	void IHCore_Free(void* Ptr) noexcept;

	//Same as APIs above
	void* Malloc(size_t Size);
	void Free(void* Ptr) noexcept;

	//Text Draw Variables
	//Recognize #(xxx) in text and replace automatically
	void SetTextDrawVariable(const char* Key, UTF8_CString Value);
	UTF8_CString GetTextDrawVariable(const char* Key);
	void DeleteTextDrawVariable(const char* Key);


	//Text Draw Router:
	//route text draw variable request to custom handlers
	//Example:
	//Route "MyVar" will match all #(MyVar.xxx) requests
	//unless there is a specific text draw variable already declared
	//Router functions:
	//if a Router is Set on Route "MyVar"
	//then a "MyVar.ABC" request will send to the router
	//the router receive "ABC" (WITHOUT ROUTE PREFIX) and give the result
	//if not found, give a nullptr
	//NOTE:
	//Route Name should NOT have '.'
	using TextDrawRouter = UTF8_CString (__cdecl *)(const char* Key);
	void SetTextDrawRouter(const char* Route, TextDrawRouter Router);
	TextDrawRouter GetTextDrawRouter(const char* Route);
	void DeleteTextDrawRouter(const char* Route);

	//Output the evaluated result
	//considering all variables and routers.
	//return nullptr if not found.
	UTF8_CString GetTextDrawValue(UTF8_CString Key);
	UTF8_CString GetTextDrawValue(const char* Key);
}