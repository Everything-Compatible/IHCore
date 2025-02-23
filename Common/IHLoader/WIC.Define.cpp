
#include "WIC.Interface.h"
#include "WIC.Template.h"

Ext::LibData WIC(nullptr);

namespace SI
{

	/*
	��ע���˴�ȷ�����ȶ����ڵ�API������ʽ������ֱ��ͨ��ֻ����С�޶ȼ���LibData::QueryFunctionʵ��
		  �����һЩ����޷���ȫȷ�ϣ���ʹ��Ext::DispatchInterface�������϶Է��Ľӿ�
		  ���磺
		char* SomeFunc(int Param)
		{
			static Ext::DispatchInterface Interface("RegName of the Lib", "RegName of the function", DoNotCheckVersion);
			auto fn = Interface.Func<decltype(SomeFunc)>();
			if (fn)return fn(Param);
			else
			{
				//������
			}
		}
	*/

	char* SI_API GetName(SIInterface_ExtendData* �ӿ�) // ���� ID ����
	{
		SIInterface(GetName)(�ӿ�);
	}

	AbstractClass* SI_API OwnerEntity(SIInterface_ExtendData* �ӿ�) // ����ֵ����Ϊ��
	{
		SIInterface(OwnerEntity)(�ӿ�);
	}

	AbstractType  SI_API WhatAmI(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(WhatAmI)(�ӿ�);
	}

	int  SI_API GetArrayIndex(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(GetArrayIndex)(�ӿ�);
	}

	bool  SI_API IsSelected(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(IsSelected)(�ӿ�);
	}

	void  SI_API GetCoords(SIInterface_ExtendData* �ӿ�, CoordStruct* ��������)
	{
		SIInterface_void(GetCoords)(�ӿ�, ��������);
	}

	void  SI_API SetForceFireWeapon(SIInterface_ExtendData* �ӿ�, WeaponTypeClass* ��������)
	{
		SIInterface_void(SetForceFireWeapon)(�ӿ�, ��������);
	}

	void  SI_API SetForceDeathWeapon(SIInterface_ExtendData* �ӿ�, WeaponTypeClass* ��������)
	{
		SIInterface_void(SetForceDeathWeapon)(�ӿ�, ��������);
	}

	CoordStruct SI_API Calculate_FireCoord(SIInterface_ExtendData* �ӿ�, CoordStruct ��������, double ���ű���)
	{
		SIInterface(Calculate_FireCoord)(�ӿ�, ��������, ���ű���);
	}

	int SI_API GetStaticEXPLevel(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(GetStaticEXPLevel)(�ӿ�);
	}

	double  SI_API GetLevelTotalEXPCost(SIInterface_ExtendData* �ӿ�, double �̶�����ֵ����, bool ʹ���¾���ϵͳ)
	{
		SIInterface(GetLevelTotalEXPCost)(�ӿ�, �̶�����ֵ����, ʹ���¾���ϵͳ);
	}

	double SI_API Calculate_PropertyGroup_Speed(SIInterface_ExtendData* �ӿ�, double ԭʼ�ƶ��ٶ�)
	{
		SIInterface(Calculate_PropertyGroup_Speed)(�ӿ�, ԭʼ�ƶ��ٶ�);
	}
	double SI_API Calculate_PropertyGroup_Defence(SIInterface_ExtendData* �ӿ�, double ԭʼ�˺�)
	{
		SIInterface(Calculate_PropertyGroup_Defence)(�ӿ�, ԭʼ�˺�);
	}

	double SI_API Calculate_PropertyGroup_Attack(SIInterface_ExtendData* �ӿ�, double ԭʼ�˺�)
	{
		SIInterface(Calculate_PropertyGroup_Attack)(�ӿ�, ԭʼ�˺�);
	}

	double SI_API Calculate_PropertyGroup_ROF(SIInterface_ExtendData* �ӿ�, double ԭʼ����)
	{
		SIInterface(Calculate_PropertyGroup_ROF)(�ӿ�, ԭʼ����);
	}

	double SI_API Calculate_PropertyGroup_EXP(SIInterface_ExtendData* �ӿ�, double ԭʼ��õľ���ֵ)
	{
		SIInterface(Calculate_PropertyGroup_EXP)(�ӿ�, ԭʼ��õľ���ֵ);
	}

	double SI_API Calculate_PropertyGroup_EXPProvide(SIInterface_ExtendData* �ӿ�, double ԭʼ�ṩ�ľ���ֵ)
	{
		SIInterface(Calculate_PropertyGroup_EXPProvide)(�ӿ�, ԭʼ�ṩ�ľ���ֵ);
	}

	double SI_API Calculate_PropertyGroup_EXPCost(SIInterface_ExtendData* �ӿ�, double ��λ��������ľ���ֵ)
	{
		SIInterface(Calculate_PropertyGroup_EXPCost)(�ӿ�, ��λ��������ľ���ֵ);
	}

	double SI_API Calculate_PropertyGroup_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ԭʼ�������)
	{
		SIInterface(Calculate_PropertyGroup_RangeWeapon)(�ӿ�, ԭʼ�������);
	}

	double SI_API Calculate_PropertyGroup_CellSpread(SIInterface_ExtendData* �ӿ�, double ԭʼ��ͷ���䷶Χ)
	{
		SIInterface(Calculate_PropertyGroup_CellSpread)(�ӿ�, ԭʼ��ͷ���䷶Χ);
	}

	int SI_API GetTintColor(SIInterface_ExtendData* �ӿ�, bool ��ĻȾɫ, bool ��ϮȾɫ, bool ��Ⱦɫ)
	{
		SIInterface(GetTintColor)(�ӿ�, ��ĻȾɫ, ��ϮȾɫ, ��Ⱦɫ);
	}

	void SI_API Calculate_CustomTintValues(SIInterface_ExtendData* �ӿ�, int& Ⱦɫ��ɫ, int& Ⱦɫǿ��)
	{
		SIInterface_void(Calculate_CustomTintValues)(�ӿ�, Ⱦɫ��ɫ, Ⱦɫǿ��);
	}

	int SI_API Calculate_CustomTintValues_Color(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(Calculate_CustomTintValues_Color)(�ӿ�);
	}

	int SI_API Calculate_CustomTintValues_Intensity(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(Calculate_CustomTintValues_Intensity)(�ӿ�);
	}

	double SI_API Property_Merge_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_Speed)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_Armor)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_Versus)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_Attack)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_ROF)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_EXP)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_EXPProvide)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_EXPCost)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_RangeWeapon)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Multiplier_CellSpread)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Speed)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Armor)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Versus)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_Attack)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_ROF)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXP)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXPProvide)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_EXPCost)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_RangeWeapon)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_ExtraMultiplier_CellSpread)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_Speed)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_Damage(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_Damage)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_Attack)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_ROF)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_EXP)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_EXPProvide)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_EXPCost)(�ӿ�, �ɵ�������, �µ�������);
	}

	// ��λ�Ǹ��
	double SI_API Property_Merge_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_RangeWeapon)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_Addon_CellSpread)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_Speed)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_Damage)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_Attack)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_ROF)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_EXP)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_EXPProvide)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_EXPCost)(�ӿ�, �ɵ�������, �µ�������);
	}

	// ��λ�Ǹ��
	double SI_API Property_Merge_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_RangeWeapon)(�ӿ�, �ɵ�������, �µ�������);
	}

	double SI_API Property_Merge_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������)
	{
		SIInterface(Property_Merge_FinalAddon_CellSpread)(�ӿ�, �ɵ�������, �µ�������);
	}

	int SI_API Property_Register_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_Speed)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_Armor)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_Versus)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_Attack)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_ROF)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_EXP)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_EXPProvide)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_EXPCost)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_RangeWeapon)(�ӿ�, ������);
	}

	int SI_API Property_Register_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Multiplier_CellSpread)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_Speed)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_Armor)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_Versus)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_Attack)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_ROF)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXP)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXPProvide)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_EXPCost)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_RangeWeapon)(�ӿ�, ������);
	}

	int SI_API Property_Register_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_ExtraMultiplier_CellSpread)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_Speed(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_Speed)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_Damage(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_Damage)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_Attack(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_Attack)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_ROF(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_ROF)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_EXP(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_EXP)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_EXPProvide)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_EXPCost)(�ӿ�, ������);
	}

	// ��λ�Ǹ��
	int SI_API Property_Register_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_RangeWeapon)(�ӿ�, ������);
	}

	int SI_API Property_Register_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_Addon_CellSpread)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_Speed)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_Damage)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_Attack)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_ROF)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_EXP)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_EXPProvide)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_EXPCost)(�ӿ�, ������);
	}

	// ��λ�Ǹ��
	int SI_API Property_Register_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_RangeWeapon)(�ӿ�, ������);
	}

	int SI_API Property_Register_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, double ������)
	{
		SIInterface(Property_Register_FinalAddon_CellSpread)(�ӿ�, ������);
	}

	void SI_API Property_Unregister_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_Speed)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_Armor)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_Versus)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_Attack)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_ROF)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXP)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXPProvide)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_EXPCost)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_RangeWeapon)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Multiplier_CellSpread)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Speed)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Armor)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Versus)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_Attack)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_ROF)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXP)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXPProvide)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_EXPCost)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_RangeWeapon)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_ExtraMultiplier_CellSpread)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_Speed(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_Speed)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_Damage(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_Damage)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_Attack(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_Attack)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_ROF(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_ROF)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_EXP(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_EXP)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_EXPProvide)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_EXPCost)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_RangeWeapon)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_Addon_CellSpread)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Speed)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Damage)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_Attack)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_ROF)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXP)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXPProvide)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_EXPCost)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_RangeWeapon)(�ӿ�, ע������);
	}

	void SI_API Property_Unregister_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_FinalAddon_CellSpread)(�ӿ�, ע������);
	}

	void SI_API Property_Merge_Ability_ImmuneCrush(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneCrush)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmuneCrushAdvance(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneCrushAdvance)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmuneRadiation(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneRadiation)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmunePsionicDamage(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePsionicDamage)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmunePsionic(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePsionic)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmuneBerzerk(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneBerzerk)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmunePoison(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmunePoison)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmuneWarp(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneWarp)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_ImmuneEMP(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_ImmuneEMP)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_SelfHealing(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Percent(SIInterface_ExtendData* �ӿ�, double �ɵ�����ǿ��, double �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Percent)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Cap(SIInterface_ExtendData* �ӿ�, double �ɵ�����ǿ��, double �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Cap)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_SelfHealing_Delay(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_Delay)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	void SI_API Property_Merge_Ability_SelfHealing_CombatDelay(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��)
	{
		SIInterface_void(Property_Merge_Ability_SelfHealing_CombatDelay)(�ӿ�, �ɵ�����ǿ��, �µ�����ǿ��);
	}

	int SI_API Property_Register_CustomTintValues_A(SIInterface_ExtendData* �ӿ�, ColorStruct Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������)
	{
		SIInterface(Property_Register_CustomTintValues_A)(�ӿ�, Ⱦɫ��ɫ, Ⱦɫǿ��, ��ս������);
	}

	int SI_API Property_Register_CustomTintValues_B(SIInterface_ExtendData* �ӿ�, int Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������)
	{
		SIInterface(Property_Register_CustomTintValues_B)(�ӿ�, Ⱦɫ��ɫ, Ⱦɫǿ��, ��ս������);
	}

	void SI_API Property_Unregister_CustomTintValues(SIInterface_ExtendData* �ӿ�, int ע������)
	{
		SIInterface_void(Property_Unregister_CustomTintValues)(�ӿ�, ע������);
	}

	// �۳�����ʹ�ø���
	void SI_API EXP_Add(SIInterface_ExtendData* �ӿ�, const double �ṩ�ľ���ֵ, SIEXPSourceType ����ֵ��Դ, SIValues_EXPMerge* �ϲ�����ֵ������)
	{
		SIInterface_void(EXP_Add)(�ӿ�, �ṩ�ľ���ֵ, ����ֵ��Դ, �ϲ�����ֵ������);
	}

	bool SI_API Buff_IsPowerOffline(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(Buff_IsPowerOffline)(�ӿ�);
	}

	bool SI_API Buff_IsNeedHide(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(Buff_IsNeedHide)(�ӿ�);
	}

	int SI_API Buff_GetEffectDuration(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, int ԭʼ�Ĺ��س���ʱ��)
	{
		SIInterface(Buff_GetEffectDuration)(�ӿ�, Buff����, ԭʼ�Ĺ��س���ʱ��);
	}

	double SI_API Buff_GetEffectHealth(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, double ԭʼ������ֵ)
	{
		SIInterface(Buff_GetEffectHealth)(�ӿ�, Buff����, ԭʼ������ֵ);
	}

	double SI_API Buff_GetEffectPower(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, double ԭʼ��ǿ��ֵ)
	{
		SIInterface(Buff_GetEffectPower)(�ӿ�, Buff����, ԭʼ��ǿ��ֵ);
	}

	SIBuffClass* SI_API Buff_CreateOrMerge(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, AbstractClass* ��Դ, HouseClass* ��Դ������ս��)
	{
		SIInterface(Buff_CreateOrMerge)(�ӿ�, Buff����, Buff�������ð�, ��Դ, ��Դ������ս��);
	}

	void SI_API Buff_TryActive_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�)
	{
		SIInterface_void(Buff_TryActive_A)(�ӿ�, Buff����, Buff�������ð�);
	}

	void SI_API Buff_TryActive_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
	{
		SIInterface_void(Buff_TryActive_B)(�ӿ�, Buff����, Buff�������ð�, ��鵥λ���Բ�����);
	}

	// [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryActive_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�))
	{
		SIInterface_void(Buff_TryActive_C)(�ӿ�, ��Դ, �жϴ�����);
	}

	void SI_API Buff_TryAfter_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����)
	{
		SIInterface_void(Buff_TryAfter_A)(�ӿ�, Buff����);
	}

	void SI_API Buff_TryAfter_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
	{
		SIInterface_void(Buff_TryAfter_B)(�ӿ�, Buff����, ��鵥λ���Բ�����);
	}

	// [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_TryAfter_C)(�ӿ�, ��Դ, �жϴ�����);
	}

	void SI_API Buff_TryRemove_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����)
	{
		SIInterface_void(Buff_TryRemove_A)(�ӿ�, Buff����);
	}

	void SI_API Buff_TryRemove_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
	{
		SIInterface_void(Buff_TryRemove_B)(�ӿ�, Buff����, ��鵥λ���Բ�����);
	}

	// [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_TryRemove_C)(�ӿ�, ��Դ, �жϴ�����);
	}

	void SI_API Buff_TryMergeSetting_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�)
	{
		SIInterface_void(Buff_TryMergeSetting_A)(�ӿ�, Buff����, Buff�������ð�);
	}

	void SI_API Buff_TryMergeSetting_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
	{
		SIInterface_void(Buff_TryMergeSetting_B)(�ӿ�, Buff����, Buff�������ð�, ��鵥λ���Բ�����);
	}

	// [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�))
	{
		SIInterface_void(Buff_TryMergeSetting_C)(�ӿ�, ��Դ, �жϴ�����);
	}

	void SI_API Buff_TryChange_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��)
	{
		SIInterface_void(Buff_TryChange_A)(�ӿ�, ��Buff����, ��Buff����, Buff�������ð�, �������س���ʱ��������, ǿ��ֵ������, ��Դ, ��Դ������ս��);
	}

	void SI_API Buff_TryChange_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
	{
		SIInterface_void(Buff_TryChange_B)(�ӿ�, ��Buff����, ��Buff����, Buff�������ð�, �������س���ʱ��������, ǿ��ֵ������, ��Դ, ��Դ������ս��, ��鵥λ���Բ�����);
	}

	bool SI_API Buff_HasBuffed_A(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface(Buff_HasBuffed_A)(�ӿ�);
	}

	bool SI_API Buff_HasBuffed_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����)
	{
		SIInterface(Buff_HasBuffed_B)(�ӿ�, Buff����);
	}

	bool SI_API Buff_HasBuffed_C(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIBuffClass_State Buff״̬)
	{
		SIInterface(Buff_HasBuffed_C)(�ӿ�, Buff����, Buff״̬);
	}

	bool SI_API Buff_HasBuffed_D(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, AbstractClass* �ų�����Դ)
	{
		SIInterface(Buff_HasBuffed_D)(�ӿ�, Buff����, �ų�����Դ);
	}

	bool SI_API Buff_HasBuffed_E(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, AbstractClass* �������Դ, bool û�õı�ʶ)
	{
		SIInterface(Buff_HasBuffed_E)(�ӿ�, Buff����, �������Դ, û�õı�ʶ);
	}

	bool SI_API Buff_HasBuffed_F(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass_EffectType Ч������)
	{
		SIInterface(Buff_HasBuffed_F)(�ӿ�, Ч������);
	}

	bool SI_API Buff_HasBuffed_G(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass_EffectType Ч������, SIBuffClass_State Buff״̬)
	{
		SIInterface(Buff_HasBuffed_G)(�ӿ�, Ч������, Buff״̬);
	}

	// �˴����������Ƴ���Ч�� Buff
	void SI_API Buff_LookUp(SIInterface_ExtendData* �ӿ�, void* ��Դ, void(*��������)(void* ��Դ, SIBuffClass* Buff))
	{
		SIInterface_void(Buff_LookUp)(�ӿ�, ��Դ, ��������);
	}

	void SI_API Buff_Update(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(Buff_Update)(�ӿ�);
	}

	bool SI_API Buff_TriggerFire(SIInterface_ExtendData* �ӿ�, AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������)
	{
		SIInterface(Buff_TriggerFire)(�ӿ�, Ŀ��, ��������, ��������, ���忪������, ��������);
	}

	double SI_API Buff_TriggerAttacker(SIInterface_ExtendData* �ӿ�, TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
	{
		SIInterface(Buff_TriggerAttacker)(�ӿ�, Ŀ�굥λ, �˺�����, ��ǰ�˺�);
	}

	double SI_API Buff_TriggerDefender(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
	{
		SIInterface(Buff_TriggerDefender)(�ӿ�, �˺�����, ��ǰ�˺�);
	}

	double SI_API Buff_TriggerDefender_Final(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
	{
		SIInterface(Buff_TriggerDefender_Final)(�ӿ�, �˺�����, ��ǰ�˺�);
	}

	void SI_API Buff_TriggerDeath(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����)
	{
		SIInterface_void(Buff_TriggerDeath)(�ӿ�, �˺�����);
	}

	void SI_API Buff_TriggerKiller(SIInterface_ExtendData* �ӿ�, TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����)
	{
		SIInterface_void(Buff_TriggerKiller)(�ӿ�, Ŀ�굥λ, �˺�����);
	}

	double SI_API Buff_TriggerSelf(SIInterface_ExtendData* �ӿ�, double ��ǰ�˺�)
	{
		SIInterface(Buff_TriggerSelf)(�ӿ�, ��ǰ�˺�);
	}

	void SI_API Buff_DoDamages(SIInterface_ExtendData* �ӿ�, double ׼����ɵ��˺�)
	{
		SIInterface_void(Buff_DoDamages)(�ӿ�, ׼����ɵ��˺�);
	}

	void SI_API Element_Merge_ResistanceType(SIInterface_ExtendData* �ӿ�, SIElementResistanceTypeClass* Ԫ�ؿ�������, int ��������)
	{
		SIInterface_void(Element_Merge_ResistanceType)(�ӿ�, Ԫ�ؿ�������, ��������);
	}

	void SI_API Element_Merge_ImmuneElementType(SIInterface_ExtendData* �ӿ�, SIElementTypeClass* Ԫ������, int ��������)
	{
		SIInterface_void(Element_Merge_ImmuneElementType)(�ӿ�, Ԫ������, ��������);
	}

	void SI_API Update(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(Update)(�ӿ�);
	}

	void SI_API UpdateBar_UpdateAllow(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(UpdateBar_UpdateAllow)(�ӿ�);
	}

	void SI_API UpdateDrawBar(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(UpdateDrawBar)(�ӿ�);
	}

	bool SI_API UpdateClickBar_Hover(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, Point2D* �������ƫ��)
	{
		SIInterface(UpdateClickBar_Hover)(�ӿ�, �������ս��, �������ƫ��);
	}

	void SI_API UpdateClickBar_TryClick(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, Point2D* �������ƫ��, int& Buff����, SIDigitalClickBarIndex& �������, double& ��ǰֵ, bool& ���ñ�ʶ)
	{
		SIInterface_void(UpdateClickBar_TryClick)(�ӿ�, �������ս��, �������ƫ��, Buff����, �������, ��ǰֵ, ���ñ�ʶ);
	}

	void SI_API UpdateClickBar_Effect(SIInterface_ExtendData* �ӿ�, int Buff����, SIDigitalClickBarIndex �������, HouseClass* �������ս��, double ��ǰֵ)
	{
		SIInterface_void(UpdateClickBar_Effect)(�ӿ�, Buff����, �������, �������ս��, ��ǰֵ);
	}

	bool SI_API UpdateKeyExecuteBar_TryExecute(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, SICommandCode ��ݼ�����)
	{
		SIInterface(UpdateKeyExecuteBar_TryExecute)(�ӿ�, �������ս��, ��ݼ�����);
	}

	void SI_API UpdateKeyExecuteBar_Effect(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, SICommandCode ��ݼ�����)
	{
		SIInterface_void(UpdateKeyExecuteBar_Effect)(�ӿ�, �������ս��, ��ݼ�����);
	}

	SIBuffClass* SI_API Buff_GetBuff_A(SIInterface_ExtendData* �ӿ�, int Buff����) // ����ֵ����Ϊ��
	{
		SIInterface(Buff_GetBuff_A)(�ӿ�, Buff����);
	}
	SIBuffClass* SI_API Buff_GetBuff_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����) // ����ֵ����Ϊ��
	{
		SIInterface(Buff_GetBuff_B)(�ӿ�, Buff����);
	}
	void SI_API Init(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(Init)(�ӿ�);
	}
	void SI_API UnInit(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(UnInit)(�ӿ�);
	}
	void SI_API UpdateTechnoType(SIInterface_ExtendData* �ӿ�, TechnoTypeClass* �ɵĵ�λ����)
	{
		SIInterface_void(UpdateTechnoType)(�ӿ�, �ɵĵ�λ����);
	}
	void SI_API UpdateOwningHouse(SIInterface_ExtendData* �ӿ�, const HouseClass* const �ɵĵ�λ������ս��, const HouseClass* const �µĵ�λ������ս��)
	{
		SIInterface_void(UpdateOwningHouse)(�ӿ�, �ɵĵ�λ������ս��, �µĵ�λ������ս��);
	}
	void SI_API AvailableEffect(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(AvailableEffect)(�ӿ�);
	}
	void SI_API UnavailableEffect(SIInterface_ExtendData* �ӿ�)
	{
		SIInterface_void(UnavailableEffect)(�ӿ�);
	}

}

namespace SIBroadcast
{
	void SI_API FreshBroadcastPower(SIBroadcastClass* �ӿ�, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIInterfaceDecl("WIC.Broadcast.FreshBroadcastPower", FreshBroadcastPower)(�ӿ�, �㲥ǿ��, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	void SI_API PostBroadcast(SIBroadcastClass* �ӿ�, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIInterfaceDecl("WIC.Broadcast.PostBroadcast", PostBroadcast)(�ӿ�, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	int SI_API GetBroadcastListenerCount(SIBroadcastClass* �ӿ�, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastListenerCount", GetBroadcastListenerCount)(�ӿ�, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	void SI_API AddListener(SIBroadcastClass* �ӿ�, SIBuffClass* Ҫ��ӵļ���Buff)
	{
		SIInterfaceDecl("WIC.Broadcast.AddListener", AddListener)(�ӿ�, Ҫ��ӵļ���Buff);
	}
	void SI_API RemoveListener(SIBroadcastClass* �ӿ�, SIBuffClass* Ҫ�Ƴ��ļ���Buff)
	{
		SIInterfaceDecl("WIC.Broadcast.RemoveListener", RemoveListener)(�ӿ�, Ҫ�Ƴ��ļ���Buff);
	}
	bool SI_API IsEmpty(SIBroadcastClass* �ӿ�)
	{
		SIInterfaceDecl("WIC.Broadcast.IsEmpty", IsEmpty)(�ӿ�);
	}
	SIConstVector<SIBroadcastClass*>& SI_API GetBroadcastArray()
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastArray", GetBroadcastArray)();
	}
	HouseClass* SI_API OwnerObject(SIBroadcastClass* �ӿ�)
	{
		SIInterfaceDecl("WIC.Broadcast.OwnerObject", OwnerObject)(�ӿ�);
	}
	double SI_API GetBroadcastPower(SIBroadcastClass* �ӿ�)
	{
		SIInterfaceDecl("WIC.Broadcast.GetBroadcastPower", GetBroadcastPower)(�ӿ�);
	}
	const SIDataList<SIBuffClass*>& SI_API GetListenerBuffList(SIBroadcastClass* �ӿ�)
	{
		SIInterfaceDecl("WIC.Broadcast.GetListenerBuffList", GetListenerBuffList)(�ӿ�);
	}
}


namespace SIHouseExt
{
	void SI_API Counter_AddOriginTechnoType(SIHouse_ExtendData* �ӿ�, TechnoTypeClass* ��λ����)
	{
		SIHouseExtDecl(Counter_AddOriginTechnoType)(�ӿ�, ��λ����);
	}
	void SI_API Counter_RemoveOriginTechnoType(SIHouse_ExtendData* �ӿ�, TechnoTypeClass* ��λ����)
	{
		SIHouseExtDecl(Counter_RemoveOriginTechnoType)(�ӿ�, ��λ����);
	}
	bool SI_API AlwaysShowCameo(const SIHouse_ExtendData* const �ӿ�, const TechnoTypeClass* const ��λ����, bool �������ڽ���ĵ�λ)
	{
		SIHouseExtDecl(AlwaysShowCameo)(�ӿ�, ��λ����, �������ڽ���ĵ�λ);
	}
	void SI_API Factory_CalculateProduction(SIHouse_ExtendData* �ӿ�)
	{
		SIHouseExtDecl(Factory_CalculateProduction)(�ӿ�);
	}
	void SI_API Factory_MergeProduction(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ)
	{
		SIHouseExtDecl(Factory_MergeProduction)(�ӿ�, ��λ);
	}
	void SI_API Factory_RemoveProduction(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ)
	{
		SIHouseExtDecl(Factory_RemoveProduction)(�ӿ�, ��λ);
	}
	void SI_API EXP_Init(SIHouse_ExtendData* �ӿ�)
	{
		SIHouseExtDecl(EXP_Init)(�ӿ�);
	}
	void SI_API EXP_Get(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, SIValues_HouseInitEXP* ���Ҿ���ֵ������)
	{
		SIHouseExtDecl(EXP_Get)(�ӿ�, ��λ, ���Ҿ���ֵ������);
	}
	bool SI_API EXP_MergeAcademy(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, TechnoTypeClass* ��λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	{
		SIHouseExtDecl(EXP_MergeAcademy)(�ӿ�, ��λ, ��λ����);
	}
	bool SI_API EXP_RemoveAcademy(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, TechnoTypeClass* ��λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	{
		SIHouseExtDecl(EXP_RemoveAcademy)(�ӿ�, ��λ, ��λ����);
	}
	bool SI_API EXP_UpdateSpyEffect(SIHouse_ExtendData* �ӿ�, BuildingClass* ������λ, BuildingTypeClass* ������λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	{
		SIHouseExtDecl(EXP_UpdateSpyEffect)(�ӿ�, ������λ, ������λ����);
	}
	void SI_API SetHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ, double ��ֵ)
	{
		SIHouseExtDecl(SetHouseVar)(�ӿ�, ����ֵ, ��ֵ);
	}
	double SI_API GetHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ)
	{
		SIHouseExtDecl(GetHouseVar)(�ӿ�, ����ֵ);
	}
	void SI_API DeleteHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ)
	{
		SIHouseExtDecl(DeleteHouseVar)(�ӿ�, ����ֵ);
	}
	SIBroadcastClass* SI_API Broadcast_FindOrAllocate(SIHouse_ExtendData* �ӿ�, int Ƶ��)// ����ֵ����Ϊ��
	{
		SIHouseExtDecl(Broadcast_FindOrAllocate)(�ӿ�, Ƶ��);
	}
	SIBroadcastClass* SI_API Broadcast_Find(SIHouse_ExtendData* �ӿ�, int Ƶ��)// ����ֵ����Ϊ��
	{
		SIHouseExtDecl(Broadcast_Find)(�ӿ�, Ƶ��);
	}
	void SI_API FreshBroadcastPower(SIHouse_ExtendData* �ӿ�, int Ƶ��, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIHouseExtDecl(FreshBroadcastPower)(�ӿ�, Ƶ��, �㲥ǿ��, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	void SI_API PostBroadcast(SIHouse_ExtendData* �ӿ�, int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIHouseExtDecl(PostBroadcast)(�ӿ�, Ƶ��, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	int SI_API GetBroadcastListenerCount(SIHouse_ExtendData* �ӿ�, int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
	{
		SIHouseExtDecl(GetBroadcastListenerCount)(�ӿ�, Ƶ��, ������������, ��������, ������λ, ָ��Ŀ��);
	}
	double SI_API GetBroadcastPower(SIHouse_ExtendData* �ӿ�, int Ƶ��)
	{
		SIHouseExtDecl(GetBroadcastPower)(�ӿ�, Ƶ��);
	}
	void SI_API AddBroadcastListener(SIHouse_ExtendData* �ӿ�, int Ƶ��, SIBuffClass* ����Buff)
	{
		SIHouseExtDecl(AddBroadcastListener)(�ӿ�, Ƶ��, ����Buff);
	}
	void SI_API RemoveBroadcastListener(SIHouse_ExtendData* �ӿ�, int Ƶ��, SIBuffClass* ����Buff)
	{
		SIHouseExtDecl(RemoveBroadcastListener)(�ӿ�, Ƶ��, ����Buff);
	}
	void SI_API EXPPool_Add(SIHouse_ExtendData* �ӿ�, double ���⾭��ֵ)
	{
		SIHouseExtDecl(EXPPool_Add)(�ӿ�, ���⾭��ֵ);
	}
	double SI_API EXPPool_Pop(SIHouse_ExtendData* �ӿ�, double ����Ķ��⾭��ֵ)
	{
		SIHouseExtDecl(EXPPool_Pop)(�ӿ�, ����Ķ��⾭��ֵ);
	}
	void SI_API ForEach_HouseVar(SIHouse_ExtendData* �ӿ�, void* ������, void (CALLBACK* ������)(void* ������, int ����, double& ֵ))
	{
		SIHouseExtDecl(ForEach_HouseVar)(�ӿ�, ������, ������);
	}
	void SI_API ForEach_Broadcast(SIHouse_ExtendData* �ӿ�, void* ������, void (CALLBACK* ������)(void* ������, int Ƶ��, SIBroadcastClass* ֵ))
	{
		SIHouseExtDecl(ForEach_Broadcast)(�ӿ�, ������, ������);
	}
	HouseClass* SI_API OwnerObject(SIHouse_ExtendData* �ӿ�)
	{
		SIHouseExtDecl(OwnerObject)(�ӿ�);
	}
}