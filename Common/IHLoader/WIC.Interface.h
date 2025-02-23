#pragma once
#include "WIC.Define.h"
#include <exception>

namespace SITool
{
	//检查WIC库是否可用。
	bool Available();
	
	//子弹：攻击的抛射体，因为完全变形存在体型差距，可能导致原来的攻击无法命中，传进来之后会修改抛射体的弹道，让它可以命中
	//      A的变形武器打了B，B变形了，填A的武器的抛射体；不是弹头命中导致变形的话，这个参数填nullptr
	//单位不完全变形
	TechnoClass* SI_API NormalConvert_TechnoTo(TechnoClass* 来源单位, TechnoTypeClass* 目标单位类型, HouseClass* 目标作战方, BulletClass* 子弹, bool 要求在地图上进行, bool 允许进行免疫);
	//单位完全变形
	TechnoClass* SI_API SuperConvert_TechnoTo(TechnoClass* 来源单位, TechnoTypeClass* 目标单位类型, HouseClass* 目标作战方, BulletClass* 子弹, bool 要求在地图上进行, bool 允许进行免疫);

	//漂浮字符串的视效
	void SI_API AddFlyingString_A(const CoordStruct 坐标, Point2D 像素偏移, int 持续时间, double 浮动速度, ColorStruct 颜色, const wchar_t* 显示内容);
	void SI_API AddFlyingString_B(const CoordStruct 坐标, Point2D 像素偏移, int 持续时间, double 浮动速度, ConvertClass* 色盘, SHPStruct* 图像, const int* 字符数据列表, int 字符数据总数);
	void SI_API AddFlyingString_Money(CoordStruct 坐标, int 金额, HouseClass* 所属作战方, int 作战方归属);

	void SI_API ForEach(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Cell(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Object(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Techno(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Animation(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Bullet(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Terrain(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_Overlay(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历
	void SI_API ForEach_House(void* 参数集, bool (CALLBACK*处理函数)(void* 参数集, SIInterface_ExtendData* 接口)); // [处理函数] 返回 true 会立刻终止遍历

}

namespace SIClassManager
{
	//通过AbstractClass*找到对应的SIInterface_ExtendData*以进行更多的操作。
	//若对应对象无SIInterface_ExtendData配套或对象不存在/不合法，则返回nullptr。
	SIInterface_ExtendData* SI_API GetExtendData(const AbstractClass* const 实体);// 返回值可能为空
	//通过HouseClass*找到对应的SIHouse_ExtendData*以进行更多的操作。
	//若对应对象无SIHouse_ExtendData配套或对象不存在/不合法，则返回nullptr。
	SIHouse_ExtendData* SI_API GetHouseExtendData(const HouseClass* const 实体);// 返回值可能为空
	//获取WIC内部对象的接口
	//以下函数的返回值都可能为空，请自行检查
	//Find为空是未找到
	//FindOrAllocate为空是出现错误，如果希望发生错误时崩溃则无需检查是否为空
	SIBuffTypeClass* SI_API BuffType_Find(const char* 名称);
	SIBuffTypeClass* SI_API BuffType_FindOrAllocate(const char* 名称);
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_Find(const char* 名称);
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_FindOrAllocate(const char* 名称);
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_Find(const char* 名称);
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_FindOrAllocate(const char* 名称);
	SIElementTypeClass* SI_API ElementType_Find(const char* 名称);
	SIElementTypeClass* SI_API ElementType_FindOrAllocate(const char* 名称);
	SIElementResistanceTypeClass* SI_API ElementResistanceType_Find(const char* 名称);
	SIElementResistanceTypeClass* SI_API ElementResistanceType_FindOrAllocate(const char* 名称);

	// 需要在载入 INI 之前就完成注册 , 返回值为效果类型 ID , 返回值为 -1 表示注册失败
	//注册方法：
	//调用方的插件应在依赖中包括WIC
	//例如：InitDependency Dependency{"SIWinterIsComing",DoNotCheckVersion,WIC_LIBRARY_VERSION,true};
	//加入依赖之后，请在OrderedInit函数当中完成注册
	//注册后的类型填写在Effect.Type="你的新Buff名字"
	//[效果种类名称] 长度不能超过 64 , 含 \0
	template<typename T>
	int RegisterBuff(const char* 效果类型名称)
	{
		static_assert(std::is_base_of<SIBuffClass, T>::value, "只能注册SIBuffClass的派生类");
		T Buff;
		return RegisterEffectType(效果类型名称, *reinterpret_cast<DWORD*>(&Buff));
	}
}

class SIHouse_ExtendData
{
public:
	//获取对应的HouseClass指针
	HouseClass* OwnerObject();

	//单位来源计数器
	void Counter_AddOriginTechnoType(TechnoTypeClass* 单位类型);
	void Counter_RemoveOriginTechnoType(TechnoTypeClass* 单位类型);

	//图标显示
	bool AlwaysShowCameo(const TechnoTypeClass* const 单位类型, bool 包括正在建造的单位) const;
	
	//产能工厂的控制
	void Factory_CalculateProduction();
	void Factory_MergeProduction(TechnoClass* 单位);
	void Factory_RemoveProduction(TechnoClass* 单位);

	//作战方的经验总控制
	void EXP_Init();
	void EXP_Get(TechnoClass* 单位, SIValues_HouseInitEXP* 国家经验值参数包);
	bool EXP_MergeAcademy(TechnoClass* 单位, TechnoTypeClass* 单位类型);//可能会失败，失败返回false
	bool EXP_RemoveAcademy(TechnoClass* 单位, TechnoTypeClass* 单位类型);//可能会失败，失败返回false
	bool EXP_UpdateSpyEffect(BuildingClass* 建筑单位, BuildingTypeClass* 建筑单位类型);//可能会失败，失败返回false
	void EXPPool_Add(double 额外经验值);
	double EXPPool_Pop(double 需求的额外经验值);

	//作战方变量的操作
	void SetHouseVar(int 索引值, double 数值);
	double GetHouseVar(int 索引值);
	void DeleteHouseVar(int 索引值);
	void ForEach_HouseVar(void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 索引, double& 值));

	//指定频道的广播控制
	void FreshBroadcastPower(int 频道, double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void PostBroadcast(int 频道, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	int GetBroadcastListenerCount(int 频道, bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	double GetBroadcastPower(int 频道);
	void AddBroadcastListener(int 频道, SIBuffClass* 监听Buff);
	void RemoveBroadcastListener(int 频道, SIBuffClass* 监听Buff);
	void ForEach_Broadcast(void* 参数集, void (CALLBACK* 处理函数)(void* 参数集, int 频道, SIBroadcastClass* 值));
	SIBroadcastClass* Broadcast_FindOrAllocate(int 频道);// 返回值可能为空，为空是未找到
	SIBroadcastClass* Broadcast_Find(int 频道);// 返回值可能为空，为空是出现错误，如果希望发生错误时崩溃则无需检查是否为空
};

class SIBroadcastClass
{
public:
	static const SIConstVector<SIBroadcastClass*>& GetArray();
	// 功能函数
	void FreshBroadcastPower(double 广播强度, bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void PostBroadcast(bool 触发被动监听, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	int GetBroadcastListenerCount(bool 无视限制条件, CoordStruct 发布坐标, TechnoClass* 发布单位, AbstractClass* 指向目标);
	void AddListener(SIBuffClass* 要添加的监听Buff);
	void RemoveListener(SIBuffClass* 要移除的监听Buff);
	bool IsEmpty();
	HouseClass* OwnerObject();
	double GetBroadcastPower();
	const SIDataList<SIBuffClass*>& GetListenerBuffList();
};

// 与单位一一对应的操作接口
class SIInterface_ExtendData
{
public:
	//获取对应单位在INI注册的类型名。
	char* GetName(); // 类型 ID 名称

	//通过SIInterface_ExtendData*找到对应的AbstractClass*。
	//若对象不存在/不合法，则返回nullptr。
	AbstractClass* OwnerEntity(); // 返回值可能为空

	//返回附着的对象的类别。
	AbstractType WhatAmI();

	//获取Interface在数组中的索引。
	int  GetArrayIndex();

	//检测是否被选中。
	bool  IsSelected();

	//获取所在坐标。
	void GetCoords(CoordStruct* 所在坐标);
	
	//部分开火相关函数
	void SetForceFireWeapon(WeaponTypeClass* 武器类型);
	void SetForceDeathWeapon(WeaponTypeClass* 武器类型);
	CoordStruct Calculate_FireCoord(CoordStruct 武器坐标, double 缩放倍率);

	//经验 & 等级的相关函数。
	int  GetStaticEXPLevel();
	double  GetLevelTotalEXPCost(double 固定经验值基数, bool 使用新经验系统);
	void EXP_Add(const double 提供的经验值, SIEXPSourceType 经验值来源, SIValues_EXPMerge* 合并经验值参数包); // 扣除经验使用负数

	//自定义的建筑染色函数。
	int GetTintColor(bool 铁幕染色, bool 空袭染色, bool 狂暴染色);
	void Calculate_CustomTintValues(int& 染色颜色, int& 染色强度);
	int Calculate_CustomTintValues_Color();
	int Calculate_CustomTintValues_Intensity();
	int Property_Register_CustomTintValues(ColorStruct 染色颜色, int 染色强度, int 作战方归属);
	int Property_Register_CustomTintValues(int 染色颜色, int 染色强度, int 作战方归属);
	void Property_Unregister_CustomTintValues(int 注册索引);

	/*
	以下是动态属性的操作接口。
	Calculate给出原始值计算最终值。
	Merge会把动态属性的相关值永久合并，合并后不可再修改，返回“新的数值”中没有成功合并的部分，如为0的除数等
	Register会加入一个新的动态属性加成，返回一个注册索引。
	Unregister会按照注册索引消除相应的动态属性加成。

	以下的动态属性按照如下方法计算。
	注：EXP和EXPCost、Armor和Versus分别是效果乘法和效果除法
	    如Armor注册一个1.25和Versus注册一个0.8具有一样的效果
	Speed、Armor、Versus、Attack、ROF、
	EXP、EXPProvide、EXPCost、
	RangeWeapon、CellSpread
	计算公式：
	Multiplier：倍率
	ExtraMultiplier：累乘
	Addon：追加
	FinalAddon：最终值
	实际值 = ( 原始值 * (1.0 + 倍率1 + 倍率2 + …) + (追加1 + 追加2 + …) ) * (1.0 * 累乘1 * 累乘2 * …) + (最终值1 + 最终值2 + …)

	Property_Merge_Ability_ImmuneXX的函数当中，填大于0的值为可以免疫。
	*/
	double Calculate_PropertyGroup_Speed(double 原始移动速度);
	double  Calculate_PropertyGroup_Defence(double 原始伤害);
	double  Calculate_PropertyGroup_Attack(double 原始伤害);
	double Calculate_PropertyGroup_ROF(double 原始射速);
	double Calculate_PropertyGroup_EXP(double 原始获得的经验值);
	double Calculate_PropertyGroup_EXPProvide(double 原始提供的经验值);
	double Calculate_PropertyGroup_EXPCost(double 单位升级所需的经验值);
	double Calculate_PropertyGroup_RangeWeapon(double 原始武器射程);
	double Calculate_PropertyGroup_CellSpread(double 原始弹头溅射范围);
	double Property_Merge_Multiplier_Speed(double 新的增加量);
	double Property_Merge_Multiplier_Armor(double 新的增加量);
	double Property_Merge_Multiplier_Versus(double 新的增加量);
	double Property_Merge_Multiplier_Attack(double 新的增加量);
	double Property_Merge_Multiplier_ROF(double 新的增加量);
	double Property_Merge_Multiplier_EXP(double 新的增加量);
	double Property_Merge_Multiplier_EXPProvide(double 新的增加量);
	double Property_Merge_Multiplier_EXPCost(double 新的增加量);
	double Property_Merge_Multiplier_RangeWeapon(double 新的增加量);
	double Property_Merge_Multiplier_CellSpread(double 新的增加量);
	double Property_Merge_ExtraMultiplier_Speed(double 新的增加量);
	double Property_Merge_ExtraMultiplier_Armor(double 新的增加量);
	double Property_Merge_ExtraMultiplier_Versus(double 新的增加量);
	double Property_Merge_ExtraMultiplier_Attack(double 新的增加量);
	double Property_Merge_ExtraMultiplier_ROF(double 新的增加量);
	double Property_Merge_ExtraMultiplier_EXP(double 新的增加量);
	double Property_Merge_ExtraMultiplier_EXPProvide(double 新的增加量);
	double Property_Merge_ExtraMultiplier_EXPCost(double 新的增加量);
	double Property_Merge_ExtraMultiplier_RangeWeapon(double 新的增加量);
	double Property_Merge_ExtraMultiplier_CellSpread(double 新的增加量);
	double Property_Merge_Addon_Speed(double 新的增加量);
	double Property_Merge_Addon_Damage(double 新的增加量);
	double Property_Merge_Addon_Attack(double 新的增加量);
	double Property_Merge_Addon_ROF(double 新的增加量);
	double Property_Merge_Addon_EXP(double 新的增加量);
	double Property_Merge_Addon_EXPProvide(double 新的增加量);
	double Property_Merge_Addon_EXPCost(double 新的增加量);
	double Property_Merge_Addon_RangeWeapon(double 新的增加量); // 单位是格点
	double Property_Merge_Addon_CellSpread(double 新的增加量);
	double Property_Merge_FinalAddon_Speed(double 新的增加量);
	double Property_Merge_FinalAddon_Damage(double 新的增加量);
	double Property_Merge_FinalAddon_Attack(double 新的增加量);
	double Property_Merge_FinalAddon_ROF(double 新的增加量);
	double Property_Merge_FinalAddon_EXP(double 新的增加量);
	double Property_Merge_FinalAddon_EXPProvide(double 新的增加量);
	double Property_Merge_FinalAddon_EXPCost(double 新的增加量);
	double Property_Merge_FinalAddon_RangeWeapon(double 新的增加量); // 单位是格点
	double Property_Merge_FinalAddon_CellSpread(double 新的增加量);
	int Property_Register_Multiplier_Speed(double 增加量);
	int Property_Register_Multiplier_Armor(double 增加量);
	int Property_Register_Multiplier_Versus(double 增加量);
	int Property_Register_Multiplier_Attack(double 增加量);
	int Property_Register_Multiplier_ROF(double 增加量);
	int Property_Register_Multiplier_EXP(double 增加量);
	int Property_Register_Multiplier_EXPProvide(double 增加量);
	int Property_Register_Multiplier_EXPCost(double 增加量);
	int Property_Register_Multiplier_RangeWeapon(double 增加量);
	int Property_Register_Multiplier_CellSpread(double 增加量);
	int Property_Register_ExtraMultiplier_Speed(double 增加量);
	int Property_Register_ExtraMultiplier_Armor(double 增加量);
	int Property_Register_ExtraMultiplier_Versus(double 增加量);
	int Property_Register_ExtraMultiplier_Attack(double 增加量);
	int Property_Register_ExtraMultiplier_ROF(double 增加量);
	int Property_Register_ExtraMultiplier_EXP(double 增加量);
	int Property_Register_ExtraMultiplier_EXPProvide(double 增加量);
	int Property_Register_ExtraMultiplier_EXPCost(double 增加量);
	int Property_Register_ExtraMultiplier_RangeWeapon(double 增加量);
	int Property_Register_ExtraMultiplier_CellSpread(double 增加量);
	int Property_Register_Addon_Speed(double 增加量);
	int Property_Register_Addon_Damage(double 增加量);
	int Property_Register_Addon_Attack(double 增加量);
	int Property_Register_Addon_ROF(double 增加量);
	int Property_Register_Addon_EXP(double 增加量);
	int Property_Register_Addon_EXPProvide(double 增加量);
	int Property_Register_Addon_EXPCost(double 增加量);
	int Property_Register_Addon_RangeWeapon(double 增加量); // 单位是格点
	int Property_Register_Addon_CellSpread(double 增加量); // 单位是格子
	int Property_Register_FinalAddon_Speed(double 增加量);
	int Property_Register_FinalAddon_Damage(double 增加量);
	int Property_Register_FinalAddon_Attack(double 增加量);
	int Property_Register_FinalAddon_ROF(double 增加量);
	int Property_Register_FinalAddon_EXP(double 增加量);
	int Property_Register_FinalAddon_EXPProvide(double 增加量);
	int Property_Register_FinalAddon_EXPCost(double 增加量);
	int Property_Register_FinalAddon_RangeWeapon(double 增加量); // 单位是格点
	int Property_Register_FinalAddon_CellSpread(double 增加量);
	void Property_Unregister_Multiplier_Speed(int 注册索引);
	void Property_Unregister_Multiplier_Armor(int 注册索引);
	void Property_Unregister_Multiplier_Versus(int 注册索引);
	void Property_Unregister_Multiplier_Attack(int 注册索引);
	void Property_Unregister_Multiplier_ROF(int 注册索引);
	void Property_Unregister_Multiplier_EXP(int 注册索引);
	void Property_Unregister_Multiplier_EXPProvide(int 注册索引);
	void Property_Unregister_Multiplier_EXPCost(int 注册索引);
	void Property_Unregister_Multiplier_RangeWeapon(int 注册索引);
	void Property_Unregister_Multiplier_CellSpread(int 注册索引);
	void Property_Unregister_ExtraMultiplier_Speed(int 注册索引);
	void Property_Unregister_ExtraMultiplier_Armor(int 注册索引);
	void Property_Unregister_ExtraMultiplier_Versus(int 注册索引);
	void Property_Unregister_ExtraMultiplier_Attack(int 注册索引);
	void Property_Unregister_ExtraMultiplier_ROF(int 注册索引);
	void Property_Unregister_ExtraMultiplier_EXP(int 注册索引);
	void Property_Unregister_ExtraMultiplier_EXPProvide(int 注册索引);
	void Property_Unregister_ExtraMultiplier_EXPCost(int 注册索引);
	void Property_Unregister_ExtraMultiplier_RangeWeapon(int 注册索引);
	void Property_Unregister_ExtraMultiplier_CellSpread(int 注册索引);
	void Property_Unregister_Addon_Speed(int 注册索引);
	void Property_Unregister_Addon_Damage(int 注册索引);
	void Property_Unregister_Addon_Attack(int 注册索引);
	void Property_Unregister_Addon_ROF(int 注册索引);
	void Property_Unregister_Addon_EXP(int 注册索引);
	void Property_Unregister_Addon_EXPProvide(int 注册索引);
	void Property_Unregister_Addon_EXPCost(int 注册索引);
	void Property_Unregister_Addon_RangeWeapon(int 注册索引);
	void Property_Unregister_Addon_CellSpread(int 注册索引);
	void Property_Unregister_FinalAddon_Speed(int 注册索引);
	void Property_Unregister_FinalAddon_Damage(int 注册索引);
	void Property_Unregister_FinalAddon_Attack(int 注册索引);
	void Property_Unregister_FinalAddon_ROF(int 注册索引);
	void Property_Unregister_FinalAddon_EXP(int 注册索引);
	void Property_Unregister_FinalAddon_EXPProvide(int 注册索引);
	void Property_Unregister_FinalAddon_EXPCost(int 注册索引);
	void Property_Unregister_FinalAddon_RangeWeapon(int 注册索引);
	void Property_Unregister_FinalAddon_CellSpread(int 注册索引);
	void Property_Merge_Ability_ImmuneCrush(int 新的能力强度);
	void Property_Merge_Ability_ImmuneCrushAdvance(int 新的能力强度);
	void Property_Merge_Ability_ImmuneRadiation(int 新的能力强度);
	void Property_Merge_Ability_ImmunePsionicDamage(int 新的能力强度);
	void Property_Merge_Ability_ImmunePsionic(int 新的能力强度);
	void Property_Merge_Ability_ImmuneBerzerk(int 新的能力强度);
	void Property_Merge_Ability_ImmunePoison(int 新的能力强度);
	void Property_Merge_Ability_ImmuneWarp(int 新的能力强度);
	void Property_Merge_Ability_ImmuneEMP(int 新的能力强度);
	void Property_Merge_Ability_SelfHealing(int 新的能力强度);
	void Property_Merge_Ability_SelfHealing_Percent(double 新的能力强度);
	void Property_Merge_Ability_SelfHealing_Cap(double 新的能力强度);
	void Property_Merge_Ability_SelfHealing_Delay(int 新的能力强度);
	void Property_Merge_Ability_SelfHealing_CombatDelay(int 新的能力强度);

	
	/*
	以下是Buff的操作接口。
	包括获取，激活，改变，遍历，删除等函数。
	*/
	//SIPack_BuffSetting_FromStatic的获取详见SIPackTypeClass_BuffSetting的函数
	SIBuffClass* Buff_CreateOrMerge(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, AbstractClass* 来源, HouseClass* 来源所属作战方);

	bool Buff_IsPowerOffline();
	bool Buff_IsNeedHide();
	int Buff_GetEffectDuration(SIBuffTypeClass* Buff类型, int 原始的挂载持续时间);
	double Buff_GetEffectHealth(SIBuffTypeClass* Buff类型, double 原始的生命值);
	double Buff_GetEffectPower(SIBuffTypeClass* Buff类型, double 原始的强度值);
	void Buff_TryActive(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包);
	void Buff_TryActive(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void Buff_TryActive(void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void Buff_TryAfter(SIBuffTypeClass* Buff类型);
	void Buff_TryAfter(SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void Buff_TryAfter(void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void Buff_TryRemove(SIBuffTypeClass* Buff类型);
	void Buff_TryRemove(SIBuffTypeClass* Buff类型, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void Buff_TryRemove(void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void Buff_TryMergeSetting(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包);
	void Buff_TryMergeSetting(SIBuffTypeClass* Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	void Buff_TryMergeSetting(void* 来源, bool (*判断处理函数)(void* 来源, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff参数设置包)); // [判断处理函数] 返回 true 则对 Buff 进行处理
	void Buff_TryChange(SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方);
	void Buff_TryChange(SIBuffTypeClass* 旧Buff类型, SIBuffTypeClass* 新Buff类型, SIPack_BuffSetting_FromStatic* Buff参数设置包, int 基础挂载持续时间增加量, double 强度值增加量, AbstractClass* 来源, HouseClass* 来源所属作战方, SIPackTypeClass_CheckTechno* 检查单位属性参数包);
	bool Buff_HasBuffed();
	bool Buff_HasBuffed(SIBuffTypeClass* Buff类型);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff类型, SIBuffClass_State Buff状态);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff类型, AbstractClass* 排除的来源);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff类型, AbstractClass* 需求的来源, bool 没用的标识);
	bool Buff_HasBuffed(SIBuffTypeClass_EffectType 效果种类);
	bool Buff_HasBuffed(SIBuffTypeClass_EffectType 效果种类, SIBuffClass_State Buff状态);
	void Buff_LookUp(void* 来源, void (*遍历函数)(void* 来源, SIBuffClass* Buff)); // 此处不会立即移除无效的 Buff
	void Buff_Update();
	bool Buff_TriggerFire(AbstractClass* 目标, int 武器索引, WeaponStruct* 武器数据, CoordStruct 本体开火坐标, bool 死亡武器);
	double Buff_TriggerAttacker(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数, double 当前伤害);
	double Buff_TriggerDefender(args_ReceiveDamage* 伤害参数, double 当前伤害);
	double Buff_TriggerDefender_Final(args_ReceiveDamage* 伤害参数, double 当前伤害);
	void Buff_TriggerDeath(args_ReceiveDamage* 伤害参数);
	void Buff_TriggerKiller(TechnoClass* 目标单位, args_ReceiveDamage* 伤害参数);
	double Buff_TriggerSelf(double 当前伤害);
	void Buff_DoDamages(double 准备造成的伤害);
	//Update 0815
	SIBuffClass* Buff_GetBuff(int Buff索引); // 返回值可能为空
	SIBuffClass* Buff_GetBuff(SIBuffTypeClass* Buff类型); // 返回值可能为空
	void AvailableEffect();
	void UnavailableEffect();

	//元素抗性 & 元素免疫相关。
	void Element_Merge_ResistanceType(SIElementResistanceTypeClass* 元素抗性类型, int 层数计数);
	void Element_Merge_ImmuneElementType(SIElementTypeClass* 元素类型, int 层数计数);

	//更新状态相关。
	//包括提交附属的按钮的状态。
	void Update();
	void UpdateBar_UpdateAllow();
	void UpdateDrawBar();
	bool UpdateClickBar_Hover(HouseClass* 点击的作战方, Point2D* 点击像素偏移);
	void UpdateClickBar_TryClick(HouseClass* 点击的作战方, Point2D* 点击像素偏移, int& Buff索引, SIDigitalClickBarIndex& 点击索引, double& 当前值, bool& 禁用标识);
	void UpdateClickBar_Effect(int Buff索引, SIDigitalClickBarIndex 点击索引, HouseClass* 点击的作战方, double 当前值);
	bool UpdateKeyExecuteBar_TryExecute(HouseClass* 点击的作战方, SICommandCode 快捷键代码);
	void UpdateKeyExecuteBar_Effect(HouseClass* 点击的作战方, SICommandCode 快捷键代码);
	void Init();
	void UnInit();

	//WIC 变形逻辑
	// 这个只是更新函数，真正的变形代码不在这
	//具体使用条件及限制详见WIC说明书
	void UpdateTechnoType(TechnoTypeClass* 旧的单位类型);

	//对应于单位的SetOwningHouse，调用的条件是不通过techno->SetOwningHouse函数改变了单位的所属作战方
	void UpdateOwningHouse(const HouseClass* const 旧的单位所属作战方, const HouseClass* const 新的单位所属作战方);

};

