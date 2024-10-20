#include "WIC.ExampleBuff.h"

void _Log(const char* pFormat, ...)
{
	JMP_STD(0x4068E0);
}

void ExampleSIBuff::OnEnterState_Init()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Mark()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Active()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Effect()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_After()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::OnEnterState_Remove()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectDataInit()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectPowerChanged()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectAI(SIBuffClass_EffectData* ��Ч����)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

bool ExampleSIBuff::EffectTriggerFire(AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return true;
}

double ExampleSIBuff::EffectTriggerAttacker(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return ��ǰ�˺�;
}

double ExampleSIBuff::EffectTriggerDefender(args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return ��ǰ�˺�;
}

double ExampleSIBuff::EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return ��ǰ�˺�;
}

double ExampleSIBuff::EffectTriggerDefender_FinalNegative(args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return ��ǰ�˺�;
}

void ExampleSIBuff::EffectTriggerDeath(args_ReceiveDamage* �˺�����)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerKiller(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

double ExampleSIBuff::EffectTriggerSelf_Positive_And_0(double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return ��ǰ�˺�;
}

double ExampleSIBuff::EffectTriggerSelf_Negative(double ��ǰ�˺�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");  return ��ǰ�˺�;
}

bool ExampleSIBuff::EffectTriggerBroadcast_Accept(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");  return true;
}

void ExampleSIBuff::EffectTriggerBroadcast(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_1_UsePrimary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_2_UseSecondary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_3_UseTertiary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");  return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerDrawBar_4_UseQuaternary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalButtonSetting* ExampleSIBuff::EffectTriggerDrawBar_5_UseButton()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

void ExampleSIBuff::EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_1_UsePrimary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
	return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_2_UseSecondary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_3_UseTertiary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalSetting* ExampleSIBuff::EffectTriggerClickBar_4_UseQuaternary()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

SIPackTypeClass_DigitalButtonSetting* ExampleSIBuff::EffectTriggerClickBar_5_UseButton()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return nullptr;
}

bool ExampleSIBuff::EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

bool ExampleSIBuff::EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n"); return false;
}

void ExampleSIBuff::EffectTriggerClickBar_1_EffectPrimary(HouseClass* �������ս��, double ��ǰֵ)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_2_EffectSecondary(HouseClass* �������ս��, double ��ǰֵ)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_3_EffectTertiary(HouseClass* �������ս��, double ��ǰֵ)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_4_EffectQuaternary(HouseClass* �������ս��, double ��ǰֵ)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerClickBar_5_EffectButton(HouseClass* �������ս��, double ��ǰֵ)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerDataReset()
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

void ExampleSIBuff::EffectTriggerPointerGotInvalid(AbstractClass* �ĵ���Ŀ��, bool ���Ƴ�)
{
	_Log("WIC.SDK : Call Function��\"" __FUNCTION__ "\"\n");
}

// ====================================================================================================
// ========== ���� ====================================================================================
// ====================================================================================================