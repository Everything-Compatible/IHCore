#pragma once
#pragma once
#include "IH.Loader.h"
#include "EC.Stream.h"
#include "EC.LoadSave.h"
#include "EC.ObjBase.h"


class ECGameClass_
{
	void Register();
	void Unregister();
public:
	//virtual ECGameClass_* TypeCast(const ECGameType& Type) noexcept;

	virtual void LoadGame(ECStreamReader& Stream) = 0;

	virtual void SaveGame(ECStreamWriter& Stream) = 0;

	virtual size_t GetSizeMax() = 0;


	//bRemoved : true=真寄了 false=临时离场
	virtual void PointerGotInvalid(AbstractClass* const pObject, bool bRemoved) = 0;

	virtual void FinalSwizzle() = 0;

	virtual void Update() = 0;


	ECGameClass_();
	ECGameClass_(const ECGameClass_&);
	ECGameClass_(ECGameClass_&&);
	virtual ~ECGameClass_();
};

//其RTTI信息注册于IHCore
ECObjType ECGameClass : EC_USERTTI, EC_USEUNIQUEID, EC_SYNC, EC_USELOADSAVE
{
	ECRTTI_DefineRTTIFunction(ECGameClass)
	ECRTTI_DefineLoadSaveFunction
	/*
	已经定义了以下接口：
	virtual const ECRTTIInfo* WhatAmI() const;
	
	int GetUniqueID() const;
	
	template<typename T>
	T* DynamicCast()

	template<typename T>
	const T* DynamicCast() const

	virtual void ComputeCRC(CRCEngine& crc) = 0;

	virtual void LoadGame(ECStreamReader& Stream) = 0;
	virtual void SaveGame(ECStreamWriter& Stream) = 0;
	virtual size_t GetSizeMax() = 0;
	virtual void FinalSwizzle() = 0;
	virtual void PointerGotInvalid(AbstractClass* const pObject, bool bRemoved) = 0;
	//暂时无效
	virtual void PointerGotInvalid(ECGameClass* const pObject, bool bRemoved) = 0;
	//暂时无效
	virtual void PointerGotInvalid(SIBuffClass* const pObject, bool bRemoved) = 0;
	*/

	virtual void Update() = 0;

private:
	//在派生类当中自行实现Serialize函数
	template <typename T>
	void Serialize(T& Stream) {};
};


ECObjType ECGameTypeClass : public ECGameClass
{
	ECRTTI_DefineRTTIFunction(ECGameTypeClass)
	ECRTTI_DefineLoadSaveFunction

	virtual void Update() {}

	virtual void InitializeConstants() = 0;
	virtual void LoadFromINI(CCINIClass* INI) = 0;

private:
	//在派生类当中自行实现Serialize函数
	template <typename T>
	void Serialize(T& Stream) {};
};