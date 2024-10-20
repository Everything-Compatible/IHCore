#pragma once
#include "WIC.Buff.h"

//����WIC�½�Buff�����壬���Է����������Լ���Buff
class ExampleSIBuff final : public SIBuffClass
{
public:
	//���캯���뱣������ʲô��û�С������ĳ�ʼ����д��EffectDataInit������
	ExampleSIBuff() : SIBuffClass() {}
	~ExampleSIBuff() = default;

protected:
	// Ч������
	virtual void OnEnterState_Init() override; // ���� Removed , ���� Modified
	virtual void OnEnterState_Mark() override; // ���� Removed , ���� Modified
	virtual void OnEnterState_Active() override; // ���� Removed , ���� Modified
	virtual void OnEnterState_Effect() override; // ���� Removed , ���� Modified
	virtual void OnEnterState_After() override; // ���� Removed , ���� Modified
	virtual void OnEnterState_Remove() override; // ���� Removed , ���� Modified
	virtual void EffectDataInit() override; // ���� Removed , ���� Modified
	virtual void EffectPowerChanged() override; // ���� Removed , ���� Modified
	virtual void EffectAI(SIBuffClass_EffectData* ��Ч����) override; // ���� Removed , ���� Modified
	virtual bool EffectTriggerFire(AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerAttacker(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender_FinalNegative(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual void EffectTriggerDeath(args_ReceiveDamage* �˺�����) override; // ���� Removed , ���� Modified
	virtual void EffectTriggerKiller(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerSelf_Positive_And_0(double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual double EffectTriggerSelf_Negative(double ��ǰ�˺�) override; // ���� Removed , ���� Modified
	virtual bool EffectTriggerBroadcast_Accept(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��) override;
	virtual void EffectTriggerBroadcast(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��) override; // ���� Removed , ���� Modified
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_1_UsePrimary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_2_UseSecondary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_3_UseTertiary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_4_UseQuaternary() override;
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerDrawBar_5_UseButton() override;
	virtual void EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) override;
	virtual void EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) override;
	virtual void EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) override;
	virtual void EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) override;
	virtual void EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_1_UsePrimary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_2_UseSecondary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_3_UseTertiary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_4_UseQuaternary() override;
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerClickBar_5_UseButton() override;
	virtual bool EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual bool EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) override;
	virtual void EffectTriggerClickBar_1_EffectPrimary(HouseClass* �������ս��, double ��ǰֵ) override;
	virtual void EffectTriggerClickBar_2_EffectSecondary(HouseClass* �������ս��, double ��ǰֵ) override;
	virtual void EffectTriggerClickBar_3_EffectTertiary(HouseClass* �������ս��, double ��ǰֵ) override;
	virtual void EffectTriggerClickBar_4_EffectQuaternary(HouseClass* �������ս��, double ��ǰֵ) override;
	virtual void EffectTriggerClickBar_5_EffectButton(HouseClass* �������ս��, double ��ǰֵ) override;
	virtual void EffectTriggerDataReset() override;
	virtual void EffectTriggerPointerGotInvalid(AbstractClass* �ĵ���Ŀ��, bool ���Ƴ�) override;
};