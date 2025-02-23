
#include "WIC.Interface.h"
#include "WIC.Template.h"

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

const SIConstVector<SIBroadcastClass*>& SIBroadcastClass::GetArray()
{
	return SIBroadcast::GetBroadcastArray();
}
// ���ܺ���
void SIBroadcastClass::FreshBroadcastPower(double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	SIBroadcast::FreshBroadcastPower(this, �㲥ǿ��, ������������, ��������, ������λ, ָ��Ŀ��);
}
void SIBroadcastClass::PostBroadcast(bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	SIBroadcast::PostBroadcast(this, ������������, ��������, ������λ, ָ��Ŀ��);
}
int SIBroadcastClass::GetBroadcastListenerCount(bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	return SIBroadcast::GetBroadcastListenerCount(this, ������������, ��������, ������λ, ָ��Ŀ��);
}
void SIBroadcastClass::AddListener(SIBuffClass* Ҫ��ӵļ���Buff)
{
	SIBroadcast::AddListener(this, Ҫ��ӵļ���Buff);
}
void SIBroadcastClass::RemoveListener(SIBuffClass* Ҫ�Ƴ��ļ���Buff)
{
	SIBroadcast::RemoveListener(this, Ҫ�Ƴ��ļ���Buff);
}
bool SIBroadcastClass::IsEmpty()
{
	return SIBroadcast::IsEmpty(this);
}
HouseClass* SIBroadcastClass::OwnerObject()
{
	return SIBroadcast::OwnerObject(this);
}
double SIBroadcastClass::GetBroadcastPower()
{
	return SIBroadcast::GetBroadcastPower(this);
}
const SIDataList<SIBuffClass*>& SIBroadcastClass::GetListenerBuffList()
{
	return SIBroadcast::GetListenerBuffList(this);
}


void SIHouse_ExtendData::Counter_AddOriginTechnoType(TechnoTypeClass* ��λ����)
{
	SIHouseExt::Counter_AddOriginTechnoType(this, ��λ����);
}
void SIHouse_ExtendData::Counter_RemoveOriginTechnoType(TechnoTypeClass* ��λ����)
{
	SIHouseExt::Counter_RemoveOriginTechnoType(this, ��λ����);
}
bool SIHouse_ExtendData::AlwaysShowCameo(const TechnoTypeClass* const ��λ����, bool �������ڽ���ĵ�λ) const
{
	return SIHouseExt::AlwaysShowCameo(this, ��λ����, �������ڽ���ĵ�λ);
}
void SIHouse_ExtendData::Factory_CalculateProduction()
{
	SIHouseExt::Factory_CalculateProduction(this);
}
void SIHouse_ExtendData::Factory_MergeProduction(TechnoClass* ��λ)
{
	SIHouseExt::Factory_MergeProduction(this, ��λ);
}
void SIHouse_ExtendData::Factory_RemoveProduction(TechnoClass* ��λ)
{
	SIHouseExt::Factory_RemoveProduction(this, ��λ);
}
void SIHouse_ExtendData::EXP_Init()
{
	SIHouseExt::EXP_Init(this);
}
void SIHouse_ExtendData::EXP_Get(TechnoClass* ��λ, SIValues_HouseInitEXP* ���Ҿ���ֵ������)
{
	SIHouseExt::EXP_Get(this, ��λ, ���Ҿ���ֵ������);
}
bool SIHouse_ExtendData::EXP_MergeAcademy(TechnoClass* ��λ, TechnoTypeClass* ��λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
{
	return SIHouseExt::EXP_MergeAcademy(this, ��λ, ��λ����);
}
bool SIHouse_ExtendData::EXP_RemoveAcademy(TechnoClass* ��λ, TechnoTypeClass* ��λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
{
	return SIHouseExt::EXP_RemoveAcademy(this, ��λ, ��λ����);
}
bool SIHouse_ExtendData::EXP_UpdateSpyEffect(BuildingClass* ������λ, BuildingTypeClass* ������λ����)//���ܻ�ʧ�ܣ�ʧ�ܷ���false
{
	return SIHouseExt::EXP_UpdateSpyEffect(this, ������λ, ������λ����);
}
void SIHouse_ExtendData::SetHouseVar(int ����ֵ, double ��ֵ)
{
	SIHouseExt::SetHouseVar(this, ����ֵ, ��ֵ);
}
double SIHouse_ExtendData::GetHouseVar(int ����ֵ)
{
	return SIHouseExt::GetHouseVar(this, ����ֵ);
}
void SIHouse_ExtendData::DeleteHouseVar(int ����ֵ)
{
	SIHouseExt::DeleteHouseVar(this, ����ֵ);
}
SIBroadcastClass* SIHouse_ExtendData::Broadcast_FindOrAllocate(int Ƶ��)// ����ֵ����Ϊ��
{
	return SIHouseExt::Broadcast_FindOrAllocate(this, Ƶ��);
}
SIBroadcastClass* SIHouse_ExtendData::Broadcast_Find(int Ƶ��)// ����ֵ����Ϊ��
{
	return SIHouseExt::Broadcast_Find(this, Ƶ��);
}
void SIHouse_ExtendData::FreshBroadcastPower(int Ƶ��, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	SIHouseExt::FreshBroadcastPower(this, Ƶ��, �㲥ǿ��, ������������, ��������, ������λ, ָ��Ŀ��);
}
void SIHouse_ExtendData::PostBroadcast(int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	SIHouseExt::PostBroadcast(this, Ƶ��, ������������, ��������, ������λ, ָ��Ŀ��);
}
int SIHouse_ExtendData::GetBroadcastListenerCount(int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	return SIHouseExt::GetBroadcastListenerCount(this, Ƶ��, ������������, ��������, ������λ, ָ��Ŀ��);
}
double SIHouse_ExtendData::GetBroadcastPower(int Ƶ��)
{
	return SIHouseExt::GetBroadcastPower(this, Ƶ��);
}
void SIHouse_ExtendData::AddBroadcastListener(int Ƶ��, SIBuffClass* ����Buff)
{
	SIHouseExt::AddBroadcastListener(this, Ƶ��, ����Buff);
}
void SIHouse_ExtendData::RemoveBroadcastListener(int Ƶ��, SIBuffClass* ����Buff)
{
	SIHouseExt::RemoveBroadcastListener(this, Ƶ��, ����Buff);
}
void SIHouse_ExtendData::EXPPool_Add(double ���⾭��ֵ)
{
	SIHouseExt::EXPPool_Add(this, ���⾭��ֵ);
}
double SIHouse_ExtendData::EXPPool_Pop(double ����Ķ��⾭��ֵ)
{
	return SIHouseExt::EXPPool_Pop(this, ����Ķ��⾭��ֵ);
}
void SIHouse_ExtendData::ForEach_HouseVar(void* ������, void (CALLBACK* ������)(void* ������, int ����, double& ֵ))
{
	SIHouseExt::ForEach_HouseVar(this, ������, ������);
}
void SIHouse_ExtendData::ForEach_Broadcast(void* ������, void (CALLBACK* ������)(void* ������, int Ƶ��, SIBroadcastClass* ֵ))
{
	SIHouseExt::ForEach_Broadcast(this, ������, ������);
}
HouseClass* SIHouse_ExtendData::OwnerObject()
{
	return SIHouseExt::OwnerObject(this);
}


extern Ext::LibData WIC;

namespace SI
{
	SIInterface_ExtendData* SI_API GetInterface(const AbstractClass* const ʵ��)
	{
		SIInterface(GetInterface)(ʵ��);
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

	void SI_API ForEach(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach)(������, ������);
	}
	void SI_API ForEach_Cell(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Cell)(������, ������);
	}
	void SI_API ForEach_Object(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Object)(������, ������);
	}
	void SI_API ForEach_Techno(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Techno)(������, ������);
	}
	void SI_API ForEach_Animation(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach)(������, ������);
	}
	void SI_API ForEach_Bullet(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Bullet)(������, ������);
	}
	void SI_API ForEach_Terrain(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Terrain)(������, ������);
	}
	void SI_API ForEach_Overlay(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
	{
		SIInterface_void(ForEach_Overlay)(������, ������);
	}
	void SI_API ForEach_House(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)) // [������] ���� true ��������ֹ����
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
	//ͨ��HouseClass*�ҵ���Ӧ��SIHouse_ExtendData*�Խ��и���Ĳ�����
	//����Ӧ������SIHouse_ExtendData���׻���󲻴���/���Ϸ����򷵻�nullptr��
	SIHouse_ExtendData* SI_API GetHouseExtendData(const HouseClass* const ʵ��)
	{
		SIHouseExtDecl(GetHouseExtendData)(ʵ��);
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