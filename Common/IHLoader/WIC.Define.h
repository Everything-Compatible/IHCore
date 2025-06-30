#pragma once

/*
����������ʹ�ô�ͷ�ļ����е�ԭʼ�ӿڣ���
*/


#include "IH.Ext.h"

#define WIC_LIBRARY_VERSION 10

#define WIC_LibName "SIWinterIsComing"
#define SIInterfaceDecl_Impl(Name) \
	static FuncInfo* fn { nullptr };\
	if (!WIC.Available())WIC = Ext::GetLib(WIC_LibName);\
	if (!WIC.Available())throw SIException("SIInterface : �޷����� " WIC_LibName " ��");\
	if(!fn) fn = WIC.QueryFunction(Name, WIC_LIBRARY_VERSION);\
	if(!fn || !fn->Func)throw SIException("SIInterface : �ӿ� " Name " �����á�");
#define SIInterfaceDecl(Name, Function) \
	SIInterfaceDecl_Impl(Name)\
	return fn->SeeAsType<decltype(Function)>()
#define SIInterfaceDecl_void(Name, Function) \
	SIInterfaceDecl_Impl(Name)\
	fn->SeeAsType<decltype(Function)>()
#define SIInterface_ExtendDataTable__InitFunction__STR( Value ) #Value

#define SIInterface_ExtendDataTable__InitFunction__Prefix( FunctionName ) \
SIInterface_ExtendDataTable__InitFunction__STR( WIC.IExt.##FunctionName )
#define SIInterface(Function) \
	SIInterfaceDecl(SIInterface_ExtendDataTable__InitFunction__Prefix(Function), Function)
#define SIInterface_void(Function) \
	SIInterfaceDecl_void(SIInterface_ExtendDataTable__InitFunction__Prefix(Function), Function)

#define SIInterface_Buff__InitFunction__Prefix( FunctionName ) \
SIInterface_ExtendDataTable__InitFunction__STR( WIC.Buff.##FunctionName )
#define SIBuffDecl(Function) \
	SIInterfaceDecl(SIInterface_Buff__InitFunction__Prefix(Function), Function)
#define SIBuffDecl_void(Function) \
	SIInterfaceDecl_void(SIInterface_Buff__InitFunction__Prefix(Function), Function)

#define SIInterface_HouseExt__InitFunction__Prefix( FunctionName ) \
SIInterface_ExtendDataTable__InitFunction__STR( WIC.HExt.##FunctionName )
#define SIHouseExtDecl(Function) \
	SIInterfaceDecl(SIInterface_HouseExt__InitFunction__Prefix(Function), Function)
#define SIHouseExtDecl_void(Function) \
	SIInterfaceDecl_void(SIInterface_HouseExt__InitFunction__Prefix(Function), Function)


extern Ext::LibData WIC;


/*
 SIException ������WIC�ӿ�ʱ���ܻ��׳����쳣��
 �����׳�SIException����������֣�
 һ��WICδ�ɹ����롣���ڵ���WIC��API֮ǰͨ��SI::Available()��顣
	�ڷ���һ��true֮��WIC�����ǿ�����ȷ���ص��ˡ�
	�������뷢���ڶԾֿ�ʼ֮ǰ��ͨ��ֻ�迪��ʱ���һ�Ρ�
 ����WIC�����ˣ����޷���ȡAPI��
	�ܿ����ǰ������°��SDK��ʵ��װ���˾ɵ�DLL��
	ͨ������WIC�İ汾���Ի���������⡣
 ������ڵ���API��ʱ��Ž�try�鵱�У�Ȼ��catch������⣬
 ��ȷ���׳��쳣��������ᷢ����
*/
class SIException :public std::exception
{
	const char* Info;
public:
	SIException(const char* info);
	const char* what() const noexcept override;
};

class SIBuffTypeClass;
class SIBuffClass;
class SIElementResistanceTypeClass;
class SIElementTypeClass;
class SIPackTypeClass_CheckTechno;
class args_ReceiveDamage;
class SIInterface_ExtendData;
class SIPackTypeClass_BuffSetting;
class SIPackTypeClass_DigitalSetting;
class SIPackTypeClass_DigitalButtonSetting;
class SIDataPackTypeClass;
class SIPackTypeClass_OffsetMotion;
class SIStreamReader;
class SIStreamWriter;
class SIBroadcastClass;
template<typename T>
class SIConstVector;
template <typename T>
class SIDataList;
class SIHouse_ExtendData;//Actually SIExtend_House::ExtendData

enum SIEXPSourceType : unsigned int
{
	������Դ_δ֪ = 0x0,

	������Դ_��ɱ = 0x1,
	������Դ_�������� = 0x2,
	������Դ_ѵ���� = 0x4,
	������Դ_�����͸ = 0x8,
	������Դ_��ͷ�ṩ = 0x10,
	������Դ_Buff�ṩ = 0x20,
	������Դ_���侭��ֵ = 0x40,

	������Դ_ϵͳ�ṩ = 0x10000,
	������Դ_ϵͳ�۳� = 0x20000
};

enum SICommandCode : int
{
	��Ч���� = 0,

	����_01_��ݼ�A = 1,
	����_02_��ݼ�B = 2,
	����_03_��ݼ�C = 3,
	����_04_��ݼ�D = 4,
	����_05_��ݼ�E = 5,
	����_06_��ݼ�F = 6,
	����_07_��ݼ�G = 7,
	����_08_��ݼ�H = 8,
	����_09_��ݼ�I = 9,
	����_10_��ݼ�J = 10,
	����_11_��ݼ�K = 11,
	����_12_��ݼ�L = 12,

	����_��Сֵ = ����_01_��ݼ�A,
	����_���ֵ = ����_12_��ݼ�L
};

enum SIDigitalClickBarIndex : int
{
	��Ч��� = 0,
	���_1_Primary = 1,
	���_2_Secondary = 2,
	���_3_Tertiary = 3,
	���_4_Quaternary = 4,
	���_5_Button = 5
};

struct SIBuffClass_EffectData
{
public:
	// ����
	bool ��ֹ��Ч = false;
};

#define SIBuffClass_InfinityValue -1
#define SIBuffClass_CounterDefaultValue 0
#define SIBuffClass_Count_MaxValue 1000000000

#define SIBuffClass_CountLeft_MAX 1000000000 // ���ֵΪ 10e
#define SIBuffClass_CountLeft_MIN 0 // ��СֵΪ 0

#define SIBuffClass_DelayLeft_MAX 1000000000 // ���ֵΪ 10e
#define SIBuffClass_DelayLeft_MIN 0 // ��СֵΪ 0

#define SIBuffClass_TimerLeft_MAX 1000000000 // ���ֵΪ 10e
#define SIBuffClass_TimerLeft_MIN 0 // ��СֵΪ 0

#define SIBuffClass_PowerLeft_MAX 1000000000.0 // ���ֵΪ 10e
#define SIBuffClass_PowerLeft_MIN -1000000000.0 // ��СֵΪ -10e

#define SIBuffClass_DefaultValue_CacheTargetValue 0.0

enum SIBuffClass_State : int
{
	δ��ʼ�� = 99,
	���� = 0,
	���� = 1,
	��Ч = 2,
	���� = 3,
	�Ƴ� = 100
};

// ˮƽ��������
#define SIAlignHorizontalType_None "None"
#define SIAlignHorizontalType_Left "Left"
#define SIAlignHorizontalType_Right "Right"
#define SIAlignHorizontalType_Center "Center"

enum SIAlignHorizontalType : unsigned int
{
	ˮƽ����_�� = 0,
	ˮƽ����_�� = 1,
	ˮƽ����_�� = 2,
	ˮƽ����_���� = 3
};

// ��ֱ��������
#define SIAlignVerticalType_None "None"
#define SIAlignVerticalType_Top "Top"
#define SIAlignVerticalType_Bottom "Bottom"
#define SIAlignVerticalType_Center "Center"

enum SIAlignVerticalType : unsigned int
{
	��ֱ����_�� = 0,
	��ֱ����_�� = 1,
	��ֱ����_�� = 2,
	��ֱ����_���� = 3
};

// ��ֵ��ʾ_��ʾ����
#define SIDigitalDisplayType_Default "Default"
#define SIDigitalDisplayType_Horizontal "Horizontal"
#define SIDigitalDisplayType_Hexagon "Hexagon"
#define SIDigitalDisplayType_Hexagon2 "Hexagon2"

enum SIDigitalDisplayType
{
	��ֵ��ʾ_��ʾ����_Ĭ�� = 0,
	��ֵ��ʾ_��ʾ����_ˮƽ = 1,
	��ֵ��ʾ_��ʾ����_������ = 3,
	��ֵ��ʾ_��ʾ����_������2 = 4
};

// ��ֵ��ʾ_��������
#define SIDigitalDirectionType_Left "Left"
#define SIDigitalDirectionType_Right "Right"
#define SIDigitalDirectionType_Top "Top"
#define SIDigitalDirectionType_Bottom "Bottom"

enum SIDigitalDirectionType
{
	��ֵ��ʾ_��������_��� = 0,
	��ֵ��ʾ_��������_�Ҳ� = 1,
	��ֵ��ʾ_��������_�ϲ� = 2,
	��ֵ��ʾ_��������_�²� = 3
};

// ��ֵ��ʾ_��������
#define SIDigitalColumnType_None "None"
#define SIDigitalColumnType_Left "Left"
#define SIDigitalColumnType_Right "Right"
#define SIDigitalColumnType_Center "Center"
#define SIDigitalColumnType_Screen "Screen"

enum SIDigitalColumnType
{
	��ֵ��ʾ_��������_�� = 0,
	��ֵ��ʾ_��������_��� = 1,
	��ֵ��ʾ_��������_�Ҳ� = 2,
	��ֵ��ʾ_��������_�м� = 3,
	��ֵ��ʾ_��������_��Ļ = 4
};

#define SIBuffTypeClass_After_EnterAfter "After"
#define SIBuffTypeClass_After_EnterReset "Reset"
#define SIBuffTypeClass_After_EnterRemove "Remove"
#define SIBuffTypeClass_After_EnterNextBuff "Next"
#define SIBuffTypeClass_After_EnterNextBuffAfter "AfterNext"

enum SIBuffTypeClass_AfterType : unsigned int
{
	������� = 0,
	������� = 1,
	�����Ƴ� = 2,
	�����л�Buff = 3,
	�����л�Buff�ӳ� = 4
};


// ������ת����
#define SIRotateType_None "None"
#define SIRotateType_Self "Self"
#define SIRotateType_Body "Body"
#define SIRotateType_Target "Target"
#define SIRotateType_Random "Random"
#define SIRotateType_Same "Same"

enum SIRotateType : unsigned int
{
	����ת = 0,
	����ʵ�ʳ�����ת = 1,
	�������峯����ת = 2,
	����Ŀ��ķ�����ת = 3,
	�����ת = 4,
	ͬ����ת = 5
};

#define SIPackTypeClass_DigitalSetting_DefaultValue_MaxCharacterIndex 25

#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_0 0
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_1 1
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_2 2
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_3 3
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_4 4
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_5 5
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_6 6
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_7 7
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_8 8
#define SIPackTypeClass_DigitalSetting_DefaultValue_Number_9 9
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Point 10
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Addition 11
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Subtraction 12
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Multiplication 13
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Division 14
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Equal 15
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Space 16
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Line 17
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Percent 18
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Sharp 19
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_Dollar 20
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_SquareBracketLeft 21
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_SquareBracketRight 22
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_AngleBracketLeft 23
#define SIPackTypeClass_DigitalSetting_DefaultValue_Operator_AngleBracketRight 24

#define SIPackTypeClass_DigitalSetting_DefaultValue_BarFull_Head 0
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarFull_Body 1
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarFull_Tail 2
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarEmpty_Head 3
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarEmpty_Body 4
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarEmpty_Tail 5
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarBuffer_Head 6
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarBuffer_Body 7
#define SIPackTypeClass_DigitalSetting_DefaultValue_BarBuffer_Tail 8

struct SIPack_DigitalSetting
{
public:
	// ��������
	SIDigitalDisplayType ��ʾ���� = SIDigitalDisplayType::��ֵ��ʾ_��ʾ����_ˮƽ;
	int ��ʼ����ƫ��_X = 0;
	int ��ʼ����ƫ��_Y = 0;
	int ��λѪ��ƫ��_X = 0;
	int ��λѪ��ƫ��_Y = 0;
	// ƫ�Ʋ���
	int ���ƫ��_X = 0;
	int ���ƫ��_Y = 0;
	int ����� = 0;
	int ���߶� = 0;
	int ���Ѫ���������� = 0;
	int �Ҳ�ƫ��_X = 0;
	int �Ҳ�ƫ��_Y = 0;
	int �Ҳ��� = 0;
	int �Ҳ�߶� = 0;
	int �Ҳ�Ѫ���������� = 0;
	int �м�ƫ��_X = 0;
	int �м�ƫ��_Y = 0;
	int �м��� = 0;
	int �м�߶� = 0;
	int �м�Ѫ���������� = 0;
	// ���Ʋ���
	RectangleStruct* ��������;
	int ���� = 1000;
	int Ⱦɫ = 0;
	// ���ò���
	bool ����̬ƫ�� = true;
	// ��ֵ����
	double ��ǰֵ = 0.0;
	double ���ֵ = 0.0;
	double ��Сֵ = 0.0;
	double �䶯ֵ = 0.0;
	double ����ֵ = 0.0;
	bool ������Ѫ�� = false;
};


#define SIBuffTypeClass_EffectType_None "None"
#define SIBuffTypeClass_EffectType_Empty "Empty"
#define SIBuffTypeClass_EffectType_OnlyDuration "OnlyDuration"
#define SIBuffTypeClass_EffectType_OnlyClickable "OnlyClickable"
#define SIBuffTypeClass_EffectType_DelayAnimation "DelayAnimation"
// 01X ����
#define SIBuffTypeClass_EffectType_Warhead "Warhead"
#define SIBuffTypeClass_EffectType_WarheadDeath "WarheadDeath"
#define SIBuffTypeClass_EffectType_Death "Death"
#define SIBuffTypeClass_EffectType_LinkedDeath "LinkedDeath"
#define SIBuffTypeClass_EffectType_Sell "Sell"
#define SIBuffTypeClass_EffectType_Hunt "Hunt"
#define SIBuffTypeClass_EffectType_StateSilent "StateSilent"
#define SIBuffTypeClass_EffectType_StateTaunt "StateTaunt"
#define SIBuffTypeClass_EffectType_SkipStageActive "SkipStageActive"
#define SIBuffTypeClass_EffectType_EXP "EXP"
#define SIBuffTypeClass_EffectType_EXPPool "EXPPool"
#define SIBuffTypeClass_EffectType_Ammo "Ammo"
#define SIBuffTypeClass_EffectType_AmmoCheck "AmmoCheck"
#define SIBuffTypeClass_EffectType_MobileRefinery "MobileRefinery"
#define SIBuffTypeClass_EffectType_OreSpawner "OreSpawner"
#define SIBuffTypeClass_EffectType_Team "Team"
#define SIBuffTypeClass_EffectType_TeamDeath "TeamDeath"
#define SIBuffTypeClass_EffectType_Factory "Factory"
#define SIBuffTypeClass_EffectType_Skill "Skill"
// 02X Buff ����
#define SIBuffTypeClass_EffectTypePrefix_Buff "Buff"
#define SIBuffTypeClass_EffectTypePrefix_AttackBuff "AttackBuff"
#define SIBuffTypeClass_EffectTypePrefix_AttackTransBuff "AttackTransBuff"
#define SIBuffTypeClass_EffectTypePrefix_DefendBuff "DefendBuff"
#define SIBuffTypeClass_EffectTypePrefix_DefendTransBuff "DefendTransBuff"
#define SIBuffTypeClass_EffectTypePrefix_DamagedBuff "DamagedBuff"
#define SIBuffTypeClass_EffectTypePrefix_DamagedTransBuff "DamagedTransBuff"
#define SIBuffTypeClass_EffectTypePrefix_KillBuff "KillBuff"
#define SIBuffTypeClass_EffectTypePrefix_KillTransBuff "KillTransBuff"
#define SIBuffTypeClass_EffectTypePrefix_ListenerBuff "ListenerBuff"

#define SIBuffTypeClass_EffectType_BuffMark "BuffMark"
#define SIBuffTypeClass_EffectType_BuffActive "BuffActive"
#define SIBuffTypeClass_EffectType_BuffAfter "BuffAfter"
#define SIBuffTypeClass_EffectType_BuffRemove "BuffRemove"
#define SIBuffTypeClass_EffectType_BuffPower "BuffPower"
#define SIBuffTypeClass_EffectType_BuffChange "BuffChange"
#define SIBuffTypeClass_EffectType_BuffExtraCode "BuffExtraCode"

#define SIBuffTypeClass_EffectType_AttackBuffMark "AttackBuffMark"
#define SIBuffTypeClass_EffectType_AttackBuffActive "AttackBuffActive"
#define SIBuffTypeClass_EffectType_AttackBuffAfter "AttackBuffAfter"
#define SIBuffTypeClass_EffectType_AttackBuffRemove "AttackBuffRemove"
#define SIBuffTypeClass_EffectType_AttackBuffPower "AttackBuffPower"
#define SIBuffTypeClass_EffectType_AttackBuffChange "AttackBuffChange"
#define SIBuffTypeClass_EffectType_AttackTransBuffMark "AttackTransBuffMark"
#define SIBuffTypeClass_EffectType_AttackTransBuffActive "AttackTransBuffActive"
#define SIBuffTypeClass_EffectType_AttackTransBuffPower "AttackTransBuffPower"
#define SIBuffTypeClass_EffectType_AttackTransBuffChange "AttackTransBuffChange"

#define SIBuffTypeClass_EffectType_DefendBuffMark "DefendBuffMark"
#define SIBuffTypeClass_EffectType_DefendBuffActive "DefendBuffActive"
#define SIBuffTypeClass_EffectType_DefendBuffAfter "DefendBuffAfter"
#define SIBuffTypeClass_EffectType_DefendBuffRemove "DefendBuffRemove"
#define SIBuffTypeClass_EffectType_DefendBuffPower "DefendBuffPower"
#define SIBuffTypeClass_EffectType_DefendBuffChange "DefendBuffChange"
#define SIBuffTypeClass_EffectType_DefendTransBuffMark "DefendTransBuffMark"
#define SIBuffTypeClass_EffectType_DefendTransBuffActive "DefendTransBuffActive"
#define SIBuffTypeClass_EffectType_DefendTransBuffPower "DefendTransBuffPower"
#define SIBuffTypeClass_EffectType_DefendTransBuffChange "DefendTransBuffChange"

#define SIBuffTypeClass_EffectType_DamagedBuffMark "DamagedBuffMark"
#define SIBuffTypeClass_EffectType_DamagedBuffActive "DamagedBuffActive"
#define SIBuffTypeClass_EffectType_DamagedBuffAfter "DamagedBuffAfter"
#define SIBuffTypeClass_EffectType_DamagedBuffRemove "DamagedBuffRemove"
#define SIBuffTypeClass_EffectType_DamagedBuffPower "DamagedBuffPower"
#define SIBuffTypeClass_EffectType_DamagedBuffChange "DamagedBuffChange"
#define SIBuffTypeClass_EffectType_DamagedTransBuffMark "DamagedTransBuffMark"
#define SIBuffTypeClass_EffectType_DamagedTransBuffActive "DamagedTransBuffActive"
#define SIBuffTypeClass_EffectType_DamagedTransBuffPower "DamagedTransBuffPower"
#define SIBuffTypeClass_EffectType_DamagedTransBuffChange "DamagedTransBuffChange"

#define SIBuffTypeClass_EffectType_KillBuffMark "KillBuffMark"
#define SIBuffTypeClass_EffectType_KillBuffActive "KillBuffActive"
#define SIBuffTypeClass_EffectType_KillBuffAfter "KillBuffAfter"
#define SIBuffTypeClass_EffectType_KillBuffRemove "KillBuffRemove"
#define SIBuffTypeClass_EffectType_KillBuffPower "KillBuffPower"
#define SIBuffTypeClass_EffectType_KillBuffChange "KillBuffChange"
#define SIBuffTypeClass_EffectType_KillTransBuffMark "KillTransBuffMark"
#define SIBuffTypeClass_EffectType_KillTransBuffActive "KillTransBuffActive"
#define SIBuffTypeClass_EffectType_KillTransBuffPower "KillTransBuffPower"
#define SIBuffTypeClass_EffectType_KillTransBuffChange "KillTransBuffChange"

#define SIBuffTypeClass_EffectType_ListenerBuffMark "ListenerBuffMark"
#define SIBuffTypeClass_EffectType_ListenerBuffActive "ListenerBuffActive"
#define SIBuffTypeClass_EffectType_ListenerBuffAfter "ListenerBuffAfter"
#define SIBuffTypeClass_EffectType_ListenerBuffRemove "ListenerBuffRemove"
#define SIBuffTypeClass_EffectType_ListenerBuffPower "ListenerBuffPower"
// 03X �˺�����
#define SIBuffTypeClass_EffectTypePrefix_Damage "Damage"

#define SIBuffTypeClass_EffectType_DamageShield "DamageShield"
#define SIBuffTypeClass_EffectType_DamageBlock "DamageBlock"
#define SIBuffTypeClass_EffectType_DamageShare "DamageShare"
#define SIBuffTypeClass_EffectType_DamageImmune "DamageImmune"
#define SIBuffTypeClass_EffectType_DamageAvoid "DamageAvoid"
#define SIBuffTypeClass_EffectType_DamageDefer "DamageDefer"
#define SIBuffTypeClass_EffectType_DamageLimit "DamageLimit"
#define SIBuffTypeClass_EffectType_DamageRatio "DamageRatio"
#define SIBuffTypeClass_EffectType_DamagePrevent "DamagePrevent"
#define SIBuffTypeClass_EffectType_DamageReflect "DamageReflect"
#define SIBuffTypeClass_EffectType_DamageLink "DamageLink"
#define SIBuffTypeClass_EffectType_DamageKill "DamageKill"
#define SIBuffTypeClass_EffectType_DamageRefuse "DamageRefuse"
#define SIBuffTypeClass_EffectType_DamageRecord "DamageRecord"
// 04X ��������
#define SIBuffTypeClass_EffectTypePrefix_Health "Health"

#define SIBuffTypeClass_EffectType_HealthVampire "HealthVampire"
#define SIBuffTypeClass_EffectType_HealthLock "HealthLock"
#define SIBuffTypeClass_EffectType_HealthHeal "HealthHeal"
#define SIBuffTypeClass_EffectType_HealthReset "HealthReset"
// 05X ��������
#define SIBuffTypeClass_EffectTypePrefix_Attack "Attack"

#define SIBuffTypeClass_EffectType_AttackRatio "AttackRatio"
// 06X �㲥�����
#define SIBuffTypeClass_EffectTypePrefix_Broadcast "Broadcast"
#define SIBuffTypeClass_EffectTypePrefix_Listener "Listener"

#define SIBuffTypeClass_EffectType_Broadcast "Broadcast"
#define SIBuffTypeClass_EffectType_BroadcastDamage "BroadcastDamage"

#define SIBuffTypeClass_EffectType_ListenerBlank "ListenerBlank"
#define SIBuffTypeClass_EffectType_ListenerHospital "ListenerHospital"
#define SIBuffTypeClass_EffectType_ListenerWarhead "ListenerWarhead"
// 07X ��ֵ��ʾ
#define SIBuffTypeClass_EffectTypePrefix_Digital "Digital"

#define SIBuffTypeClass_EffectType_DigitalHouseVar "DigitalHouseVar"
#define SIBuffTypeClass_EffectType_DigitalEXPPool "DigitalEXPPool"
// 08X ����Ч��
// 09X ����Ч��
// 10X ��Ա����
#define SIBuffTypeClass_EffectTypePrefix_Transport "Transport"
#define SIBuffTypeClass_EffectTypePrefix_Passenger "Passenger"

#define SIBuffTypeClass_EffectType_TransportFill "TransportFill"
#define SIBuffTypeClass_EffectType_TransportEnter "TransportEnter"
#define SIBuffTypeClass_EffectType_TransportExit "TransportExit"
#define SIBuffTypeClass_EffectType_TransportRemove "TransportRemove"
#define SIBuffTypeClass_EffectType_PassengerEnter "PassengerEnter"
#define SIBuffTypeClass_EffectType_PassengerExit "PassengerExit"

#define SIBuffTypeClass_EffectType_TransportAttackFill "TransportAttackFill"
#define SIBuffTypeClass_EffectType_TransportAttackEnter "TransportAttackEnter"
#define SIBuffTypeClass_EffectType_TransportAttackExit "TransportAttackExit"
#define SIBuffTypeClass_EffectType_TransportAttackRemove "TransportAttackRemove"
#define SIBuffTypeClass_EffectType_PassengerAttackEnter "PassengerAttackEnter"
#define SIBuffTypeClass_EffectType_PassengerAttackExit "PassengerAttackExit"

#define SIBuffTypeClass_EffectType_TransportDefendFill "TransportDefendFill"
#define SIBuffTypeClass_EffectType_TransportDefendEnter "TransportDefendEnter"
#define SIBuffTypeClass_EffectType_TransportDefendExit "TransportDefendExit"
#define SIBuffTypeClass_EffectType_TransportDefendRemove "TransportDefendRemove"
#define SIBuffTypeClass_EffectType_PassengerDefendEnter "PassengerDefendEnter"
#define SIBuffTypeClass_EffectType_PassengerDefendExit "PassengerDefendExit"
// 11X Buff ����
#define SIBuffTypeClass_EffectTypePrefix_Weapon "Weapon"
#define SIBuffTypeClass_EffectTypePrefix_Revenge "Revenge"
#define SIBuffTypeClass_EffectTypePrefix_Martyrdom "Martyrdom"

#define SIBuffTypeClass_EffectType_WeaponChange "WeaponChange"
#define SIBuffTypeClass_EffectType_WeaponChangeDeath "WeaponChangeDeath"
#define SIBuffTypeClass_EffectType_WeaponExpelliarmus "WeaponExpelliarmus"

#define SIBuffTypeClass_EffectType_WeaponFire "WeaponFire"
#define SIBuffTypeClass_EffectType_WeaponSelf "WeaponSelf"
#define SIBuffTypeClass_EffectType_WeaponShoot "WeaponShoot"
#define SIBuffTypeClass_EffectType_WeaponShootAll "WeaponShootAll"
#define SIBuffTypeClass_EffectType_WeaponShrapnel "WeaponShrapnel"

#define SIBuffTypeClass_EffectType_RevengeDeath "RevengeDeath"
#define SIBuffTypeClass_EffectType_RevengeAttack "RevengeAttack"
#define SIBuffTypeClass_EffectType_RevengeDamage "RevengeDamage"

#define SIBuffTypeClass_EffectType_MartyrdomDeath "MartyrdomDeath"
#define SIBuffTypeClass_EffectType_MartyrdomAttack "MartyrdomAttack"
#define SIBuffTypeClass_EffectType_MartyrdomDamage "MartyrdomDamage"
// 12X ��λ����
#define SIBuffTypeClass_EffectTypePrefix_Property "Prop."

#define SIBuffTypeClass_EffectType_Property_MultSpeed "Prop.MultSpeed"
#define SIBuffTypeClass_EffectType_Property_MultArmor "Prop.MultArmor"
#define SIBuffTypeClass_EffectType_Property_MultVersus "Prop.MultVersus"
#define SIBuffTypeClass_EffectType_Property_MultAttack "Prop.MultAttack"
#define SIBuffTypeClass_EffectType_Property_MultROF "Prop.MultROF"
#define SIBuffTypeClass_EffectType_Property_MultEXP "Prop.MultEXP"
#define SIBuffTypeClass_EffectType_Property_MultEXPProvide "Prop.MultEXPProvide"
#define SIBuffTypeClass_EffectType_Property_MultEXPCost "Prop.MultEXPCost"
#define SIBuffTypeClass_EffectType_Property_MultRangeWeapon "Prop.MultRangeWeapon"
#define SIBuffTypeClass_EffectType_Property_MultCellSpread "Prop.MultCellSpread"

#define SIBuffTypeClass_EffectType_Property_ExMultSpeed "Prop.ExMultSpeed"
#define SIBuffTypeClass_EffectType_Property_ExMultArmor "Prop.ExMultArmor"
#define SIBuffTypeClass_EffectType_Property_ExMultVersus "Prop.ExMultVersus"
#define SIBuffTypeClass_EffectType_Property_ExMultAttack "Prop.ExMultAttack"
#define SIBuffTypeClass_EffectType_Property_ExMultROF "Prop.ExMultROF"
#define SIBuffTypeClass_EffectType_Property_ExMultEXP "Prop.ExMultEXP"
#define SIBuffTypeClass_EffectType_Property_ExMultEXPProvide "Prop.ExMultEXPProvide"
#define SIBuffTypeClass_EffectType_Property_ExMultEXPCost "Prop.ExMultEXPCost"
#define SIBuffTypeClass_EffectType_Property_ExMultRangeWeapon "Prop.ExMultRangeWeapon"
#define SIBuffTypeClass_EffectType_Property_ExMultCellSpread "Prop.ExMultCellSpread"

#define SIBuffTypeClass_EffectType_Property_AddSpeed "Prop.AddSpeed"
#define SIBuffTypeClass_EffectType_Property_AddDamage "Prop.AddDamage"
#define SIBuffTypeClass_EffectType_Property_AddAttack "Prop.AddAttack"
#define SIBuffTypeClass_EffectType_Property_AddROF "Prop.AddROF"
#define SIBuffTypeClass_EffectType_Property_AddEXP "Prop.AddEXP"
#define SIBuffTypeClass_EffectType_Property_AddEXPProvide "Prop.AddEXPProvide"
#define SIBuffTypeClass_EffectType_Property_AddEXPCost "Prop.AddEXPCost"
#define SIBuffTypeClass_EffectType_Property_AddRangeWeapon "Prop.AddRangeWeapon"
#define SIBuffTypeClass_EffectType_Property_AddCellSpread "Prop.AddCellSpread"

#define SIBuffTypeClass_EffectType_Property_FinalAddSpeed "Prop.FinalSpeed"
#define SIBuffTypeClass_EffectType_Property_FinalAddDamage "Prop.FinalDamage"
#define SIBuffTypeClass_EffectType_Property_FinalAddAttack "Prop.FinalAttack"
#define SIBuffTypeClass_EffectType_Property_FinalAddROF "Prop.FinalROF"
#define SIBuffTypeClass_EffectType_Property_FinalAddEXP "Prop.FinalEXP"
#define SIBuffTypeClass_EffectType_Property_FinalAddEXPProvide "Prop.FinalEXPProvide"
#define SIBuffTypeClass_EffectType_Property_FinalAddEXPCost "Prop.FinalEXPCost"
#define SIBuffTypeClass_EffectType_Property_FinalAddRangeWeapon "Prop.FinalRangeWeapon"
#define SIBuffTypeClass_EffectType_Property_FinalAddCellSpread "Prop.FinalCellSpread"

#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneCrush "Prop.ImmuneCrush"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneCrushAdvance "Prop.ImmuneCrushAdvance"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneRadiation "Prop.ImmuneRadiation"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmunePsionicDamage "Prop.ImmunePsionicDamage"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmunePsionic "Prop.ImmunePsionic"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneBerzerk "Prop.ImmuneBerzerk"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmunePoison "Prop.ImmunePoison"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneWarp "Prop.ImmuneWarp"
#define SIBuffTypeClass_EffectType_Property_Ability_ImmuneEMP "Prop.ImmuneEMP"

#define SIBuffTypeClass_EffectType_Property_Ability_SelfHealing "Prop.SelfHealing"
#define SIBuffTypeClass_EffectType_Property_Ability_SelfHealing_Percent "Prop.SelfHealing.Percent"
#define SIBuffTypeClass_EffectType_Property_Ability_SelfHealing_Cap "Prop.SelfHealing.Cap"
#define SIBuffTypeClass_EffectType_Property_Ability_SelfHealing_Delay "Prop.SelfHealing.Delay"
#define SIBuffTypeClass_EffectType_Property_Ability_SelfHealing_CombatDelay "Prop.SelfHealing.CombatDelay"

#define SIBuffTypeClass_EffectType_Property_Tint "Prop.Tint"
// 502 ������ս���ֲ�����
#define SIBuffTypeClass_EffectTypePrefix_HouseVar "HouseVar."
#define SIBuffTypeClass_EffectTypePrefix_HouseVarDeath "HouseVarDeath."

#define SIBuffTypeClass_EffectType_HouseVar_Add "HouseVar.Add"
#define SIBuffTypeClass_EffectType_HouseVar_Mult "HouseVar.Mult"
#define SIBuffTypeClass_EffectType_HouseVar_Pow "HouseVar.Pow"
#define SIBuffTypeClass_EffectType_HouseVar_Log "HouseVar.Log"
#define SIBuffTypeClass_EffectType_HouseVar_Mod "HouseVar.Mod"
#define SIBuffTypeClass_EffectType_HouseVar_Eq "HouseVar.Eq"
#define SIBuffTypeClass_EffectType_HouseVar_NE "HouseVar.NE"
#define SIBuffTypeClass_EffectType_HouseVar_GT "HouseVar.GT"
#define SIBuffTypeClass_EffectType_HouseVar_GE "HouseVar.GE"
#define SIBuffTypeClass_EffectType_HouseVar_LT "HouseVar.LT"
#define SIBuffTypeClass_EffectType_HouseVar_LE "HouseVar.LE"
#define SIBuffTypeClass_EffectType_HouseVar_Not "HouseVar.Not"
#define SIBuffTypeClass_EffectType_HouseVar_Abs "HouseVar.Abs"
#define SIBuffTypeClass_EffectType_HouseVar_Ceil "HouseVar.Ceil"
#define SIBuffTypeClass_EffectType_HouseVar_Floor "HouseVar.Floor"
#define SIBuffTypeClass_EffectType_HouseVar_Round "HouseVar.Round"
#define SIBuffTypeClass_EffectType_HouseVar_Set "HouseVar.Set"
#define SIBuffTypeClass_EffectType_HouseVar_Max "HouseVar.Max"
#define SIBuffTypeClass_EffectType_HouseVar_Min "HouseVar.Min"
#define SIBuffTypeClass_EffectType_HouseVar_Random "HouseVar.Random"
#define SIBuffTypeClass_EffectType_HouseVar_Delete "HouseVar.Delete"

#define SIBuffTypeClass_EffectType_HouseVarDeath_Add "HouseVarDeath.Add"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Mult "HouseVarDeath.Mult"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Pow "HouseVarDeath.Pow"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Log "HouseVarDeath.Log"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Mod "HouseVarDeath.Mod"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Eq "HouseVarDeath.Eq"
#define SIBuffTypeClass_EffectType_HouseVarDeath_NE "HouseVarDeath.NE"
#define SIBuffTypeClass_EffectType_HouseVarDeath_GT "HouseVarDeath.GT"
#define SIBuffTypeClass_EffectType_HouseVarDeath_GE "HouseVarDeath.GE"
#define SIBuffTypeClass_EffectType_HouseVarDeath_LT "HouseVarDeath.LT"
#define SIBuffTypeClass_EffectType_HouseVarDeath_LE "HouseVarDeath.LE"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Not "HouseVarDeath.Not"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Abs "HouseVarDeath.Abs"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Ceil "HouseVarDeath.Ceil"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Floor "HouseVarDeath.Floor"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Round "HouseVarDeath.Round"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Set "HouseVarDeath.Set"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Max "HouseVarDeath.Max"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Min "HouseVarDeath.Min"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Random "HouseVarDeath.Random"
#define SIBuffTypeClass_EffectType_HouseVarDeath_Delete "HouseVarDeath.Delete"
// 504 �ж���ֵ
#define SIBuffTypeClass_EffectTypePrefix_CheckNumber "CheckNumber."
#define SIBuffTypeClass_EffectTypePrefix_CheckNumberDeath "CheckNumberDeath."

#define SIBuffTypeClass_EffectType_CheckNumber_Eq "CheckNumber.Eq"
#define SIBuffTypeClass_EffectType_CheckNumber_NE "CheckNumber.NE"
#define SIBuffTypeClass_EffectType_CheckNumber_GT "CheckNumber.GT"
#define SIBuffTypeClass_EffectType_CheckNumber_GE "CheckNumber.GE"
#define SIBuffTypeClass_EffectType_CheckNumber_LT "CheckNumber.LT"
#define SIBuffTypeClass_EffectType_CheckNumber_LE "CheckNumber.LE"

#define SIBuffTypeClass_EffectType_CheckNumberDeath_Eq "CheckNumberDeath.Eq"
#define SIBuffTypeClass_EffectType_CheckNumberDeath_NE "CheckNumberDeath.NE"
#define SIBuffTypeClass_EffectType_CheckNumberDeath_GT "CheckNumberDeath.GT"
#define SIBuffTypeClass_EffectType_CheckNumberDeath_GE "CheckNumberDeath.GE"
#define SIBuffTypeClass_EffectType_CheckNumberDeath_LT "CheckNumberDeath.LT"
#define SIBuffTypeClass_EffectType_CheckNumberDeath_LE "CheckNumberDeath.LE"

enum SIBuffTypeClass_EffectType : int
{
	// 00X ��Ч��
	��Ч�� = 0,
	��Ч�� = 1,
	ֻ�г���ʱ�� = 2,
	ֻ�п��Ե�� = 10,
	ֻ�м�Ъ���� = 20,
	// 01X ����
	������ͷ = 1001,
	������ͷ����ʱ = 1002,
	ԭ��ȥ�� = 1020,
	�������� = 1021,
	�������� = 1028,
	�������� = 1029,
	��Ĭ״̬ = 1030,
	����״̬ = 1031,
	������������׶� = 1040,
	����ֵ = 1070,
	����� = 1071,
	��ҩ���� = 1072,
	��ҩ��� = 1073,
	���տ��� = 1074,
	�������� = 1075,
	Ͷ�ŵ�λ = 1120,
	Ͷ�ŵ�λ����ʱ = 1121,
	���� = 1130,
	���� = 1131,
	// 02X Buff ����
	Buff���� = 2000,
	Buff���� = 2001,
	Buff���� = 2002,
	Buff�Ƴ� = 2003,
	Buffǿ�� = 2004,
	Buffת�� = 2005,
	Buff������� = 2006,

	����Buff���� = 2010,
	����Buff���� = 2011,
	����Buff���� = 2012,
	����Buff�Ƴ� = 2013,
	����Buffǿ�� = 2014,
	����Buffת�� = 2015,
	����ת��Buff���� = 2016,
	����ת��Buff���� = 2017,
	����ת��Buffǿ�� = 2018,
	����ת��Buffת�� = 2019,

	����Buff���� = 2020,
	����Buff���� = 2021,
	����Buff���� = 2022,
	����Buff�Ƴ� = 2023,
	����Buffǿ�� = 2024,
	����Buffת�� = 2025,
	����ת��Buff���� = 2026,
	����ת��Buff���� = 2027,
	����ת��Buffǿ�� = 2028,
	����ת��Buffת�� = 2029,

	����Buff���� = 2030,
	����Buff���� = 2031,
	����Buff���� = 2032,
	����Buff�Ƴ� = 2033,
	����Buffǿ�� = 2034,
	����Buffת�� = 2035,
	����ת��Buff���� = 2036,
	����ת��Buff���� = 2037,
	����ת��Buffǿ�� = 2038,
	����ת��Buffת�� = 2039,

	��ɱBuff���� = 2050,
	��ɱBuff���� = 2051,
	��ɱBuff���� = 2052,
	��ɱBuff�Ƴ� = 2053,
	��ɱBuffǿ�� = 2054,
	��ɱBuffת�� = 2055,
	��ɱת��Buff���� = 2056,
	��ɱת��Buff���� = 2057,
	��ɱת��Buffǿ�� = 2058,
	��ɱת��Buffת�� = 2059,

	Buff���ؼ��� = 2080,
	Buff�������� = 2081,
	Buff�������� = 2082,
	Buff�Ƴ����� = 2083,
	Buffǿ�ȼ��� = 2084,
	// 03X �˺�����
	�˺��е� = 3001, // ����Ч��
	�˺��� = 3002, // ���Եֵ�ָ�������Ĺ��� , ����ָ���ٽ��˺�ֵ
	�˺���̯ = 3003, // ����Χ�ĵ�λ��̯�ܵ����˺�
	�˺����� = 3004, // �����ض����͵��˺�
	�˺����� = 3005, // һ�����������˺�
	�˺��ӳ� = 3006, // �����˺������˺�תΪ�ϵ͵�֡�˺�
	�˺����� = 3007, // ���������ܵ���������С���˺�ֵ
	�˺����� = 3008, // ������
	�˺����� = 3009, // �ܵ����߻���͵��˺�ʱ�����˺�
	�˺����� = 3010, // �����ܵ����˺�
	�˺����� = 3011, // �ܵ����˺��ᴫ�������� Buff �ĵ�λ
	�˺����� = 3012,
	�˺����� = 3013,
	�˺���¼ = 3014,
	// 04X ��������
	������ȡ = 4001, // ��Ѫ
	�������� = 4002, // ����ֵ�޷�����һ������ֵ
	�����ָ� = 4003, // ����
	�������� = 4004,
	// 05X ��������
	�������� = 5001,
	// 06X �㲥�����
	�㲥 = 6000,
	�˺�ת���㲥 = 6001,

	�հ׼��� = 6100,
	ҽԺ���� = 6101,
	��ͷ���� = 6102,
	// 07X ��ֵ��ʾ
	��ʾ��ս���ֲ����� = 7001,
	��ʾ��ս������� = 7002,
	// 08X ����Ч��
	// 09X ����Ч��
	// 10X ��Ա����
	�ؾ����_���� = 10000,
	�ؾ�����_���� = 10001,
	�ؾ��ͷ�_���� = 10002,
	�ؾ��Ƴ�_���� = 10003,
	��Ա����_���� = 10010,
	��Ա�ѳ�_���� = 10011,

	�ؾ����_���� = 10100,
	�ؾ�����_���� = 10101,
	�ؾ��ͷ�_���� = 10102,
	�ؾ��Ƴ�_���� = 10103,
	��Ա����_���� = 10110,
	��Ա�ѳ�_���� = 10111,

	�ؾ����_���� = 10200,
	�ؾ�����_���� = 10201,
	�ؾ��ͷ�_���� = 10202,
	�ؾ��Ƴ�_���� = 10203,
	��Ա����_���� = 10210,
	��Ա�ѳ�_���� = 10211,
	// 11X Buff ����
	����_�滻���� = 11000,
	����_�滻�������� = 11001,
	����_�������� = 11002,

	����_�������� = 11010,
	����_�������� = 11011,
	����_�������� = 11012,
	����_ɢ������ = 11013,
	����_�������� = 11014,

	����_�������� = 11100,
	����_�������� = 11101,
	����_���˸��� = 11102,

	����_����ѳ�� = 11110,
	����_����ѳ�� = 11111,
	����_����ѳ�� = 11112,
	// 12X ��λ����
	��λ����_�ӳ�_�����ƶ� = 12000,
	��λ����_�ӳ�_���ʻ��� = 12001,
	��λ����_�ӳ�_���ʻ����˺� = 12002,
	��λ����_�ӳ�_���ʻ��� = 12003,
	��λ����_�ӳ�_�������� = 12004,
	��λ����_�ӳ�_���ʾ���ֵ��ȡ = 12005,
	��λ����_�ӳ�_���ʾ���ֵ�ṩ = 12006,
	��λ����_�ӳ�_���ʾ���ֵ���� = 12007,
	��λ����_�ӳ�_����������� = 12008,
	��λ����_�ӳ�_���ʵ�ͷ���䷶Χ = 12020,

	��λ����_�ӳ�_�۳��ƶ� = 12100,
	��λ����_�ӳ�_�۳˻��� = 12101,
	��λ����_�ӳ�_�۳˻����˺� = 12102,
	��λ����_�ӳ�_�۳˻��� = 12103,
	��λ����_�ӳ�_�۳����� = 12104,
	��λ����_�ӳ�_�۳˾���ֵ��ȡ = 12105,
	��λ����_�ӳ�_�۳˾���ֵ�ṩ = 12106,
	��λ����_�ӳ�_�۳˾���ֵ���� = 12107,
	��λ����_�ӳ�_�۳�������� = 12108,
	��λ����_�ӳ�_�۳˵�ͷ���䷶Χ = 12120,

	��λ����_�ӳ�_׷���ƶ� = 12200,
	��λ����_�ӳ�_׷���˺� = 12202,
	��λ����_�ӳ�_׷�ӹ��� = 12203,
	��λ����_�ӳ�_׷������ = 12204,
	��λ����_�ӳ�_׷�Ӿ���ֵ��ȡ = 12205,
	��λ����_�ӳ�_׷�Ӿ���ֵ�ṩ = 12206,
	��λ����_�ӳ�_׷�Ӿ���ֵ���� = 12207,
	��λ����_�ӳ�_׷��������� = 12208,
	��λ����_�ӳ�_׷�ӵ�ͷ���䷶Χ = 12220,

	��λ����_�ӳ�_�����ƶ� = 12300,
	��λ����_�ӳ�_�����˺� = 12302,
	��λ����_�ӳ�_���չ��� = 12303,
	��λ����_�ӳ�_�������� = 12304,
	��λ����_�ӳ�_���վ���ֵ��ȡ = 12305,
	��λ����_�ӳ�_���վ���ֵ�ṩ = 12306,
	��λ����_�ӳ�_���վ���ֵ���� = 12307,
	��λ����_�ӳ�_����������� = 12308,
	��λ����_�ӳ�_���յ�ͷ���䷶Χ = 12320,

	��λ����_����_������ѹ = 12400,
	��λ����_����_���߸߼���ѹ = 12401,
	��λ����_����_���߷��䵯ͷ = 12402,
	��λ����_����_���������˺���ͷ = 12403,
	��λ����_����_�����Ŀص�ͷ = 12404,
	��λ����_����_���߿񱩵�ͷ = 12405,
	��λ����_����_���߶��ص�ͷ = 12406,
	��λ����_����_���߳�ʱ�մ��͵�ͷ = 12407,
	��λ����_����_���ߵ�����嵯ͷ = 12408,

	��λ����_����_���� = 12450,
	��λ����_����_����_ÿ�λָ�������ֵ���� = 12451,
	��λ����_����_����_�������ֵ���� = 12452,
	��λ����_����_����_�ָ�ʱ���� = 12453,
	��λ����_����_����_��սʱ���� = 12454,

	��λ����_Ⱦɫ = 12500,
	// 502 ������ս���ֲ�����
	������ս���ֲ�����_����_�� = 50200,
	������ս���ֲ�����_����_�� = 50201,
	������ս���ֲ�����_����_ָ�� = 50202,
	������ս���ֲ�����_����_���� = 50203,
	������ս���ֲ�����_����_ȡģ = 50204,
	������ս���ֲ�����_����_���� = 50210,
	������ս���ֲ�����_����_������ = 50211,
	������ս���ֲ�����_����_���� = 50212,
	������ս���ֲ�����_����_���ڵ��� = 50213,
	������ս���ֲ�����_����_С�� = 50214,
	������ս���ֲ�����_����_С�ڵ��� = 50215,
	������ս���ֲ�����_����_ȡ�� = 50220,
	������ս���ֲ�����_����_����ֵ = 50221,
	������ս���ֲ�����_����_����ȡ�� = 50222,
	������ս���ֲ�����_����_����ȡ�� = 50223,
	������ս���ֲ�����_����_�������� = 50224,
	������ս���ֲ�����_����_���� = 50230,
	������ս���ֲ�����_����_��� = 50231,
	������ս���ֲ�����_����_��С = 50232,
	������ս���ֲ�����_����_��� = 50233,
	������ս���ֲ�����_����_�Ƴ� = 50234,

	������ս���ֲ�����_����_�� = -50200,
	������ս���ֲ�����_����_�� = -50201,
	������ս���ֲ�����_����_ָ�� = -50202,
	������ս���ֲ�����_����_���� = -50203,
	������ս���ֲ�����_����_ȡģ = -50204,
	������ս���ֲ�����_����_���� = -50210,
	������ս���ֲ�����_����_������ = -50211,
	������ս���ֲ�����_����_���� = -50212,
	������ս���ֲ�����_����_���ڵ��� = -50213,
	������ս���ֲ�����_����_С�� = -50214,
	������ս���ֲ�����_����_С�ڵ��� = -50215,
	������ս���ֲ�����_����_ȡ�� = -50220,
	������ս���ֲ�����_����_����ֵ = -50221,
	������ս���ֲ�����_����_����ȡ�� = -50222,
	������ս���ֲ�����_����_����ȡ�� = -50223,
	������ս���ֲ�����_����_�������� = -50224,
	������ս���ֲ�����_����_���� = -50230,
	������ս���ֲ�����_����_��� = -50231,
	������ս���ֲ�����_����_��С = -50232,
	������ս���ֲ�����_����_��� = -50233,
	������ս���ֲ�����_����_�Ƴ� = -50234,
	// 504 �ж���ֵ
	�ж���ֵ_����_���� = 50400,
	�ж���ֵ_����_������ = 50401,
	�ж���ֵ_����_���� = 50402,
	�ж���ֵ_����_���ڵ��� = 50403,
	�ж���ֵ_����_С�� = 50404,
	�ж���ֵ_����_С�ڵ��� = 50405,

	�ж���ֵ_����_���� = -50400,
	�ж���ֵ_����_������ = -50401,
	�ж���ֵ_����_���� = -50402,
	�ж���ֵ_����_���ڵ��� = -50403,
	�ж���ֵ_����_С�� = -50404,
	�ж���ֵ_����_С�ڵ��� = -50405
};

#define SIVariableType_Normal "Normal"
#define SIVariableType_Global "Global"
#define SIVariableType_Local "Local"
#define SIVariableType_House "House"

enum SIVariableType : unsigned int
{
	��ͨ���� = 0,
	ȫ�ֱ��� = 1,
	�ֲ����� = 2,
	��ս���ֲ����� = 3
};

#define SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMax 1000000000.0 // 10e
#define SIPackTypeClass_OffsetMotion_DefaultValue_TranslationXYZMin -1000000000.0 // -10e

#define SIBuffSetting_DefaultValue_Duration_Self false
#define SIBuffSetting_DefaultValue_Duration 0
#define SIBuffSetting_DefaultValue_Duration_Max 1000000000
#define SIBuffSetting_DefaultValue_Duration_Min 0
#define SIBuffSetting_DefaultValue_Duration_Reset false
#define SIBuffSetting_DefaultValue_Duration_Effect true
#define SIBuffSetting_DefaultValue_Duration_AffectAfter false

#define SIBuffSetting_DefaultValue_Health_Self false
#define SIBuffSetting_DefaultValue_Health 0.0
#define SIBuffSetting_DefaultValue_Health_Max 1000000000.0
#define SIBuffSetting_DefaultValue_Health_Min 0.0
#define SIBuffSetting_DefaultValue_Health_Reset false
#define SIBuffSetting_DefaultValue_Health_Effect true

#define SIBuffSetting_DefaultValue_Power 0.0
#define SIBuffSetting_DefaultValue_Power_Max 1000000000.0
#define SIBuffSetting_DefaultValue_Power_Min -1000000000.0
#define SIBuffSetting_DefaultValue_Power_Reset false
#define SIBuffSetting_DefaultValue_Power_Effect true

#define SIBuffSetting_Duration_MAX 1000000000
#define SIBuffSetting_Duration_MIN 0
#define SIBuffSetting_Duration_MIN_Self -1

#define SIBuffSetting_Health_MAX 1000000000.0
#define SIBuffSetting_Health_MIN 0.0
#define SIBuffSetting_Health_MIN_Self -1.0

#define SIBuffSetting_Power_MAX 1000000000.0
#define SIBuffSetting_Power_MIN -1000000000.0

// �˺�����ʽ
#define SIDamageProcessType_Detonate "Detonate"
#define SIDamageProcessType_Damage "Damage"
#define SIDamageProcessType_Health "Health"
#define SIDamageProcessType_OnlyHealth "OnlyHealth"

enum SIDamageProcessType : unsigned int
{
	�˺�����_���� = 0,
	�˺�����_�˺� = 1,
	�˺�����_���� = 2,
	�˺�����_������ = 3
};

//��ȡ���SIPackTypeClass_BuffSetting�ĺ���
struct SIPack_BuffSetting_FromStatic
{
public:
	// ����ʱ��
	bool SIDuration_Self;
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	bool SIDuration_Reset;
	bool SIDuration_Effect;
	bool SIDuration_AffectAfter;
	// ����ֵ
	bool SIHealth_Self;
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Reset;
	bool SIHealth_Effect;
	// ǿ��ֵ
	double SIPower;
	double SIPower_Max;
	double SIPower_Min;
	bool SIPower_Reset;
	bool SIPower_Effect;
};

struct SIValues_EXPMerge
{
public:
	double ����ֵ����;
	HouseClass* �ṩ����ֵ�ĵ�λ��������ս��;
	bool ��Ҫ��λ�����þ���ֵ;
	bool ��Ҫ��λ������پ���ֵ;
	bool ���õ�λ���Եľ���ӳ�;
	bool ������侭��ֵ;

	SIValues_EXPMerge(double ����ֵ����, HouseClass* �ṩ����ֵ�ĵ�λ��������ս��, bool ��Ҫ��λ�����þ���ֵ, bool ��Ҫ��λ������پ���ֵ, bool ���õ�λ���Եľ���ӳ�, bool ������侭��ֵ)
		: ����ֵ����(����ֵ����)
		, �ṩ����ֵ�ĵ�λ��������ս��(�ṩ����ֵ�ĵ�λ��������ս��)
		, ��Ҫ��λ�����þ���ֵ(��Ҫ��λ�����þ���ֵ)
		, ��Ҫ��λ������پ���ֵ(��Ҫ��λ������پ���ֵ)
		, ���õ�λ���Եľ���ӳ�(���õ�λ���Եľ���ӳ�)
		, ������侭��ֵ(������侭��ֵ)
	{}
};

// Ⱦɫ������
struct SIValues_CustomTint
{
	int Ⱦɫ��ɫ;
	int Ⱦɫǿ��;
	int ��ս������;
};

// ���Ҿ���ֵ������
struct SIValues_HouseInitEXP
{
	double ����ֵ_��������;
	double ����ȼ�_��������;
	double ����ֵ_ѵ����;
	double ����ȼ�_ѵ����;
	double ����ֵ_�����͸;
	double ����ȼ�_�����͸;

	SIValues_HouseInitEXP()
		: ����ֵ_��������(0.0)
		, ����ȼ�_��������(0.0)
		, ����ֵ_ѵ����(0.0)
		, ����ȼ�_ѵ����(0.0)
		, ����ֵ_�����͸(0.0)
		, ����ȼ�_�����͸(0.0)
	{}

	bool IsEmpty()
	{
		return this->����ֵ_�������� == 0
			&& this->����ȼ�_�������� == 0
			&& this->����ֵ_ѵ���� == 0
			&& this->����ȼ�_ѵ���� == 0
			&& this->����ֵ_�����͸ == 0
			&& this->����ȼ�_�����͸ == 0;
	}

	void Reset()
	{
		this->����ֵ_�������� = 0.0;
		this->����ȼ�_�������� = 0.0;
		this->����ֵ_ѵ���� = 0.0;
		this->����ȼ�_ѵ���� = 0.0;
		this->����ֵ_�����͸ = 0.0;
		this->����ȼ�_�����͸ = 0.0;
	}

	void Merge(SIValues_HouseInitEXP* ���Ҿ���ֵ������)
	{
		this->����ֵ_�������� = ���Ҿ���ֵ������->����ֵ_��������;
		this->����ȼ�_�������� = ���Ҿ���ֵ������->����ȼ�_��������;
		this->����ֵ_ѵ���� = ���Ҿ���ֵ������->����ֵ_ѵ����;
		this->����ȼ�_ѵ���� = ���Ҿ���ֵ������->����ȼ�_ѵ����;
		this->����ֵ_�����͸ = ���Ҿ���ֵ������->����ֵ_�����͸;
		this->����ȼ�_�����͸ = ���Ҿ���ֵ������->����ȼ�_�����͸;
	}
};

#define SI_API __stdcall
/*
	�ⲿ����SI��ԭʼ�ӿڣ�������ֱ�ӵ��ã���ͨ��SIInterface_ExtendData��ٿأ�
	���ǣ���ľ�̬�����ĵ���Լ����__stdcall����

	��ע���˴��Ѿ�ȷ�����ȶ����ڵ�API������ʽ������ֱ��ͨ��ֻ����С�޶ȼ���LibData::QueryInterfaceʵ��
		  �����һЩ����޷���ȫȷ�ϣ���ʹ��Ext::DispatchInterface�������϶Է��Ľӿ�
		  ���磺
		char* SomeFunc(int Param)
		{
			static Ext::DispatchInterface Interface("RegName of the Lib", "RegName of the function", DoNotCheckVersion);
			auto fn = Interface.Func<decltype(SomeFunc)>();
			if (fn)return fn(Param);
			else
			{
				//������
			}
		}
*/
namespace SI
{
	char* SI_API GetName(SIInterface_ExtendData* �ӿ�); // ���� ID ����
	AbstractClass* SI_API OwnerEntity(SIInterface_ExtendData* �ӿ�); // ����ֵ����Ϊ��
	AbstractType  SI_API WhatAmI(SIInterface_ExtendData* �ӿ�);
	int  SI_API GetArrayIndex(SIInterface_ExtendData* �ӿ�);
	bool  SI_API IsSelected(SIInterface_ExtendData* �ӿ�);
	void  SI_API GetCoords(SIInterface_ExtendData* �ӿ�, CoordStruct* ��������);
	void  SI_API SetForceFireWeapon(SIInterface_ExtendData* �ӿ�, WeaponTypeClass* ��������);
	void  SI_API SetForceDeathWeapon(SIInterface_ExtendData* �ӿ�, WeaponTypeClass* ��������);
	CoordStruct  SI_API Calculate_FireCoord(SIInterface_ExtendData* �ӿ�, CoordStruct ��������, double ���ű���);
	int  SI_API GetStaticEXPLevel(SIInterface_ExtendData* �ӿ�);
	double  SI_API GetLevelTotalEXPCost(SIInterface_ExtendData* �ӿ�, double �̶�����ֵ����, bool ʹ���¾���ϵͳ);
	double SI_API Calculate_PropertyGroup_Speed(SIInterface_ExtendData* �ӿ�, double ԭʼ�ƶ��ٶ�);
	double SI_API Calculate_PropertyGroup_Defence(SIInterface_ExtendData* �ӿ�, double ԭʼ�˺�);
	double SI_API Calculate_PropertyGroup_Attack(SIInterface_ExtendData* �ӿ�, double ԭʼ�˺�);
	double SI_API Calculate_PropertyGroup_ROF(SIInterface_ExtendData* �ӿ�, double ԭʼ����);
	double SI_API Calculate_PropertyGroup_EXP(SIInterface_ExtendData* �ӿ�, double ԭʼ��õľ���ֵ);
	double SI_API Calculate_PropertyGroup_EXPProvide(SIInterface_ExtendData* �ӿ�, double ԭʼ�ṩ�ľ���ֵ);
	double SI_API Calculate_PropertyGroup_EXPCost(SIInterface_ExtendData* �ӿ�, double ��λ��������ľ���ֵ);
	double SI_API Calculate_PropertyGroup_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ԭʼ�������);
	double SI_API Calculate_PropertyGroup_CellSpread(SIInterface_ExtendData* �ӿ�, double ԭʼ��ͷ���䷶Χ);
	int SI_API GetTintColor(SIInterface_ExtendData* �ӿ�, bool ��ĻȾɫ, bool ��ϮȾɫ, bool ��Ⱦɫ);
	void SI_API Calculate_CustomTintValues(SIInterface_ExtendData* �ӿ�, int& Ⱦɫ��ɫ, int& Ⱦɫǿ��);
	int SI_API Calculate_CustomTintValues_Color(SIInterface_ExtendData* �ӿ�);
	int SI_API Calculate_CustomTintValues_Intensity(SIInterface_ExtendData* �ӿ�);
	double SI_API Property_Merge_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_Damage(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������); // ��λ�Ǹ��
	double SI_API Property_Merge_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	double SI_API Property_Merge_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������); // ��λ�Ǹ��
	double SI_API Property_Merge_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, double �ɵ�������, double �µ�������);
	int SI_API Property_Register_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_Speed(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_Damage(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_Attack(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_ROF(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_EXP(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������); // ��λ�Ǹ��
	int SI_API Property_Register_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, double ������);
	int SI_API Property_Register_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, double ������); // ��λ�Ǹ��
	int SI_API Property_Register_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, double ������);
	void SI_API Property_Unregister_Multiplier_Speed(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_Armor(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_Versus(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_Attack(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_ROF(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_EXP(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Multiplier_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_Speed(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_Armor(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_Versus(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_Attack(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_ROF(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_EXP(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_ExtraMultiplier_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_Speed(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_Damage(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_Attack(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_ROF(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_EXP(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_Addon_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_Speed(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_Damage(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_Attack(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_ROF(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_EXP(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_EXPProvide(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_EXPCost(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_RangeWeapon(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Unregister_FinalAddon_CellSpread(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API Property_Merge_Ability_ImmuneCrush(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmuneCrushAdvance(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmuneRadiation(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmunePsionicDamage(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmunePsionic(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmuneBerzerk(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmunePoison(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmuneWarp(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_ImmuneEMP(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_SelfHealing(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_SelfHealing_Percent(SIInterface_ExtendData* �ӿ�, double �ɵ�����ǿ��, double �µ�����ǿ��);
	void SI_API Property_Merge_Ability_SelfHealing_Cap(SIInterface_ExtendData* �ӿ�, double �ɵ�����ǿ��, double �µ�����ǿ��);
	void SI_API Property_Merge_Ability_SelfHealing_Delay(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	void SI_API Property_Merge_Ability_SelfHealing_CombatDelay(SIInterface_ExtendData* �ӿ�, int �ɵ�����ǿ��, int �µ�����ǿ��);
	int SI_API Property_Register_CustomTintValues_A(SIInterface_ExtendData* �ӿ�, ColorStruct Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������);
	int SI_API Property_Register_CustomTintValues_B(SIInterface_ExtendData* �ӿ�, int Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������);
	void SI_API Property_Unregister_CustomTintValues(SIInterface_ExtendData* �ӿ�, int ע������);
	void SI_API EXP_Add(SIInterface_ExtendData* �ӿ�, const double �ṩ�ľ���ֵ, SIEXPSourceType ����ֵ��Դ, SIValues_EXPMerge* �ϲ�����ֵ������); // �۳�����ʹ�ø���
	bool SI_API Buff_IsPowerOffline(SIInterface_ExtendData* �ӿ�);
	bool SI_API Buff_IsNeedHide(SIInterface_ExtendData* �ӿ�);
	int SI_API Buff_GetEffectDuration(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, int ԭʼ�Ĺ��س���ʱ��);
	double SI_API Buff_GetEffectHealth(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, double ԭʼ������ֵ);
	double SI_API Buff_GetEffectPower(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, double ԭʼ��ǿ��ֵ);
	SIBuffClass* SI_API Buff_CreateOrMerge(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, AbstractClass* ��Դ, HouseClass* ��Դ������ս��);
	void SI_API Buff_TryActive_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�);
	void SI_API Buff_TryActive_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void SI_API Buff_TryActive_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)); // [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryAfter_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����);
	void SI_API Buff_TryAfter_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff)); // [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryRemove_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����);
	void SI_API Buff_TryRemove_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff)); // [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryMergeSetting_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�);
	void SI_API Buff_TryMergeSetting_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* �ӿ�, void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)); // [�жϴ�����] ���� true ��� Buff ���д���
	void SI_API Buff_TryChange_A(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��);
	void SI_API Buff_TryChange_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	bool SI_API Buff_HasBuffed_A(SIInterface_ExtendData* �ӿ�);
	bool SI_API Buff_HasBuffed_B(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����);
	bool SI_API Buff_HasBuffed_C(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, SIBuffClass_State Buff״̬);
	bool SI_API Buff_HasBuffed_D(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, AbstractClass* �ų�����Դ);
	bool SI_API Buff_HasBuffed_E(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass* Buff����, AbstractClass* �������Դ, bool û�õı�ʶ);
	bool SI_API Buff_HasBuffed_F(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass_EffectType Ч������);
	bool SI_API Buff_HasBuffed_G(SIInterface_ExtendData* �ӿ�, SIBuffTypeClass_EffectType Ч������, SIBuffClass_State Buff״̬);
	void SI_API Buff_LookUp(SIInterface_ExtendData* �ӿ�, void* ��Դ, void (*��������)(void* ��Դ, SIBuffClass* Buff)); // �˴����������Ƴ���Ч�� Buff
	void SI_API Buff_Update(SIInterface_ExtendData* �ӿ�);
	bool SI_API Buff_TriggerFire(SIInterface_ExtendData* �ӿ�, AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������);
	double SI_API Buff_TriggerAttacker(SIInterface_ExtendData* �ӿ�, TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	double SI_API Buff_TriggerDefender(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	double SI_API Buff_TriggerDefender_Final(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	void SI_API Buff_TriggerDeath(SIInterface_ExtendData* �ӿ�, args_ReceiveDamage* �˺�����);
	void SI_API Buff_TriggerKiller(SIInterface_ExtendData* �ӿ�, TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����);
	double SI_API Buff_TriggerSelf(SIInterface_ExtendData* �ӿ�, double ��ǰ�˺�);
	void SI_API Buff_DoDamages(SIInterface_ExtendData* �ӿ�, double ׼����ɵ��˺�);
	void SI_API Element_Merge_ResistanceType(SIInterface_ExtendData* �ӿ�, SIElementResistanceTypeClass* Ԫ�ؿ�������, int ��������);
	void SI_API Element_Merge_ImmuneElementType(SIInterface_ExtendData* �ӿ�, SIElementTypeClass* Ԫ������, int ��������);
	void SI_API Update(SIInterface_ExtendData* �ӿ�);
	void SI_API UpdateBar_UpdateAllow(SIInterface_ExtendData* �ӿ�);
	void SI_API UpdateDrawBar(SIInterface_ExtendData* �ӿ�);
	bool SI_API UpdateClickBar_Hover(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, Point2D* �������ƫ��);
	void SI_API UpdateClickBar_TryClick(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, Point2D* �������ƫ��, int& Buff����, SIDigitalClickBarIndex& �������, double& ��ǰֵ, bool& ���ñ�ʶ);
	void SI_API UpdateClickBar_Effect(SIInterface_ExtendData* �ӿ�, int Buff����, SIDigitalClickBarIndex �������, HouseClass* �������ս��, double ��ǰֵ);
	bool SI_API UpdateKeyExecuteBar_TryExecute(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, SICommandCode ��ݼ�����);
	void SI_API UpdateKeyExecuteBar_Effect(SIInterface_ExtendData* �ӿ�, HouseClass* �������ս��, SICommandCode ��ݼ�����);

	//Update 0815
	SIBuffClass* SI_API Buff_GetBuff_A( SIInterface_ExtendData* �ӿ� , int Buff���� ); // ����ֵ����Ϊ��
	SIBuffClass* SI_API Buff_GetBuff_B( SIInterface_ExtendData* �ӿ� , SIBuffTypeClass* Buff���� ); // ����ֵ����Ϊ��
	void SI_API Init( SIInterface_ExtendData* �ӿ� );
	void SI_API UnInit( SIInterface_ExtendData* �ӿ� );
	void SI_API UpdateTechnoType( SIInterface_ExtendData* �ӿ� , TechnoTypeClass* �ɵĵ�λ���� );
	//��Ӧ�ڵ�λ��SetOwningHouse�����õ������ǲ�ͨ��techno->SetOwningHouse�����ı��˵�λ��������ս��
	void SI_API UpdateOwningHouse(SIInterface_ExtendData* �ӿ�, const HouseClass* const �ɵĵ�λ������ս��, const HouseClass* const �µĵ�λ������ս��);
	void SI_API AvailableEffect(SIInterface_ExtendData* �ӿ�);
	void SI_API UnavailableEffect(SIInterface_ExtendData* �ӿ�);

	
}

namespace SIBuff
{
	void SI_API TryRemark(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	void SI_API TryActive(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	void SI_API TryAfter(SIBuffClass* Buff); // ���� Removed , ���� Modified
	void SI_API TryDamage(SIBuffClass* Buff, double ׼����ɵ��˺�); // ���� Removed , ���� Modified
	// �ӿں��� - ���Ժ���
	void SI_API EnterState(SIBuffClass* Buff, SIBuffClass_State ״̬); // ���� Removed , ���� Modified
	void SI_API MergeSetting(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�); // ���� Removed , ���� Modified
	CoordStruct SI_API GetCenterCoords(SIBuffClass* Buff);
	TechnoClass* SI_API GetOwnerTechno(SIBuffClass* Buff); // ����ֵ����Ϊ��
	TechnoClass* SI_API GetSourceTechno(SIBuffClass* Buff); // ����ֵ����Ϊ��
	TechnoTypeClass* SI_API GetOwnerTechnoType(SIBuffClass* Buff); // ����ֵ����Ϊ��
	TechnoTypeClass* SI_API GetSourceTechnoType(SIBuffClass* Buff); // ����ֵ����Ϊ��
	HouseClass* SI_API GetActiveOwnerHouse(SIBuffClass* Buff);
	HouseClass* SI_API GetActiveSourceHouse(SIBuffClass* Buff);
	// �ӿں��� - ���ߺ���
	int SI_API GetEffectMode_A(SIBuffClass* Buff, int ����������, int ���ֵ);
	int SI_API GetEffectMode_B(SIBuffClass* Buff, int ����������, int ���ֵ, int ��Сֵ);
	double SI_API GetEffectPower_A(SIBuffClass* Buff, int ����������);
	double SI_API GetEffectPower_B(SIBuffClass* Buff, int ����������, double ��Сֵ);
	double SI_API GetEffectTotal_A(SIBuffClass* Buff, int ����������, double �ϲ����ʵ��Ч��ǿ��);
	double SI_API GetEffectTotal_B(SIBuffClass* Buff, int ����������, double �ϲ����ʵ��Ч��ǿ��, double ��Сֵ);
	double SI_API GetEffectTotalMax(SIBuffClass* Buff, int ����������);
	double SI_API GetEffectTotalMin(SIBuffClass* Buff, int ����������);
	bool SI_API Effect_NotPassCommonCheck_A(SIBuffClass* Buff);
	bool SI_API Effect_NotPassCommonCheck_B(SIBuffClass* Buff, SIBuffClass_EffectData* ��Ч����);
	bool SI_API Effect_NotPassCommonCheck_C(SIBuffClass* Buff, args_ReceiveDamage* �˺�����);
	bool SI_API Effect_NotPassCommonCheck_D(SIBuffClass* Buff, args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	bool SI_API Effect_NotPassCommonCheck_E(SIBuffClass* Buff, double ��ǰ�˺�);
	bool SI_API Effect_NotPassCommonCheck_NoIronCurtain(SIBuffClass* Buff);
	bool SI_API Effect_PassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����);
	bool SI_API Effect_NotPassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����);
	bool SI_API Effect_Match_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����);
	bool SI_API Effect_NotMatch_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����);
	void SI_API ReceiveWarheadDamage_A(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����); // ������ SIDamageLeft ����
	void SI_API ReceiveWarheadDamage_B(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����, HouseClass* ������λ������ս��); // ������ SIDamageLeft ����
	void SI_API ReceiveHealthDamage_A(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, double ����ֵ���, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������); // ������ SIDamageLeft ����
	void SI_API ReceiveHealthDamage_B(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, double ����ֵ���, double ����ֵ�������, double ����ֵ�������, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������); // ������ SIDamageLeft ����
	bool SI_API FreshOrPostBroadcast(SIBuffClass* Buff, double ��Ҫ���µĹ㲥ǿ��, AbstractClass* ָ��Ŀ��);
	void SI_API ResetBroadcastPower(SIBuffClass* Buff, double ��Ҫ���µĹ㲥ǿ��);
	int SI_API GetBroadcastListenerCount(SIBuffClass* Buff, bool ������������, AbstractClass* ָ��Ŀ��);
	bool SI_API AddAsBroadcastListener(SIBuffClass* Buff);
	void SI_API RemoveAsBroadcastListener(SIBuffClass* Buff);
	double SI_API GetBroadcastPower_Total(SIBuffClass* Buff);
	TechnoClass* SI_API GetFireOwner(SIBuffClass* Buff); // ����ֵ����Ϊ��
	bool SI_API GetFireOwnerRawOffset(SIBuffClass* Buff, CoordStruct& ��������);
	bool SI_API GetFireTargetRawOffset(SIBuffClass* Buff, AbstractClass* Ŀ��, CoordStruct& Ŀ������);
	double SI_API GetFacingRotate(SIBuffClass* Buff, SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������);
	double SI_API GetFacingRotate_Same(SIBuffClass* Buff, SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������, SIRotateType ����ͬ���ĸ�����ת����, double ����ͬ������ת�Ƕ�_����);
	void SI_API FreshFacingAndAimingRadius_Common(SIBuffClass* Buff, CoordStruct ��������, CoordStruct ���忪������, CoordStruct Ŀ������, CoordStruct& ��������, CoordStruct& ��������);
	void SI_API OffsetMotion_Init(SIBuffClass* Buff);
	void SI_API OffsetMotion_AI(SIBuffClass* Buff);
	void SI_API DigitalButtonData_Init(SIBuffClass* Buff, int ��λ����);
	

}

namespace SIBroadcast
{
	void SI_API FreshBroadcastPower(SIBroadcastClass* �ӿ�, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void SI_API PostBroadcast(SIBroadcastClass* �ӿ�, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	int SI_API GetBroadcastListenerCount(SIBroadcastClass* �ӿ�, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void SI_API AddListener(SIBroadcastClass* �ӿ�, SIBuffClass* Ҫ��ӵļ���Buff);
	void SI_API RemoveListener(SIBroadcastClass* �ӿ�, SIBuffClass* Ҫ�Ƴ��ļ���Buff);
	bool SI_API IsEmpty(SIBroadcastClass* �ӿ�);
	SIConstVector<SIBroadcastClass*>& SI_API GetBroadcastArray();
	HouseClass* SI_API OwnerObject(SIBroadcastClass* �ӿ�);
	double SI_API GetBroadcastPower(SIBroadcastClass* �ӿ�);
	const SIDataList<SIBuffClass*>& SI_API GetListenerBuffList(SIBroadcastClass* �ӿ�);
}

namespace SIHouseExt
{
	void SI_API Counter_AddOriginTechnoType(SIHouse_ExtendData* �ӿ�, TechnoTypeClass* ��λ����);
	void SI_API Counter_RemoveOriginTechnoType(SIHouse_ExtendData* �ӿ�, TechnoTypeClass* ��λ����);
	bool SI_API AlwaysShowCameo(const SIHouse_ExtendData* const �ӿ�, const TechnoTypeClass* const ��λ����, bool �������ڽ���ĵ�λ);// const;
	void SI_API Factory_CalculateProduction(SIHouse_ExtendData* �ӿ�);
	void SI_API Factory_MergeProduction(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ);
	void SI_API Factory_RemoveProduction(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ);
	void SI_API EXP_Init(SIHouse_ExtendData* �ӿ�);
	void SI_API EXP_Get(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, SIValues_HouseInitEXP* ���Ҿ���ֵ������);
	bool SI_API EXP_MergeAcademy(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, TechnoTypeClass* ��λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	bool SI_API EXP_RemoveAcademy(SIHouse_ExtendData* �ӿ�, TechnoClass* ��λ, TechnoTypeClass* ��λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	bool SI_API EXP_UpdateSpyEffect(SIHouse_ExtendData* �ӿ�, BuildingClass* ������λ, BuildingTypeClass* ������λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	void SI_API SetHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ, double ��ֵ);
	double SI_API GetHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ);
	void SI_API DeleteHouseVar(SIHouse_ExtendData* �ӿ�, int ����ֵ);
	void SI_API FreshBroadcastPower(SIHouse_ExtendData* �ӿ�, int Ƶ��, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void SI_API PostBroadcast(SIHouse_ExtendData* �ӿ�, int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	int SI_API GetBroadcastListenerCount(SIHouse_ExtendData* �ӿ�, int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	double SI_API GetBroadcastPower(SIHouse_ExtendData* �ӿ�, int Ƶ��);
	void SI_API AddBroadcastListener(SIHouse_ExtendData* �ӿ�, int Ƶ��, SIBuffClass* ����Buff);
	void SI_API RemoveBroadcastListener(SIHouse_ExtendData* �ӿ�, int Ƶ��, SIBuffClass* ����Buff);
	void SI_API EXPPool_Add(SIHouse_ExtendData* �ӿ�, double ���⾭��ֵ);
	double SI_API EXPPool_Pop(SIHouse_ExtendData* �ӿ�, double ����Ķ��⾭��ֵ);
	void SI_API ForEach_HouseVar(SIHouse_ExtendData* �ӿ�, void* ������, void (CALLBACK* ������)(void* ������, int ����, double& ֵ));
	void SI_API ForEach_Broadcast(SIHouse_ExtendData* �ӿ�, void* ������, void (CALLBACK* ������)(void* ������, int Ƶ��, SIBroadcastClass* ֵ));
	SIBroadcastClass* SI_API Broadcast_FindOrAllocate(SIHouse_ExtendData* �ӿ�, int Ƶ��);// ����ֵ����Ϊ��
	SIBroadcastClass* SI_API Broadcast_Find(SIHouse_ExtendData* �ӿ�, int Ƶ��);// ����ֵ����Ϊ��
	HouseClass* SI_API OwnerObject(SIHouse_ExtendData* �ӿ�);
}

namespace SIClassManager
{
	//��ʲ������ ��Ȱ����RegisterBuff
	int SI_API RegisterEffectType(const char* Ч����������, long ���ָ��);



	SIPack_BuffSetting_FromStatic SI_API MakePack(SIPackTypeClass_BuffSetting* ����, HouseClass* ��ǰ��ս��);
	SIPack_BuffSetting_FromStatic SI_API MakePack_Empty();
	SIPack_BuffSetting_FromStatic SI_API MakePack_DurationOnly(int ����ʱ��, bool ���ǳ���ʱ��, bool ���ܳ���ʱ�俹��);
	SIPack_BuffSetting_FromStatic SI_API MakePack_PowerOnly(double ǿ��ֵ, bool ����ǿ��ֵ, bool ����ǿ��ֵ����);
}