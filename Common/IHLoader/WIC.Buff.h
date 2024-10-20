#pragma once
#include "WIC.Define.h"
#include "WIC.Template.h"

/*
BuffClass的函数调用是有限制的，
可以看到有些函数后面跟着“存在 Removed”字样，表示函数会影响Buff的生命周期，
只有在拥有这个字样的virtual函数里才能调用改变生命期的函数
*/

class SIBuffClass
{
public:
	//构造函数请保持里面什么都没有。真正的初始化请写到EffectDataInit函数。
	SIBuffClass() : SICacheTargetList(noinit_t()) , SICacheTargetMap(noinit_t()) {};

	//非虚函数
	//SIPack_BuffSetting_FromStatic的获取详见SIPackTypeClass_BuffSetting的函数
	void TryRemark(SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	void TryActive(SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	void TryAfter(); // 存在 Removed , 存在 Modified
	void TryDamage(double 准备造成的伤害); // 存在 Removed , 存在 Modified
	// 接口函数 - 属性函数
	void EnterState(SIBuffClass_State 状态); // 存在 Removed , 存在 Modified
	void MergeSetting(SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	CoordStruct GetCenterCoords();
	TechnoClass* GetOwnerTechno(); // 返回值可能为空
	TechnoClass* GetSourceTechno(); // 返回值可能为空
	TechnoTypeClass* GetOwnerTechnoType(); // 返回值可能为空
	TechnoTypeClass* GetSourceTechnoType(); // 返回值可能为空
	HouseClass* GetActiveOwnerHouse();
	HouseClass* GetActiveSourceHouse();
	// 接口函数 - 工具函数
	int GetEffectMode(int 参数项索引, int 最大值);
	int GetEffectMode(int 参数项索引, int 最大值, int 最小值);
	double GetEffectPower(int 参数项索引);
	double GetEffectPower(int 参数项索引, double 最小值);
	double GetEffectTotal(int 参数项索引, double 合并后的实际效果强度);
	double GetEffectTotal(int 参数项索引, double 合并后的实际效果强度, double 最小值);
	double GetEffectTotalMax(int 参数项索引);
	double GetEffectTotalMin(int 参数项索引);
	bool Effect_NotPassCommonCheck();
	bool Effect_NotPassCommonCheck(SIBuffClass_EffectData* 生效数据);
	bool Effect_NotPassCommonCheck(args_ReceiveDamage* 伤害参数);
	bool Effect_NotPassCommonCheck(args_ReceiveDamage* 伤害参数, double 当前伤害);
	bool Effect_NotPassCommonCheck(double 当前伤害);
	bool Effect_NotPassCommonCheck_NoIronCurtain();
	bool Effect_PassCheck_TechnoTypeList(TechnoTypeClass* 目标单位类型);
	bool Effect_NotPassCheck_TechnoTypeList(TechnoTypeClass* 目标单位类型);
	bool Effect_Match_TechnoTypeList(TechnoTypeClass* 目标单位类型);
	bool Effect_NotMatch_TechnoTypeList(TechnoTypeClass* 目标单位类型);
	void ReceiveWarheadDamage(TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型); // 会设置 SIDamageLeft 属性
	void ReceiveWarheadDamage(TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型, HouseClass* 攻击单位所属作战方); // 会设置 SIDamageLeft 属性
	void ReceiveHealthDamage(TechnoClass* 目标单位, double 生命值损耗, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程); // 会设置 SIDamageLeft 属性
	void ReceiveHealthDamage(TechnoClass* 目标单位, double 生命值损耗, double 生命值损耗上限, double 生命值损耗下限, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程); // 会设置 SIDamageLeft 属性
	bool FreshOrPostBroadcast(double 需要更新的广播强度, AbstractClass* 指向目标);
	void ResetBroadcastPower(double 需要更新的广播强度);
	int GetBroadcastListenerCount(bool 无视限制条件, AbstractClass* 指向目标);
	bool AddAsBroadcastListener();
	void RemoveAsBroadcastListener();
	double GetBroadcastPower_Total();
	TechnoClass* GetFireOwner(); // 返回值可能为空
	bool GetFireOwnerRawOffset(CoordStruct& 本体坐标);
	bool GetFireTargetRawOffset(AbstractClass* 目标, CoordStruct& 目标坐标);
	double GetFacingRotate(SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标);
	double GetFacingRotate_Same(SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标, SIRotateType 用于同步的跟随旋转类型, double 用于同步的旋转角度_弧度);
	void FreshFacingAndAimingRadius_Common(CoordStruct 本体坐标, CoordStruct 本体开火坐标, CoordStruct 目标坐标, CoordStruct& 开火坐标, CoordStruct& 命中坐标);
	void OffsetMotion_Init();
	void OffsetMotion_AI();
	void DigitalButtonData_Init(int 单位数量);

	//虚函数
	virtual void OnEnterState_Init() {} // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Mark() {} // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Active() {} // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Effect() {} // 存在 Removed , 存在 Modified
	virtual void OnEnterState_After() {} // 存在 Removed , 存在 Modified
	virtual void OnEnterState_Remove() {} // 存在 Removed , 存在 Modified
	virtual void EffectDataInit() {} // 存在 Removed , 存在 Modified
	virtual void EffectPowerChanged() {} // 存在 Removed , 存在 Modified
	virtual void EffectTrigger_Lifecycle_Placeholder_0() {} // 预留
	virtual void EffectTrigger_Lifecycle_Placeholder_1() {} // 预留
	virtual void EffectAI(SIBuffClass_EffectData* 生效数据) {} // 存在 Removed , 存在 Modified
	virtual bool EffectTriggerFire(AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器) { return true; } // 存在 Removed , 存在 Modified
	virtual double EffectTriggerAttacker(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender(args_ReceiveDamage* 伤害参数, double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* 伤害参数, double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual double EffectTriggerDefender_FinalNegative(args_ReceiveDamage* 伤害参数, double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual void EffectTriggerDeath(args_ReceiveDamage* 伤害参数) {} // 存在 Removed , 存在 Modified
	virtual void EffectTriggerKiller(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数) {} // 存在 Removed , 存在 Modified
	virtual void EffectTrigger_Outside_Placeholder_0() {} // 预留
	virtual void EffectTrigger_Outside_Placeholder_1() {} // 预留
	virtual void EffectTrigger_Outside_Placeholder_2() {} // 预留
	virtual void EffectTrigger_Outside_Placeholder_3() {} // 预留
	virtual void EffectTrigger_Outside_Placeholder_4() {} // 预留
	virtual void EffectTrigger_Outside_Placeholder_5() {} // 预留
	virtual double EffectTriggerSelf_Positive_And_0(double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual double EffectTriggerSelf_Negative(double 当前伤害) { return 当前伤害; } // 存在 Removed , 存在 Modified
	virtual void EffectTrigger_Self_Placeholder_0() {} // 预留
	virtual void EffectTrigger_Self_Placeholder_1() {} // 预留
	virtual void EffectTrigger_Self_Placeholder_2() {} // 预留
	virtual void EffectTrigger_Self_Placeholder_3() {} // 预留
	virtual bool EffectTriggerBroadcast_Accept(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标) { return true; } // 存在 Removed , 存在 Modified
	virtual void EffectTriggerBroadcast(double 广播强度, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标) {} // 存在 Removed , 存在 Modified
	virtual void EffectTrigger_Inside_Placeholder_0() {} // 预留
	virtual void EffectTrigger_Inside_Placeholder_1() {} // 预留
	virtual void EffectTrigger_Inside_Placeholder_2() {} // 预留
	virtual void EffectTrigger_Inside_Placeholder_3() {} // 预留
	virtual void EffectTrigger_Inside_Placeholder_4() {} // 预留
	virtual void EffectTrigger_Inside_Placeholder_5() {} // 预留
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_1_UsePrimary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_2_UseSecondary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_3_UseTertiary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_4_UseQuaternary() { return nullptr; }
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerDrawBar_5_UseButton() { return nullptr; }
	virtual void EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* 数值显示设置包) {}
	virtual void EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* 数值显示设置包) {}
	virtual void EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* 数值显示设置包) {}
	virtual void EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* 数值显示设置包) {}
	virtual void EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* 数值显示设置包) {}
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_1_UsePrimary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_2_UseSecondary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_3_UseTertiary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_4_UseQuaternary() { return nullptr; }
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerClickBar_5_UseButton() { return nullptr; }
	virtual bool EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual bool EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* 数值显示设置包, HouseClass* 点击的所属方, Point2D* 点击像素偏移) { return false; }
	virtual void EffectTriggerClickBar_1_EffectPrimary(HouseClass* 点击的作战方, double 当前值) {} // 存在 Removed , 存在 Modified
	virtual void EffectTriggerClickBar_2_EffectSecondary(HouseClass* 点击的作战方, double 当前值) {} // 存在 Removed , 存在 Modified
	virtual void EffectTriggerClickBar_3_EffectTertiary(HouseClass* 点击的作战方, double 当前值) {} // 存在 Removed , 存在 Modified
	virtual void EffectTriggerClickBar_4_EffectQuaternary(HouseClass* 点击的作战方, double 当前值) {} // 存在 Removed , 存在 Modified
	virtual void EffectTriggerClickBar_5_EffectButton(HouseClass* 点击的作战方, double 当前值) {} // 存在 Removed , 存在 Modified
	virtual void EffectTrigger_GUI_Placeholder_0() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_1() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_2() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_3() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_4() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_5() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_6() {} // 预留
	virtual void EffectTrigger_GUI_Placeholder_7() {} // 预留
	virtual void EffectTriggerDataReset() {}
	virtual void EffectTriggerPointerGotInvalid(AbstractClass* 寄掉的目标, bool 已移除) {}
	virtual void EffectTrigger_Data_Placeholder_0() {} // 预留
	virtual void EffectTrigger_Data_Placeholder_1() {} // 预留


	// 基础属性，不可在外部改动
	SIBuffTypeClass* Type;
	SIBuffTypeClass_EffectType EffectType;
	int UID;
	// 主体属性，不可在外部改动
	AbstractClass* SIOwner;
	TechnoTypeClass* SIOwner_TechnoType;
	SIInterface_ExtendData* SIOwner_Extend;
	HouseClass* SIOwner_House;
	bool SIOwner_IsCell;
	bool SIOwner_IsObject;
	bool SIOwner_IsTechno;
	bool SIOwner_IsAnimation;
	bool SIOwner_IsBullet;
	bool SIOwner_IsPlaceholder_A;
	bool SIOwner_IsPlaceholder_B;
	bool SIOwner_IsPlaceholder_C;
	AbstractClass* SISource;
	TechnoTypeClass* SISource_TechnoType;
	HouseClass* SISource_House;
	bool SISource_IsCell;
	bool SISource_IsObject;
	bool SISource_IsTechno;
	bool SISource_IsAnimation;
	bool SISource_IsBullet;
	bool SISource_IsPlaceholder_A;
	bool SISource_IsPlaceholder_B;
	bool SISource_IsPlaceholder_C;
	// 状态属性，不可在外部改动
	SIBuffClass_State SIBuffState;
	int SITimeLeft_ActiveAuto;
	int SITimeLeft_ActiveBefore;
	int SITimeLeft_ActiveDelay;
	int SICounterLeft_ActiveCheckDelay;
	int SIDurationLeft;
	double SIHealthLeft;
	double SIPowerLeft;
	int SIPlaceholderLeft_A;
	int SIPlaceholderLeft_B;
	int SIPlaceholderLeft_C;
	int SIPlaceholderLeft_D;
	bool SIIsTemporary;
	bool SICanDamage;
	bool SIPlaceholderState_A;
	bool SIPlaceholderState_B;
	bool SIPlaceholderState_C;
	bool SIPlaceholderState_D;
	//效果属性,可以修改
	//如果想要添加属性，请直接使用SIExtraCode_A到SIEffectData_15的成员存储而不新增
	//这些成员可以在保证占据空间不变的前提下改变其中的类型
	//这些成员会自动存读档，请不要存储指针，否则请自行Swizzle
	AnimClass* SIAnimation;//这个成员改起来小心点，改不好就爆炸
	int SICountLeft;
	int SIDelayLeft;
	int SITimerLeft;
	int SIDamageLeft;
	int SIExtraLeft_0;
	int SIExtraLeft_1;
	int SIExtraLeft_2;
	int SIExtraLeft_3;
	AbstractClass* SICacheTarget;
	SIDataList<AbstractClass*> SICacheTargetList;
	SIDataMap<AbstractClass*, double> SICacheTargetMap;
	int SIExtraCode_A;
	int SIExtraCode_B;
	int SIExtraCode_C;
	int SIExtraCode_D;
	int SIEffectMode_0;
	int SIEffectMode_1;
	int SIEffectMode_2;
	int SIEffectMode_3;
	int SIEffectMode_4;
	int SIEffectMode_5;
	int SIEffectMode_6;
	int SIEffectMode_7;
	int SIEffectMode_8;
	int SIEffectMode_9;
	int SIEffectValue_0;
	int SIEffectValue_1;
	int SIEffectValue_2;
	int SIEffectValue_3;
	int SIEffectValue_4;
	int SIEffectValue_5;
	double SIEffectData_0;
	double SIEffectData_1;
	double SIEffectData_2;
	double SIEffectData_3;
	double SIEffectData_4;
	double SIEffectData_5;
	double SIEffectData_6;
	double SIEffectData_7;
	double SIEffectData_8;
	double SIEffectData_9;
	double SIEffectData_10;
	double SIEffectData_11;
	double SIEffectData_12;
	double SIEffectData_13;
	double SIEffectData_14;
	double SIEffectData_15;
	//其后的成员暂不保证二进制兼容，故不列出
};



class NOVTABLE SIBuffTypeClass final : public SIEnumerable<SIBuffTypeClass>
{
public:
	// 初始化属性
	bool SIIsFirstTimeLoad;
	// 基础属性
	int SIOrder;
	bool SIAllowExist_Transfer;
	bool SIAllowExist_Passenger;
	bool SIAllowExist_Occupant;
	bool SIAllowExist_Powered;
	bool SIAllowPlayer;
	bool SIAllowAI_Easy;
	bool SIAllowAI_Normal;
	bool SIAllowAI_Hard;
	bool SIAllowGameMode_Campaign;
	bool SIAllowGameMode_Other;
	bool SIEnableCodeDamage;
	bool SINeedPowered;
	bool SIWaitingIronCurtain;
	bool SILockOwnerHouse;
	SIDamageProcessType SIDamageProcessType;
	// 动画相关属性
	SIDataList<AnimTypeClass*> SIAnim;
	SIDataList<AnimTypeClass*> SIAnim_Death;
	SIDataList<AnimTypeClass*> SIAnim_Removed;
	SIDataList<AnimTypeClass*> SIAnim_End;
	SIDataList<AnimTypeClass*> SIAnim_BuffDeath;
	// 挂载持续时间相关属性
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	// 生命值相关属性
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Damage;
	// 状态控制相关属性
	bool SIBuff_Multy;
	int SIActive_Auto;
	SIPackTypeClass_BuffSetting* SIActive_Auto_Setting;
	SIPackTypeClass_CheckTechno* SIActive_Auto_Check;
	int SIActive_Auto_CheckDelay;
	int SIActive_Before;
	int SIActive_After;
	int SIActive_Delay;
	bool SIActive_Multy;
	SIBuffTypeClass_AfterType SIAfter_Type;
	SIDataList<SIBuffTypeClass*> SIAfter_NextBuffs;
	SIDataList<SIPackTypeClass_BuffSetting*> SIAfter_NextBuff_Settings;
	// 效果种类相关属性
	SIBuffTypeClass_EffectType SIEffect_Type;
	SIDataList<SIBuffTypeClass*> SIEffect_AcceptBuffs;
	SIDataList<SIBuffTypeClass*> SIEffect_ExceptBuffs;
	SIDataList<TechnoTypeClass*> SIEffect_Technos;
	SIDataList<WeaponTypeClass*> SIEffect_Weapons;
	SIDataList<WarheadTypeClass*> SIEffect_Warheads;
	SIDataList<AnimTypeClass*> SIEffect_Anims;
	SIDataList<AnimTypeClass*> SIEffect_AnimsOthers;
	SIDataPackTypeClass* SIEffect_DataPack;
	SIDataList<int> SIEffect_Damages;
	SIDataList<int> SIEffect_Counts;
	SIDataList<int> SIEffect_Amounts;
	SIDataList<int> SIEffect_AnimDelays;
	SIDataList<int> SIEffect_Modes;
	SIDataList<double> SIEffect_Values;
	SIDataList<double> SIEffect_Healths;
	SIDataList<double> SIEffect_Power_Limits;
	int SIEffect_UnitType;
	int SIEffect_Owner;
	int SIEffect_ExtraCodeA;
	int SIEffect_ExtraCodeB;
	int SIEffect_ExtraCodeC;
	int SIEffect_ExtraCodeD;
	int SIEffect_Timer;
	int SIEffect_Delay;
	double SIEffect_Range;
	bool SIEffect_Random;
	bool SIEffect_Display;
	bool SIEffect_Self;
	bool SIEffect_Other;
	bool SIEffect_Attacker;
	bool SIEffect_Source;
	bool SIEffect_Target;
	bool SIEffect_FromSource;
	bool SIEffect_TargetSource;
	SIDataList<int> SIEffect_OffsetSource;
	SIDataList<int> SIEffect_OffsetSourceBase;
	SIDataList<int> SIEffect_OffsetTarget;
	SIDataList<int> SIEffect_OffsetTargetBase;
	SIPackTypeClass_OffsetMotion* SIEffect_OffsetSourceMotion;
	SIPackTypeClass_OffsetMotion* SIEffect_OffsetSourceBaseMotion;
	SIPackTypeClass_OffsetMotion* SIEffect_OffsetTargetMotion;
	SIPackTypeClass_OffsetMotion* SIEffect_OffsetTargetBaseMotion;
	SIRotateType SIEffect_OffsetSourceDirection;
	SIRotateType SIEffect_OffsetSourceBaseDirection;
	SIRotateType SIEffect_OffsetTargetDirection;
	SIRotateType SIEffect_OffsetTargetBaseDirection;
	// 效果强度值相关属性
	SIDataList<double> SIPower_Bases;
	SIDataList<double> SIPower_Mults;
	SIDataList<double> SIPower_Maxs;
	SIDataList<double> SIPower_Mins;
	SIDataList<double> SIPower_Maxs_Total;
	SIDataList<double> SIPower_Mins_Total;
	SIDataList<double> SIPower_Maxs_Real;
	SIDataList<double> SIPower_Mins_Real;
	SIDataList<double> SIPower_Maxs_Effect;
	SIDataList<double> SIPower_Mins_Effect;
	// 广播与监听相关属性
	bool SIBroadcast;
	SIDataList<int> SIBroadcast_Channels;
	int SIBroadcast_Owner;
	bool SIBroadcast_Fresh;
	bool SIListener;
	SIDataList<int> SIListener_Channels;
	int SIListener_Owner;
	double SIListener_Range;
	bool SIListener_ActiveMode;
	// 数值显示相关属性
	bool SIDigital;
	SIPackTypeClass_DigitalSetting* SIDigital_Primary;
	SIPackTypeClass_DigitalSetting* SIDigital_Secondary;
	SIPackTypeClass_DigitalSetting* SIDigital_Tertiary;
	SIPackTypeClass_DigitalSetting* SIDigital_Quaternary;
	SIPackTypeClass_DigitalButtonSetting* SIDigital_Button;
	SIDataList<SICommandCode> SIDigital_Button_KeyBinds;
	int SIDigital_Button_Owner;
	bool SIDigital_Button_Observer;
	bool SIDigital_AutoOffset;
	double SIDigital_BufferSpeed;
	// 自动生成
	bool SIEffect_NeedCheckBuffs;
	bool SIEffect_IsFull_OffsetSource;
	bool SIEffect_IsFull_OffsetSourceBase;
	bool SIEffect_IsFull_OffsetTarget;
	bool SIEffect_IsFull_OffsetTargetBase;
	// 数据处理函数
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& 流) {};
	virtual void SaveToStream(SIStreamWriter& 流) {};

	SIBuffTypeClass() = delete;
	SIBuffTypeClass(const SIBuffTypeClass&) = delete;
	SIBuffTypeClass(SIBuffTypeClass&& ) = delete;
public:

	// 功能函数 接口被牢大肘飞了
	//WarheadTypeClass* GetRandomWarhead(int& 弹头索引);
	//int GetRandomWarheadIndex();
	//WeaponTypeClass* GetRandomWeapon();
	//int GetRandomWeaponIndex();
	//bool CanEffectBuff(SIBuffTypeClass* Buff类型);
	//bool CanPassBuffsCheck(SIInterface_ExtendData* 数据扩展);
	//bool CannotPassBuffsCheck(SIInterface_ExtendData* 数据扩展);
};


class NOVTABLE SIPackTypeClass_BuffSetting : public SIEnumerable<SIPackTypeClass_BuffSetting>
{
public:
	// 持续时间
	bool SIDuration_Self;
	SIVariableType SIDuration_ValueType;
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	bool SIDuration_Reset;
	bool SIDuration_Effect;
	bool SIDuration_AffectAfter;
	// 生命值
	bool SIHealth_Self;
	SIVariableType SIHealth_ValueType;
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Reset;
	bool SIHealth_Effect;
	// 强度值
	SIVariableType SIPower_ValueType;
	double SIPower;
	double SIPower_Max;
	double SIPower_Min;
	bool SIPower_Reset;
	bool SIPower_Effect;
	// 初始化：！！这种方式不会注册
	SIPackTypeClass_BuffSetting(const char* 片段标题) : SIEnumerable<SIPackTypeClass_BuffSetting>(片段标题)
		// 持续时间
		, SIDuration_Self(SIBuffSetting_DefaultValue_Duration_Self)
		, SIDuration_ValueType(SIVariableType::普通变量)
		, SIDuration(SIBuffSetting_DefaultValue_Duration)
		, SIDuration_Max(SIBuffSetting_DefaultValue_Duration_Max)
		, SIDuration_Min(SIBuffSetting_DefaultValue_Duration_Min)
		, SIDuration_Reset(SIBuffSetting_DefaultValue_Duration_Reset)
		, SIDuration_Effect(SIBuffSetting_DefaultValue_Duration_Effect)
		, SIDuration_AffectAfter(SIBuffSetting_DefaultValue_Duration_AffectAfter)
		// 生命值
		, SIHealth_Self(SIBuffSetting_DefaultValue_Health_Self)
		, SIHealth_ValueType(SIVariableType::普通变量)
		, SIHealth(SIBuffSetting_DefaultValue_Health)
		, SIHealth_Max(SIBuffSetting_DefaultValue_Health_Max)
		, SIHealth_Min(SIBuffSetting_DefaultValue_Health_Min)
		, SIHealth_Reset(SIBuffSetting_DefaultValue_Health_Reset)
		, SIHealth_Effect(SIBuffSetting_DefaultValue_Health_Effect)
		// 强度值
		, SIPower_ValueType(SIVariableType::普通变量)
		, SIPower(SIBuffSetting_DefaultValue_Power)
		, SIPower_Max(SIBuffSetting_DefaultValue_Power_Max)
		, SIPower_Min(SIBuffSetting_DefaultValue_Power_Min)
		, SIPower_Reset(SIBuffSetting_DefaultValue_Power_Reset)
		, SIPower_Effect(SIBuffSetting_DefaultValue_Power_Effect)
	{}
	// 数据处理函数
	~SIPackTypeClass_BuffSetting() = default;
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& 流) {};
	virtual void SaveToStream(SIStreamWriter& 流) {};
public:
	// 功能函数
	SIPack_BuffSetting_FromStatic MakePack(HouseClass* 当前作战方);
	// 构建函数
	static SIPack_BuffSetting_FromStatic MakePack_Empty();
	static SIPack_BuffSetting_FromStatic MakePack_DurationOnly(int 持续时间, bool 覆盖持续时间, bool 承受持续时间抗性);
	static SIPack_BuffSetting_FromStatic MakePack_PowerOnly(double 强度值, bool 覆盖强度值, bool 承受强度值抗性);
};



class NOVTABLE SIPackTypeClass_OffsetMotion final : public SIEnumerable<SIPackTypeClass_OffsetMotion>
{
public:
	// 基础属性
	int SIDuration;
	SIPackTypeClass_OffsetMotion* SINext;
	bool SINextReset;
	// 坐标平移控制
	double SITranslationXSpeed;
	double SITranslationYSpeed;
	double SITranslationZSpeed;
	double SITranslationXStart;
	double SITranslationYStart;
	double SITranslationZStart;
	double SITranslationXMax;
	double SITranslationYMax;
	double SITranslationZMax;
	double SITranslationXMin;
	double SITranslationYMin;
	double SITranslationZMin;
	bool SITranslationXReciprocating;
	bool SITranslationYReciprocating;
	bool SITranslationZReciprocating;
	// 旋转速度控制
	double SIRotateSpeed;
	double SIRotateStart;
	double SIRotateMax;
	double SIRotateMin;
	bool SIRotateReciprocating;
	// 径向速度控制
	double SIRadiusSpeed;
	double SIRadiusStart;
	double SIRadiusMax;
	double SIRadiusMin;
	bool SIRadiusReciprocating;
	// 初始化：！！这种方式不会注册
	SIPackTypeClass_OffsetMotion(const char* 片段标题) : SIEnumerable<SIPackTypeClass_OffsetMotion>(片段标题)
		// 基础属性
		, SIDuration(0)
		, SINext(nullptr)
		, SINextReset(false)
		// 坐标平移控制
		, SITranslationXSpeed(0.0)
		, SITranslationYSpeed(0.0)
		, SITranslationZSpeed(0.0)
		, SITranslationXStart(0.0)
		, SITranslationYStart(0.0)
		, SITranslationZStart(0.0)
		, SITranslationXMax(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMax)
		, SITranslationYMax(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMax)
		, SITranslationZMax(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMax)
		, SITranslationXMin(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMin)
		, SITranslationYMin(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMin)
		, SITranslationZMin(SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMin)
		, SITranslationXReciprocating(false)
		, SITranslationYReciprocating(false)
		, SITranslationZReciprocating(false)
		// 旋转速度控制
		, SIRotateSpeed(0.0)
		, SIRotateStart(0.0)
		, SIRotateMax(360.0)
		, SIRotateMin(0.0)
		, SIRotateReciprocating(false)
		// 径向速度控制
		, SIRadiusSpeed(0.0)
		, SIRadiusStart(1.0)
		, SIRadiusMax(1.0)
		, SIRadiusMin(1.0)
		, SIRadiusReciprocating(false)
	{}
	// 数据处理函数
	~SIPackTypeClass_OffsetMotion() = default;
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& 流) {};
	virtual void SaveToStream(SIStreamWriter& 流) {};
public:
	// 功能函数
};