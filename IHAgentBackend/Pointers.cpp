#include "AgentBackend.h"
#include "Pointers.h"
#include <YRPP.h>
#include <Unsorted.h>
#include <EventClass.h>


// PointerExpiredProcess 在 IHAgentBackend.Marker.cpp 中实现
// 此文件的目的：
// 1) 提供 Pointers.h 中声明的链接满足
// 2) 为 IHAgentBackend 提供扩展指针生命周期管理的场所
//
// 不要对 pDyingObj 调用 GetClassName/vtable —— 它已被释放。
// pDyingObj 可能被多次广播（多个引用持有它）。
// bRemoved=false 表示对象暂时离开地图，而非销毁。


void __cdecl IHAgentBackend_ManualTest(JsonObject Context)
{
	if (!_GameStarted.load(std::memory_order_acquire)) { ECDebug::ReturnStdError(ERROR_NOT_READY); return; }


	auto oName = Context.GetObjectItem("Name");
	if (!oName || !oName.IsTypeString())
		ECDebug::ReturnStdError(ERROR_BAD_ARGUMENTS);
	auto Name = oName.GetString();

	auto pTechnoType = TechnoTypeClass::Find(Name.c_str());
	if (!pTechnoType)
	{
		ECDebug::ReturnStdError(ERROR_NOT_FOUND);
		return;
	}
	
	auto oOption = Context.GetObjectItem("Option");
	int opt = 0;
	if(oOption && oOption.IsTypeNumber())
		opt = oOption.GetInt();

	auto oX = Context.GetObjectItem("X");
	auto oY = Context.GetObjectItem("Y");
	int X = 0, Y = 0;
	if (oX && oX.IsTypeNumber())
		X = oX.GetInt();
	if (oY && oY.IsTypeNumber())
		Y = oY.GetInt();

	auto AbsTypeOfType = pTechnoType->WhatAmI();

	if (opt == 0)
	{
		// 触发 Produce 事件，等价于玩家点 cameo
		auto Success = EventClass::AddEvent(EventClass(
			HouseClass::CurrentPlayer->ArrayIndex,  
			EventType::PRODUCE,     
			(int)AbsTypeOfType,
			pTechnoType->GetArrayIndex(), 
			pTechnoType->Naval          
		));
		ECDebug::ReturnString(Success ? u8"true" : u8"false");
	}
	else if (opt == 1)
	{
		// 触发 PLACE 事件，等价于玩家点 cameo
		auto Success = EventClass::AddEvent(EventClass(
			HouseClass::CurrentPlayer->ArrayIndex,
			EventType::PLACE,
			AbsTypeOfType,
			pTechnoType->GetArrayIndex(),
			pTechnoType->Naval,
			CellStruct{ (short)X, (short)Y }
		));
		ECDebug::ReturnString(Success ? u8"true" : u8"false");
	}
	else
	{
		ECDebug::ReturnStdError(ERROR_INVALID_INDEX);
	}
}