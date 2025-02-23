/*

SIWinterIsComing V0.1.0 SDK
引擎作者：Sidoupiar QQ：827632323    航味麻酱 QQ：1460015544
SDK作者：钢铁之锤  QQ：2482911962
交流QQ群：486835964

SIWinterIsComing（简称WIC）是Sidoupiar、航味麻酱开发的YR引擎扩展。
这是WIC引擎的配套SDK，提供了调用WIC内部功能的接口。
如果您想要调用WIC的接口，请在项目当中包含此SDK。
在把IH组件（IHCore.dll IHLibList.dll等）和SIWinterIsComing.dll放置于游戏目录，
并确保Syringe识别了它们后，且正确地将您的DLL通过IHLoader注册到IH组件后，
此SDK当中的接口即为可用的。
详细的接口说明请查阅WIC.Interface.h、WIC.Buff.h等头文件内接口处的注释。
如有问题或反馈请联系作者。

2024年12月

*/
#pragma once

#include  "WIC.Interface.h"

#include  "WIC.Buff.h"

#include  "WIC.ExampleBuff.h"

#include  "WIC.Template.h"
//注意：接口在WIC无效时调用或接口找不到时会抛异常(SIException)的