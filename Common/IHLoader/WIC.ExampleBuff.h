#pragma once
#include "WIC.Buff.h"

//这是WIC新建Buff的样板，可以仿照着制作自己的Buff
class ExampleSIBuff final : public SIBuffClass
{
public:
	//构造函数请保持里面什么都没有。真正的初始化请写到EffectDataInit函数。
	ExampleSIBuff() : SIBuffClass() {}
	~ExampleSIBuff() = default;

protected:
	// 效果函数
	virtual void OnEnterState_Init() override; // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Mark() override; // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Active() override; // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Effect() override; // 存在 Removed , 存在 Modified
	virtual void OnEnterState_After() override; // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Remove() override; // 存在 Removed , 存在 Modified
	virtual void EffectDataInit() override; // 存在 Removed , 存在 Modified
	virtual void EffectPowerChanged() override; // 存在 Removed , 存在 Modified
	virtual void EffectAI(SIBuffClass_EffectData* 生效数据) override; // 存在 Removed , 存在 Modified
	virtual bool EffectTriggerFire(AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerAttacker(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender(args_ReceiveDamage* 伤害参数, double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* 伤害参数, double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender_FinalNegative(args_ReceiveDamage* 伤害参数, double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual void EffectTriggerDeath(args_ReceiveDamage* 伤害参数) override; // 存在 Removed , 存在 Modified
	virtual void EffectTriggerKiller(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerSelf_Positive_And_0(double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual double EffectTriggerSelf_Negative(double 当前伤害) override; // 存在 Removed , 存在 Modified
	virtual bool EffectTriggerBroadcast_Accept(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标) override;
	virtual void EffectTriggerBroadcast(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标) override; // 存在 Removed , 存在 Modified
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_1_UsePrimary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_2_UseSecondary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_3_UseTertiary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_4_UseQuaternary() override;
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerDrawBar_5_UseButton() override;
	virtual void EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* 数值显示设置包) override;
	virtual void EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* 数值显示设置包) override;
	virtual void EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* 数值显示设置包) override;
	virtual void EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* 数值显示设置包) override;
	virtual void EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* 数值显示设置包) override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_1_UsePrimary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_2_UseSecondary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_3_UseTertiary() override;
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_4_UseQuaternary() override;
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerClickBar_5_UseButton() override;
	virtual bool EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual bool EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) override;
	virtual void EffectTriggerClickBar_1_EffectPrimary(HouseClass* 点击的作战方, double 当前值) override;
	virtual void EffectTriggerClickBar_2_EffectSecondary(HouseClass* 点击的作战方, double 当前值) override;
	virtual void EffectTriggerClickBar_3_EffectTertiary(HouseClass* 点击的作战方, double 当前值) override;
	virtual void EffectTriggerClickBar_4_EffectQuaternary(HouseClass* 点击的作战方, double 当前值) override;
	virtual void EffectTriggerClickBar_5_EffectButton(HouseClass* 点击的作战方, double 当前值) override;
	virtual void EffectTriggerDataReset() override;
	virtual void EffectTriggerPointerGotInvalid(AbstractClass* 寄掉的目标, bool 已移除) override;
};