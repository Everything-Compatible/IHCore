#include "MiscFunc.h"


bool __declspec(dllexport) PlayAnimOnObject(AnimTypeClass* pAnimType, ObjectClass* Object, byte Iteration = 0x01u)//0xFFu = Infinite Loop
{
	AnimClass* pAnim = GameCreate<AnimClass>(pAnimType, Object->Location);
	pAnim->SetOwnerObject(Object);
	pAnim->RemainingIterations = Iteration;//0xFFu;
	//AnimClass::Array->AddItem(pAnim);
	return true;
}
bool __declspec(dllexport) PlayAnimOnObject(const char* AnimID, ObjectClass* Object, byte Iteration = 0x01u)//0xFFu = Infinite Loop
{
	auto pAnimType = AnimTypeClass::Find(AnimID);
	if (pAnimType == nullptr)return false;
	else
	{
		PlayAnimOnObject(pAnimType, Object, Iteration);
	}
	return true;
}

//YRMemory
/*
bool __declspec(dllexport) PlayAnimOnObj(AnimTypeClass* pAnimType, ObjectClass* Object, byte Iteration = 0x01u)//0xFFu = Infinite Loop
{
	AnimClass* pAnim = (AnimClass*)YRMemory::Allocate(sizeof(AnimClass));
	pAnim = new(pAnim) AnimClass(pAnimType, Object->Location);
	pAnim->SetOwnerObject(Object);
	pAnim->RemainingIterations = Iteration;//0xFFu;
	return true;
}*/