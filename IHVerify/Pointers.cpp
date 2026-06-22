#include "IHVerify.h"
#include "Pointers.h"
#include <YRPP.h>
#include <Unsorted.h>
#include <EventClass.h>


// PointerExpiredProcess is implemented in IHVerify.Marker.cpp
// This file exists to:
// 1) satisfy the linker with the declaration from Pointers.h
// 2) provide a place for IHVerify to expand pointer-lifetime management
//
// DO NOT call GetClassName/vtable on pDyingObj — it has already been freed.
// pDyingObj may be broadcast multiple times (multiple references holding it).
// bRemoved=false means the object is temporarily leaving the map, not dying.


void __cdecl IHVerify_ManualTest(JsonObject Context)
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