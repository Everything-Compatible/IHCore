
#include "WIC.Interface.h"
#include "WIC.Template.h"

Ext::LibData WIC(nullptr);

namespace SI
{

	/*
	备注：此处确认了稳定存在的API及其形式，所以直接通过只有最小限度检查的LibData::QueryFunction实现
		  如果对一些检查无法完全确认，请使用Ext::DispatchInterface来连接上对方的接口
		  例如：
		char* SomeFunc(int Param)
		{
			static Ext::DispatchInterface Interface("RegName of the Lib", "RegName of the function", DoNotCheckVersion);
			auto fn = Interface.Func<decltype(SomeFunc)>();
			if (fn)return fn(Param);
			else
			{
				//错误处理
			}
		}
	*/

	char* SI_API GetName(SIInterface_ExtendData* 接口) // 类型 ID 名称
	{
		SIInterface(GetName)(接口);
	}

	AbstractClass* SI_API OwnerEntity(SIInterface_ExtendData* 接口) // 返回值可能为空
	{
		SIInterface(OwnerEntity)(接口);
	}

	AbstractType  SI_API WhatAmI(SIInterface_ExtendData* 接口)
	{
		SIInterface(WhatAmI)(接口);
	}

	int  SI_API GetArrayIndex(SIInterface_ExtendData* 接口)
	{
		SIInterface(GetArrayIndex)(接口);
	}

	bool  SI_API IsSelected(SIInterface_ExtendData* 接口)
	{
		SIInterface(IsSelected)(接口);
	}

	void  SI_API GetCoords(SIInterface_ExtendData* 接口, CoordStruct* 所在坐标)
	{
		SIInterface_void(GetCoords)(接口, 所在坐标);
	}

	void  SI_API SetForceFireWeapon(SIInterface_ExtendData* 接口, WeaponTypeClass* 武器类型)
	{
		SIInterface_void(SetForceFireWeapon)(接口, 武器类型);
	}

	void  SI_API SetForceDeathWeapon(SIInterface_ExtendData* 接口, WeaponTypeClass* 武器类型)
	{
		SIInterface_void(SetForceDeathWeapon)(接口, 武器类型);
	}

	CoordStruct SI_API Calculate_FireCoord(SIInterface_ExtendData* 接口, CoordStruct 武器坐标, double 缩放倍率)
	{
		SIInterface(Calculate_FireCoord)(接口, 武器坐标, 缩放倍率);
	}

	int SI_API GetStaticEXPLevel(SIInterface_ExtendData* 接口)
	{
		SIInterface(GetStaticEXPLevel)(接口);
	}

	double  SI_API GetLevelTotalEXPCost(SIInterface_ExtendData* 接口, double 固定经验值基数, bool 使用新经验系统)
	{
		SIInterface(GetLevelTotalEXPCost)(接口, 固定经验值基数, 使用新经验系统);
	}

	double SI_API Calculate_PropertyGroup_Speed(SIInterface_ExtendData* 接口, double 原始移动速度)
	{
		SIInterface(Calculate_PropertyGroup_Speed)(接口, 原始移动速度);
	}
	double SI_API Calculate_PropertyGroup_Defence(SIInterface_ExtendData* 接口, double 原始伤害)
	{
		SIInterface(Calculate_PropertyGroup_Defence)(接口, 原始伤害);
	}

	double SI_API Calculate_PropertyGroup_Attack(SIInterface_ExtendData* 接口, double 原始伤害)
	{
		SIInterface(Calculate_PropertyGroup_Attack)(接口, 原始伤害);
	}

	double SI_API Calculate_PropertyGroup_ROF(SIInterface_ExtendData* 接口, double 原始射速)
	{
		SIInterface(Calculate_PropertyGroup_ROF)(接口, 原始射速);
	}

	double SI_API Calculate_PropertyGroup_EXP(SIInterface_ExtendData* 接口, double 原始获得的经验值)
	{
		SIInterface(Calculate_PropertyGroup_EXP)(接口, 原始获得的经验值);
	}

	double SI_API Calculate_PropertyGroup_EXPProvide(SIInterface_ExtendData* 接口, double 原始提供的经验值)
	{
		SIInterface(Calculate_PropertyGroup_EXPProvide)(接口, 原始提供的经验值);
	}

	double SI_API Calculate_PropertyGroup_EXPCost(SIInterface_ExtendData* 接口, double 单位升级所需的经验值)
	{
		SIInterface(Calculate_PropertyGroup_EXPCost)(接口, 单位升级所需的经验值);
	}

	double SI_API Calculate_PropertyGroup_RangeWeapon(SIInterface_ExtendData* 接口, double 原始武器射程)
	{
		SIInterface(Calculate_PropertyGroup_RangeWeapon)(接口, 原始武器射程);
	}

	double SI_API Calculate_PropertyGroup_CellSpread(SIInterface_ExtendData* 接口, double 原始弹头溅射范围)
	{
		SIInterface(Calculate_PropertyGroup_CellSpread)(接口, 原始弹头溅射范围);
	}

	int SI_API GetTintColor(SIInterface_ExtendData* 接口, bool 铁幕染色, bool 空袭染色, bool 狂暴染色)
	{
		SIInterface(GetTintColor)(接口, 铁幕染色, 空袭染色, 狂暴染色);
	}

	void SI_API Calculate_CustomTintValues(SIInterface_ExtendData* 接口, int& 染色颜色, int& 染色强度)
	{
		SIInterface_void(Calculate_CustomTintValues)(接口, 染色颜色, 染色强度);
	}

	int SI_API Calculate_CustomTintValues_Color(SIInterface_ExtendData* 接口)
	{
		SIInterface(Calculate_CustomTintValues_Color)(接口);
	}

	int SI_API Calculate_CustomTintValues_Intensity(SIInterface_ExtendData* 接口)
	{
		SIInterface(Calculate_CustomTintValues_Intensity)(接口);
	}

	double SI_API Property_Merge_Multiplier_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_Speed)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_Armor(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_Armor)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_Versus(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_Versus)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_Attack)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_ROF)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_EXP)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_EXPProvide)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_EXPCost)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_RangeWeapon)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Multiplier_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Multiplier_CellSpread)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Speed)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Armor)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Versus)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Attack)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_ROF)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXP)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXPProvide)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXPCost)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_RangeWeapon)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_ExtraMultiplier_CellSpread)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_Speed)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_Damage(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_Damage)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_Attack)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_ROF)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_EXP)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_EXPProvide)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_EXPCost)(接口, 旧的增加量, 新的增加量);
	}

	// 单位是格点
	double SI_API Property_Merge_Addon_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_RangeWeapon)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_Addon_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_Addon_CellSpread)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_Speed)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_Damage(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_Damage)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_Attack)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_ROF)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_EXP)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_EXPProvide)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_EXPCost)(接口, 旧的增加量, 新的增加量);
	}

	// 单位是格点
	double SI_API Property_Merge_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_RangeWeapon)(接口, 旧的增加量, 新的增加量);
	}

	double SI_API Property_Merge_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量)
	{
		SIInterface(Property_Merge_FinalAddon_CellSpread)(接口, 旧的增加量, 新的增加量);
	}

	int SI_API Property_Register_Multiplier_Speed(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_Speed)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_Armor(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_Armor)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_Versus(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_Versus)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_Attack(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_Attack)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_ROF(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_ROF)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_EXP(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_EXP)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_EXPProvide)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_EXPCost(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_EXPCost)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_RangeWeapon)(接口, 增加量);
	}

	int SI_API Property_Register_Multiplier_CellSpread(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Multiplier_CellSpread)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_Speed)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_Armor)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_Versus)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_Attack)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_ROF)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXP)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXPProvide)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXPCost)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_RangeWeapon)(接口, 增加量);
	}

	int SI_API Property_Register_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_ExtraMultiplier_CellSpread)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_Speed(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_Speed)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_Damage(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_Damage)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_Attack(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_Attack)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_ROF(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_ROF)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_EXP(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_EXP)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_EXPProvide(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_EXPProvide)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_EXPCost(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_EXPCost)(接口, 增加量);
	}

	// 单位是格点
	int SI_API Property_Register_Addon_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_RangeWeapon)(接口, 增加量);
	}

	int SI_API Property_Register_Addon_CellSpread(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_Addon_CellSpread)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_Speed(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_Speed)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_Damage(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_Damage)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_Attack(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_Attack)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_ROF(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_ROF)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_EXP(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_EXP)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_EXPProvide)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_EXPCost)(接口, 增加量);
	}

	// 单位是格点
	int SI_API Property_Register_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_RangeWeapon)(接口, 增加量);
	}

	int SI_API Property_Register_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, double 增加量)
	{
		SIInterface(Property_Register_FinalAddon_CellSpread)(接口, 增加量);
	}

	void SI_API Property_Unregister_Multiplier_Speed(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_Speed)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_Armor(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_Armor)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_Versus(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_Versus)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_Attack(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_Attack)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_ROF(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_ROF)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_EXP(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXP)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXPProvide)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_EXPCost(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXPCost)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_RangeWeapon)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Multiplier_CellSpread(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Multiplier_CellSpread)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Speed)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Armor)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Versus)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Attack)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_ROF)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXP)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXPProvide)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXPCost)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_RangeWeapon)(接口, 注册索引);
	}

	void SI_API Property_Unregister_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_CellSpread)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_Speed(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_Speed)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_Damage(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_Damage)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_Attack(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_Attack)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_ROF(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_ROF)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_EXP(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_EXP)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_EXPProvide)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_EXPCost(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_EXPCost)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_RangeWeapon)(接口, 注册索引);
	}

	void SI_API Property_Unregister_Addon_CellSpread(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_Addon_CellSpread)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_Speed(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Speed)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_Damage(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Damage)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_Attack(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Attack)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_ROF(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_ROF)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_EXP(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXP)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXPProvide)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXPCost)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_RangeWeapon)(接口, 注册索引);
	}

	void SI_API Property_Unregister_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_FinalAddon_CellSpread)(接口, 注册索引);
	}

	void SI_API Property_Merge_Ability_ImmuneCrush(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneCrush)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmuneCrushAdvance(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneCrushAdvance)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmuneRadiation(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneRadiation)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmunePsionicDamage(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePsionicDamage)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmunePsionic(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePsionic)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmuneBerzerk(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneBerzerk)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmunePoison(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePoison)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmuneWarp(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneWarp)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_ImmuneEMP(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneEMP)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_SelfHealing(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Percent(SIInterface_ExtendData* 接口, double 旧的能力强度, double 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Percent)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Cap(SIInterface_ExtendData* 接口, double 旧的能力强度, double 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Cap)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Delay(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Delay)(接口, 旧的能力强度, 新的能力强度);
	}

	void SI_API Property_Merge_Ability_SelfHealing_CombatDelay(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_CombatDelay)(接口, 旧的能力强度, 新的能力强度);
	}

	int SI_API Property_Register_CustomTintValues_A(SIInterface_ExtendData* 接口, ColorStruct 染色颜色, int 染色强度, int 作战方归属)
	{
		SIInterface(Property_Register_CustomTintValues_A)(接口, 染色颜色, 染色强度, 作战方归属);
	}

	int SI_API Property_Register_CustomTintValues_B(SIInterface_ExtendData* 接口, int 染色颜色, int 染色强度, int 作战方归属)
	{
		SIInterface(Property_Register_CustomTintValues_B)(接口, 染色颜色, 染色强度, 作战方归属);
	}

	void SI_API Property_Unregister_CustomTintValues(SIInterface_ExtendData* 接口, int 注册索引)
	{
		SIInterface_void(Property_Unregister_CustomTintValues)(接口, 注册索引);
	}

	// 扣除经验使用负数
	void SI_API EXP_Add(SIInterface_ExtendData* 接口, const double 提供的经验值, SIEXPSourceType 经验值来源, SIValues_EXPMerge* 合并经验值参数包)
	{
		SIInterface_void(EXP_Add)(接口, 提供的经验值, 经验值来源, 合并经验值参数包);
	}

	bool SI_API Buff_IsPowerOffline(SIInterface_ExtendData* 接口)
	{
		SIInterface(Buff_IsPowerOffline)(接口);
	}

	bool SI_API Buff_IsNeedHide(SIInterface_ExtendData* 接口)
	{
		SIInterface(Buff_IsNeedHide)(接口);
	}

	int SI_API Buff_GetEffectDuration(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, int 原始的挂载持续时间)
	{
		SIInterface(Buff_GetEffectDuration)(接口, Buff类型, 原始的挂载持续时间);
	}

	double SI_API Buff_GetEffectHealth(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, double 原始的生命值)
	{
		SIInterface(Buff_GetEffectHealth)(接口, Buff类型, 原始的生命值);
	}

	double SI_API Buff_GetEffectPower(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, double 原始的强度值)
	{
		SIInterface(Buff_GetEffectPower)(接口, Buff类型, 原始的强度值);
	}

	SIBuffClass* SI_API Buff_CreateOrMerge(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, AbstractClass* 来源, HouseClass* 来源所属作战方)
	{
		SIInterface(Buff_CreateOrMerge)(接口, Buff类型, Buff参数设置包, 来源, 来源所属作战方);
	}

	void SI_API Buff_TryActive_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包)
	{
		SIInterface_void(Buff_TryActive_A)(接口, Buff类型, Buff参数设置包);
	}

	void SI_API Buff_TryActive_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
	{
		SIInterface_void(Buff_TryActive_B)(接口, Buff类型, Buff参数设置包, 检查单位属性参数包);
	}

	// [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryActive_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包))
	{
		SIInterface_void(Buff_TryActive_C)(接口, 来源, 判断处理函数);
	}

	void SI_API Buff_TryAfter_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型)
	{
		SIInterface_void(Buff_TryAfter_A)(接口, Buff类型);
	}

	void SI_API Buff_TryAfter_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
	{
		SIInterface_void(Buff_TryAfter_B)(接口, Buff类型, 检查单位属性参数包);
	}

	// [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_TryAfter_C)(接口, 来源, 判断处理函数);
	}

	void SI_API Buff_TryRemove_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型)
	{
		SIInterface_void(Buff_TryRemove_A)(接口, Buff类型);
	}

	void SI_API Buff_TryRemove_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
	{
		SIInterface_void(Buff_TryRemove_B)(接口, Buff类型, 检查单位属性参数包);
	}

	// [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_TryRemove_C)(接口, 来源, 判断处理函数);
	}

	void SI_API Buff_TryMergeSetting_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包)
	{
		SIInterface_void(Buff_TryMergeSetting_A)(接口, Buff类型, Buff参数设置包);
	}

	void SI_API Buff_TryMergeSetting_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
	{
		SIInterface_void(Buff_TryMergeSetting_B)(接口, Buff类型, Buff参数设置包, 检查单位属性参数包);
	}

	// [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包))
	{
		SIInterface_void(Buff_TryMergeSetting_C)(接口, 来源, 判断处理函数);
	}

	void SI_API Buff_TryChange_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方)
	{
		SIInterface_void(Buff_TryChange_A)(接口, 旧Buff类型, 新Buff类型, Buff参数设置包, 基础挂载持续时间增加量, 强度值增加量, 来源, 来源所属作战方);
	}

	void SI_API Buff_TryChange_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
	{
		SIInterface_void(Buff_TryChange_B)(接口, 旧Buff类型, 新Buff类型, Buff参数设置包, 基础挂载持续时间增加量, 强度值增加量, 来源, 来源所属作战方, 检查单位属性参数包);
	}

	bool SI_API Buff_HasBuffed_A(SIInterface_ExtendData* 接口)
	{
		SIInterface(Buff_HasBuffed_A)(接口);
	}

	bool SI_API Buff_HasBuffed_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型)
	{
		SIInterface(Buff_HasBuffed_B)(接口, Buff类型);
	}

	bool SI_API Buff_HasBuffed_C(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIBuffClass_State Buff状态)
	{
		SIInterface(Buff_HasBuffed_C)(接口, Buff类型, Buff状态);
	}

	bool SI_API Buff_HasBuffed_D(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, AbstractClass* 排除的来源)
	{
		SIInterface(Buff_HasBuffed_D)(接口, Buff类型, 排除的来源);
	}

	bool SI_API Buff_HasBuffed_E(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, AbstractClass* 需求的来源, bool 没用的标识)
	{
		SIInterface(Buff_HasBuffed_E)(接口, Buff类型, 需求的来源, 没用的标识);
	}

	bool SI_API Buff_HasBuffed_F(SIInterface_ExtendData* 接口, SIBuffTypeClass_EffectType 效果种类)
	{
		SIInterface(Buff_HasBuffed_F)(接口, 效果种类);
	}

	bool SI_API Buff_HasBuffed_G(SIInterface_ExtendData* 接口, SIBuffTypeClass_EffectType 效果种类, SIBuffClass_State Buff状态)
	{
		SIInterface(Buff_HasBuffed_G)(接口, 效果种类, Buff状态);
	}

	// 此处不会立即移除无效的 Buff
	void SI_API Buff_LookUp(SIInterface_ExtendData* 接口, void* 来源, void(*遍历函数)(void* 来源, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_LookUp)(接口, 来源, 遍历函数);
	}

	void SI_API Buff_Update(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(Buff_Update)(接口);
	}

	bool SI_API Buff_TriggerFire(SIInterface_ExtendData* 接口, AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器)
	{
		SIInterface(Buff_TriggerFire)(接口, 目标, 武器索引, 武器数据, 本体开火坐标, 死亡武器);
	}

	double SI_API Buff_TriggerAttacker(SIInterface_ExtendData* 接口, TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害)
	{
		SIInterface(Buff_TriggerAttacker)(接口, 目标单位, 伤害参数, 当前伤害);
	}

	double SI_API Buff_TriggerDefender(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数, double 当前伤害)
	{
		SIInterface(Buff_TriggerDefender)(接口, 伤害参数, 当前伤害);
	}

	double SI_API Buff_TriggerDefender_Final(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数, double 当前伤害)
	{
		SIInterface(Buff_TriggerDefender_Final)(接口, 伤害参数, 当前伤害);
	}

	void SI_API Buff_TriggerDeath(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数)
	{
		SIInterface_void(Buff_TriggerDeath)(接口, 伤害参数);
	}

	void SI_API Buff_TriggerKiller(SIInterface_ExtendData* 接口, TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数)
	{
		SIInterface_void(Buff_TriggerKiller)(接口, 目标单位, 伤害参数);
	}

	double SI_API Buff_TriggerSelf(SIInterface_ExtendData* 接口, double 当前伤害)
	{
		SIInterface(Buff_TriggerSelf)(接口, 当前伤害);
	}

	void SI_API Buff_DoDamages(SIInterface_ExtendData* 接口, double 准备造成的伤害)
	{
		SIInterface_void(Buff_DoDamages)(接口, 准备造成的伤害);
	}

	void SI_API Element_Merge_ResistanceType(SIInterface_ExtendData* 接口, SIElementResistanceTypeClass* 元素抗性类型, int 层数计数)
	{
		SIInterface_void(Element_Merge_ResistanceType)(接口, 元素抗性类型, 层数计数);
	}

	void SI_API Element_Merge_ImmuneElementType(SIInterface_ExtendData* 接口, SIElementTypeClass* 元素类型, int 层数计数)
	{
		SIInterface_void(Element_Merge_ImmuneElementType)(接口, 元素类型, 层数计数);
	}

	void SI_API Update(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(Update)(接口);
	}

	void SI_API UpdateBar_UpdateAllow(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(UpdateBar_UpdateAllow)(接口);
	}

	void SI_API UpdateDrawBar(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(UpdateDrawBar)(接口);
	}

	bool SI_API UpdateClickBar_Hover(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, Point2D* 点击像素偏移)
	{
		SIInterface(UpdateClickBar_Hover)(接口, 点击的作战方, 点击像素偏移);
	}

	void SI_API UpdateClickBar_TryClick(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, Point2D* 点击像素偏移, int& Buff索引, SIDigitalClickBarIndex& 点击索引, double& 当前值, bool& 禁用标识)
	{
		SIInterface_void(UpdateClickBar_TryClick)(接口, 点击的作战方, 点击像素偏移, Buff索引, 点击索引, 当前值, 禁用标识);
	}

	void SI_API UpdateClickBar_Effect(SIInterface_ExtendData* 接口, int Buff索引, SIDigitalClickBarIndex 点击索引, HouseClass* 点击的作战方, double 当前值)
	{
		SIInterface_void(UpdateClickBar_Effect)(接口, Buff索引, 点击索引, 点击的作战方, 当前值);
	}

	bool SI_API UpdateKeyExecuteBar_TryExecute(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, SICommandCode 快捷键代码)
	{
		SIInterface(UpdateKeyExecuteBar_TryExecute)(接口, 点击的作战方, 快捷键代码);
	}

	void SI_API UpdateKeyExecuteBar_Effect(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, SICommandCode 快捷键代码)
	{
		SIInterface_void(UpdateKeyExecuteBar_Effect)(接口, 点击的作战方, 快捷键代码);
	}

	SIBuffClass* SI_API Buff_GetBuff_A(SIInterface_ExtendData* 接口, int Buff索引) // 返回值可能为空
	{
		SIInterface(Buff_GetBuff_A)(接口, Buff索引);
	}
	SIBuffClass* SI_API Buff_GetBuff_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型) // 返回值可能为空
	{
		SIInterface(Buff_GetBuff_B)(接口, Buff类型);
	}
	void SI_API Init(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(Init)(接口);
	}
	void SI_API UnInit(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(UnInit)(接口);
	}
	void SI_API UpdateTechnoType(SIInterface_ExtendData* 接口, TechnoTypeClass* 旧的单位类型)
	{
		SIInterface_void(UpdateTechnoType)(接口, 旧的单位类型);
	}
	void SI_API UpdateOwningHouse(SIInterface_ExtendData* 接口, const HouseClass* const 旧的单位所属作战方, const HouseClass* const 新的单位所属作战方)
	{
		SIInterface_void(UpdateOwningHouse)(接口, 旧的单位所属作战方, 新的单位所属作战方);
	}
	void SI_API AvailableEffect(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(AvailableEffect)(接口);
	}
	void SI_API UnavailableEffect(SIInterface_ExtendData* 接口)
	{
		SIInterface_void(UnavailableEffect)(接口);
	}

}

namespace SIBroadcast
{
	void SI_API FreshBroadcastPower(SIBroadcastClass* 接口, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIInterfaceDecl("WIC.Broadcast.FreshBroadcastPower", FreshBroadcastPower)(接口, 广播强度, 触发被动监听, 发布坐标, 发布单位, 指向目标);
	}
	void SI_API PostBroadcast(SIBroadcastClass* 接口, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIInterfaceDecl("WIC.Broadcast.PostBroadcast", PostBroadcast)(接口, 触发被动监听, 发布坐标, 发布单位, 指向目标);
	}
	int SI_API GetBroadcastListenerCount(SIBroadcastClass* 接口, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastListenerCount", GetBroadcastListenerCount)(接口, 无视限制条件, 发布坐标, 发布单位, 指向目标);
	}
	void SI_API AddListener(SIBroadcastClass* 接口, SIBuffClass* 要添加的监听Buff)
	{
		SIInterfaceDecl("WIC.Broadcast.AddListener", AddListener)(接口, 要添加的监听Buff);
	}
	void SI_API RemoveListener(SIBroadcastClass* 接口, SIBuffClass* 要移除的监听Buff)
	{
		SIInterfaceDecl("WIC.Broadcast.RemoveListener", RemoveListener)(接口, 要移除的监听Buff);
	}
	bool SI_API IsEmpty(SIBroadcastClass* 接口)
	{
		SIInterfaceDecl("WIC.Broadcast.IsEmpty", IsEmpty)(接口);
	}
	SIConstVector<SIBroadcastClass*>& SI_API GetBroadcastArray()
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastArray", GetBroadcastArray)();
	}
	HouseClass* SI_API OwnerObject(SIBroadcastClass* 接口)
	{
		SIInterfaceDecl("WIC.Broadcast.OwnerObject", OwnerObject)(接口);
	}
	double SI_API GetBroadcastPower(SIBroadcastClass* 接口)
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastPower", GetBroadcastPower)(接口);
	}
	const SIDataList<SIBuffClass*>& SI_API GetListenerBuffList(SIBroadcastClass* 接口)
	{
		SIInterfaceDecl("WIC.Broadcast.GetListenerBuffList", GetListenerBuffList)(接口);
	}
}


namespace SIHouseExt
{
	void SI_API Counter_AddOriginTechnoType(SIHouse_ExtendData* 接口, TechnoTypeClass* 单位类型)
	{
		SIHouseExtDecl(Counter_AddOriginTechnoType)(接口, 单位类型);
	}
	void SI_API Counter_RemoveOriginTechnoType(SIHouse_ExtendData* 接口, TechnoTypeClass* 单位类型)
	{
		SIHouseExtDecl(Counter_RemoveOriginTechnoType)(接口, 单位类型);
	}
	bool SI_API AlwaysShowCameo(const SIHouse_ExtendData* const 接口, const TechnoTypeClass* const 单位类型, bool 包括正在建造的单位)
	{
		SIHouseExtDecl(AlwaysShowCameo)(接口, 单位类型, 包括正在建造的单位);
	}
	void SI_API Factory_CalculateProduction(SIHouse_ExtendData* 接口)
	{
		SIHouseExtDecl(Factory_CalculateProduction)(接口);
	}
	void SI_API Factory_MergeProduction(SIHouse_ExtendData* 接口, TechnoClass* 单位)
	{
		SIHouseExtDecl(Factory_MergeProduction)(接口, 单位);
	}
	void SI_API Factory_RemoveProduction(SIHouse_ExtendData* 接口, TechnoClass* 单位)
	{
		SIHouseExtDecl(Factory_RemoveProduction)(接口, 单位);
	}
	void SI_API EXP_Init(SIHouse_ExtendData* 接口)
	{
		SIHouseExtDecl(EXP_Init)(接口);
	}
	void SI_API EXP_Get(SIHouse_ExtendData* 接口, TechnoClass* 单位, SIValues_HouseInitEXP* 国家经验值参数包)
	{
		SIHouseExtDecl(EXP_Get)(接口, 单位, 国家经验值参数包);
	}
	bool SI_API EXP_MergeAcademy(SIHouse_ExtendData* 接口, TechnoClass* 单位, TechnoTypeClass* 单位类型)//可能会失败，失败返回false
	{
		SIHouseExtDecl(EXP_MergeAcademy)(接口, 单位, 单位类型);
	}
	bool SI_API EXP_RemoveAcademy(SIHouse_ExtendData* 接口, TechnoClass* 单位, TechnoTypeClass* 单位类型)//可能会失败，失败返回false
	{
		SIHouseExtDecl(EXP_RemoveAcademy)(接口, 单位, 单位类型);
	}
	bool SI_API EXP_UpdateSpyEffect(SIHouse_ExtendData* 接口, BuildingClass* 建筑单位, BuildingTypeClass* 建筑单位类型)//可能会失败，失败返回false
	{
		SIHouseExtDecl(EXP_UpdateSpyEffect)(接口, 建筑单位, 建筑单位类型);
	}
	void SI_API SetHouseVar(SIHouse_ExtendData* 接口, int 索引值, double 数值)
	{
		SIHouseExtDecl(SetHouseVar)(接口, 索引值, 数值);
	}
	double SI_API GetHouseVar(SIHouse_ExtendData* 接口, int 索引值)
	{
		SIHouseExtDecl(GetHouseVar)(接口, 索引值);
	}
	void SI_API DeleteHouseVar(SIHouse_ExtendData* 接口, int 索引值)
	{
		SIHouseExtDecl(DeleteHouseVar)(接口, 索引值);
	}
	SIBroadcastClass* SI_API Broadcast_FindOrAllocate(SIHouse_ExtendData* 接口, int 频道)// 返回值可能为空
	{
		SIHouseExtDecl(Broadcast_FindOrAllocate)(接口, 频道);
	}
	SIBroadcastClass* SI_API Broadcast_Find(SIHouse_ExtendData* 接口, int 频道)// 返回值可能为空
	{
		SIHouseExtDecl(Broadcast_Find)(接口, 频道);
	}
	void SI_API FreshBroadcastPower(SIHouse_ExtendData* 接口, int 频道, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIHouseExtDecl(FreshBroadcastPower)(接口, 频道, 广播强度, 触发被动监听, 发布坐标, 发布单位, 指向目标);
	}
	void SI_API PostBroadcast(SIHouse_ExtendData* 接口, int 频道, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIHouseExtDecl(PostBroadcast)(接口, 频道, 触发被动监听, 发布坐标, 发布单位, 指向目标);
	}
	int SI_API GetBroadcastListenerCount(SIHouse_ExtendData* 接口, int 频道, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
	{
		SIHouseExtDecl(GetBroadcastListenerCount)(接口, 频道, 无视限制条件, 发布坐标, 发布单位, 指向目标);
	}
	double SI_API GetBroadcastPower(SIHouse_ExtendData* 接口, int 频道)
	{
		SIHouseExtDecl(GetBroadcastPower)(接口, 频道);
	}
	void SI_API AddBroadcastListener(SIHouse_ExtendData* 接口, int 频道, SIBuffClass* 监听Buff)
	{
		SIHouseExtDecl(AddBroadcastListener)(接口, 频道, 监听Buff);
	}
	void SI_API RemoveBroadcastListener(SIHouse_ExtendData* 接口, int 频道, SIBuffClass* 监听Buff)
	{
		SIHouseExtDecl(RemoveBroadcastListener)(接口, 频道, 监听Buff);
	}
	void SI_API EXPPool_Add(SIHouse_ExtendData* 接口, double 额外经验值)
	{
		SIHouseExtDecl(EXPPool_Add)(接口, 额外经验值);
	}
	double SI_API EXPPool_Pop(SIHouse_ExtendData* 接口, double 需求的额外经验值)
	{
		SIHouseExtDecl(EXPPool_Pop)(接口, 需求的额外经验值);
	}
	void SI_API ForEach_HouseVar(SIHouse_ExtendData* 接口, void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 索引, double& 值))
	{
		SIHouseExtDecl(ForEach_HouseVar)(接口, 参数集, 处理函数);
	}
	void SI_API ForEach_Broadcast(SIHouse_ExtendData* 接口, void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 频道, SIBroadcastClass* 值))
	{
		SIHouseExtDecl(ForEach_Broadcast)(接口, 参数集, 处理函数);
	}
	HouseClass* SI_API OwnerObject(SIHouse_ExtendData* 接口)
	{
		SIHouseExtDecl(OwnerObject)(接口);
	}
}