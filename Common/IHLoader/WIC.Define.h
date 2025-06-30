#pragma once

/*
！！不建议使用此头文件当中的原始接口！！
*/


#include "IH.Ext.h"

#define WIC_LIBRARY_VERSION 10

#define WIC_LibName "SIWinterIsComing"
#define SIInterfaceDecl_Impl(Name) \
	static FuncInfo* fn { nullptr };\
	if (!WIC.Available())WIC = Ext::GetLib(WIC_LibName);\
	if (!WIC.Available())throw SIException("SIInterface : 无法载入 " WIC_LibName " 。");\
	if(!fn) fn = WIC.QueryFunction(Name, WIC_LIBRARY_VERSION);\
	if(!fn || !fn->Func)throw SIException("SIInterface : 接口 " Name " 不可用。");
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
 SIException ：调用WIC接口时可能会抛出的异常。
 可能抛出SIException的情况有两种：
 一、WIC未成功载入。请在调用WIC的API之前通过SI::Available()检查。
	在返回一次true之后，WIC就总是可以正确加载的了。
	由于载入发生在对局开始之前，通常只需开局时检查一次。
 二、WIC载入了，但无法获取API。
	很可能是包含了新版的SDK而实际装载了旧的DLL。
	通过更新WIC的版本可以基本解决问题。
 你可以在调用API的时候放进try块当中，然后catch相关问题，
 或确保抛出异常的情况不会发生。
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
	经验来源_未知 = 0x0,

	经验来源_击杀 = 0x1,
	经验来源_国家能力 = 0x2,
	经验来源_训练所 = 0x4,
	经验来源_间谍渗透 = 0x8,
	经验来源_弹头提供 = 0x10,
	经验来源_Buff提供 = 0x20,
	经验来源_分配经验值 = 0x40,

	经验来源_系统提供 = 0x10000,
	经验来源_系统扣除 = 0x20000
};

enum SICommandCode : int
{
	无效按键 = 0,

	按键_01_快捷键A = 1,
	按键_02_快捷键B = 2,
	按键_03_快捷键C = 3,
	按键_04_快捷键D = 4,
	按键_05_快捷键E = 5,
	按键_06_快捷键F = 6,
	按键_07_快捷键G = 7,
	按键_08_快捷键H = 8,
	按键_09_快捷键I = 9,
	按键_10_快捷键J = 10,
	按键_11_快捷键K = 11,
	按键_12_快捷键L = 12,

	按键_最小值 = 按键_01_快捷键A,
	按键_最大值 = 按键_12_快捷键L
};

enum SIDigitalClickBarIndex : int
{
	无效点击 = 0,
	点击_1_Primary = 1,
	点击_2_Secondary = 2,
	点击_3_Tertiary = 3,
	点击_4_Quaternary = 4,
	点击_5_Button = 5
};

struct SIBuffClass_EffectData
{
public:
	// 属性
	bool 禁止生效 = false;
};

#define SIBuffClass_InfinityValue -1
#define SIBuffClass_CounterDefaultValue 0
#define SIBuffClass_Count_MaxValue 1000000000

#define SIBuffClass_CountLeft_MAX 1000000000 // 最大值为 10e
#define SIBuffClass_CountLeft_MIN 0 // 最小值为 0

#define SIBuffClass_DelayLeft_MAX 1000000000 // 最大值为 10e
#define SIBuffClass_DelayLeft_MIN 0 // 最小值为 0

#define SIBuffClass_TimerLeft_MAX 1000000000 // 最大值为 10e
#define SIBuffClass_TimerLeft_MIN 0 // 最小值为 0

#define SIBuffClass_PowerLeft_MAX 1000000000.0 // 最大值为 10e
#define SIBuffClass_PowerLeft_MIN -1000000000.0 // 最小值为 -10e

#define SIBuffClass_DefaultValue_CacheTargetValue 0.0

enum SIBuffClass_State : int
{
	未初始化 = 99,
	挂载 = 0,
	激发 = 1,
	生效 = 2,
	结束 = 3,
	移除 = 100
};

// 水平对齐类型
#define SIAlignHorizontalType_None "None"
#define SIAlignHorizontalType_Left "Left"
#define SIAlignHorizontalType_Right "Right"
#define SIAlignHorizontalType_Center "Center"

enum SIAlignHorizontalType : unsigned int
{
	水平对齐_空 = 0,
	水平对齐_左 = 1,
	水平对齐_右 = 2,
	水平对齐_居中 = 3
};

// 垂直对齐类型
#define SIAlignVerticalType_None "None"
#define SIAlignVerticalType_Top "Top"
#define SIAlignVerticalType_Bottom "Bottom"
#define SIAlignVerticalType_Center "Center"

enum SIAlignVerticalType : unsigned int
{
	垂直对齐_空 = 0,
	垂直对齐_上 = 1,
	垂直对齐_下 = 2,
	垂直对齐_居中 = 3
};

// 数值显示_显示类型
#define SIDigitalDisplayType_Default "Default"
#define SIDigitalDisplayType_Horizontal "Horizontal"
#define SIDigitalDisplayType_Hexagon "Hexagon"
#define SIDigitalDisplayType_Hexagon2 "Hexagon2"

enum SIDigitalDisplayType
{
	数值显示_显示类型_默认 = 0,
	数值显示_显示类型_水平 = 1,
	数值显示_显示类型_六边形 = 3,
	数值显示_显示类型_六边形2 = 4
};

// 数值显示_方向类型
#define SIDigitalDirectionType_Left "Left"
#define SIDigitalDirectionType_Right "Right"
#define SIDigitalDirectionType_Top "Top"
#define SIDigitalDirectionType_Bottom "Bottom"

enum SIDigitalDirectionType
{
	数值显示_方向类型_左侧 = 0,
	数值显示_方向类型_右侧 = 1,
	数值显示_方向类型_上侧 = 2,
	数值显示_方向类型_下侧 = 3
};

// 数值显示_队列类型
#define SIDigitalColumnType_None "None"
#define SIDigitalColumnType_Left "Left"
#define SIDigitalColumnType_Right "Right"
#define SIDigitalColumnType_Center "Center"
#define SIDigitalColumnType_Screen "Screen"

enum SIDigitalColumnType
{
	数值显示_队列类型_空 = 0,
	数值显示_队列类型_左侧 = 1,
	数值显示_队列类型_右侧 = 2,
	数值显示_队列类型_中间 = 3,
	数值显示_队列类型_屏幕 = 4
};

#define SIBuffTypeClass_After_EnterAfter "After"
#define SIBuffTypeClass_After_EnterReset "Reset"
#define SIBuffTypeClass_After_EnterRemove "Remove"
#define SIBuffTypeClass_After_EnterNextBuff "Next"
#define SIBuffTypeClass_After_EnterNextBuffAfter "AfterNext"

enum SIBuffTypeClass_AfterType : unsigned int
{
	进入结束 = 0,
	进入挂载 = 1,
	进入移除 = 2,
	进入切换Buff = 3,
	进入切换Buff延迟 = 4
};


// 跟随旋转类型
#define SIRotateType_None "None"
#define SIRotateType_Self "Self"
#define SIRotateType_Body "Body"
#define SIRotateType_Target "Target"
#define SIRotateType_Random "Random"
#define SIRotateType_Same "Same"

enum SIRotateType : unsigned int
{
	不旋转 = 0,
	自身实际朝向旋转 = 1,
	自身身体朝向旋转 = 2,
	自身到目标的方向旋转 = 3,
	随机旋转 = 4,
	同步旋转 = 5
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
	// 基础参数
	SIDigitalDisplayType 显示类型 = SIDigitalDisplayType::数值显示_显示类型_水平;
	int 初始像素偏移_X = 0;
	int 初始像素偏移_Y = 0;
	int 单位血条偏移_X = 0;
	int 单位血条偏移_Y = 0;
	// 偏移参数
	int 左侧偏移_X = 0;
	int 左侧偏移_Y = 0;
	int 左侧宽度 = 0;
	int 左侧高度 = 0;
	int 左侧血条格子数量 = 0;
	int 右侧偏移_X = 0;
	int 右侧偏移_Y = 0;
	int 右侧宽度 = 0;
	int 右侧高度 = 0;
	int 右侧血条格子数量 = 0;
	int 中间偏移_X = 0;
	int 中间偏移_Y = 0;
	int 中间宽度 = 0;
	int 中间高度 = 0;
	int 中间血条格子数量 = 0;
	// 绘制参数
	RectangleStruct* 绘制区域;
	int 亮度 = 1000;
	int 染色 = 0;
	// 设置参数
	bool 允许动态偏移 = true;
	// 数值参数
	double 当前值 = 0.0;
	double 最大值 = 0.0;
	double 最小值 = 0.0;
	double 变动值 = 0.0;
	double 缓冲值 = 0.0;
	bool 仅绘制血条 = false;
};


#define SIBuffTypeClass_EffectType_None "None"
#define SIBuffTypeClass_EffectType_Empty "Empty"
#define SIBuffTypeClass_EffectType_OnlyDuration "OnlyDuration"
#define SIBuffTypeClass_EffectType_OnlyClickable "OnlyClickable"
#define SIBuffTypeClass_EffectType_DelayAnimation "DelayAnimation"
// 01X 杂项
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
// 02X Buff 处理
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
// 03X 伤害控制
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
// 04X 生命控制
#define SIBuffTypeClass_EffectTypePrefix_Health "Health"

#define SIBuffTypeClass_EffectType_HealthVampire "HealthVampire"
#define SIBuffTypeClass_EffectType_HealthLock "HealthLock"
#define SIBuffTypeClass_EffectType_HealthHeal "HealthHeal"
#define SIBuffTypeClass_EffectType_HealthReset "HealthReset"
// 05X 攻击控制
#define SIBuffTypeClass_EffectTypePrefix_Attack "Attack"

#define SIBuffTypeClass_EffectType_AttackRatio "AttackRatio"
// 06X 广播与监听
#define SIBuffTypeClass_EffectTypePrefix_Broadcast "Broadcast"
#define SIBuffTypeClass_EffectTypePrefix_Listener "Listener"

#define SIBuffTypeClass_EffectType_Broadcast "Broadcast"
#define SIBuffTypeClass_EffectType_BroadcastDamage "BroadcastDamage"

#define SIBuffTypeClass_EffectType_ListenerBlank "ListenerBlank"
#define SIBuffTypeClass_EffectType_ListenerHospital "ListenerHospital"
#define SIBuffTypeClass_EffectType_ListenerWarhead "ListenerWarhead"
// 07X 数值显示
#define SIBuffTypeClass_EffectTypePrefix_Digital "Digital"

#define SIBuffTypeClass_EffectType_DigitalHouseVar "DigitalHouseVar"
#define SIBuffTypeClass_EffectType_DigitalEXPPool "DigitalEXPPool"
// 08X 开火效果
// 09X 特殊效果
// 10X 载员控制
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
// 11X Buff 武器
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
// 12X 单位属性
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
// 502 操作作战方局部变量
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
// 504 判断数值
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
	// 00X 空效果
	无效果 = 0,
	空效果 = 1,
	只有持续时间 = 2,
	只有可以点击 = 10,
	只有间歇动画 = 20,
	// 01X 杂项
	引爆弹头 = 1001,
	引爆弹头死亡时 = 1002,
	原地去世 = 1020,
	连锁死亡 = 1021,
	建筑变卖 = 1028,
	立刻狩猎 = 1029,
	静默状态 = 1030,
	嘲讽状态 = 1031,
	跳过主动处理阶段 = 1040,
	经验值 = 1070,
	经验池 = 1071,
	弹药调整 = 1072,
	弹药检测 = 1073,
	吸收矿物 = 1074,
	生产矿物 = 1075,
	投放单位 = 1120,
	投放单位死亡时 = 1121,
	工厂 = 1130,
	技能 = 1131,
	// 02X Buff 处理
	Buff挂载 = 2000,
	Buff激发 = 2001,
	Buff结束 = 2002,
	Buff移除 = 2003,
	Buff强度 = 2004,
	Buff转换 = 2005,
	Buff额外代码 = 2006,

	攻击Buff挂载 = 2010,
	攻击Buff激发 = 2011,
	攻击Buff结束 = 2012,
	攻击Buff移除 = 2013,
	攻击Buff强度 = 2014,
	攻击Buff转换 = 2015,
	攻击转化Buff挂载 = 2016,
	攻击转化Buff激发 = 2017,
	攻击转化Buff强度 = 2018,
	攻击转化Buff转换 = 2019,

	防御Buff挂载 = 2020,
	防御Buff激发 = 2021,
	防御Buff结束 = 2022,
	防御Buff移除 = 2023,
	防御Buff强度 = 2024,
	防御Buff转换 = 2025,
	防御转化Buff挂载 = 2026,
	防御转化Buff激发 = 2027,
	防御转化Buff强度 = 2028,
	防御转化Buff转换 = 2029,

	受伤Buff挂载 = 2030,
	受伤Buff激发 = 2031,
	受伤Buff结束 = 2032,
	受伤Buff移除 = 2033,
	受伤Buff强度 = 2034,
	受伤Buff转换 = 2035,
	受伤转化Buff挂载 = 2036,
	受伤转化Buff激发 = 2037,
	受伤转化Buff强度 = 2038,
	受伤转化Buff转换 = 2039,

	击杀Buff挂载 = 2050,
	击杀Buff激发 = 2051,
	击杀Buff结束 = 2052,
	击杀Buff移除 = 2053,
	击杀Buff强度 = 2054,
	击杀Buff转换 = 2055,
	击杀转化Buff挂载 = 2056,
	击杀转化Buff激发 = 2057,
	击杀转化Buff强度 = 2058,
	击杀转化Buff转换 = 2059,

	Buff挂载监听 = 2080,
	Buff激发监听 = 2081,
	Buff结束监听 = 2082,
	Buff移除监听 = 2083,
	Buff强度监听 = 2084,
	// 03X 伤害控制
	伤害承担 = 3001, // 护盾效果
	伤害格挡 = 3002, // 可以抵挡指定次数的攻击 , 可以指定临界伤害值
	伤害分摊 = 3003, // 向周围的单位分摊受到的伤害
	伤害免疫 = 3004, // 免疫特定类型的伤害
	伤害闪避 = 3005, // 一定概率闪避伤害
	伤害延迟 = 3006, // 免疫伤害并把伤害转为较低的帧伤害
	伤害限制 = 3007, // 设置允许受到的最大和最小的伤害值
	伤害增幅 = 3008, // 增减伤
	伤害隔离 = 3009, // 受到过高或过低的伤害时免疫伤害
	伤害反射 = 3010, // 反射受到的伤害
	伤害连锁 = 3011, // 受到的伤害会传递至挂载 Buff 的单位
	伤害即死 = 3012,
	伤害免死 = 3013,
	伤害记录 = 3014,
	// 04X 生命控制
	生命汲取 = 4001, // 吸血
	生命锁定 = 4002, // 生命值无法低于一定的数值
	生命恢复 = 4003, // 自疗
	生命重置 = 4004,
	// 05X 攻击控制
	攻击增幅 = 5001,
	// 06X 广播与监听
	广播 = 6000,
	伤害转化广播 = 6001,

	空白监听 = 6100,
	医院监听 = 6101,
	弹头监听 = 6102,
	// 07X 数值显示
	显示作战方局部变量 = 7001,
	显示作战方经验池 = 7002,
	// 08X 开火效果
	// 09X 特殊效果
	// 10X 载员控制
	载具填充_主动 = 10000,
	载具吸纳_主动 = 10001,
	载具释放_主动 = 10002,
	载具移除_主动 = 10003,
	载员进入_主动 = 10010,
	载员脱出_主动 = 10011,

	载具填充_攻击 = 10100,
	载具吸纳_攻击 = 10101,
	载具释放_攻击 = 10102,
	载具移除_攻击 = 10103,
	载员进入_攻击 = 10110,
	载员脱出_攻击 = 10111,

	载具填充_防御 = 10200,
	载具吸纳_防御 = 10201,
	载具释放_防御 = 10202,
	载具移除_防御 = 10203,
	载员进入_防御 = 10210,
	载员脱出_防御 = 10211,
	// 11X Buff 武器
	武器_替换武器 = 11000,
	武器_替换死亡武器 = 11001,
	武器_除你武器 = 11002,

	武器_发射武器 = 11010,
	武器_自瞄武器 = 11011,
	武器_齐射武器 = 11012,
	武器_散射武器 = 11013,
	武器_溅射武器 = 11014,

	武器_死亡复仇 = 11100,
	武器_攻击复仇 = 11101,
	武器_受伤复仇 = 11102,

	武器_死亡殉爆 = 11110,
	武器_攻击殉爆 = 11111,
	武器_受伤殉爆 = 11112,
	// 12X 单位属性
	单位属性_加成_倍率移动 = 12000,
	单位属性_加成_倍率护甲 = 12001,
	单位属性_加成_倍率护甲伤害 = 12002,
	单位属性_加成_倍率火力 = 12003,
	单位属性_加成_倍率射速 = 12004,
	单位属性_加成_倍率经验值获取 = 12005,
	单位属性_加成_倍率经验值提供 = 12006,
	单位属性_加成_倍率经验值需求 = 12007,
	单位属性_加成_倍率武器射程 = 12008,
	单位属性_加成_倍率弹头溅射范围 = 12020,

	单位属性_加成_累乘移动 = 12100,
	单位属性_加成_累乘护甲 = 12101,
	单位属性_加成_累乘护甲伤害 = 12102,
	单位属性_加成_累乘火力 = 12103,
	单位属性_加成_累乘射速 = 12104,
	单位属性_加成_累乘经验值获取 = 12105,
	单位属性_加成_累乘经验值提供 = 12106,
	单位属性_加成_累乘经验值需求 = 12107,
	单位属性_加成_累乘武器射程 = 12108,
	单位属性_加成_累乘弹头溅射范围 = 12120,

	单位属性_加成_追加移动 = 12200,
	单位属性_加成_追加伤害 = 12202,
	单位属性_加成_追加攻击 = 12203,
	单位属性_加成_追加射速 = 12204,
	单位属性_加成_追加经验值获取 = 12205,
	单位属性_加成_追加经验值提供 = 12206,
	单位属性_加成_追加经验值需求 = 12207,
	单位属性_加成_追加武器射程 = 12208,
	单位属性_加成_追加弹头溅射范围 = 12220,

	单位属性_加成_最终移动 = 12300,
	单位属性_加成_最终伤害 = 12302,
	单位属性_加成_最终攻击 = 12303,
	单位属性_加成_最终射速 = 12304,
	单位属性_加成_最终经验值获取 = 12305,
	单位属性_加成_最终经验值提供 = 12306,
	单位属性_加成_最终经验值需求 = 12307,
	单位属性_加成_最终武器射程 = 12308,
	单位属性_加成_最终弹头溅射范围 = 12320,

	单位属性_能力_免疫碾压 = 12400,
	单位属性_能力_免疫高级碾压 = 12401,
	单位属性_能力_免疫辐射弹头 = 12402,
	单位属性_能力_免疫心灵伤害弹头 = 12403,
	单位属性_能力_免疫心控弹头 = 12404,
	单位属性_能力_免疫狂暴弹头 = 12405,
	单位属性_能力_免疫毒素弹头 = 12406,
	单位属性_能力_免疫超时空传送弹头 = 12407,
	单位属性_能力_免疫电磁脉冲弹头 = 12408,

	单位属性_能力_自愈 = 12450,
	单位属性_能力_自愈_每次恢复的生命值比例 = 12451,
	单位属性_能力_自愈_最大生命值比例 = 12452,
	单位属性_能力_自愈_恢复时间间隔 = 12453,
	单位属性_能力_自愈_脱战时间间隔 = 12454,

	单位属性_染色 = 12500,
	// 502 操作作战方局部变量
	操作作战方局部变量_主动_加 = 50200,
	操作作战方局部变量_主动_乘 = 50201,
	操作作战方局部变量_主动_指数 = 50202,
	操作作战方局部变量_主动_对数 = 50203,
	操作作战方局部变量_主动_取模 = 50204,
	操作作战方局部变量_主动_等于 = 50210,
	操作作战方局部变量_主动_不等于 = 50211,
	操作作战方局部变量_主动_大于 = 50212,
	操作作战方局部变量_主动_大于等于 = 50213,
	操作作战方局部变量_主动_小于 = 50214,
	操作作战方局部变量_主动_小于等于 = 50215,
	操作作战方局部变量_主动_取反 = 50220,
	操作作战方局部变量_主动_绝对值 = 50221,
	操作作战方局部变量_主动_向上取整 = 50222,
	操作作战方局部变量_主动_向下取整 = 50223,
	操作作战方局部变量_主动_四舍五入 = 50224,
	操作作战方局部变量_主动_覆盖 = 50230,
	操作作战方局部变量_主动_最大 = 50231,
	操作作战方局部变量_主动_最小 = 50232,
	操作作战方局部变量_主动_随机 = 50233,
	操作作战方局部变量_主动_移除 = 50234,

	操作作战方局部变量_死亡_加 = -50200,
	操作作战方局部变量_死亡_乘 = -50201,
	操作作战方局部变量_死亡_指数 = -50202,
	操作作战方局部变量_死亡_对数 = -50203,
	操作作战方局部变量_死亡_取模 = -50204,
	操作作战方局部变量_死亡_等于 = -50210,
	操作作战方局部变量_死亡_不等于 = -50211,
	操作作战方局部变量_死亡_大于 = -50212,
	操作作战方局部变量_死亡_大于等于 = -50213,
	操作作战方局部变量_死亡_小于 = -50214,
	操作作战方局部变量_死亡_小于等于 = -50215,
	操作作战方局部变量_死亡_取反 = -50220,
	操作作战方局部变量_死亡_绝对值 = -50221,
	操作作战方局部变量_死亡_向上取整 = -50222,
	操作作战方局部变量_死亡_向下取整 = -50223,
	操作作战方局部变量_死亡_四舍五入 = -50224,
	操作作战方局部变量_死亡_覆盖 = -50230,
	操作作战方局部变量_死亡_最大 = -50231,
	操作作战方局部变量_死亡_最小 = -50232,
	操作作战方局部变量_死亡_随机 = -50233,
	操作作战方局部变量_死亡_移除 = -50234,
	// 504 判断数值
	判断数值_主动_等于 = 50400,
	判断数值_主动_不等于 = 50401,
	判断数值_主动_大于 = 50402,
	判断数值_主动_大于等于 = 50403,
	判断数值_主动_小于 = 50404,
	判断数值_主动_小于等于 = 50405,

	判断数值_死亡_等于 = -50400,
	判断数值_死亡_不等于 = -50401,
	判断数值_死亡_大于 = -50402,
	判断数值_死亡_大于等于 = -50403,
	判断数值_死亡_小于 = -50404,
	判断数值_死亡_小于等于 = -50405
};

#define SIVariableType_Normal "Normal"
#define SIVariableType_Global "Global"
#define SIVariableType_Local "Local"
#define SIVariableType_House "House"

enum SIVariableType : unsigned int
{
	普通变量 = 0,
	全局变量 = 1,
	局部变量 = 2,
	作战方局部变量 = 3
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

// 伤害处理方式
#define SIDamageProcessType_Detonate "Detonate"
#define SIDamageProcessType_Damage "Damage"
#define SIDamageProcessType_Health "Health"
#define SIDamageProcessType_OnlyHealth "OnlyHealth"

enum SIDamageProcessType : unsigned int
{
	伤害处理_引爆 = 0,
	伤害处理_伤害 = 1,
	伤害处理_生命 = 2,
	伤害处理_仅生命 = 3
};

//获取详见SIPackTypeClass_BuffSetting的函数
struct SIPack_BuffSetting_FromStatic
{
public:
	// 持续时间
	bool SIDuration_Self;
	int SIDuration;
	int SIDuration_Max;
	int SIDuration_Min;
	bool SIDuration_Reset;
	bool SIDuration_Effect;
	bool SIDuration_AffectAfter;
	// 生命值
	bool SIHealth_Self;
	double SIHealth;
	double SIHealth_Max;
	double SIHealth_Min;
	bool SIHealth_Reset;
	bool SIHealth_Effect;
	// 强度值
	double SIPower;
	double SIPower_Max;
	double SIPower_Min;
	bool SIPower_Reset;
	bool SIPower_Effect;
};

struct SIValues_EXPMerge
{
public:
	double 经验值基数;
	HouseClass* 提供经验值的单位所属的作战方;
	bool 需要单位允许获得经验值;
	bool 需要单位允许减少经验值;
	bool 适用单位属性的经验加成;
	bool 允许分配经验值;

	SIValues_EXPMerge(double 经验值基数, HouseClass* 提供经验值的单位所属的作战方, bool 需要单位允许获得经验值, bool 需要单位允许减少经验值, bool 适用单位属性的经验加成, bool 允许分配经验值)
		: 经验值基数(经验值基数)
		, 提供经验值的单位所属的作战方(提供经验值的单位所属的作战方)
		, 需要单位允许获得经验值(需要单位允许获得经验值)
		, 需要单位允许减少经验值(需要单位允许减少经验值)
		, 适用单位属性的经验加成(适用单位属性的经验加成)
		, 允许分配经验值(允许分配经验值)
	{}
};

// 染色参数包
struct SIValues_CustomTint
{
	int 染色颜色;
	int 染色强度;
	int 作战方归属;
};

// 国家经验值参数包
struct SIValues_HouseInitEXP
{
	double 经验值_国家能力;
	double 经验等级_国家能力;
	double 经验值_训练所;
	double 经验等级_训练所;
	double 经验值_间谍渗透;
	double 经验等级_间谍渗透;

	SIValues_HouseInitEXP()
		: 经验值_国家能力(0.0)
		, 经验等级_国家能力(0.0)
		, 经验值_训练所(0.0)
		, 经验等级_训练所(0.0)
		, 经验值_间谍渗透(0.0)
		, 经验等级_间谍渗透(0.0)
	{}

	bool IsEmpty()
	{
		return this->经验值_国家能力 == 0
			&& this->经验等级_国家能力 == 0
			&& this->经验值_训练所 == 0
			&& this->经验等级_训练所 == 0
			&& this->经验值_间谍渗透 == 0
			&& this->经验等级_间谍渗透 == 0;
	}

	void Reset()
	{
		this->经验值_国家能力 = 0.0;
		this->经验等级_国家能力 = 0.0;
		this->经验值_训练所 = 0.0;
		this->经验等级_训练所 = 0.0;
		this->经验值_间谍渗透 = 0.0;
		this->经验等级_间谍渗透 = 0.0;
	}

	void Merge(SIValues_HouseInitEXP* 国家经验值参数包)
	{
		this->经验值_国家能力 = 国家经验值参数包->经验值_国家能力;
		this->经验等级_国家能力 = 国家经验值参数包->经验等级_国家能力;
		this->经验值_训练所 = 国家经验值参数包->经验值_训练所;
		this->经验等级_训练所 = 国家经验值参数包->经验等级_训练所;
		this->经验值_间谍渗透 = 国家经验值参数包->经验值_间谍渗透;
		this->经验等级_间谍渗透 = 国家经验值参数包->经验等级_间谍渗透;
	}
};

#define SI_API __stdcall
/*
	这部分是SI的原始接口，不建议直接调用，请通过SIInterface_ExtendData类操控！
	谨记：类的静态函数的调用约定是__stdcall！！

	备注：此处已经确认了稳定存在的API及其形式，所以直接通过只有最小限度检查的LibData::QueryInterface实现
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
namespace SI
{
	char* SI_API GetName(SIInterface_ExtendData* 接口); // 类型 ID 名称
	AbstractClass* SI_API OwnerEntity(SIInterface_ExtendData* 接口); // 返回值可能为空
	AbstractType  SI_API WhatAmI(SIInterface_ExtendData* 接口);
	int  SI_API GetArrayIndex(SIInterface_ExtendData* 接口);
	bool  SI_API IsSelected(SIInterface_ExtendData* 接口);
	void  SI_API GetCoords(SIInterface_ExtendData* 接口, CoordStruct* 所在坐标);
	void  SI_API SetForceFireWeapon(SIInterface_ExtendData* 接口, WeaponTypeClass* 武器类型);
	void  SI_API SetForceDeathWeapon(SIInterface_ExtendData* 接口, WeaponTypeClass* 武器类型);
	CoordStruct  SI_API Calculate_FireCoord(SIInterface_ExtendData* 接口, CoordStruct 武器坐标, double 缩放倍率);
	int  SI_API GetStaticEXPLevel(SIInterface_ExtendData* 接口);
	double  SI_API GetLevelTotalEXPCost(SIInterface_ExtendData* 接口, double 固定经验值基数, bool 使用新经验系统);
	double SI_API Calculate_PropertyGroup_Speed(SIInterface_ExtendData* 接口, double 原始移动速度);
	double SI_API Calculate_PropertyGroup_Defence(SIInterface_ExtendData* 接口, double 原始伤害);
	double SI_API Calculate_PropertyGroup_Attack(SIInterface_ExtendData* 接口, double 原始伤害);
	double SI_API Calculate_PropertyGroup_ROF(SIInterface_ExtendData* 接口, double 原始射速);
	double SI_API Calculate_PropertyGroup_EXP(SIInterface_ExtendData* 接口, double 原始获得的经验值);
	double SI_API Calculate_PropertyGroup_EXPProvide(SIInterface_ExtendData* 接口, double 原始提供的经验值);
	double SI_API Calculate_PropertyGroup_EXPCost(SIInterface_ExtendData* 接口, double 单位升级所需的经验值);
	double SI_API Calculate_PropertyGroup_RangeWeapon(SIInterface_ExtendData* 接口, double 原始武器射程);
	double SI_API Calculate_PropertyGroup_CellSpread(SIInterface_ExtendData* 接口, double 原始弹头溅射范围);
	int SI_API GetTintColor(SIInterface_ExtendData* 接口, bool 铁幕染色, bool 空袭染色, bool 狂暴染色);
	void SI_API Calculate_CustomTintValues(SIInterface_ExtendData* 接口, int& 染色颜色, int& 染色强度);
	int SI_API Calculate_CustomTintValues_Color(SIInterface_ExtendData* 接口);
	int SI_API Calculate_CustomTintValues_Intensity(SIInterface_ExtendData* 接口);
	double SI_API Property_Merge_Multiplier_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_Armor(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_Versus(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Multiplier_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_Damage(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_Addon_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量); // 单位是格点
	double SI_API Property_Merge_Addon_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_Speed(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_Damage(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_Attack(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_ROF(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_EXP(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	double SI_API Property_Merge_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量); // 单位是格点
	double SI_API Property_Merge_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, double 旧的增加量, double 新的增加量);
	int SI_API Property_Register_Multiplier_Speed(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_Armor(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_Versus(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_Attack(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_ROF(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_EXP(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_EXPCost(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Multiplier_CellSpread(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_Speed(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_Damage(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_Attack(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_ROF(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_EXP(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_EXPProvide(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_EXPCost(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_Addon_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量); // 单位是格点
	int SI_API Property_Register_Addon_CellSpread(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_Speed(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_Damage(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_Attack(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_ROF(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_EXP(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, double 增加量);
	int SI_API Property_Register_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, double 增加量); // 单位是格点
	int SI_API Property_Register_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, double 增加量);
	void SI_API Property_Unregister_Multiplier_Speed(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_Armor(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_Versus(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_Attack(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_ROF(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_EXP(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_EXPCost(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Multiplier_CellSpread(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_Speed(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_Armor(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_Versus(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_Attack(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_ROF(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_EXP(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_EXPCost(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_ExtraMultiplier_CellSpread(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_Speed(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_Damage(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_Attack(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_ROF(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_EXP(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_EXPCost(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_Addon_CellSpread(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_Speed(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_Damage(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_Attack(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_ROF(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_EXP(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_EXPProvide(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_EXPCost(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_RangeWeapon(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Unregister_FinalAddon_CellSpread(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API Property_Merge_Ability_ImmuneCrush(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmuneCrushAdvance(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmuneRadiation(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmunePsionicDamage(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmunePsionic(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmuneBerzerk(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmunePoison(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmuneWarp(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_ImmuneEMP(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_SelfHealing(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_SelfHealing_Percent(SIInterface_ExtendData* 接口, double 旧的能力强度, double 新的能力强度);
	void SI_API Property_Merge_Ability_SelfHealing_Cap(SIInterface_ExtendData* 接口, double 旧的能力强度, double 新的能力强度);
	void SI_API Property_Merge_Ability_SelfHealing_Delay(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	void SI_API Property_Merge_Ability_SelfHealing_CombatDelay(SIInterface_ExtendData* 接口, int 旧的能力强度, int 新的能力强度);
	int SI_API Property_Register_CustomTintValues_A(SIInterface_ExtendData* 接口, ColorStruct 染色颜色, int 染色强度, int 作战方归属);
	int SI_API Property_Register_CustomTintValues_B(SIInterface_ExtendData* 接口, int 染色颜色, int 染色强度, int 作战方归属);
	void SI_API Property_Unregister_CustomTintValues(SIInterface_ExtendData* 接口, int 注册索引);
	void SI_API EXP_Add(SIInterface_ExtendData* 接口, const double 提供的经验值, SIEXPSourceType 经验值来源, SIValues_EXPMerge* 合并经验值参数包); // 扣除经验使用负数
	bool SI_API Buff_IsPowerOffline(SIInterface_ExtendData* 接口);
	bool SI_API Buff_IsNeedHide(SIInterface_ExtendData* 接口);
	int SI_API Buff_GetEffectDuration(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, int 原始的挂载持续时间);
	double SI_API Buff_GetEffectHealth(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, double 原始的生命值);
	double SI_API Buff_GetEffectPower(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, double 原始的强度值);
	SIBuffClass* SI_API Buff_CreateOrMerge(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, AbstractClass* 来源, HouseClass* 来源所属作战方);
	void SI_API Buff_TryActive_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包);
	void SI_API Buff_TryActive_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void SI_API Buff_TryActive_C(SIInterface_ExtendData* 接口, void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryAfter_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型);
	void SI_API Buff_TryAfter_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void SI_API Buff_TryAfter_C(SIInterface_ExtendData* 接口, void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryRemove_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型);
	void SI_API Buff_TryRemove_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void SI_API Buff_TryRemove_C(SIInterface_ExtendData* 接口, void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryMergeSetting_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包);
	void SI_API Buff_TryMergeSetting_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void SI_API Buff_TryMergeSetting_C(SIInterface_ExtendData* 接口, void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void SI_API Buff_TryChange_A(SIInterface_ExtendData* 接口, SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方);
	void SI_API Buff_TryChange_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	bool SI_API Buff_HasBuffed_A(SIInterface_ExtendData* 接口);
	bool SI_API Buff_HasBuffed_B(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型);
	bool SI_API Buff_HasBuffed_C(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, SIBuffClass_State Buff状态);
	bool SI_API Buff_HasBuffed_D(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, AbstractClass* 排除的来源);
	bool SI_API Buff_HasBuffed_E(SIInterface_ExtendData* 接口, SIBuffTypeClass* Buff类型, AbstractClass* 需求的来源, bool 没用的标识);
	bool SI_API Buff_HasBuffed_F(SIInterface_ExtendData* 接口, SIBuffTypeClass_EffectType 效果种类);
	bool SI_API Buff_HasBuffed_G(SIInterface_ExtendData* 接口, SIBuffTypeClass_EffectType 效果种类, SIBuffClass_State Buff状态);
	void SI_API Buff_LookUp(SIInterface_ExtendData* 接口, void* 来源, void (*遍历函数)(void* 来源, SIBuffClass* Buff)); // 此处不会立即移除无效的 Buff
	void SI_API Buff_Update(SIInterface_ExtendData* 接口);
	bool SI_API Buff_TriggerFire(SIInterface_ExtendData* 接口, AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器);
	double SI_API Buff_TriggerAttacker(SIInterface_ExtendData* 接口, TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害);
	double SI_API Buff_TriggerDefender(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数, double 当前伤害);
	double SI_API Buff_TriggerDefender_Final(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数, double 当前伤害);
	void SI_API Buff_TriggerDeath(SIInterface_ExtendData* 接口, args_ReceiveDamage* 伤害参数);
	void SI_API Buff_TriggerKiller(SIInterface_ExtendData* 接口, TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数);
	double SI_API Buff_TriggerSelf(SIInterface_ExtendData* 接口, double 当前伤害);
	void SI_API Buff_DoDamages(SIInterface_ExtendData* 接口, double 准备造成的伤害);
	void SI_API Element_Merge_ResistanceType(SIInterface_ExtendData* 接口, SIElementResistanceTypeClass* 元素抗性类型, int 层数计数);
	void SI_API Element_Merge_ImmuneElementType(SIInterface_ExtendData* 接口, SIElementTypeClass* 元素类型, int 层数计数);
	void SI_API Update(SIInterface_ExtendData* 接口);
	void SI_API UpdateBar_UpdateAllow(SIInterface_ExtendData* 接口);
	void SI_API UpdateDrawBar(SIInterface_ExtendData* 接口);
	bool SI_API UpdateClickBar_Hover(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, Point2D* 点击像素偏移);
	void SI_API UpdateClickBar_TryClick(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, Point2D* 点击像素偏移, int& Buff索引, SIDigitalClickBarIndex& 点击索引, double& 当前值, bool& 禁用标识);
	void SI_API UpdateClickBar_Effect(SIInterface_ExtendData* 接口, int Buff索引, SIDigitalClickBarIndex 点击索引, HouseClass* 点击的作战方, double 当前值);
	bool SI_API UpdateKeyExecuteBar_TryExecute(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, SICommandCode 快捷键代码);
	void SI_API UpdateKeyExecuteBar_Effect(SIInterface_ExtendData* 接口, HouseClass* 点击的作战方, SICommandCode 快捷键代码);

	//Update 0815
	SIBuffClass* SI_API Buff_GetBuff_A( SIInterface_ExtendData* 接口 , int Buff索引 ); // 返回值可能为空
	SIBuffClass* SI_API Buff_GetBuff_B( SIInterface_ExtendData* 接口 , SIBuffTypeClass* Buff类型 ); // 返回值可能为空
	void SI_API Init( SIInterface_ExtendData* 接口 );
	void SI_API UnInit( SIInterface_ExtendData* 接口 );
	void SI_API UpdateTechnoType( SIInterface_ExtendData* 接口 , TechnoTypeClass* 旧的单位类型 );
	//对应于单位的SetOwningHouse，调用的条件是不通过techno->SetOwningHouse函数改变了单位的所属作战方
	void SI_API UpdateOwningHouse(SIInterface_ExtendData* 接口, const HouseClass* const 旧的单位所属作战方, const HouseClass* const 新的单位所属作战方);
	void SI_API AvailableEffect(SIInterface_ExtendData* 接口);
	void SI_API UnavailableEffect(SIInterface_ExtendData* 接口);

	
}

namespace SIBuff
{
	void SI_API TryRemark(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	void SI_API TryActive(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	void SI_API TryAfter(SIBuffClass* Buff); // 存在 Removed , 存在 Modified
	void SI_API TryDamage(SIBuffClass* Buff, double 准备造成的伤害); // 存在 Removed , 存在 Modified
	// 接口函数 - 属性函数
	void SI_API EnterState(SIBuffClass* Buff, SIBuffClass_State 状态); // 存在 Removed , 存在 Modified
	void SI_API MergeSetting(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包); // 存在 Removed , 存在 Modified
	CoordStruct SI_API GetCenterCoords(SIBuffClass* Buff);
	TechnoClass* SI_API GetOwnerTechno(SIBuffClass* Buff); // 返回值可能为空
	TechnoClass* SI_API GetSourceTechno(SIBuffClass* Buff); // 返回值可能为空
	TechnoTypeClass* SI_API GetOwnerTechnoType(SIBuffClass* Buff); // 返回值可能为空
	TechnoTypeClass* SI_API GetSourceTechnoType(SIBuffClass* Buff); // 返回值可能为空
	HouseClass* SI_API GetActiveOwnerHouse(SIBuffClass* Buff);
	HouseClass* SI_API GetActiveSourceHouse(SIBuffClass* Buff);
	// 接口函数 - 工具函数
	int SI_API GetEffectMode_A(SIBuffClass* Buff, int 参数项索引, int 最大值);
	int SI_API GetEffectMode_B(SIBuffClass* Buff, int 参数项索引, int 最大值, int 最小值);
	double SI_API GetEffectPower_A(SIBuffClass* Buff, int 参数项索引);
	double SI_API GetEffectPower_B(SIBuffClass* Buff, int 参数项索引, double 最小值);
	double SI_API GetEffectTotal_A(SIBuffClass* Buff, int 参数项索引, double 合并后的实际效果强度);
	double SI_API GetEffectTotal_B(SIBuffClass* Buff, int 参数项索引, double 合并后的实际效果强度, double 最小值);
	double SI_API GetEffectTotalMax(SIBuffClass* Buff, int 参数项索引);
	double SI_API GetEffectTotalMin(SIBuffClass* Buff, int 参数项索引);
	bool SI_API Effect_NotPassCommonCheck_A(SIBuffClass* Buff);
	bool SI_API Effect_NotPassCommonCheck_B(SIBuffClass* Buff, SIBuffClass_EffectData* 生效数据);
	bool SI_API Effect_NotPassCommonCheck_C(SIBuffClass* Buff, args_ReceiveDamage* 伤害参数);
	bool SI_API Effect_NotPassCommonCheck_D(SIBuffClass* Buff, args_ReceiveDamage* 伤害参数, double 当前伤害);
	bool SI_API Effect_NotPassCommonCheck_E(SIBuffClass* Buff, double 当前伤害);
	bool SI_API Effect_NotPassCommonCheck_NoIronCurtain(SIBuffClass* Buff);
	bool SI_API Effect_PassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型);
	bool SI_API Effect_NotPassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型);
	bool SI_API Effect_Match_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型);
	bool SI_API Effect_NotMatch_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* 目标单位类型);
	void SI_API ReceiveWarheadDamage_A(SIBuffClass* Buff, TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型); // 会设置 SIDamageLeft 属性
	void SI_API ReceiveWarheadDamage_B(SIBuffClass* Buff, TechnoClass* 目标单位, TechnoClass* 攻击单位, double 伤害, WarheadTypeClass* 伤害弹头类型, HouseClass* 攻击单位所属作战方); // 会设置 SIDamageLeft 属性
	void SI_API ReceiveHealthDamage_A(SIBuffClass* Buff, TechnoClass* 目标单位, double 生命值损耗, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程); // 会设置 SIDamageLeft 属性
	void SI_API ReceiveHealthDamage_B(SIBuffClass* Buff, TechnoClass* 目标单位, double 生命值损耗, double 生命值损耗上限, double 生命值损耗下限, WarheadTypeClass* 结算护甲弹头类型, WarheadTypeClass* 死亡伤害弹头类型, bool 执行自身处理过程); // 会设置 SIDamageLeft 属性
	bool SI_API FreshOrPostBroadcast(SIBuffClass* Buff, double 需要更新的广播强度, AbstractClass* 指向目标);
	void SI_API ResetBroadcastPower(SIBuffClass* Buff, double 需要更新的广播强度);
	int SI_API GetBroadcastListenerCount(SIBuffClass* Buff, bool 无视限制条件, AbstractClass* 指向目标);
	bool SI_API AddAsBroadcastListener(SIBuffClass* Buff);
	void SI_API RemoveAsBroadcastListener(SIBuffClass* Buff);
	double SI_API GetBroadcastPower_Total(SIBuffClass* Buff);
	TechnoClass* SI_API GetFireOwner(SIBuffClass* Buff); // 返回值可能为空
	bool SI_API GetFireOwnerRawOffset(SIBuffClass* Buff, CoordStruct& 本体坐标);
	bool SI_API GetFireTargetRawOffset(SIBuffClass* Buff, AbstractClass* 目标, CoordStruct& 目标坐标);
	double SI_API GetFacingRotate(SIBuffClass* Buff, SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标);
	double SI_API GetFacingRotate_Same(SIBuffClass* Buff, SIRotateType 跟随旋转类型, CoordStruct 本体坐标, CoordStruct 目标坐标, SIRotateType 用于同步的跟随旋转类型, double 用于同步的旋转角度_弧度);
	void SI_API FreshFacingAndAimingRadius_Common(SIBuffClass* Buff, CoordStruct 本体坐标, CoordStruct 本体开火坐标, CoordStruct 目标坐标, CoordStruct& 开火坐标, CoordStruct& 命中坐标);
	void SI_API OffsetMotion_Init(SIBuffClass* Buff);
	void SI_API OffsetMotion_AI(SIBuffClass* Buff);
	void SI_API DigitalButtonData_Init(SIBuffClass* Buff, int 单位数量);
	

}

namespace SIBroadcast
{
	void SI_API FreshBroadcastPower(SIBroadcastClass* 接口, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void SI_API PostBroadcast(SIBroadcastClass* 接口, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	int SI_API GetBroadcastListenerCount(SIBroadcastClass* 接口, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void SI_API AddListener(SIBroadcastClass* 接口, SIBuffClass* 要添加的监听Buff);
	void SI_API RemoveListener(SIBroadcastClass* 接口, SIBuffClass* 要移除的监听Buff);
	bool SI_API IsEmpty(SIBroadcastClass* 接口);
	SIConstVector<SIBroadcastClass*>& SI_API GetBroadcastArray();
	HouseClass* SI_API OwnerObject(SIBroadcastClass* 接口);
	double SI_API GetBroadcastPower(SIBroadcastClass* 接口);
	const SIDataList<SIBuffClass*>& SI_API GetListenerBuffList(SIBroadcastClass* 接口);
}

namespace SIHouseExt
{
	void SI_API Counter_AddOriginTechnoType(SIHouse_ExtendData* 接口, TechnoTypeClass* 单位类型);
	void SI_API Counter_RemoveOriginTechnoType(SIHouse_ExtendData* 接口, TechnoTypeClass* 单位类型);
	bool SI_API AlwaysShowCameo(const SIHouse_ExtendData* const 接口, const TechnoTypeClass* const 单位类型, bool 包括正在建造的单位);// const;
	void SI_API Factory_CalculateProduction(SIHouse_ExtendData* 接口);
	void SI_API Factory_MergeProduction(SIHouse_ExtendData* 接口, TechnoClass* 单位);
	void SI_API Factory_RemoveProduction(SIHouse_ExtendData* 接口, TechnoClass* 单位);
	void SI_API EXP_Init(SIHouse_ExtendData* 接口);
	void SI_API EXP_Get(SIHouse_ExtendData* 接口, TechnoClass* 单位, SIValues_HouseInitEXP* 国家经验值参数包);
	bool SI_API EXP_MergeAcademy(SIHouse_ExtendData* 接口, TechnoClass* 单位, TechnoTypeClass* 单位类型);//可能会失败，失败返回false
	bool SI_API EXP_RemoveAcademy(SIHouse_ExtendData* 接口, TechnoClass* 单位, TechnoTypeClass* 单位类型);//可能会失败，失败返回false
	bool SI_API EXP_UpdateSpyEffect(SIHouse_ExtendData* 接口, BuildingClass* 建筑单位, BuildingTypeClass* 建筑单位类型);//可能会失败，失败返回false
	void SI_API SetHouseVar(SIHouse_ExtendData* 接口, int 索引值, double 数值);
	double SI_API GetHouseVar(SIHouse_ExtendData* 接口, int 索引值);
	void SI_API DeleteHouseVar(SIHouse_ExtendData* 接口, int 索引值);
	void SI_API FreshBroadcastPower(SIHouse_ExtendData* 接口, int 频道, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void SI_API PostBroadcast(SIHouse_ExtendData* 接口, int 频道, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	int SI_API GetBroadcastListenerCount(SIHouse_ExtendData* 接口, int 频道, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	double SI_API GetBroadcastPower(SIHouse_ExtendData* 接口, int 频道);
	void SI_API AddBroadcastListener(SIHouse_ExtendData* 接口, int 频道, SIBuffClass* 监听Buff);
	void SI_API RemoveBroadcastListener(SIHouse_ExtendData* 接口, int 频道, SIBuffClass* 监听Buff);
	void SI_API EXPPool_Add(SIHouse_ExtendData* 接口, double 额外经验值);
	double SI_API EXPPool_Pop(SIHouse_ExtendData* 接口, double 需求的额外经验值);
	void SI_API ForEach_HouseVar(SIHouse_ExtendData* 接口, void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 索引, double& 值));
	void SI_API ForEach_Broadcast(SIHouse_ExtendData* 接口, void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 频道, SIBroadcastClass* 值));
	SIBroadcastClass* SI_API Broadcast_FindOrAllocate(SIHouse_ExtendData* 接口, int 频道);// 返回值可能为空
	SIBroadcastClass* SI_API Broadcast_Find(SIHouse_ExtendData* 接口, int 频道);// 返回值可能为空
	HouseClass* SI_API OwnerObject(SIHouse_ExtendData* 接口);
}

namespace SIClassManager
{
	//布什·戈门 我劝你用RegisterBuff
	int SI_API RegisterEffectType(const char* 效果类型名称, long 虚表指针);



	SIPack_BuffSetting_FromStatic SI_API MakePack(SIPackTypeClass_BuffSetting* 类型, HouseClass* 当前作战方);
	SIPack_BuffSetting_FromStatic SI_API MakePack_Empty();
	SIPack_BuffSetting_FromStatic SI_API MakePack_DurationOnly(int 持续时间, bool 覆盖持续时间, bool 承受持续时间抗性);
	SIPack_BuffSetting_FromStatic SI_API MakePack_PowerOnly(double 强度值, bool 覆盖强度值, bool 承受强度值抗性);
}