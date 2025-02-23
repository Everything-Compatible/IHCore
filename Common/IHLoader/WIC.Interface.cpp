
#include "WIC.Interface.h"
#include "WIC.Template.h"

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

const SIConstVector<SIBroadcastClass*>& SIBroadcastClass::GetArray()
{
	return SIBroadcast::GetBroadcastArray();
}
// 功能函数
void SIBroadcastClass::FreshBroadcastPower(double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	SIBroadcast::FreshBroadcastPower(this, 广播强度, 触发被动监听, 发布坐标, 发布单位, 指向目标);
}
void SIBroadcastClass::PostBroadcast(bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	SIBroadcast::PostBroadcast(this, 触发被动监听, 发布坐标, 发布单位, 指向目标);
}
int SIBroadcastClass::GetBroadcastListenerCount(bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	return SIBroadcast::GetBroadcastListenerCount(this, 无视限制条件, 发布坐标, 发布单位, 指向目标);
}
void SIBroadcastClass::AddListener(SIBuffClass* 要添加的监听Buff)
{
	SIBroadcast::AddListener(this, 要添加的监听Buff);
}
void SIBroadcastClass::RemoveListener(SIBuffClass* 要移除的监听Buff)
{
	SIBroadcast::RemoveListener(this, 要移除的监听Buff);
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


void SIHouse_ExtendData::Counter_AddOriginTechnoType(TechnoTypeClass* 单位类型)
{
	SIHouseExt::Counter_AddOriginTechnoType(this, 单位类型);
}
void SIHouse_ExtendData::Counter_RemoveOriginTechnoType(TechnoTypeClass* 单位类型)
{
	SIHouseExt::Counter_RemoveOriginTechnoType(this, 单位类型);
}
bool SIHouse_ExtendData::AlwaysShowCameo(const TechnoTypeClass* const 单位类型, bool 包括正在建造的单位) const
{
	return SIHouseExt::AlwaysShowCameo(this, 单位类型, 包括正在建造的单位);
}
void SIHouse_ExtendData::Factory_CalculateProduction()
{
	SIHouseExt::Factory_CalculateProduction(this);
}
void SIHouse_ExtendData::Factory_MergeProduction(TechnoClass* 单位)
{
	SIHouseExt::Factory_MergeProduction(this, 单位);
}
void SIHouse_ExtendData::Factory_RemoveProduction(TechnoClass* 单位)
{
	SIHouseExt::Factory_RemoveProduction(this, 单位);
}
void SIHouse_ExtendData::EXP_Init()
{
	SIHouseExt::EXP_Init(this);
}
void SIHouse_ExtendData::EXP_Get(TechnoClass* 单位, SIValues_HouseInitEXP* 国家经验值参数包)
{
	SIHouseExt::EXP_Get(this, 单位, 国家经验值参数包);
}
bool SIHouse_ExtendData::EXP_MergeAcademy(TechnoClass* 单位, TechnoTypeClass* 单位类型)//可能会失败，失败返回false
{
	return SIHouseExt::EXP_MergeAcademy(this, 单位, 单位类型);
}
bool SIHouse_ExtendData::EXP_RemoveAcademy(TechnoClass* 单位, TechnoTypeClass* 单位类型)//可能会失败，失败返回false
{
	return SIHouseExt::EXP_RemoveAcademy(this, 单位, 单位类型);
}
bool SIHouse_ExtendData::EXP_UpdateSpyEffect(BuildingClass* 建筑单位, BuildingTypeClass* 建筑单位类型)//可能会失败，失败返回false
{
	return SIHouseExt::EXP_UpdateSpyEffect(this, 建筑单位, 建筑单位类型);
}
void SIHouse_ExtendData::SetHouseVar(int 索引值, double 数值)
{
	SIHouseExt::SetHouseVar(this, 索引值, 数值);
}
double SIHouse_ExtendData::GetHouseVar(int 索引值)
{
	return SIHouseExt::GetHouseVar(this, 索引值);
}
void SIHouse_ExtendData::DeleteHouseVar(int 索引值)
{
	SIHouseExt::DeleteHouseVar(this, 索引值);
}
SIBroadcastClass* SIHouse_ExtendData::Broadcast_FindOrAllocate(int 频道)// 返回值可能为空
{
	return SIHouseExt::Broadcast_FindOrAllocate(this, 频道);
}
SIBroadcastClass* SIHouse_ExtendData::Broadcast_Find(int 频道)// 返回值可能为空
{
	return SIHouseExt::Broadcast_Find(this, 频道);
}
void SIHouse_ExtendData::FreshBroadcastPower(int 频道, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	SIHouseExt::FreshBroadcastPower(this, 频道, 广播强度, 触发被动监听, 发布坐标, 发布单位, 指向目标);
}
void SIHouse_ExtendData::PostBroadcast(int 频道, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	SIHouseExt::PostBroadcast(this, 频道, 触发被动监听, 发布坐标, 发布单位, 指向目标);
}
int SIHouse_ExtendData::GetBroadcastListenerCount(int 频道, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	return SIHouseExt::GetBroadcastListenerCount(this, 频道, 无视限制条件, 发布坐标, 发布单位, 指向目标);
}
double SIHouse_ExtendData::GetBroadcastPower(int 频道)
{
	return SIHouseExt::GetBroadcastPower(this, 频道);
}
void SIHouse_ExtendData::AddBroadcastListener(int 频道, SIBuffClass* 监听Buff)
{
	SIHouseExt::AddBroadcastListener(this, 频道, 监听Buff);
}
void SIHouse_ExtendData::RemoveBroadcastListener(int 频道, SIBuffClass* 监听Buff)
{
	SIHouseExt::RemoveBroadcastListener(this, 频道, 监听Buff);
}
void SIHouse_ExtendData::EXPPool_Add(double 额外经验值)
{
	SIHouseExt::EXPPool_Add(this, 额外经验值);
}
double SIHouse_ExtendData::EXPPool_Pop(double 需求的额外经验值)
{
	return SIHouseExt::EXPPool_Pop(this, 需求的额外经验值);
}
void SIHouse_ExtendData::ForEach_HouseVar(void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 索引, double& 值))
{
	SIHouseExt::ForEach_HouseVar(this, 参数集, 处理函数);
}
void SIHouse_ExtendData::ForEach_Broadcast(void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 频道, SIBroadcastClass* 值))
{
	SIHouseExt::ForEach_Broadcast(this, 参数集, 处理函数);
}
HouseClass* SIHouse_ExtendData::OwnerObject()
{
	return SIHouseExt::OwnerObject(this);
}


extern Ext::LibData WIC;

namespace SI
{
	SIInterface_ExtendData* SI_API GetInterface(const AbstractClass* const 实体)
	{
		SIInterface(GetInterface)(实体);
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

	void SI_API ForEach(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach)(参数集, 处理函数);
	}
	void SI_API ForEach_Cell(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Cell)(参数集, 处理函数);
	}
	void SI_API ForEach_Object(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Object)(参数集, 处理函数);
	}
	void SI_API ForEach_Techno(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Techno)(参数集, 处理函数);
	}
	void SI_API ForEach_Animation(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach)(参数集, 处理函数);
	}
	void SI_API ForEach_Bullet(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Bullet)(参数集, 处理函数);
	}
	void SI_API ForEach_Terrain(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Terrain)(参数集, 处理函数);
	}
	void SI_API ForEach_Overlay(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
	{
		SIInterface_void(ForEach_Overlay)(参数集, 处理函数);
	}
	void SI_API ForEach_House(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)) // [处理函数] 返回 true 会立刻终止遍历
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
	//通过HouseClass*找到对应的SIHouse_ExtendData*以进行更多的操作。
	//若对应对象无SIHouse_ExtendData配套或对象不存在/不合法，则返回nullptr。
	SIHouse_ExtendData* SI_API GetHouseExtendData(const HouseClass* const 实体)
	{
		SIHouseExtDecl(GetHouseExtendData)(实体);
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