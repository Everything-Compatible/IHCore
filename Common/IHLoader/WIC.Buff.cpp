#include "WIC.Buff.h"

// ====================================================================================================
// ==== BuffClass 接口 - 流程函数 =====================================================================
// ====================================================================================================

void SIBuffClass::TryRemark( SIPack_BuffSetting_FromStatic* Buff参数设置包)
{
	SIBuff::TryRemark(this, Buff参数设置包);
}

void SIBuffClass::TryActive( SIPack_BuffSetting_FromStatic* Buff参数设置包)
{
	SIBuff::TryActive(this, Buff参数设置包);
}

void SIBuffClass::TryAfter()
{
	SIBuff::TryAfter(this);
}

void SIBuffClass::TryDamage( double 准备造成的伤害)
{
	SIBuff::TryDamage(this, 准备造成的伤害);
}

// ====================================================================================================
// ==== BuffClass 接口 - 属性函数 =====================================================================
// ====================================================================================================

void SIBuffClass::EnterState( SIBuffClass_State 状态)
{
	SIBuff::EnterState(this, 状态);
}

void SIBuffClass::MergeSetting( SIPack_BuffSetting_FromStatic* Buff参数设置包)
{
	SIBuff::MergeSetting(this, Buff参数设置包);
}

CoordStruct SIBuffClass::GetCenterCoords()
{
	return SIBuff::GetCenterCoords(this);
}

// 返回值可能为空
TechnoClass* SIBuffClass::GetOwnerTechno()
{
	return SIBuff::GetOwnerTechno(this);
}

// 返回值可能为空
TechnoClass* SIBuffClass::GetSourceTechno()
{
	return SIBuff::GetSourceTechno(this);
}

// 返回值可能为空
TechnoTypeClass* SIBuffClass::GetOwnerTechnoType()
{
	return SIBuff::GetOwnerTechnoType(this);
}

// 返回值可能为空
TechnoTypeClass* SIBuffClass::GetSourceTechnoType()
{
	return SIBuff::GetSourceTechnoType(this);
}

HouseClass* SIBuffClass::GetActiveOwnerHouse()
{
	return SIBuff::GetActiveOwnerHouse(this);
}

HouseClass* SIBuffClass::GetActiveSourceHouse()
{
	return SIBuff::GetActiveSourceHouse(this);
}

// ====================================================================================================
// ==== BuffClass 接口 - 工具函数 =====================================================================
// ====================================================================================================

int SIBuffClass::GetEffectMode( int 参数项索引, int 最大值)
{
	return SIBuff::GetEffectMode_A(this, 参数项索引, 最大值);
}

int SIBuffClass::GetEffectMode( int 参数项索引, int 最大值, int 最小值)
{
	return SIBuff::GetEffectMode_B(this, 参数项索引, 最大值, 最小值);
}

double SIBuffClass::GetEffectPower( int 参数项索引)
{
	return SIBuff::GetEffectPower_A(this, 参数项索引);
}

double SIBuffClass::GetEffectPower( int 参数项索引, double 最小值)
{
	return SIBuff::GetEffectPower_B(this, 参数项索引, 最小值);
}

double SIBuffClass::GetEffectTotal( int 参数项索引, double 合并后的实际效果强度)
{
	return SIBuff::GetEffectTotal_A(this, 参数项索引, 合并后的实际效果强度);
}

double SIBuffClass::GetEffectTotal( int 参数项索引, double 合并后的实际效果强度, double 最小值)
{
	return SIBuff::GetEffectTotal_B(this, 参数项索引, 合并后的实际效果强度, 最小值);
}

double SIBuffClass::GetEffectTotalMax( int 参数项索引)
{
	return SIBuff::GetEffectTotalMax(this, 参数项索引);
}

double SIBuffClass::GetEffectTotalMin( int 参数项索引)
{
	return SIBuff::GetEffectTotalMin(this, 参数项索引);
}

bool SIBuffClass::Effect_NotPassCommonCheck()
{
	return SIBuff::Effect_NotPassCommonCheck_A(this);
}

bool SIBuffClass::Effect_NotPassCommonCheck(SIBuffClass_EffectData* 生效数据)
{
	return SIBuff::Effect_NotPassCommonCheck_B(this, 生效数据);
}

bool SIBuffClass::Effect_NotPassCommonCheck( args_ReceiveDamage* 伤害参数)
{
	return SIBuff::Effect_NotPassCommonCheck_C(this, 伤害参数);
}

bool SIBuffClass::Effect_NotPassCommonCheck( args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	return SIBuff::Effect_NotPassCommonCheck_D(this, 伤害参数, 当前伤害);
}

bool SIBuffClass::Effect_NotPassCommonCheck( double 当前伤害)
{
	return SIBuff::Effect_NotPassCommonCheck_E(this, 当前伤害);
}

bool SIBuffClass::Effect_NotPassCommonCheck_NoIronCurtain()
{
	return SIBuff::Effect_NotPassCommonCheck_NoIronCurtain(this);
}

bool SIBuffClass::Effect_PassCheck_TechnoTypeList( TechnoTypeClass* 目标单位类型)
{
	return SIBuff::Effect_PassCheck_TechnoTypeList(this, 目标单位类型);
}

bool SIBuffClass::Effect_NotPassCheck_TechnoTypeList( TechnoTypeClass* 目标单位类型)
{
	return SIBuff::Effect_NotPassCheck_TechnoTypeList(this, 目标单位类型);
}

bool SIBuffClass::Effect_Match_TechnoTypeList( TechnoTypeClass* 目标单位类型)
{
	return SIBuff::Effect_Match_TechnoTypeList(this, 目标单位类型);
}

bool SIBuffClass::Effect_NotMatch_TechnoTypeList( TechnoTypeClass* 目标单位类型)
{
	return SIBuff::Effect_NotMatch_TechnoTypeList(this, 目标单位类型);
}

// 会设置 SIDamageLeft 属性
void SIBuffClass::ReceiveWarheadDamage( TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型)
{
	SIBuff::ReceiveWarheadDamage_A(this, 目标单位, 攻击单位, 伤害, 伤害弹头类型);
}

// 会设置 SIDamageLeft 属性
void SIBuffClass::ReceiveWarheadDamage( TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型, HouseClass* 攻击单位所属作战方)
{
	SIBuff::ReceiveWarheadDamage_B(this, 目标单位, 攻击单位, 伤害, 伤害弹头类型, 攻击单位所属作战方);
}

// 会设置 SIDamageLeft 属性
void SIBuffClass::ReceiveHealthDamage( TechnoClass* 目标单位, double 生命值损耗, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程)
{
	SIBuff::ReceiveHealthDamage_A(this, 目标单位, 生命值损耗, 结算护甲弹头类型, 死亡伤害弹头类型, 执行自身处理过程);
}

// 会设置 SIDamageLeft 属性
void SIBuffClass::ReceiveHealthDamage( TechnoClass* 目标单位, double 生命值损耗, double 生命值损耗上限, double 生命值损耗下限, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程)
{
	SIBuff::ReceiveHealthDamage_B(this, 目标单位, 生命值损耗, 生命值损耗上限, 生命值损耗下限, 结算护甲弹头类型, 死亡伤害弹头类型, 执行自身处理过程);
}

bool SIBuffClass::FreshOrPostBroadcast( double 需要更新的广播强度, AbstractClass* 指向目标)
{
	return SIBuff::FreshOrPostBroadcast(this, 需要更新的广播强度, 指向目标);
}

void SIBuffClass::ResetBroadcastPower( double 需要更新的广播强度)
{
	SIBuff::ResetBroadcastPower(this, 需要更新的广播强度);
}

int SIBuffClass::GetBroadcastListenerCount( bool 无视限制条件, AbstractClass* 指向目标)
{
	return SIBuff::GetBroadcastListenerCount(this, 无视限制条件, 指向目标);
}

bool SIBuffClass::AddAsBroadcastListener()
{
	return SIBuff::AddAsBroadcastListener(this);
}

void SIBuffClass::RemoveAsBroadcastListener()
{
	SIBuff::RemoveAsBroadcastListener(this);
}

double SIBuffClass::GetBroadcastPower_Total()
{
	return SIBuff::GetBroadcastPower_Total(this);
}

// 返回值可能为空
TechnoClass* SIBuffClass::GetFireOwner()
{
	return SIBuff::GetFireOwner(this);
}

bool SIBuffClass::GetFireOwnerRawOffset( CoordStruct& 本体坐标)
{
	return SIBuff::GetFireOwnerRawOffset(this, 本体坐标);
}

bool SIBuffClass::GetFireTargetRawOffset( AbstractClass* 目标, CoordStruct& 目标坐标)
{
	return SIBuff::GetFireTargetRawOffset(this, 目标, 目标坐标);
}

double SIBuffClass::GetFacingRotate( SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标)
{
	return SIBuff::GetFacingRotate(this, 跟随旋转类型, 本体坐标, 目标坐标);
}

double SIBuffClass::GetFacingRotate_Same( SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标, SIRotateType 用于同步的跟随旋转类型, double 用于同步的旋转角度_弧度)
{
	return SIBuff::GetFacingRotate_Same(this, 跟随旋转类型, 本体坐标, 目标坐标, 用于同步的跟随旋转类型, 用于同步的旋转角度_弧度);
}

void SIBuffClass::FreshFacingAndAimingRadius_Common( CoordStruct 本体坐标, CoordStruct 本体开火坐标, CoordStruct 目标坐标, CoordStruct& 开火坐标, CoordStruct& 命中坐标)
{
	SIBuff::FreshFacingAndAimingRadius_Common(this, 本体坐标, 本体开火坐标, 目标坐标, 开火坐标, 命中坐标);
}

void SIBuffClass::OffsetMotion_Init()
{
	SIBuff::OffsetMotion_Init(this);
}

void SIBuffClass::OffsetMotion_AI()
{
	SIBuff::OffsetMotion_AI(this);
}

void SIBuffClass::DigitalButtonData_Init( int 单位数量)
{
	SIBuff::DigitalButtonData_Init(this, 单位数量);
}

namespace SIBuff
{

	// ====================================================================================================
	// ==== BuffClass 接口 - 流程函数 =====================================================================
	// ====================================================================================================

	void SI_API TryRemark(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)
	{
		SIBuffDecl_void(TryRemark)(Buff, Buff参数设置包);
	}

	void SI_API TryActive(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)
	{
		SIBuffDecl_void(TryActive)(Buff, Buff参数设置包);
	}

	void SI_API TryAfter(SIBuffClass* Buff)
	{
		SIBuffDecl_void(TryAfter)(Buff);
	}

	void SI_API TryDamage(SIBuffClass* Buff, double 准备造成的伤害)
	{
		SIBuffDecl_void(TryDamage)(Buff, 准备造成的伤害);
	}

	// ====================================================================================================
	// ==== BuffClass 接口 - 属性函数 =====================================================================
	// ====================================================================================================

	void SI_API EnterState(SIBuffClass* Buff, SIBuffClass_State 状态)
	{
		SIBuffDecl_void(EnterState)(Buff, 状态);
	}

	void SI_API MergeSetting(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)
	{
		SIBuffDecl_void(MergeSetting)(Buff, Buff参数设置包);
	}

	CoordStruct SI_API GetCenterCoords(SIBuffClass* Buff)
	{
		SIBuffDecl(GetCenterCoords)(Buff);
	}

	// 返回值可能为空
	TechnoClass* SI_API GetOwnerTechno(SIBuffClass* Buff)
	{
		SIBuffDecl(GetOwnerTechno)(Buff);
	}

	// 返回值可能为空
	TechnoClass* SI_API GetSourceTechno(SIBuffClass* Buff)
	{
		SIBuffDecl(GetSourceTechno)(Buff);
	}

	// 返回值可能为空
	TechnoTypeClass* SI_API GetOwnerTechnoType(SIBuffClass* Buff)
	{
		SIBuffDecl(GetOwnerTechnoType)(Buff);
	}

	// 返回值可能为空
	TechnoTypeClass* SI_API GetSourceTechnoType(SIBuffClass* Buff)
	{
		SIBuffDecl(GetSourceTechnoType)(Buff);
	}

	HouseClass* SI_API GetActiveOwnerHouse(SIBuffClass* Buff)
	{
		SIBuffDecl(GetActiveOwnerHouse)(Buff);
	}

	HouseClass* SI_API GetActiveSourceHouse(SIBuffClass* Buff)
	{
		SIBuffDecl(GetActiveSourceHouse)(Buff);
	}

	// ====================================================================================================
	// ==== BuffClass 接口 - 工具函数 =====================================================================
	// ====================================================================================================

	int SI_API GetEffectMode_A(SIBuffClass* Buff, int 参数项索引, int 最大值)
	{
		SIBuffDecl(GetEffectMode_A)(Buff, 参数项索引, 最大值);
	}

	int SI_API GetEffectMode_B(SIBuffClass* Buff, int 参数项索引, int 最大值, int 最小值)
	{
		SIBuffDecl(GetEffectMode_B)(Buff, 参数项索引, 最大值, 最小值);
	}

	double SI_API GetEffectPower_A(SIBuffClass* Buff, int 参数项索引)
	{
		SIBuffDecl(GetEffectPower_A)(Buff, 参数项索引);
	}

	double SI_API GetEffectPower_B(SIBuffClass* Buff, int 参数项索引, double 最小值)
	{
		SIBuffDecl(GetEffectPower_B)(Buff, 参数项索引, 最小值);
	}

	double SI_API GetEffectTotal_A(SIBuffClass* Buff, int 参数项索引, double 合并后的实际效果强度)
	{
		SIBuffDecl(GetEffectTotal_A)(Buff, 参数项索引, 合并后的实际效果强度);
	}

	double SI_API GetEffectTotal_B(SIBuffClass* Buff, int 参数项索引, double 合并后的实际效果强度, double 最小值)
	{
		SIBuffDecl(GetEffectTotal_B)(Buff, 参数项索引, 合并后的实际效果强度, 最小值);
	}

	double SI_API GetEffectTotalMax(SIBuffClass* Buff, int 参数项索引)
	{
		SIBuffDecl(GetEffectTotalMax)(Buff, 参数项索引);
	}

	double SI_API GetEffectTotalMin(SIBuffClass* Buff, int 参数项索引)
	{
		SIBuffDecl(GetEffectTotalMin)(Buff, 参数项索引);
	}

	bool SI_API Effect_NotPassCommonCheck_A(SIBuffClass* Buff)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_A)(Buff);
	}

	bool SI_API Effect_NotPassCommonCheck_B(SIBuffClass* Buff, SIBuffClass_EffectData* 生效数据)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_B)(Buff, 生效数据);
	}

	bool SI_API Effect_NotPassCommonCheck_C(SIBuffClass* Buff, args_ReceiveDamage* 伤害参数)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_C)(Buff, 伤害参数);
	}

	bool SI_API Effect_NotPassCommonCheck_D(SIBuffClass* Buff, args_ReceiveDamage* 伤害参数, double 当前伤害)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_D)(Buff, 伤害参数, 当前伤害);
	}

	bool SI_API Effect_NotPassCommonCheck_E(SIBuffClass* Buff, double 当前伤害)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_E)(Buff, 当前伤害);
	}

	bool SI_API Effect_NotPassCommonCheck_NoIronCurtain(SIBuffClass* Buff)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_NoIronCurtain)(Buff);
	}

	bool SI_API Effect_PassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型)
	{
		SIBuffDecl(Effect_PassCheck_TechnoTypeList)(Buff, 目标单位类型);
	}

	bool SI_API Effect_NotPassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型)
	{
		SIBuffDecl(Effect_NotPassCheck_TechnoTypeList)(Buff, 目标单位类型);
	}

	bool SI_API Effect_Match_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型)
	{
		SIBuffDecl(Effect_Match_TechnoTypeList)(Buff, 目标单位类型);
	}

	bool SI_API Effect_NotMatch_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型)
	{
		SIBuffDecl(Effect_NotMatch_TechnoTypeList)(Buff, 目标单位类型);
	}

	// 会设置 SIDamageLeft 属性
	void SI_API ReceiveWarheadDamage_A(SIBuffClass* Buff, TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型)
	{
		SIBuffDecl_void(ReceiveWarheadDamage_A)(Buff, 目标单位, 攻击单位, 伤害, 伤害弹头类型);
	}

	// 会设置 SIDamageLeft 属性
	void SI_API ReceiveWarheadDamage_B(SIBuffClass* Buff, TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型, HouseClass* 攻击单位所属作战方)
	{
		SIBuffDecl_void(ReceiveWarheadDamage_B)(Buff, 目标单位, 攻击单位, 伤害, 伤害弹头类型, 攻击单位所属作战方);
	}

	// 会设置 SIDamageLeft 属性
	void SI_API ReceiveHealthDamage_A(SIBuffClass* Buff, TechnoClass* 目标单位, double 生命值损耗, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程)
	{
		SIBuffDecl_void(ReceiveHealthDamage_A)(Buff, 目标单位, 生命值损耗, 结算护甲弹头类型, 死亡伤害弹头类型, 执行自身处理过程);
	}

	// 会设置 SIDamageLeft 属性
	void SI_API ReceiveHealthDamage_B(SIBuffClass* Buff, TechnoClass* 目标单位, double 生命值损耗, double 生命值损耗上限, double 生命值损耗下限, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程)
	{
		SIBuffDecl_void(ReceiveHealthDamage_B)(Buff, 目标单位, 生命值损耗, 生命值损耗上限, 生命值损耗下限, 结算护甲弹头类型, 死亡伤害弹头类型, 执行自身处理过程);
	}

	bool SI_API FreshOrPostBroadcast(SIBuffClass* Buff, double 需要更新的广播强度, AbstractClass* 指向目标)
	{
		SIBuffDecl(FreshOrPostBroadcast)(Buff, 需要更新的广播强度, 指向目标);
	}

	void SI_API ResetBroadcastPower(SIBuffClass* Buff, double 需要更新的广播强度)
	{
		SIBuffDecl_void(ResetBroadcastPower)(Buff, 需要更新的广播强度);
	}

	int SI_API GetBroadcastListenerCount(SIBuffClass* Buff, bool 无视限制条件, AbstractClass* 指向目标)
	{
		SIBuffDecl(GetBroadcastListenerCount)(Buff, 无视限制条件, 指向目标);
	}

	bool SI_API AddAsBroadcastListener(SIBuffClass* Buff)
	{
		SIBuffDecl(AddAsBroadcastListener)(Buff);
	}

	void SI_API RemoveAsBroadcastListener(SIBuffClass* Buff)
	{
		SIBuffDecl_void(RemoveAsBroadcastListener)(Buff);
	}

	double SI_API GetBroadcastPower_Total(SIBuffClass* Buff)
	{
		SIBuffDecl(GetBroadcastPower_Total)(Buff);
	}

	// 返回值可能为空
	TechnoClass* SI_API GetFireOwner(SIBuffClass* Buff)
	{
		SIBuffDecl(GetFireOwner)(Buff);
	}

	bool SI_API GetFireOwnerRawOffset(SIBuffClass* Buff, CoordStruct& 本体坐标)
	{
		SIBuffDecl(GetFireOwnerRawOffset)(Buff, 本体坐标);
	}

	bool SI_API GetFireTargetRawOffset(SIBuffClass* Buff, AbstractClass* 目标, CoordStruct& 目标坐标)
	{
		SIBuffDecl(GetFireTargetRawOffset)(Buff, 目标, 目标坐标);
	}

	double SI_API GetFacingRotate(SIBuffClass* Buff, SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标)
	{
		SIBuffDecl(GetFacingRotate)(Buff, 跟随旋转类型, 本体坐标, 目标坐标);
	}

	double SI_API GetFacingRotate_Same(SIBuffClass* Buff, SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标, SIRotateType 用于同步的跟随旋转类型, double 用于同步的旋转角度_弧度)
	{
		SIBuffDecl(GetFacingRotate_Same)(Buff, 跟随旋转类型, 本体坐标, 目标坐标, 用于同步的跟随旋转类型, 用于同步的旋转角度_弧度);
	}

	void SI_API FreshFacingAndAimingRadius_Common(SIBuffClass* Buff, CoordStruct 本体坐标, CoordStruct 本体开火坐标, CoordStruct 目标坐标, CoordStruct& 开火坐标, CoordStruct& 命中坐标)
	{
		SIBuffDecl_void(FreshFacingAndAimingRadius_Common)(Buff, 本体坐标, 本体开火坐标, 目标坐标, 开火坐标, 命中坐标);
	}

	void SI_API OffsetMotion_Init(SIBuffClass* Buff)
	{
		SIBuffDecl_void(OffsetMotion_Init)(Buff);
	}

	void SI_API OffsetMotion_AI(SIBuffClass* Buff)
	{
		SIBuffDecl_void(OffsetMotion_AI)(Buff);
	}

	void SI_API DigitalButtonData_Init(SIBuffClass* Buff, int 单位数量)
	{
		SIBuffDecl_void(DigitalButtonData_Init)(Buff, 单位数量);
	}
}


// 功能函数
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack(HouseClass* 当前作战方)
{
	return SIClassManager::MakePack(this, 当前作战方);
}
// 构建函数
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_Empty()
{
	return SIClassManager::MakePack_Empty();
}
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_DurationOnly(int 持续时间, bool 覆盖持续时间, bool 承受持续时间抗性)
{
	return SIClassManager::MakePack_DurationOnly(持续时间, 覆盖持续时间, 承受持续时间抗性);
}
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_PowerOnly(double 强度值, bool 覆盖强度值, bool 承受强度值抗性)
{
	return SIClassManager::MakePack_PowerOnly(强度值, 覆盖强度值, 承受强度值抗性);
}