
#include "WIC.Interface.h"


SIException::SIException(const char* info) : Info(info) {};
const char* SIException::what() const noexcept
{
	return Info;
}

char* SIInterface_ExtendData::GetName() // 类型 ID 名称
{
	return SI::GetName(this);
}
 
AbstractClass* SIInterface_ExtendData::OwnerEntity() // 返回值可能为空
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

void SIInterface_ExtendData::GetCoords(CoordStruct* 所在坐标)
{
	SI::GetCoords(this, 所在坐标);
}

void SIInterface_ExtendData::SetForceFireWeapon(WeaponTypeClass* 武器类型)
{
	SI::SetForceFireWeapon(this, 武器类型);
}

void SIInterface_ExtendData::SetForceDeathWeapon(WeaponTypeClass* 武器类型)
{
	SI::SetForceDeathWeapon(this, 武器类型);
}

CoordStruct SIInterface_ExtendData::Calculate_FireCoord(CoordStruct 武器坐标, double 缩放倍率)
{
	return SI::Calculate_FireCoord(this, 武器坐标, 缩放倍率);
}

int  SIInterface_ExtendData::GetStaticEXPLevel()
{
	return SI::IsSelected(this);
}

double  SIInterface_ExtendData::GetLevelTotalEXPCost(double 固定经验值基数, bool 使用新经验系统)
{
	return SI::IsSelected(this);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_Speed(double 原始移动速度)
{
	return SI::Calculate_PropertyGroup_Speed(this, 原始移动速度);
}

double  SIInterface_ExtendData::Calculate_PropertyGroup_Defence(double 原始伤害)
{
	return SI::Calculate_PropertyGroup_Defence(this, 原始伤害);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_Attack(double 原始伤害)
{
	return SI::Calculate_PropertyGroup_Attack(this, 原始伤害);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_ROF(double 原始射速)
{
	return SI::Calculate_PropertyGroup_ROF(this, 原始射速);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXP(double 原始获得的经验值)
{
	return SI::Calculate_PropertyGroup_EXP(this, 原始获得的经验值);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXPProvide(double 原始提供的经验值)
{
	return SI::Calculate_PropertyGroup_EXPProvide(this, 原始提供的经验值);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_EXPCost(double 单位升级所需的经验值)
{
	return SI::Calculate_PropertyGroup_EXPCost(this, 单位升级所需的经验值);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_RangeWeapon(double 原始武器射程)
{
	return SI::Calculate_PropertyGroup_RangeWeapon(this, 原始武器射程);
}

double SIInterface_ExtendData::Calculate_PropertyGroup_CellSpread(double 原始弹头溅射范围)
{
	return SI::Calculate_PropertyGroup_CellSpread(this, 原始弹头溅射范围);
}

int SIInterface_ExtendData::GetTintColor(bool 铁幕染色, bool 空袭染色, bool 狂暴染色)
{
	return SI::GetTintColor(this, 铁幕染色, 空袭染色, 狂暴染色);
}

void SIInterface_ExtendData::Calculate_CustomTintValues(int& 染色颜色, int& 染色强度)
{
	return SI::Calculate_CustomTintValues(this, 染色颜色, 染色强度);
}

int SIInterface_ExtendData::Calculate_CustomTintValues_Color()
{
	return SI::Calculate_CustomTintValues_Color(this);
}

int SIInterface_ExtendData::Calculate_CustomTintValues_Intensity()
{
	return SI::Calculate_CustomTintValues_Intensity(this);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Speed(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_Speed(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Armor(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_Armor(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Versus(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_Versus(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_Attack(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_Attack(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_ROF(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_ROF(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXP(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_EXP(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXPProvide(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_EXPProvide(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_EXPCost(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_EXPCost(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_RangeWeapon(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_RangeWeapon(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Multiplier_CellSpread(double 新的增加量)
{
	return SI::Property_Merge_Multiplier_CellSpread(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Speed(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_Speed(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Armor(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_Armor(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Versus(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_Versus(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_Attack(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_Attack(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_ROF(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_ROF(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXP(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_EXP(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXPProvide(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_EXPProvide(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_EXPCost(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_EXPCost(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_RangeWeapon(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_RangeWeapon(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_ExtraMultiplier_CellSpread(double 新的增加量)
{
	return SI::Property_Merge_ExtraMultiplier_CellSpread(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_Speed(double 新的增加量)
{
	return SI::Property_Merge_Addon_Speed(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_Damage(double 新的增加量)
{
	return SI::Property_Merge_Addon_Damage(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_Attack(double 新的增加量)
{
	return SI::Property_Merge_Addon_Attack(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_ROF(double 新的增加量)
{
	return SI::Property_Merge_Addon_ROF(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXP(double 新的增加量)
{
	return SI::Property_Merge_Addon_EXP(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXPProvide(double 新的增加量)
{
	return SI::Property_Merge_Addon_EXPProvide(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_EXPCost(double 新的增加量)
{
	return SI::Property_Merge_Addon_EXPCost(this, 0.0, 新的增加量);
}

// 单位是格点
double SIInterface_ExtendData::Property_Merge_Addon_RangeWeapon(double 新的增加量)
{
	return SI::Property_Merge_Addon_RangeWeapon(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_Addon_CellSpread(double 新的增加量) // 单位是格子
{
	return SI::Property_Merge_Addon_CellSpread(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Speed(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_Speed(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Damage(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_Damage(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_Attack(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_Attack(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_ROF(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_ROF(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXP(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_EXP(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXPProvide(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_EXPProvide(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_EXPCost(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_EXPCost(this, 0.0, 新的增加量);
}

// 单位是格点
double SIInterface_ExtendData::Property_Merge_FinalAddon_RangeWeapon(double 新的增加量)
{
	return SI::Property_Merge_FinalAddon_RangeWeapon(this, 0.0, 新的增加量);
}

double SIInterface_ExtendData::Property_Merge_FinalAddon_CellSpread(double 新的增加量) // 单位是格子
{
	return SI::Property_Merge_FinalAddon_CellSpread(this, 0.0, 新的增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Speed(double 增加量)
{
	return SI::Property_Register_Multiplier_Speed(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Armor(double 增加量)
{
	return SI::Property_Register_Multiplier_Armor(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Versus(double 增加量)
{
	return SI::Property_Register_Multiplier_Versus(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_Attack(double 增加量)
{
	return SI::Property_Register_Multiplier_Attack(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_ROF(double 增加量)
{
	return SI::Property_Register_Multiplier_ROF(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXP(double 增加量)
{
	return SI::Property_Register_Multiplier_EXP(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXPProvide(double 增加量)
{
	return SI::Property_Register_Multiplier_EXPProvide(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_EXPCost(double 增加量)
{
	return SI::Property_Register_Multiplier_EXPCost(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_RangeWeapon(double 增加量)
{
	return SI::Property_Register_Multiplier_RangeWeapon(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Multiplier_CellSpread(double 增加量)
{
	return SI::Property_Register_Multiplier_CellSpread(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Speed(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_Speed(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Armor(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_Armor(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Versus(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_Versus(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_Attack(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_Attack(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_ROF(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_ROF(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXP(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_EXP(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXPProvide(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_EXPProvide(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_EXPCost(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_EXPCost(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_RangeWeapon(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_RangeWeapon(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_ExtraMultiplier_CellSpread(double 增加量)
{
	return SI::Property_Register_ExtraMultiplier_CellSpread(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_Speed(double 增加量)
{
	return SI::Property_Register_Addon_Speed(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_Damage(double 增加量)
{
	return SI::Property_Register_Addon_Damage(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_Attack(double 增加量)
{
	return SI::Property_Register_Addon_Attack(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_ROF(double 增加量)
{
	return SI::Property_Register_Addon_ROF(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_EXP(double 增加量)
{
	return SI::Property_Register_Addon_EXP(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_EXPProvide(double 增加量)
{
	return SI::Property_Register_Addon_EXPProvide(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_EXPCost(double 增加量)
{
	return SI::Property_Register_Addon_EXPCost(this, 增加量);
}

// 单位是格点
int SIInterface_ExtendData::Property_Register_Addon_RangeWeapon(double 增加量)
{
	return SI::Property_Register_Addon_RangeWeapon(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_Addon_CellSpread(double 增加量)
{
	return SI::Property_Register_Addon_CellSpread(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Speed(double 增加量)
{
	return SI::Property_Register_FinalAddon_Speed(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Damage(double 增加量)
{
	return SI::Property_Register_FinalAddon_Damage(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_Attack(double 增加量)
{
	return SI::Property_Register_FinalAddon_Attack(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_ROF(double 增加量)
{
	return SI::Property_Register_FinalAddon_ROF(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXP(double 增加量)
{
	return SI::Property_Register_FinalAddon_EXP(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXPProvide(double 增加量)
{
	return SI::Property_Register_FinalAddon_EXPProvide(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_EXPCost(double 增加量)
{
	return SI::Property_Register_FinalAddon_EXPCost(this, 增加量);
}

// 单位是格点
int SIInterface_ExtendData::Property_Register_FinalAddon_RangeWeapon(double 增加量)
{
	return SI::Property_Register_FinalAddon_RangeWeapon(this, 增加量);
}

int SIInterface_ExtendData::Property_Register_FinalAddon_CellSpread(double 增加量) // 单位是格子
{
	return SI::Property_Register_FinalAddon_CellSpread(this, 增加量);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Speed(int 注册索引)
{
	SI::Property_Unregister_Multiplier_Speed(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Armor(int 注册索引)
{
	SI::Property_Unregister_Multiplier_Armor(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Versus(int 注册索引)
{
	SI::Property_Unregister_Multiplier_Versus(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_Attack(int 注册索引)
{
	SI::Property_Unregister_Multiplier_Attack(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_ROF(int 注册索引)
{
	SI::Property_Unregister_Multiplier_ROF(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXP(int 注册索引)
{
	SI::Property_Unregister_Multiplier_EXP(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXPProvide(int 注册索引)
{
	SI::Property_Unregister_Multiplier_EXPProvide(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_EXPCost(int 注册索引)
{
	SI::Property_Unregister_Multiplier_EXPCost(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_RangeWeapon(int 注册索引)
{
	SI::Property_Unregister_Multiplier_RangeWeapon(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Multiplier_CellSpread(int 注册索引)
{
	SI::Property_Unregister_Multiplier_CellSpread(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Speed(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_Speed(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Armor(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_Armor(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Versus(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_Versus(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_Attack(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_Attack(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_ROF(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_ROF(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXP(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_EXP(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXPProvide(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_EXPProvide(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_EXPCost(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_EXPCost(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_RangeWeapon(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_RangeWeapon(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_ExtraMultiplier_CellSpread(int 注册索引)
{
	SI::Property_Unregister_ExtraMultiplier_CellSpread(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Speed(int 注册索引)
{
	SI::Property_Unregister_Addon_Speed(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Damage(int 注册索引)
{
	SI::Property_Unregister_Addon_Damage(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_Attack(int 注册索引)
{
	SI::Property_Unregister_Addon_Attack(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_ROF(int 注册索引)
{
	SI::Property_Unregister_Addon_ROF(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXP(int 注册索引)
{
	SI::Property_Unregister_Addon_EXP(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXPProvide(int 注册索引)
{
	SI::Property_Unregister_Addon_EXPProvide(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_EXPCost(int 注册索引)
{
	SI::Property_Unregister_Addon_EXPCost(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_RangeWeapon(int 注册索引)
{
	SI::Property_Unregister_Addon_RangeWeapon(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_Addon_CellSpread(int 注册索引)
{
	SI::Property_Unregister_Addon_CellSpread(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Speed(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_Speed(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Damage(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_Damage(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_Attack(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_Attack(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_ROF(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_ROF(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXP(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_EXP(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXPProvide(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_EXPProvide(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_EXPCost(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_EXPCost(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_RangeWeapon(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_RangeWeapon(this, 注册索引);
}

void SIInterface_ExtendData::Property_Unregister_FinalAddon_CellSpread(int 注册索引)
{
	SI::Property_Unregister_FinalAddon_CellSpread(this, 注册索引);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneCrush(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneCrush(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneCrushAdvance(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneCrushAdvance(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneRadiation(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneRadiation(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePsionicDamage(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmunePsionicDamage(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePsionic(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmunePsionic(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneBerzerk(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneBerzerk(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmunePoison(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmunePoison(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneWarp(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneWarp(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_ImmuneEMP(int 新的能力强度)
{
	SI::Property_Merge_Ability_ImmuneEMP(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing(int 新的能力强度)
{
	SI::Property_Merge_Ability_SelfHealing(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Percent(double 新的能力强度)
{
	SI::Property_Merge_Ability_SelfHealing_Percent(this, 0.0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Cap(double 新的能力强度)
{
	SI::Property_Merge_Ability_SelfHealing_Cap(this, 0.0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_Delay(int 新的能力强度)
{
	SI::Property_Merge_Ability_SelfHealing_Delay(this, 0, 新的能力强度);
}

void SIInterface_ExtendData::Property_Merge_Ability_SelfHealing_CombatDelay(int 新的能力强度)
{
	SI::Property_Merge_Ability_SelfHealing_CombatDelay(this, 0, 新的能力强度);
}

int SIInterface_ExtendData::Property_Register_CustomTintValues(ColorStruct 染色颜色, int 染色强度, int 作战方归属)
{
	return SI::Property_Register_CustomTintValues_A(this, 染色颜色, 染色强度, 作战方归属);
}

int SIInterface_ExtendData::Property_Register_CustomTintValues(int 染色颜色, int 染色强度, int 作战方归属)
{
	return SI::Property_Register_CustomTintValues_B(this, 染色颜色, 染色强度, 作战方归属);
}

void SIInterface_ExtendData::Property_Unregister_CustomTintValues(int 注册索引)
{
	SI::Property_Unregister_CustomTintValues(this, 注册索引);
}

// 扣除经验使用负数
void SIInterface_ExtendData::EXP_Add(const double 提供的经验值, SIEXPSourceType 经验值来源, SIValues_EXPMerge* 合并经验值参数包)
{
	SI::EXP_Add(this, 提供的经验值, 经验值来源, 合并经验值参数包);
}

bool SIInterface_ExtendData::Buff_IsPowerOffline()
{
	return SI::Buff_IsPowerOffline(this);
}

bool SIInterface_ExtendData::Buff_IsNeedHide()
{
	return SI::Buff_IsNeedHide(this);
}

int SIInterface_ExtendData::Buff_GetEffectDuration(SIBuffTypeClass* Buff类型, int 原始的挂载持续时间)
{
	return SI::Buff_GetEffectDuration(this, Buff类型, 原始的挂载持续时间);
}

double SIInterface_ExtendData::Buff_GetEffectHealth(SIBuffTypeClass* Buff类型, double 原始的生命值)
{
	return SI::Buff_GetEffectHealth(this, Buff类型, 原始的生命值);
}

double SIInterface_ExtendData::Buff_GetEffectPower(SIBuffTypeClass* Buff类型, double 原始的强度值)
{
	return SI::Buff_GetEffectPower(this, Buff类型, 原始的强度值);
}

SIBuffClass* SIInterface_ExtendData::Buff_CreateOrMerge(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, AbstractClass* 来源, HouseClass* 来源所属作战方)
{
	return SI::Buff_CreateOrMerge(this, Buff类型, Buff参数设置包, 来源, 来源所属作战方);
}

void SIInterface_ExtendData::Buff_TryActive(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包)
{
	SI::Buff_TryActive_A(this, Buff类型, Buff参数设置包);
}

void SIInterface_ExtendData::Buff_TryActive(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
{
	SI::Buff_TryActive_B(this, Buff类型, Buff参数设置包, 检查单位属性参数包);
}

// [判断处理函数] 返回 true 则对 Buff 进行处理
void SIInterface_ExtendData::Buff_TryActive(void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包))
{
	SI::Buff_TryActive_C(this, 来源, 判断处理函数);
}

void SIInterface_ExtendData::Buff_TryAfter(SIBuffTypeClass* Buff类型)
{
	SI::Buff_TryAfter_A(this, Buff类型);
}

void SIInterface_ExtendData::Buff_TryAfter(SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
{
	SI::Buff_TryAfter_B(this, Buff类型, 检查单位属性参数包);
}

// [判断处理函数] 返回 true 则对 Buff 进行处理
void SIInterface_ExtendData::Buff_TryAfter(void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff))
{
	SI::Buff_TryAfter_C(this, 来源, 判断处理函数);
}

void SIInterface_ExtendData::Buff_TryRemove(SIBuffTypeClass* Buff类型)
{
	SI::Buff_TryRemove_A(this, Buff类型);
}

void SIInterface_ExtendData::Buff_TryRemove(SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
{
	SI::Buff_TryRemove_B(this, Buff类型, 检查单位属性参数包);
}

// [判断处理函数] 返回 true 则对 Buff 进行处理
void SIInterface_ExtendData::Buff_TryRemove(void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff))
{
	SI::Buff_TryRemove_C(this, 来源, 判断处理函数);
}

void SIInterface_ExtendData::Buff_TryMergeSetting(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包)
{
	SI::Buff_TryMergeSetting_A(this, Buff类型, Buff参数设置包);
}

void SIInterface_ExtendData::Buff_TryMergeSetting(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
{
	SI::Buff_TryMergeSetting_B(this, Buff类型, Buff参数设置包, 检查单位属性参数包);
}

// [判断处理函数] 返回 true 则对 Buff 进行处理
void SIInterface_ExtendData::Buff_TryMergeSetting(void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包))
{
	SI::Buff_TryMergeSetting_C(this, 来源, 判断处理函数);
}

void SIInterface_ExtendData::Buff_TryChange(SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方)
{
	SI::Buff_TryChange_A(this, 旧Buff类型, 新Buff类型, Buff参数设置包, 基础挂载持续时间增加量, 强度值增加量, 来源, 来源所属作战方);
}

void SIInterface_ExtendData::Buff_TryChange(SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方, SIPackTypeClass_CheckTechno* 检查单位属性参数包)
{
	SI::Buff_TryChange_B(this, 旧Buff类型, 新Buff类型, Buff参数设置包, 基础挂载持续时间增加量, 强度值增加量, 来源, 来源所属作战方, 检查单位属性参数包);
}

bool SIInterface_ExtendData::Buff_HasBuffed()
{
	return SI::Buff_HasBuffed_A(this);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff类型)
{
	return SI::Buff_HasBuffed_B(this, Buff类型);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff类型, SIBuffClass_State Buff状态)
{
	return SI::Buff_HasBuffed_C(this, Buff类型, Buff状态);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff类型, AbstractClass* 排除的来源)
{
	return SI::Buff_HasBuffed_D(this, Buff类型, 排除的来源);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass* Buff类型, AbstractClass* 需求的来源, bool 没用的标识)
{
	return SI::Buff_HasBuffed_E(this, Buff类型, 需求的来源, 没用的标识);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass_EffectType 效果种类)
{
	return SI::Buff_HasBuffed_F(this, 效果种类);
}

bool SIInterface_ExtendData::Buff_HasBuffed(SIBuffTypeClass_EffectType 效果种类, SIBuffClass_State Buff状态)
{
	return SI::Buff_HasBuffed_G(this, 效果种类, Buff状态);
}

// 此处不会立即移除无效的 Buff
void SIInterface_ExtendData::Buff_LookUp(void* 来源, void(*遍历函数)(void* 来源, SIBuffClass* Buff))
{
	SI::Buff_LookUp(this, 来源, 遍历函数);
}

void SIInterface_ExtendData::Buff_Update()
{
	SI::Buff_Update(this);
}

bool SIInterface_ExtendData::Buff_TriggerFire(AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器)
{
	return SI::Buff_TriggerFire(this, 目标, 武器索引, 武器数据, 本体开火坐标, 死亡武器);
}

double SIInterface_ExtendData::Buff_TriggerAttacker(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	return SI::Buff_TriggerAttacker(this, 目标单位, 伤害参数, 当前伤害);
}

double SIInterface_ExtendData::Buff_TriggerDefender(args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	return SI::Buff_TriggerDefender(this, 伤害参数, 当前伤害);
}

double SIInterface_ExtendData::Buff_TriggerDefender_Final(args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	return SI::Buff_TriggerDefender_Final(this, 伤害参数, 当前伤害);
}

void SIInterface_ExtendData::Buff_TriggerDeath(args_ReceiveDamage* 伤害参数)
{
	SI::Buff_TriggerDeath(this, 伤害参数);
}

void SIInterface_ExtendData::Buff_TriggerKiller(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数)
{
	SI::Buff_TriggerKiller(this, 目标单位, 伤害参数);
}

double SIInterface_ExtendData::Buff_TriggerSelf(double 当前伤害)
{
	return SI::Buff_TriggerSelf(this, 当前伤害);
}

void SIInterface_ExtendData::Buff_DoDamages(double 准备造成的伤害)
{
	SI::Buff_DoDamages(this, 准备造成的伤害);
}

SIBuffClass* SIInterface_ExtendData::Buff_GetBuff(int Buff索引) // 返回值可能为空
{
	return SI::Buff_GetBuff_A(this, Buff索引);
}
SIBuffClass* SIInterface_ExtendData::Buff_GetBuff(SIBuffTypeClass* Buff类型) // 返回值可能为空
{
	return SI::Buff_GetBuff_B(this, Buff类型);
}

void SIInterface_ExtendData::AvailableEffect()
{
	return SI::AvailableEffect(this);
}

void SIInterface_ExtendData::UnavailableEffect()
{
	return SI::UnavailableEffect(this);
}

void SIInterface_ExtendData::Element_Merge_ResistanceType(SIElementResistanceTypeClass* 元素抗性类型, int 层数计数)
{
	SI::Element_Merge_ResistanceType(this, 元素抗性类型, 层数计数);
}

void SIInterface_ExtendData::Element_Merge_ImmuneElementType(SIElementTypeClass* 元素类型, int 层数计数)
{
	SI::Element_Merge_ImmuneElementType(this, 元素类型, 层数计数);
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

bool SIInterface_ExtendData::UpdateClickBar_Hover(HouseClass* 点击的作战方, Point2D* 点击像素偏移)
{
	return SI::UpdateClickBar_Hover(this, 点击的作战方, 点击像素偏移);
}

void SIInterface_ExtendData::UpdateClickBar_TryClick(HouseClass* 点击的作战方, Point2D* 点击像素偏移, int& Buff索引, SIDigitalClickBarIndex& 点击索引, double& 当前值, bool& 禁用标识)
{
	SI::UpdateClickBar_TryClick(this, 点击的作战方, 点击像素偏移, Buff索引, 点击索引, 当前值, 禁用标识);
}

void SIInterface_ExtendData::UpdateClickBar_Effect(int Buff索引, SIDigitalClickBarIndex 点击索引, HouseClass* 点击的作战方, double 当前值)
{
	SI::UpdateClickBar_Effect(this, Buff索引, 点击索引, 点击的作战方, 当前值);
}

bool SIInterface_ExtendData::UpdateKeyExecuteBar_TryExecute(HouseClass* 点击的作战方, SICommandCode 快捷键代码)
{
	return SI::UpdateKeyExecuteBar_TryExecute(this, 点击的作战方, 快捷键代码);
}

void SIInterface_ExtendData::UpdateKeyExecuteBar_Effect(HouseClass* 点击的作战方, SICommandCode 快捷键代码)
{
	SI::UpdateKeyExecuteBar_Effect(this, 点击的作战方, 快捷键代码);
}

void SIInterface_ExtendData::Init()
{
	SI::Init(this);
}

void SIInterface_ExtendData::UnInit()
{
	SI::Init(this);
}

//WIC 变形逻辑
//具体使用条件及限制详见WIC说明书
void SIInterface_ExtendData::UpdateTechnoType(TechnoTypeClass* 旧的单位类型)
{
	SI::UpdateTechnoType(this, 旧的单位类型);
}

void SIInterface_ExtendData::UpdateOwningHouse(const HouseClass* const 旧的单位所属作战方, const HouseClass* const 新的单位所属作战方)
{
	SI::UpdateOwningHouse(this, 旧的单位所属作战方, 新的单位所属作战方);
}

Ext::LibData WIC(nullptr);
namespace SI
{
	
	/*
	备注：此处和Sidoupiar确认了稳定存在的API及其形式，所以直接通过只有最小限度检查的LibData::QueryFunction实现
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

	SIInterface_ExtendData* SI_API GetInterface(const AbstractClass* const 实体)
	{
		SIInterface(GetInterface)(实体);
	}

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
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* 接口, void* 来源, bool( *判断处理函数)( void* 来源, SIBuffClass* Buff) )
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
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff) )
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
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* 接口, void* 来源, bool(*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包) )
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

namespace SITool
{
	TechnoClass* SI_API NormalConvert_TechnoTo(TechnoClass* 来源单位, TechnoTypeClass* 目标单位类型, HouseClass* 目标作战方, BulletClass* 子弹, bool 要求在地图上进行, bool 允许进行免疫)
	{
		SIInterfaceDecl("WIC.Tool.NormalConvert_TechnoTo", NormalConvert_TechnoTo)(来源单位, 目标单位类型, 目标作战方, 子弹, 要求在地图上进行, 允许进行免疫);
	}
	TechnoClass* SI_API SuperConvert_TechnoTo(TechnoClass* 来源单位, TechnoTypeClass* 目标单位类型, HouseClass* 目标作战方, BulletClass* 子弹, bool 要求在地图上进行, bool 允许进行免疫)
	{
		SIInterfaceDecl("WIC.Tool.SuperConvert_TechnoTo", SuperConvert_TechnoTo)(来源单位, 目标单位类型, 目标作战方, 子弹, 要求在地图上进行, 允许进行免疫);
	}
	void SI_API AddFlyingString_A(const CoordStruct 坐标, Point2D 像素偏移, int 持续时间, double 浮动速度, ColorStruct 颜色, const wchar_t* 显示内容)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_A", AddFlyingString_A)(坐标, 像素偏移, 持续时间, 浮动速度, 颜色, 显示内容);
	}
	void SI_API AddFlyingString_B(const CoordStruct 坐标, Point2D 像素偏移, int 持续时间, double 浮动速度, ConvertClass* 色盘, SHPStruct* 图像, const int* 字符数据列表, int 字符数据总数)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_B", AddFlyingString_B)(坐标, 像素偏移, 持续时间, 浮动速度, 色盘, 图像, 字符数据列表, 字符数据总数);
	}
	void SI_API AddFlyingString_Money(CoordStruct 坐标, int 金额, HouseClass* 所属作战方, int 作战方归属)
	{
		SIInterfaceDecl("WIC.Tool.AddFlyingString_Money", AddFlyingString_Money)(坐标, 金额, 所属作战方, 作战方归属);
	}

	bool Available()
	{
		static bool Check{ false };
		if (Check)return true;
		auto wic = Ext::GetLib(WIC_LibName);
		if (wic.Available())Check = true;
		return wic.Available();
	}

	void SI_API ForEach(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach)(参数集, 处理函数);
	}
	void SI_API ForEach_Cell(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Cell)(参数集, 处理函数);
	}
	void SI_API ForEach_Object(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Object)(参数集, 处理函数);
	}
	void SI_API ForEach_Techno(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Techno)(参数集, 处理函数);
	}
	void SI_API ForEach_Animation(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach)(参数集, 处理函数);
	}
	void SI_API ForEach_Bullet(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Bullet)(参数集, 处理函数);
	}
	void SI_API ForEach_Terrain(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Terrain)(参数集, 处理函数);
	}
	void SI_API ForEach_Overlay(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Overlay)(参数集, 处理函数);
	}
	void SI_API ForEach_House(void* 参数集, bool (*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_House)(参数集, 处理函数);
	}
}


namespace SIClassManager
{
	//通过AbstractClass*找到对应的SIInterface_ExtendData*以进行更多的操作。
	//若对应对象无SIInterface_ExtendData配套或对象不存在/不合法，则返回nullptr。
	SIInterface_ExtendData* SI_API GetExtendData(const AbstractClass* const 实体)
	{
		return SI::GetInterface(实体);
	}
	//获取WIC内部对象的接口
	//以下函数的返回值都可能为空，请自行检查
	SIBuffTypeClass* SI_API BuffType_Find(const char* 名称)
	{
		SIInterfaceDecl("WIC.BuffType.Find", BuffType_Find)(名称);
	}
	SIBuffTypeClass* SI_API BuffType_FindOrAllocate(const char* 名称)
	{
		SIInterfaceDecl("WIC.BuffType.FindOrAllocate", BuffType_FindOrAllocate)(名称);
	}
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_Find(const char* 名称)
	{
		SIInterfaceDecl("WIC.PBuffSetting.Find", BuffSetting_Find)(名称);
	}
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_FindOrAllocate(const char* 名称)
	{
		SIInterfaceDecl("WIC.PBuffSetting.FindOrAllocate", BuffSetting_FindOrAllocate)(名称);
	}
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_Find(const char* 名称)
	{
		SIInterfaceDecl("WIC.PCheckTechno.Find", CheckTechno_Find)(名称);
	}
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_FindOrAllocate(const char* 名称)
	{
		SIInterfaceDecl("WIC.PCheckTechno.FindOrAllocate", CheckTechno_Find)(名称);
	}
	SIElementTypeClass* SI_API ElementType_Find(const char* 名称)
	{
		SIInterfaceDecl("WIC.ElementType.Find", ElementType_Find)(名称);
	}
	SIElementTypeClass* SI_API ElementType_FindOrAllocate(const char* 名称)
	{
		SIInterfaceDecl("WIC.ElementType.Find", ElementType_FindOrAllocate)(名称);
	}
	SIElementResistanceTypeClass* SI_API ElementResistanceType_Find(const char* 名称)
	{
		SIInterfaceDecl("WIC.ElementResistanceType.Find", ElementResistanceType_Find)(名称);
	}
	SIElementResistanceTypeClass* SI_API ElementResistanceType_FindOrAllocate(const char* 名称)
	{
		SIInterfaceDecl("WIC.ElementResistanceType.Find", ElementResistanceType_FindOrAllocate)(名称);
	}

	// 需要在载入 INI 之前就完成注册 , 返回值为效果类型 ID , 返回值为 -1 表示注册失败
	//注册方法：
	//调用方的插件应在依赖中包括WIC
	//例如：InitDependency Dependency{"SIWinterIsComing",DoNotCheckVersion,WIC_LIBRARY_VERSION,true};
	//加入依赖之后，请在OrderedInit函数当中完成注册
	int SI_API RegisterEffectType(const char* 效果类型名称, long 虚表指针)
	{
		SIInterfaceDecl("WIC.BuffType.RegisterEffectType", RegisterEffectType)(效果类型名称, 虚表指针);
	}

	// 与Buff设置配套的打包函数
	SIPack_BuffSetting_FromStatic SI_API MakePack(SIPackTypeClass_BuffSetting* 类型, HouseClass* 当前作战方)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack", MakePack)(类型, 当前作战方);
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_Empty()
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_Empty", MakePack_Empty)();
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_DurationOnly(int 持续时间, bool 覆盖持续时间, bool 承受持续时间抗性)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_DurationOnly", MakePack_DurationOnly)(持续时间, 覆盖持续时间, 承受持续时间抗性);
	}
	SIPack_BuffSetting_FromStatic SI_API MakePack_PowerOnly(double 强度值, bool 覆盖强度值, bool 承受强度值抗性)
	{
		SIInterfaceDecl("WIC.PBuffSetting.MakePack_PowerOnly", MakePack_PowerOnly)(强度值, 覆盖强度值, 承受强度值抗性);
	}
	
	
}