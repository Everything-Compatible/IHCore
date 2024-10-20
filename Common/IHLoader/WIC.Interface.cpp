
#include "WIC.Interface.h"


SIException::SIException(const char* info) : Info(info) {};
const char* SIException::what() const noexcept
{
	return Info;
}

char* SIInterface_ExtendData::GetName() // ���� ID ����
{
	return SI::GetName(this);
}
 
AbstractClass* SIInterface_ExtendData::OwnerEntity() // ����ֵ����Ϊ��
{
	return SI::OwnerEntity(this);
}

AbstractType SIInterface_ExtendData::WhatAmI()
{
	return SI::WhatAmI(this);
}

int  SIInterface_ExtendData::GetArrayIndex()
{
	return SI::GetArrayIndex(this);
}

bool SIInterface_ExtendData::IsSelected()
{
	return SI::IsSelected(this);
}

void SIInterface_ExtendData::GetCoords(CoordStruct* ��������)
{
	SI::GetCoords(this, ��������);
}

void SIInterface_ExtendData::SetForceFireWeapon(WeaponTypeClass* ��������)
{
	SI::SetForceFireWeapon(this, ��������);
}

void SIInterface_ExtendData::SetForceDeathWeapon(WeaponTypeClass* ��������)
{
	SI::SetForceDeathWeapon(this, ��������);
}

CoordStruct SIInterface_ExtendData::Calculate_FireCoord(CoordStruct ��������, double ���ű���)
{
	return SI::Calculate_FireCoord(this, ��������, ���ű���);
}

int  SIInterface_ExtendData::GetStaticEXPLevel()
{
	return SI::IsSelected(this);
}

double  SIInterface_ExtendData::GetLevelTotalEXPCost(double �̶�����ֵ����, bool ʹ���¾���ϵͳ)
{
	return SI::IsSelected(this);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_Speed(double ԭʼ�ƶ��ٶ�)
{
	return SI::Calculate_PropertyGroup_Speed(this, ԭʼ�ƶ��ٶ�);
}

double  SIInterface_ExtendData::Calculate_PropertyGroup_Defence(double ԭʼ�˺�)
{
	return SI::Calculate_PropertyGroup_Defence(this, ԭʼ�˺�);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_Attack(double ԭʼ�˺�)
{
	return SI::Calculate_PropertyGroup_Attack(this, ԭʼ�˺�);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_ROF(double ԭʼ����)
{
	return SI::Calculate_PropertyGroup_ROF(this, ԭʼ����);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXP(double ԭʼ��õľ���ֵ)
{
	return SI::Calculate_PropertyGroup_EXP(this, ԭʼ��õľ���ֵ);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXPProvide(double ԭʼ�ṩ�ľ���ֵ)
{
	return SI::Calculate_PropertyGroup_EXPProvide(this, ԭʼ�ṩ�ľ���ֵ);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXPCost(double ��λ��������ľ���ֵ)
{
	return SI::Calculate_PropertyGroup_EXPCost(this, ��λ��������ľ���ֵ);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_RangeWeapon(double ԭʼ�������)
{
	return SI::Calculate_PropertyGroup_RangeWeapon(this, ԭʼ�������);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_CellSpread(double ԭʼ��ͷ���䷶Χ)
{
	return SI::Calculate_PropertyGroup_CellSpread(this, ԭʼ��ͷ���䷶Χ);
}

int SIInterface_ExtendData::GetTintColor(bool ��ĻȾɫ, bool ��ϮȾɫ, bool ��Ⱦɫ)
{
	return SI::GetTintColor(this, ��ĻȾɫ, ��ϮȾɫ, ��Ⱦɫ);
}

void SIInterface_ExtendData::Calculate_CustomTintValues(int& Ⱦɫ��ɫ, int& Ⱦɫǿ��)
{
	return SI::Calculate_CustomTintValues(this, Ⱦɫ��ɫ, Ⱦɫǿ��);
}

int SIInterface_ExtendData::Calculate_CustomTintValues_Color()
{
	return SI::Calculate_CustomTintValues_Color(this);
}

int SIInterface_ExtendData::Calculate_CustomTintValues_Intensity()
{
	return SI::Calculate_CustomTintValues_Intensity(this);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Speed(double �µ�������)
{
	return SI::Property_Merge_Multiplier_Speed(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Armor(double �µ�������)
{
	return SI::Property_Merge_Multiplier_Armor(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Versus(double �µ�������)
{
	return SI::Property_Merge_Multiplier_Versus(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Attack(double �µ�������)
{
	return SI::Property_Merge_Multiplier_Attack(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_ROF(double �µ�������)
{
	return SI::Property_Merge_Multiplier_ROF(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXP(double �µ�������)
{
	return SI::Property_Merge_Multiplier_EXP(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXPProvide(double �µ�������)
{
	return SI::Property_Merge_Multiplier_EXPProvide(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXPCost(double �µ�������)
{
	return SI::Property_Merge_Multiplier_EXPCost(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_RangeWeapon(double �µ�������)
{
	return SI::Property_Merge_Multiplier_RangeWeapon(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_CellSpread(double �µ�������)
{
	return SI::Property_Merge_Multiplier_CellSpread(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Speed(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_Speed(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Armor(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_Armor(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Versus(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_Versus(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Attack(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_Attack(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_ROF(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_ROF(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXP(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_EXP(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXPProvide(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_EXPProvide(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXPCost(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_EXPCost(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_RangeWeapon(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_RangeWeapon(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_CellSpread(double �µ�������)
{
	return SI::Property_Merge_ExtraMultiplier_CellSpread(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_Speed(double �µ�������)
{
	return SI::Property_Merge_Addon_Speed(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_Damage(double �µ�������)
{
	return SI::Property_Merge_Addon_Damage(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_Attack(double �µ�������)
{
	return SI::Property_Merge_Addon_Attack(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_ROF(double �µ�������)
{
	return SI::Property_Merge_Addon_ROF(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXP(double �µ�������)
{
	return SI::Property_Merge_Addon_EXP(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXPProvide(double �µ�������)
{
	return SI::Property_Merge_Addon_EXPProvide(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXPCost(double �µ�������)
{
	return SI::Property_Merge_Addon_EXPCost(this, 0.0, �µ�������);
}

// ��λ�Ǹ��
double SIInterface_ExtendData::Property_Merge_Addon_RangeWeapon(double �µ�������)
{
	return SI::Property_Merge_Addon_RangeWeapon(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_Addon_CellSpread(double �µ�������) // ��λ�Ǹ���
{
	return SI::Property_Merge_Addon_CellSpread(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Speed(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_Speed(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Damage(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_Damage(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Attack(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_Attack(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_ROF(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_ROF(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXP(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_EXP(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXPProvide(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_EXPProvide(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXPCost(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_EXPCost(this, 0.0, �µ�������);
}

// ��λ�Ǹ��
double SIInterface_ExtendData::Property_Merge_FinalAddon_RangeWeapon(double �µ�������)
{
	return SI::Property_Merge_FinalAddon_RangeWeapon(this, 0.0, �µ�������);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_CellSpread(double �µ�������) // ��λ�Ǹ���
{
	return SI::Property_Merge_FinalAddon_CellSpread(this, 0.0, �µ�������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Speed(double ������)
{
	return SI::Property_Register_Multiplier_Speed(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Armor(double ������)
{
	return SI::Property_Register_Multiplier_Armor(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Versus(double ������)
{
	return SI::Property_Register_Multiplier_Versus(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Attack(double ������)
{
	return SI::Property_Register_Multiplier_Attack(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_ROF(double ������)
{
	return SI::Property_Register_Multiplier_ROF(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXP(double ������)
{
	return SI::Property_Register_Multiplier_EXP(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXPProvide(double ������)
{
	return SI::Property_Register_Multiplier_EXPProvide(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXPCost(double ������)
{
	return SI::Property_Register_Multiplier_EXPCost(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_RangeWeapon(double ������)
{
	return SI::Property_Register_Multiplier_RangeWeapon(this, ������);
}

int SIInterface_ExtendData::Property_Register_Multiplier_CellSpread(double ������)
{
	return SI::Property_Register_Multiplier_CellSpread(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Speed(double ������)
{
	return SI::Property_Register_ExtraMultiplier_Speed(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Armor(double ������)
{
	return SI::Property_Register_ExtraMultiplier_Armor(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Versus(double ������)
{
	return SI::Property_Register_ExtraMultiplier_Versus(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Attack(double ������)
{
	return SI::Property_Register_ExtraMultiplier_Attack(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_ROF(double ������)
{
	return SI::Property_Register_ExtraMultiplier_ROF(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXP(double ������)
{
	return SI::Property_Register_ExtraMultiplier_EXP(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXPProvide(double ������)
{
	return SI::Property_Register_ExtraMultiplier_EXPProvide(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXPCost(double ������)
{
	return SI::Property_Register_ExtraMultiplier_EXPCost(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_RangeWeapon(double ������)
{
	return SI::Property_Register_ExtraMultiplier_RangeWeapon(this, ������);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_CellSpread(double ������)
{
	return SI::Property_Register_ExtraMultiplier_CellSpread(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_Speed(double ������)
{
	return SI::Property_Register_Addon_Speed(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_Damage(double ������)
{
	return SI::Property_Register_Addon_Damage(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_Attack(double ������)
{
	return SI::Property_Register_Addon_Attack(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_ROF(double ������)
{
	return SI::Property_Register_Addon_ROF(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_EXP(double ������)
{
	return SI::Property_Register_Addon_EXP(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_EXPProvide(double ������)
{
	return SI::Property_Register_Addon_EXPProvide(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_EXPCost(double ������)
{
	return SI::Property_Register_Addon_EXPCost(this, ������);
}

// ��λ�Ǹ��
int SIInterface_ExtendData::Property_Register_Addon_RangeWeapon(double ������)
{
	return SI::Property_Register_Addon_RangeWeapon(this, ������);
}

int SIInterface_ExtendData::Property_Register_Addon_CellSpread(double ������)
{
	return SI::Property_Register_Addon_CellSpread(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Speed(double ������)
{
	return SI::Property_Register_FinalAddon_Speed(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Damage(double ������)
{
	return SI::Property_Register_FinalAddon_Damage(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Attack(double ������)
{
	return SI::Property_Register_FinalAddon_Attack(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_ROF(double ������)
{
	return SI::Property_Register_FinalAddon_ROF(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXP(double ������)
{
	return SI::Property_Register_FinalAddon_EXP(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXPProvide(double ������)
{
	return SI::Property_Register_FinalAddon_EXPProvide(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXPCost(double ������)
{
	return SI::Property_Register_FinalAddon_EXPCost(this, ������);
}

// ��λ�Ǹ��
int SIInterface_ExtendData::Property_Register_FinalAddon_RangeWeapon(double ������)
{
	return SI::Property_Register_FinalAddon_RangeWeapon(this, ������);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_CellSpread(double ������) // ��λ�Ǹ���
{
	return SI::Property_Register_FinalAddon_CellSpread(this, ������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Speed(int ע������)
{
	SI::Property_Unregister_Multiplier_Speed(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Armor(int ע������)
{
	SI::Property_Unregister_Multiplier_Armor(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Versus(int ע������)
{
	SI::Property_Unregister_Multiplier_Versus(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Attack(int ע������)
{
	SI::Property_Unregister_Multiplier_Attack(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_ROF(int ע������)
{
	SI::Property_Unregister_Multiplier_ROF(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXP(int ע������)
{
	SI::Property_Unregister_Multiplier_EXP(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXPProvide(int ע������)
{
	SI::Property_Unregister_Multiplier_EXPProvide(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXPCost(int ע������)
{
	SI::Property_Unregister_Multiplier_EXPCost(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_RangeWeapon(int ע������)
{
	SI::Property_Unregister_Multiplier_RangeWeapon(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_CellSpread(int ע������)
{
	SI::Property_Unregister_Multiplier_CellSpread(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Speed(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_Speed(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Armor(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_Armor(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Versus(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_Versus(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Attack(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_Attack(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_ROF(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_ROF(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXP(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_EXP(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXPProvide(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_EXPProvide(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXPCost(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_EXPCost(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_RangeWeapon(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_RangeWeapon(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_CellSpread(int ע������)
{
	SI::Property_Unregister_ExtraMultiplier_CellSpread(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Speed(int ע������)
{
	SI::Property_Unregister_Addon_Speed(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Damage(int ע������)
{
	SI::Property_Unregister_Addon_Damage(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Attack(int ע������)
{
	SI::Property_Unregister_Addon_Attack(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_ROF(int ע������)
{
	SI::Property_Unregister_Addon_ROF(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXP(int ע������)
{
	SI::Property_Unregister_Addon_EXP(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXPProvide(int ע������)
{
	SI::Property_Unregister_Addon_EXPProvide(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXPCost(int ע������)
{
	SI::Property_Unregister_Addon_EXPCost(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_RangeWeapon(int ע������)
{
	SI::Property_Unregister_Addon_RangeWeapon(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_Addon_CellSpread(int ע������)
{
	SI::Property_Unregister_Addon_CellSpread(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Speed(int ע������)
{
	SI::Property_Unregister_FinalAddon_Speed(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Damage(int ע������)
{
	SI::Property_Unregister_FinalAddon_Damage(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Attack(int ע������)
{
	SI::Property_Unregister_FinalAddon_Attack(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_ROF(int ע������)
{
	SI::Property_Unregister_FinalAddon_ROF(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXP(int ע������)
{
	SI::Property_Unregister_FinalAddon_EXP(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXPProvide(int ע������)
{
	SI::Property_Unregister_FinalAddon_EXPProvide(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXPCost(int ע������)
{
	SI::Property_Unregister_FinalAddon_EXPCost(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_RangeWeapon(int ע������)
{
	SI::Property_Unregister_FinalAddon_RangeWeapon(this, ע������);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_CellSpread(int ע������)
{
	SI::Property_Unregister_FinalAddon_CellSpread(this, ע������);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneCrush(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneCrush(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneCrushAdvance(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneCrushAdvance(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneRadiation(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneRadiation(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePsionicDamage(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmunePsionicDamage(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePsionic(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmunePsionic(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneBerzerk(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneBerzerk(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePoison(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmunePoison(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneWarp(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneWarp(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneEMP(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_ImmuneEMP(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_SelfHealing(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Percent(double �µ�����ǿ��)
{
	SI::Property_Merge_Ability_SelfHealing_Percent(this, 0.0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Cap(double �µ�����ǿ��)
{
	SI::Property_Merge_Ability_SelfHealing_Cap(this, 0.0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Delay(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_SelfHealing_Delay(this, 0, �µ�����ǿ��);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_CombatDelay(int �µ�����ǿ��)
{
	SI::Property_Merge_Ability_SelfHealing_CombatDelay(this, 0, �µ�����ǿ��);
}

int SIInterface_ExtendData::Property_Register_CustomTintValues(ColorStruct Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������)
{
	return SI::Property_Register_CustomTintValues_A(this, Ⱦɫ��ɫ, Ⱦɫǿ��, ��ս������);
}

int SIInterface_ExtendData::Property_Register_CustomTintValues(int Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������)
{
	return SI::Property_Register_CustomTintValues_B(this, Ⱦɫ��ɫ, Ⱦɫǿ��, ��ս������);
}

void SIInterface_ExtendData::Property_Unregister_CustomTintValues(int ע������)
{
	SI::Property_Unregister_CustomTintValues(this, ע������);
}

// �۳�����ʹ�ø���
void SIInterface_ExtendData::EXP_Add(const double �ṩ�ľ���ֵ, SIEXPSourceType ����ֵ��Դ, SIValues_EXPMerge* �ϲ�����ֵ������)
{
	SI::EXP_Add(this, �ṩ�ľ���ֵ, ����ֵ��Դ, �ϲ�����ֵ������);
}

bool SIInterface_ExtendData::Buff_IsPowerOffline()
{
	return SI::Buff_IsPowerOffline(this);
}

bool SIInterface_ExtendData::Buff_IsNeedHide()
{
	return SI::Buff_IsNeedHide(this);
}

int SIInterface_ExtendData::Buff_GetEffectDuration(SIBuffTypeClass* Buff����, int ԭʼ�Ĺ��س���ʱ��)
{
	return SI::Buff_GetEffectDuration(this, Buff����, ԭʼ�Ĺ��س���ʱ��);
}

double SIInterface_ExtendData::Buff_GetEffectHealth(SIBuffTypeClass* Buff����, double ԭʼ������ֵ)
{
	return SI::Buff_GetEffectHealth(this, Buff����, ԭʼ������ֵ);
}

double SIInterface_ExtendData::Buff_GetEffectPower(SIBuffTypeClass* Buff����, double ԭʼ��ǿ��ֵ)
{
	return SI::Buff_GetEffectPower(this, Buff����, ԭʼ��ǿ��ֵ);
}

SIBuffClass* SIInterface_ExtendData::Buff_CreateOrMerge(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, AbstractClass* ��Դ, HouseClass* ��Դ������ս��)
{
	return SI::Buff_CreateOrMerge(this, Buff����, Buff�������ð�, ��Դ, ��Դ������ս��);
}

void SIInterface_ExtendData::Buff_TryActive(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�)
{
	SI::Buff_TryActive_A(this, Buff����, Buff�������ð�);
}

void SIInterface_ExtendData::Buff_TryActive(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
{
	SI::Buff_TryActive_B(this, Buff����, Buff�������ð�, ��鵥λ���Բ�����);
}

// [�жϴ�����] ���� true ��� Buff ���д���
void SIInterface_ExtendData::Buff_TryActive(void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�))
{
	SI::Buff_TryActive_C(this, ��Դ, �жϴ�����);
}

void SIInterface_ExtendData::Buff_TryAfter(SIBuffTypeClass* Buff����)
{
	SI::Buff_TryAfter_A(this, Buff����);
}

void SIInterface_ExtendData::Buff_TryAfter(SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
{
	SI::Buff_TryAfter_B(this, Buff����, ��鵥λ���Բ�����);
}

// [�жϴ�����] ���� true ��� Buff ���д���
void SIInterface_ExtendData::Buff_TryAfter(void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff))
{
	SI::Buff_TryAfter_C(this, ��Դ, �жϴ�����);
}

void SIInterface_ExtendData::Buff_TryRemove(SIBuffTypeClass* Buff����)
{
	SI::Buff_TryRemove_A(this, Buff����);
}

void SIInterface_ExtendData::Buff_TryRemove(SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
{
	SI::Buff_TryRemove_B(this, Buff����, ��鵥λ���Բ�����);
}

// [�жϴ�����] ���� true ��� Buff ���д���
void SIInterface_ExtendData::Buff_TryRemove(void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff))
{
	SI::Buff_TryRemove_C(this, ��Դ, �жϴ�����);
}

void SIInterface_ExtendData::Buff_TryMergeSetting(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�)
{
	SI::Buff_TryMergeSetting_A(this, Buff����, Buff�������ð�);
}

void SIInterface_ExtendData::Buff_TryMergeSetting(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
{
	SI::Buff_TryMergeSetting_B(this, Buff����, Buff�������ð�, ��鵥λ���Բ�����);
}

// [�жϴ�����] ���� true ��� Buff ���д���
void SIInterface_ExtendData::Buff_TryMergeSetting(void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�))
{
	SI::Buff_TryMergeSetting_C(this, ��Դ, �жϴ�����);
}

void SIInterface_ExtendData::Buff_TryChange(SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��)
{
	SI::Buff_TryChange_A(this, ��Buff����, ��Buff����, Buff�������ð�, �������س���ʱ��������, ǿ��ֵ������, ��Դ, ��Դ������ս��);
}

void SIInterface_ExtendData::Buff_TryChange(SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����)
{
	SI::Buff_TryChange_B(this, ��Buff����, ��Buff����, Buff�������ð�, �������س���ʱ��������, ǿ��ֵ������, ��Դ, ��Դ������ս��, ��鵥λ���Բ�����);
}

bool SIInterface_ExtendData::Buff_HasBuffed()
{
	return SI::Buff_HasBuffed_A(this);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff����)
{
	return SI::Buff_HasBuffed_B(this, Buff����);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff����, SIBuffClass_State Buff״̬)
{
	return SI::Buff_HasBuffed_C(this, Buff����, Buff״̬);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff����, AbstractClass* �ų�����Դ)
{
	return SI::Buff_HasBuffed_D(this, Buff����, �ų�����Դ);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff����, AbstractClass* �������Դ, bool û�õı�ʶ)
{
	return SI::Buff_HasBuffed_E(this, Buff����, �������Դ, û�õı�ʶ);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass_EffectType Ч������)
{
	return SI::Buff_HasBuffed_F(this, Ч������);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass_EffectType Ч������, SIBuffClass_State Buff״̬)
{
	return SI::Buff_HasBuffed_G(this, Ч������, Buff״̬);
}

// �˴����������Ƴ���Ч�� Buff
void SIInterface_ExtendData::Buff_LookUp(void* ��Դ, void(*��������)(void* ��Դ, SIBuffClass* Buff))
{
	SI::Buff_LookUp(this, ��Դ, ��������);
}

void SIInterface_ExtendData::Buff_Update()
{
	SI::Buff_Update(this);
}

bool SIInterface_ExtendData::Buff_TriggerFire(AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������)
{
	return SI::Buff_TriggerFire(this, Ŀ��, ��������, ��������, ���忪������, ��������);
}

double SIInterface_ExtendData::Buff_TriggerAttacker(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	return SI::Buff_TriggerAttacker(this, Ŀ�굥λ, �˺�����, ��ǰ�˺�);
}

double SIInterface_ExtendData::Buff_TriggerDefender(args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	return SI::Buff_TriggerDefender(this, �˺�����, ��ǰ�˺�);
}

double SIInterface_ExtendData::Buff_TriggerDefender_Final(args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	return SI::Buff_TriggerDefender_Final(this, �˺�����, ��ǰ�˺�);
}

void SIInterface_ExtendData::Buff_TriggerDeath(args_ReceiveDamage* �˺�����)
{
	SI::Buff_TriggerDeath(this, �˺�����);
}

void SIInterface_ExtendData::Buff_TriggerKiller(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����)
{
	SI::Buff_TriggerKiller(this, Ŀ�굥λ, �˺�����);
}

double SIInterface_ExtendData::Buff_TriggerSelf(double ��ǰ�˺�)
{
	return SI::Buff_TriggerSelf(this, ��ǰ�˺�);
}

void SIInterface_ExtendData::Buff_DoDamages(double ׼����ɵ��˺�)
{
	SI::Buff_DoDamages(this, ׼����ɵ��˺�);
}

SIBuffClass* SIInterface_ExtendData::Buff_GetBuff(int Buff����) // ����ֵ����Ϊ��
{
	return SI::Buff_GetBuff_A(this, Buff����);
}
SIBuffClass* SIInterface_ExtendData::Buff_GetBuff(SIBuffTypeClass* Buff����) // ����ֵ����Ϊ��
{
	return SI::Buff_GetBuff_B(this, Buff����);
}

void SIInterface_ExtendData::AvailableEffect()
{
	return SI::AvailableEffect(this);
}

void SIInterface_ExtendData::UnavailableEffect()
{
	return SI::UnavailableEffect(this);
}

void SIInterface_ExtendData::Element_Merge_ResistanceType(SIElementResistanceTypeClass* Ԫ�ؿ�������, int ��������)
{
	SI::Element_Merge_ResistanceType(this, Ԫ�ؿ�������, ��������);
}

void SIInterface_ExtendData::Element_Merge_ImmuneElementType(SIElementTypeClass* Ԫ������, int ��������)
{
	SI::Element_Merge_ImmuneElementType(this, Ԫ������, ��������);
}

void SIInterface_ExtendData::Update()
{
	SI::Update(this);
}

void SIInterface_ExtendData::UpdateBar_UpdateAllow()
{
	SI::UpdateBar_UpdateAllow(this);
}

void SIInterface_ExtendData::UpdateDrawBar()
{
	SI::UpdateDrawBar(this);
}

bool SIInterface_ExtendData::UpdateClickBar_Hover(HouseClass* �������ս��, Point2D* �������ƫ��)
{
	return SI::UpdateClickBar_Hover(this, �������ս��, �������ƫ��);
}

void SIInterface_ExtendData::UpdateClickBar_TryClick(HouseClass* �������ս��, Point2D* �������ƫ��, int& Buff����, SIDigitalClickBarIndex& �������, double& ��ǰֵ, bool& ���ñ�ʶ)
{
	SI::UpdateClickBar_TryClick(this, �������ս��, �������ƫ��, Buff����, �������, ��ǰֵ, ���ñ�ʶ);
}

void SIInterface_ExtendData::UpdateClickBar_Effect(int Buff����, SIDigitalClickBarIndex �������, HouseClass* �������ս��, double ��ǰֵ)
{
	SI::UpdateClickBar_Effect(this, Buff����, �������, �������ս��, ��ǰֵ);
}

bool SIInterface_ExtendData::UpdateKeyExecuteBar_TryExecute(HouseClass* �������ս��, SICommandCode ��ݼ�����)
{
	return SI::UpdateKeyExecuteBar_TryExecute(this, �������ս��, ��ݼ�����);
}

void SIInterface_ExtendData::UpdateKeyExecuteBar_Effect(HouseClass* �������ս��, SICommandCode ��ݼ�����)
{
	SI::UpdateKeyExecuteBar_Effect(this, �������ս��, ��ݼ�����);
}

void SIInterface_ExtendData::Init()
{
	SI::Init(this);
}

void SIInterface_ExtendData::UnInit()
{
	SI::Init(this);
}

//WIC �����߼�
//����ʹ���������������WIC˵����
void SIInterface_ExtendData::UpdateTechnoType(TechnoTypeClass* �ɵĵ�λ����)
{
	SI::UpdateTechnoType(this, �ɵĵ�λ����);
}

void SIInterface_ExtendData::UpdateOwningHouse(const HouseClass* const �ɵĵ�λ������ս��, const HouseClass* const �µĵ�λ������ս��)
{
	SI::UpdateOwningHouse(this, �ɵĵ�λ������ս��, �µĵ�λ������ս��);
}

Ext::LibData WIC(nullptr);
namespace SI
{
	
	/*
	��ע���˴���Sidoupiarȷ�����ȶ����ڵ�API������ʽ������ֱ��ͨ��ֻ����С�޶ȼ���LibData::QueryFunctionʵ��
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

	SIInterface_ExtendData* SI_API GetInterface(const AbstractClass* const ʵ��)
	{
		SIInterface(GetInterface)(ʵ��);
	}

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
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool( *�жϴ�����)( void* ��Դ, SIBuffClass* Buff) )
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
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff) )
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
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool(*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�) )
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

namespace SITool
{
	TechnoClass* SI_API NormalConvert_TechnoTo(TechnoClass* ��Դ��λ, TechnoTypeClass* Ŀ�굥λ����, HouseClass* Ŀ����ս��, BulletClass* �ӵ�, bool Ҫ���ڵ�ͼ�Ͻ���, bool �����������)
	{
		SIInterfaceDecl("WIC.Tool.NormalConvert_TechnoTo", NormalConvert_TechnoTo)(��Դ��λ, Ŀ�굥λ����, Ŀ����ս��, �ӵ�, Ҫ���ڵ�ͼ�Ͻ���, �����������);
	}
	TechnoClass* SI_API SuperConvert_TechnoTo(TechnoClass* ��Դ��λ, TechnoTypeClass* Ŀ�굥λ����, HouseClass* Ŀ����ս��, BulletClass* �ӵ�, bool Ҫ���ڵ�ͼ�Ͻ���, bool �����������)
	{
		SIInterfaceDecl("WIC.Tool.SuperConvert_TechnoTo", SuperConvert_TechnoTo)(��Դ��λ, Ŀ�굥λ����, Ŀ����ս��, �ӵ�, Ҫ���ڵ�ͼ�Ͻ���, �����������);
	}
	void SI_API AddFlyingString_A(const CoordStruct ����, Point2D ����ƫ��, int ����ʱ��, double �����ٶ�, ColorStruct ��ɫ, const wchar_t* ��ʾ����)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_A", AddFlyingString_A)(����, ����ƫ��, ����ʱ��, �����ٶ�, ��ɫ, ��ʾ����);
	}
	void SI_API AddFlyingString_B(const CoordStruct ����, Point2D ����ƫ��, int ����ʱ��, double �����ٶ�, ConvertClass* ɫ��, SHPStruct* ͼ��, const int* �ַ������б�, int �ַ���������)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_B", AddFlyingString_B)(����, ����ƫ��, ����ʱ��, �����ٶ�, ɫ��, ͼ��, �ַ������б�, �ַ���������);
	}
	void SI_API AddFlyingString_Money(CoordStruct ����, int ���, HouseClass* ������ս��, int ��ս������)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_Money", AddFlyingString_Money)(����, ���, ������ս��, ��ս������);
	}

	bool Available()
	{
		static bool Check{ false };
		if (Check)return true;
		auto wic = Ext::GetLib(WIC_LibName);
		if (wic.Available())Check = true;
		return wic.Available();
	}

	void SI_API ForEach(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach)(������, ������);
	}
	void SI_API ForEach_Cell(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Cell)(������, ������);
	}
	void SI_API ForEach_Object(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Object)(������, ������);
	}
	void SI_API ForEach_Techno(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Techno)(������, ������);
	}
	void SI_API ForEach_Animation(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach)(������, ������);
	}
	void SI_API ForEach_Bullet(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Bullet)(������, ������);
	}
	void SI_API ForEach_Terrain(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Terrain)(������, ������);
	}
	void SI_API ForEach_Overlay(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Overlay)(������, ������);
	}
	void SI_API ForEach_House(void* ������, bool (*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_House)(������, ������);
	}
}


namespace SIClassManager
{
	//ͨ��AbstractClass*�ҵ���Ӧ��SIInterface_ExtendData*�Խ��и���Ĳ�����
	//����Ӧ������SIInterface_ExtendData���׻���󲻴���/���Ϸ����򷵻�nullptr��
	SIInterface_ExtendData* SI_API GetExtendData(const AbstractClass* const ʵ��)
	{
		return SI::GetInterface(ʵ��);
	}
	//��ȡWIC�ڲ�����Ľӿ�
	//���º����ķ���ֵ������Ϊ�գ������м��
	SIBuffTypeClass* SI_API BuffType_Find(const char* ����)
	{
		SIInterfaceDecl("WIC.BuffType.Find", BuffType_Find)(����);
	}
	SIBuffTypeClass* SI_API BuffType_FindOrAllocate(const char* ����)
	{
		SIInterfaceDecl("WIC.BuffType.FindOrAllocate", BuffType_FindOrAllocate)(����);
	}
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_Find(const char* ����)
	{
		SIInterfaceDecl("WIC.PBuffSetting.Find", BuffSetting_Find)(����);
	}
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_FindOrAllocate(const char* ����)
	{
		SIInterfaceDecl("WIC.PBuffSetting.FindOrAllocate", BuffSetting_FindOrAllocate)(����);
	}
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_Find(const char* ����)
	{
		SIInterfaceDecl("WIC.PCheckTechno.Find", CheckTechno_Find)(����);
	}
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_FindOrAllocate(const char* ����)
	{
		SIInterfaceDecl("WIC.PCheckTechno.FindOrAllocate", CheckTechno_Find)(����);
	}
	SIElementTypeClass* SI_API ElementType_Find(const char* ����)
	{
		SIInterfaceDecl("WIC.ElementType.Find", ElementType_Find)(����);
	}
	SIElementTypeClass* SI_API ElementType_FindOrAllocate(const char* ����)
	{
		SIInterfaceDecl("WIC.ElementType.Find", ElementType_FindOrAllocate)(����);
	}
	SIElementResistanceTypeClass* SI_API ElementResistanceType_Find(const char* ����)
	{
		SIInterfaceDecl("WIC.ElementResistanceType.Find", ElementResistanceType_Find)(����);
	}
	SIElementResistanceTypeClass* SI_API ElementResistanceType_FindOrAllocate(const char* ����)
	{
		SIInterfaceDecl("WIC.ElementResistanceType.Find", ElementResistanceType_FindOrAllocate)(����);
	}

	// ��Ҫ������ INI ֮ǰ�����ע�� , ����ֵΪЧ������ ID , ����ֵΪ -1 ��ʾע��ʧ��
	//ע�᷽����
	//���÷��Ĳ��Ӧ�������а���WIC
	//���磺InitDependency Dependency{"SIWinterIsComing",DoNotCheckVersion,WIC_LIBRARY_VERSION,true};
	//��������֮������OrderedInit�����������ע��
	int SI_API RegisterEffectType(const char* Ч����������, long ���ָ��)
	{
		SIInterfaceDecl("WIC.BuffType.RegisterEffectType", RegisterEffectType)(Ч����������, ���ָ��);
	}

	// ��Buff�������׵Ĵ������
	SIPack_BuffSetting_FromStatic SI_API MakePack(SIPackTypeClass_BuffSetting* ����, HouseClass* ��ǰ��ս��)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack", MakePack)(����, ��ǰ��ս��);
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_Empty()
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_Empty", MakePack_Empty)();
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_DurationOnly(int ����ʱ��, bool ���ǳ���ʱ��, bool ���ܳ���ʱ�俹��)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_DurationOnly", MakePack_DurationOnly)(����ʱ��, ���ǳ���ʱ��, ���ܳ���ʱ�俹��);
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_PowerOnly(double ǿ��ֵ, bool ����ǿ��ֵ, bool ����ǿ��ֵ����)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_PowerOnly", MakePack_PowerOnly)(ǿ��ֵ, ����ǿ��ֵ, ����ǿ��ֵ����);
	}
	
	
}