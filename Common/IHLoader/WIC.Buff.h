#pragma once
#include "WIC.Define.h"
#include "WIC.Template.h"

/*
BuffClass�ĺ��������������Ƶģ�
���Կ�����Щ����������š����� Removed����������ʾ������Ӱ��Buff���������ڣ�
ֻ����ӵ�����������virtual��������ܵ��øı������ڵĺ���
*/

class SIBuffClass
{
public:
	//���캯���뱣������ʲô��û�С������ĳ�ʼ����д��EffectDataInit������
	SIBuffClass() : SICacheTargetList(noinit_t()) , SICacheTargetMap(noinit_t()) {};

	//���麯��
	//SIPack_BuffSetting_FromStatic�Ļ�ȡ���SIPackTypeClass_BuffSetting�ĺ���
	void TryRemark(SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	void TryActive(SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	void TryAfter(); // ���� Removed , ���� Modified
	void TryDamage(double ׼����ɵ��˺�); // ���� Removed , ���� Modified
	// �ӿں��� - ���Ժ���
	void EnterState(SIBuffClass_State ״̬); // ���� Removed , ���� Modified
	void MergeSetting(SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	CoordStruct GetCenterCoords();
	TechnoClass* GetOwnerTechno(); // ����ֵ����Ϊ��
	TechnoClass* GetSourceTechno(); // ����ֵ����Ϊ��
	TechnoTypeClass* GetOwnerTechnoType(); // ����ֵ����Ϊ��
	TechnoTypeClass* GetSourceTechnoType(); // ����ֵ����Ϊ��
	HouseClass* GetActiveOwnerHouse();
	HouseClass* GetActiveSourceHouse();
	// �ӿں��� - ���ߺ���
	int GetEffectMode(int ����������, int ���ֵ);
	int GetEffectMode(int ����������, int ���ֵ, int ��Сֵ);
	double GetEffectPower(int ����������);
	double GetEffectPower(int ����������, double ��Сֵ);
	double GetEffectTotal(int ����������, double �ϲ����ʵ��Ч��ǿ��);
	double GetEffectTotal(int ����������, double �ϲ����ʵ��Ч��ǿ��, double ��Сֵ);
	double GetEffectTotalMax(int ����������);
	double GetEffectTotalMin(int ����������);
	bool Effect_NotPassCommonCheck();
	bool Effect_NotPassCommonCheck(SIBuffClass_EffectData* ��Ч����);
	bool Effect_NotPassCommonCheck(args_ReceiveDamage* �˺�����);
	bool Effect_NotPassCommonCheck(args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	bool Effect_NotPassCommonCheck(double ��ǰ�˺�);
	bool Effect_NotPassCommonCheck_NoIronCurtain();
	bool Effect_PassCheck_TechnoTypeList(TechnoTypeClass* Ŀ�굥λ����);
	bool Effect_NotPassCheck_TechnoTypeList(TechnoTypeClass* Ŀ�굥λ����);
	bool Effect_Match_TechnoTypeList(TechnoTypeClass* Ŀ�굥λ����);
	bool Effect_NotMatch_TechnoTypeList(TechnoTypeClass* Ŀ�굥λ����);
	void ReceiveWarheadDamage(TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����); // ������ SIDamageLeft ����
	void ReceiveWarheadDamage(TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����, HouseClass* ������λ������ս��); // ������ SIDamageLeft ����
	void ReceiveHealthDamage(TechnoClass* Ŀ�굥λ, double ����ֵ���, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������); // ������ SIDamageLeft ����
	void ReceiveHealthDamage(TechnoClass* Ŀ�굥λ, double ����ֵ���, double ����ֵ�������, double ����ֵ�������, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������); // ������ SIDamageLeft ����
	bool FreshOrPostBroadcast(double ��Ҫ���µĹ㲥ǿ��, AbstractClass* ָ��Ŀ��);
	void ResetBroadcastPower(double ��Ҫ���µĹ㲥ǿ��);
	int GetBroadcastListenerCount(bool ������������, AbstractClass* ָ��Ŀ��);
	bool AddAsBroadcastListener();
	void RemoveAsBroadcastListener();
	double GetBroadcastPower_Total();
	TechnoClass* GetFireOwner(); // ����ֵ����Ϊ��
	bool GetFireOwnerRawOffset(CoordStruct& ��������);
	bool GetFireTargetRawOffset(AbstractClass* Ŀ��, CoordStruct& Ŀ������);
	double GetFacingRotate(SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������);
	double GetFacingRotate_Same(SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������, SIRotateType ����ͬ���ĸ�����ת����, double ����ͬ������ת�Ƕ�_����);
	void FreshFacingAndAimingRadius_Common(CoordStruct ��������, CoordStruct ���忪������, CoordStruct Ŀ������, CoordStruct& ��������, CoordStruct& ��������);
	void OffsetMotion_Init();
	void OffsetMotion_AI();
	void DigitalButtonData_Init(int ��λ����);

	//�麯��
	virtual void OnEnterState_Init() {} // ���� Removed , ���� Modified
	virtual void OnEnterState_Mark() {} // ���� Removed , ���� Modified
	virtual void OnEnterState_Active() {} // ���� Removed , ���� Modified
	virtual void OnEnterState_Effect() {} // ���� Removed , ���� Modified
	virtual void OnEnterState_After() {} // ���� Removed , ���� Modified
	virtual void OnEnterState_Remove() {} // ���� Removed , ���� Modified
	virtual void EffectDataInit() {} // ���� Removed , ���� Modified
	virtual void EffectPowerChanged() {} // ���� Removed , ���� Modified
	virtual void EffectTrigger_Lifecycle_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_Lifecycle_Placeholder_1() {} // Ԥ��
	virtual void EffectAI(SIBuffClass_EffectData* ��Ч����) {} // ���� Removed , ���� Modified
	virtual bool EffectTriggerFire(AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������) { return true; } // ���� Removed , ���� Modified
	virtual double EffectTriggerAttacker(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender_FinalPositive_And_0(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual double EffectTriggerDefender_FinalNegative(args_ReceiveDamage* �˺�����, double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual void EffectTriggerDeath(args_ReceiveDamage* �˺�����) {} // ���� Removed , ���� Modified
	virtual void EffectTriggerKiller(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����) {} // ���� Removed , ���� Modified
	virtual void EffectTrigger_Outside_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_Outside_Placeholder_1() {} // Ԥ��
	virtual void EffectTrigger_Outside_Placeholder_2() {} // Ԥ��
	virtual void EffectTrigger_Outside_Placeholder_3() {} // Ԥ��
	virtual void EffectTrigger_Outside_Placeholder_4() {} // Ԥ��
	virtual void EffectTrigger_Outside_Placeholder_5() {} // Ԥ��
	virtual double EffectTriggerSelf_Positive_And_0(double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual double EffectTriggerSelf_Negative(double ��ǰ�˺�) { return ��ǰ�˺�; } // ���� Removed , ���� Modified
	virtual void EffectTrigger_Self_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_Self_Placeholder_1() {} // Ԥ��
	virtual void EffectTrigger_Self_Placeholder_2() {} // Ԥ��
	virtual void EffectTrigger_Self_Placeholder_3() {} // Ԥ��
	virtual bool EffectTriggerBroadcast_Accept(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��) { return true; } // ���� Removed , ���� Modified
	virtual void EffectTriggerBroadcast(double �㲥ǿ��, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��) {} // ���� Removed , ���� Modified
	virtual void EffectTrigger_Inside_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_Inside_Placeholder_1() {} // Ԥ��
	virtual void EffectTrigger_Inside_Placeholder_2() {} // Ԥ��
	virtual void EffectTrigger_Inside_Placeholder_3() {} // Ԥ��
	virtual void EffectTrigger_Inside_Placeholder_4() {} // Ԥ��
	virtual void EffectTrigger_Inside_Placeholder_5() {} // Ԥ��
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_1_UsePrimary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_2_UseSecondary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_3_UseTertiary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerDrawBar_4_UseQuaternary() { return nullptr; }
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerDrawBar_5_UseButton() { return nullptr; }
	virtual void EffectTriggerDrawBar_1_SetDataPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) {}
	virtual void EffectTriggerDrawBar_2_SetDataSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) {}
	virtual void EffectTriggerDrawBar_3_SetDataTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) {}
	virtual void EffectTriggerDrawBar_4_SetDataQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) {}
	virtual void EffectTriggerDrawBar_5_SetDataButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�) {}
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_1_UsePrimary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_2_UseSecondary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_3_UseTertiary() { return nullptr; }
	virtual SIPackTypeClass_DigitalSetting* EffectTriggerClickBar_4_UseQuaternary() { return nullptr; }
	virtual SIPackTypeClass_DigitalButtonSetting* EffectTriggerClickBar_5_UseButton() { return nullptr; }
	virtual bool EffectTriggerClickBar_1_HoverPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_2_HoverSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_3_HoverTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_4_HoverQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_5_HoverButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_1_TryClickPrimary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_2_TryClickSecondary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_3_TryClickTertiary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_4_TryClickQuaternary(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual bool EffectTriggerClickBar_5_TryClickButton(SIPack_DigitalSetting* ��ֵ��ʾ���ð�, HouseClass* �����������, Point2D* �������ƫ��) { return false; }
	virtual void EffectTriggerClickBar_1_EffectPrimary(HouseClass* �������ս��, double ��ǰֵ) {} // ���� Removed , ���� Modified
	virtual void EffectTriggerClickBar_2_EffectSecondary(HouseClass* �������ս��, double ��ǰֵ) {} // ���� Removed , ���� Modified
	virtual void EffectTriggerClickBar_3_EffectTertiary(HouseClass* �������ս��, double ��ǰֵ) {} // ���� Removed , ���� Modified
	virtual void EffectTriggerClickBar_4_EffectQuaternary(HouseClass* �������ս��, double ��ǰֵ) {} // ���� Removed , ���� Modified
	virtual void EffectTriggerClickBar_5_EffectButton(HouseClass* �������ս��, double ��ǰֵ) {} // ���� Removed , ���� Modified
	virtual void EffectTrigger_GUI_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_1() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_2() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_3() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_4() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_5() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_6() {} // Ԥ��
	virtual void EffectTrigger_GUI_Placeholder_7() {} // Ԥ��
	virtual void EffectTriggerDataReset() {}
	virtual void EffectTriggerPointerGotInvalid(AbstractClass* �ĵ���Ŀ��, bool ���Ƴ�) {}
	virtual void EffectTrigger_Data_Placeholder_0() {} // Ԥ��
	virtual void EffectTrigger_Data_Placeholder_1() {} // Ԥ��


	// �������ԣ��������ⲿ�Ķ�
	SIBuffTypeClass* Type;
	SIBuffTypeClass_EffectType EffectType;
	int UID;
	// �������ԣ��������ⲿ�Ķ�
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
	// ״̬���ԣ��������ⲿ�Ķ�
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
	//Ч������,�����޸�
	//�����Ҫ������ԣ���ֱ��ʹ��SIExtraCode_A��SIEffectData_15�ĳ�Ա�洢��������
	//��Щ��Ա�����ڱ�֤ռ�ݿռ䲻���ǰ���¸ı����е�����
	//��Щ��Ա���Զ���������벻Ҫ�洢ָ�룬����������Swizzle
	AnimClass* SIAnimation;//�����Ա������С�ĵ㣬�Ĳ��þͱ�ը
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
	//���ĳ�Ա�ݲ���֤�����Ƽ��ݣ��ʲ��г�
};



class NOVTABLE SIBuffTypeClass final : public SIEnumerable<SIBuffTypeClass>
{
public:
	// ��ʼ������
	bool SIIsFirstTimeLoad;
	// ��������
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
	// �����������
	SIDataList<AnimTypeClass*> SIAnim;
	SIDataList<AnimTypeClass*> SIAnim_Death;
	SIDataList<AnimTypeClass*> SIAnim_Removed;
	SIDataList<AnimTypeClass*> SIAnim_End;
	SIDataList<AnimTypeClass*> SIAnim_BuffDeath;
	// ���س���ʱ���������
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	// ����ֵ�������
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Damage;
	// ״̬�����������
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
	// Ч�������������
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
	// Ч��ǿ��ֵ�������
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
	// �㲥������������
	bool SIBroadcast;
	SIDataList<int> SIBroadcast_Channels;
	int SIBroadcast_Owner;
	bool SIBroadcast_Fresh;
	bool SIListener;
	SIDataList<int> SIListener_Channels;
	int SIListener_Owner;
	double SIListener_Range;
	bool SIListener_ActiveMode;
	// ��ֵ��ʾ�������
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
	// �Զ�����
	bool SIEffect_NeedCheckBuffs;
	bool SIEffect_IsFull_OffsetSource;
	bool SIEffect_IsFull_OffsetSourceBase;
	bool SIEffect_IsFull_OffsetTarget;
	bool SIEffect_IsFull_OffsetTargetBase;
	// ���ݴ�����
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& ��) {};
	virtual void SaveToStream(SIStreamWriter& ��) {};

	SIBuffTypeClass() = delete;
	SIBuffTypeClass(const SIBuffTypeClass&) = delete;
	SIBuffTypeClass(SIBuffTypeClass&& ) = delete;
public:

	// ���ܺ��� �ӿڱ��δ������
	//WarheadTypeClass* GetRandomWarhead(int& ��ͷ����);
	//int GetRandomWarheadIndex();
	//WeaponTypeClass* GetRandomWeapon();
	//int GetRandomWeaponIndex();
	//bool CanEffectBuff(SIBuffTypeClass* Buff����);
	//bool CanPassBuffsCheck(SIInterface_ExtendData* ������չ);
	//bool CannotPassBuffsCheck(SIInterface_ExtendData* ������չ);
};


class NOVTABLE SIPackTypeClass_BuffSetting : public SIEnumerable<SIPackTypeClass_BuffSetting>
{
public:
	// ����ʱ��
	bool SIDuration_Self;
	SIVariableType SIDuration_ValueType;
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	bool SIDuration_Reset;
	bool SIDuration_Effect;
	bool SIDuration_AffectAfter;
	// ����ֵ
	bool SIHealth_Self;
	SIVariableType SIHealth_ValueType;
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Reset;
	bool SIHealth_Effect;
	// ǿ��ֵ
	SIVariableType SIPower_ValueType;
	double SIPower;
	double SIPower_Max;
	double SIPower_Min;
	bool SIPower_Reset;
	bool SIPower_Effect;
	// ��ʼ�����������ַ�ʽ����ע��
	SIPackTypeClass_BuffSetting(const char* Ƭ�α���) : SIEnumerable<SIPackTypeClass_BuffSetting>(Ƭ�α���)
		// ����ʱ��
		, SIDuration_Self(SIBuffSetting_DefaultValue_Duration_Self)
		, SIDuration_ValueType(SIVariableType::��ͨ����)
		, SIDuration(SIBuffSetting_DefaultValue_Duration)
		, SIDuration_Max(SIBuffSetting_DefaultValue_Duration_Max)
		, SIDuration_Min(SIBuffSetting_DefaultValue_Duration_Min)
		, SIDuration_Reset(SIBuffSetting_DefaultValue_Duration_Reset)
		, SIDuration_Effect(SIBuffSetting_DefaultValue_Duration_Effect)
		, SIDuration_AffectAfter(SIBuffSetting_DefaultValue_Duration_AffectAfter)
		// ����ֵ
		, SIHealth_Self(SIBuffSetting_DefaultValue_Health_Self)
		, SIHealth_ValueType(SIVariableType::��ͨ����)
		, SIHealth(SIBuffSetting_DefaultValue_Health)
		, SIHealth_Max(SIBuffSetting_DefaultValue_Health_Max)
		, SIHealth_Min(SIBuffSetting_DefaultValue_Health_Min)
		, SIHealth_Reset(SIBuffSetting_DefaultValue_Health_Reset)
		, SIHealth_Effect(SIBuffSetting_DefaultValue_Health_Effect)
		// ǿ��ֵ
		, SIPower_ValueType(SIVariableType::��ͨ����)
		, SIPower(SIBuffSetting_DefaultValue_Power)
		, SIPower_Max(SIBuffSetting_DefaultValue_Power_Max)
		, SIPower_Min(SIBuffSetting_DefaultValue_Power_Min)
		, SIPower_Reset(SIBuffSetting_DefaultValue_Power_Reset)
		, SIPower_Effect(SIBuffSetting_DefaultValue_Power_Effect)
	{}
	// ���ݴ�����
	~SIPackTypeClass_BuffSetting() = default;
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& ��) {};
	virtual void SaveToStream(SIStreamWriter& ��) {};
public:
	// ���ܺ���
	SIPack_BuffSetting_FromStatic MakePack(HouseClass* ��ǰ��ս��);
	// ��������
	static SIPack_BuffSetting_FromStatic MakePack_Empty();
	static SIPack_BuffSetting_FromStatic MakePack_DurationOnly(int ����ʱ��, bool ���ǳ���ʱ��, bool ���ܳ���ʱ�俹��);
	static SIPack_BuffSetting_FromStatic MakePack_PowerOnly(double ǿ��ֵ, bool ����ǿ��ֵ, bool ����ǿ��ֵ����);
};



class NOVTABLE SIPackTypeClass_OffsetMotion final : public SIEnumerable<SIPackTypeClass_OffsetMotion>
{
public:
	// ��������
	int SIDuration;
	SIPackTypeClass_OffsetMotion* SINext;
	bool SINextReset;
	// ����ƽ�ƿ���
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
	// ��ת�ٶȿ���
	double SIRotateSpeed;
	double SIRotateStart;
	double SIRotateMax;
	double SIRotateMin;
	bool SIRotateReciprocating;
	// �����ٶȿ���
	double SIRadiusSpeed;
	double SIRadiusStart;
	double SIRadiusMax;
	double SIRadiusMin;
	bool SIRadiusReciprocating;
	// ��ʼ�����������ַ�ʽ����ע��
	SIPackTypeClass_OffsetMotion(const char* Ƭ�α���) : SIEnumerable<SIPackTypeClass_OffsetMotion>(Ƭ�α���)
		// ��������
		, SIDuration(0)
		, SINext(nullptr)
		, SINextReset(false)
		// ����ƽ�ƿ���
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
		// ��ת�ٶȿ���
		, SIRotateSpeed(0.0)
		, SIRotateStart(0.0)
		, SIRotateMax(360.0)
		, SIRotateMin(0.0)
		, SIRotateReciprocating(false)
		// �����ٶȿ���
		, SIRadiusSpeed(0.0)
		, SIRadiusStart(1.0)
		, SIRadiusMax(1.0)
		, SIRadiusMin(1.0)
		, SIRadiusReciprocating(false)
	{}
	// ���ݴ�����
	~SIPackTypeClass_OffsetMotion() = default;
	virtual void InitializeConstants() {};
	virtual void LoadFromINI(CCINIClass* INI) {};
	virtual void LoadFromStream(SIStreamReader& ��) {};
	virtual void SaveToStream(SIStreamWriter& ��) {};
public:
	// ���ܺ���
};