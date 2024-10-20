#include "WIC.ExampleBuff.h"

void _Log(const char* pFormat, ...)
{
	JMP_STD(0x4068E0);
}

void ExampleSIBuff::OnEnterState_Init()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Mark()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Active()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Effect()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_After()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Remove()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectDataInit()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectPowerChanged()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectAI(SIBuffClass_EffectData* 生效数据)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

bool ExampleSIBuff::EffectTriggerFire(AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return true;
}

double ExampleSIBuff::EffectTriggerAttacker(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return 当前伤害;
}

double ExampleSIBuff::EffectTriggerDefender(args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return 当前伤害;
}

double ExampleSIBuff::EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return 当前伤害;
}

double ExampleSIBuff::EffectTriggerDefender_FinalNegative(args_ReceiveDamage* 伤害参数, double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return 当前伤害;
}

void ExampleSIBuff::EffectTriggerDeath(args_ReceiveDamage* 伤害参数)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerKiller(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

double ExampleSIBuff::EffectTriggerSelf_Positive_And_0(double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return 当前伤害;
}

double ExampleSIBuff::EffectTriggerSelf_Negative(double 当前伤害)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");  return 当前伤害;
}

bool ExampleSIBuff::EffectTriggerBroadcast_Accept(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");  return true;
}

void ExampleSIBuff::EffectTriggerBroadcast(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_1_UsePrimary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_2_UseSecondary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_3_UseTertiary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");  return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_4_UseQuaternary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalButtonSetting* ExampleSIBuff::EffectTriggerDrawBar_5_UseButton()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

void ExampleSIBuff::EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* 数值显示设置包)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* 数值显示设置包)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* 数值显示设置包)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* 数值显示设置包)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* 数值显示设置包)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_1_UsePrimary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
	return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_2_UseSecondary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_3_UseTertiary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_4_UseQuaternary()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalButtonSetting* ExampleSIBuff::EffectTriggerClickBar_5_UseButton()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return nullptr;
}

bool ExampleSIBuff::EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n"); return false;
}

void ExampleSIBuff::EffectTriggerClickBar_1_EffectPrimary(HouseClass* 点击的作战方, double 当前值)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_2_EffectSecondary(HouseClass* 点击的作战方, double 当前值)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_3_EffectTertiary(HouseClass* 点击的作战方, double 当前值)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_4_EffectQuaternary(HouseClass* 点击的作战方, double 当前值)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_5_EffectButton(HouseClass* 点击的作战方, double 当前值)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDataReset()
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerPointerGotInvalid(AbstractClass* 寄掉的目标, bool 已移除)
{
	_Log("WIC.SDK : Call Function：\"" __FUNCTION__ "\"\n");
}

// ====================================================================================================
// ========== 处理 ====================================================================================
// ====================================================================================================