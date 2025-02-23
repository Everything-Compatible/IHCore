#pragma once
#include "WIC.Define.h"
#include <exception>

namespace SITool
{
	//���WIC���Ƿ���á�
	bool Available();
	
	//�ӵ��������������壬��Ϊ��ȫ���δ������Ͳ�࣬���ܵ���ԭ���Ĺ����޷����У�������֮����޸�������ĵ�����������������
	//      A�ı�����������B��B�����ˣ���A�������������壻���ǵ�ͷ���е��±��εĻ������������nullptr
	//��λ����ȫ����
	TechnoClass* SI_API NormalConvert_TechnoTo(TechnoClass* ��Դ��λ, TechnoTypeClass* Ŀ�굥λ����, HouseClass* Ŀ����ս��, BulletClass* �ӵ�, bool Ҫ���ڵ�ͼ�Ͻ���, bool �����������);
	//��λ��ȫ����
	TechnoClass* SI_API SuperConvert_TechnoTo(TechnoClass* ��Դ��λ, TechnoTypeClass* Ŀ�굥λ����, HouseClass* Ŀ����ս��, BulletClass* �ӵ�, bool Ҫ���ڵ�ͼ�Ͻ���, bool �����������);

	//Ư���ַ�������Ч
	void SI_API AddFlyingString_A(const CoordStruct ����, Point2D ����ƫ��, int ����ʱ��, double �����ٶ�, ColorStruct ��ɫ, const wchar_t* ��ʾ����);
	void SI_API AddFlyingString_B(const CoordStruct ����, Point2D ����ƫ��, int ����ʱ��, double �����ٶ�, ConvertClass* ɫ��, SHPStruct* ͼ��, const int* �ַ������б�, int �ַ���������);
	void SI_API AddFlyingString_Money(CoordStruct ����, int ���, HouseClass* ������ս��, int ��ս������);

	void SI_API ForEach(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Cell(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Object(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Techno(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Animation(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Bullet(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Terrain(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_Overlay(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����
	void SI_API ForEach_House(void* ������, bool (CALLBACK*������)(void* ������, SIInterface_ExtendData* �ӿ�)); // [������] ���� true ��������ֹ����

}

namespace SIClassManager
{
	//ͨ��AbstractClass*�ҵ���Ӧ��SIInterface_ExtendData*�Խ��и���Ĳ�����
	//����Ӧ������SIInterface_ExtendData���׻���󲻴���/���Ϸ����򷵻�nullptr��
	SIInterface_ExtendData* SI_API GetExtendData(const AbstractClass* const ʵ��);// ����ֵ����Ϊ��
	//ͨ��HouseClass*�ҵ���Ӧ��SIHouse_ExtendData*�Խ��и���Ĳ�����
	//����Ӧ������SIHouse_ExtendData���׻���󲻴���/���Ϸ����򷵻�nullptr��
	SIHouse_ExtendData* SI_API GetHouseExtendData(const HouseClass* const ʵ��);// ����ֵ����Ϊ��
	//��ȡWIC�ڲ�����Ľӿ�
	//���º����ķ���ֵ������Ϊ�գ������м��
	//FindΪ����δ�ҵ�
	//FindOrAllocateΪ���ǳ��ִ������ϣ����������ʱ�������������Ƿ�Ϊ��
	SIBuffTypeClass* SI_API BuffType_Find(const char* ����);
	SIBuffTypeClass* SI_API BuffType_FindOrAllocate(const char* ����);
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_Find(const char* ����);
	SIPackTypeClass_BuffSetting* SI_API BuffSetting_FindOrAllocate(const char* ����);
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_Find(const char* ����);
	SIPackTypeClass_CheckTechno* SI_API CheckTechno_FindOrAllocate(const char* ����);
	SIElementTypeClass* SI_API ElementType_Find(const char* ����);
	SIElementTypeClass* SI_API ElementType_FindOrAllocate(const char* ����);
	SIElementResistanceTypeClass* SI_API ElementResistanceType_Find(const char* ����);
	SIElementResistanceTypeClass* SI_API ElementResistanceType_FindOrAllocate(const char* ����);

	// ��Ҫ������ INI ֮ǰ�����ע�� , ����ֵΪЧ������ ID , ����ֵΪ -1 ��ʾע��ʧ��
	//ע�᷽����
	//���÷��Ĳ��Ӧ�������а���WIC
	//���磺InitDependency Dependency{"SIWinterIsComing",DoNotCheckVersion,WIC_LIBRARY_VERSION,true};
	//��������֮������OrderedInit�����������ע��
	//ע����������д��Effect.Type="�����Buff����"
	//[Ч����������] ���Ȳ��ܳ��� 64 , �� \0
	template<typename T>
	int RegisterBuff(const char* Ч����������)
	{
		static_assert(std::is_base_of<SIBuffClass, T>::value, "ֻ��ע��SIBuffClass��������");
		T Buff;
		return RegisterEffectType(Ч����������, *reinterpret_cast<DWORD*>(&Buff));
	}
}

class SIHouse_ExtendData
{
public:
	//��ȡ��Ӧ��HouseClassָ��
	HouseClass* OwnerObject();

	//��λ��Դ������
	void Counter_AddOriginTechnoType(TechnoTypeClass* ��λ����);
	void Counter_RemoveOriginTechnoType(TechnoTypeClass* ��λ����);

	//ͼ����ʾ
	bool AlwaysShowCameo(const TechnoTypeClass* const ��λ����, bool �������ڽ���ĵ�λ) const;
	
	//���ܹ����Ŀ���
	void Factory_CalculateProduction();
	void Factory_MergeProduction(TechnoClass* ��λ);
	void Factory_RemoveProduction(TechnoClass* ��λ);

	//��ս���ľ����ܿ���
	void EXP_Init();
	void EXP_Get(TechnoClass* ��λ, SIValues_HouseInitEXP* ���Ҿ���ֵ������);
	bool EXP_MergeAcademy(TechnoClass* ��λ, TechnoTypeClass* ��λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	bool EXP_RemoveAcademy(TechnoClass* ��λ, TechnoTypeClass* ��λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	bool EXP_UpdateSpyEffect(BuildingClass* ������λ, BuildingTypeClass* ������λ����);//���ܻ�ʧ�ܣ�ʧ�ܷ���false
	void EXPPool_Add(double ���⾭��ֵ);
	double EXPPool_Pop(double ����Ķ��⾭��ֵ);

	//��ս�������Ĳ���
	void SetHouseVar(int ����ֵ, double ��ֵ);
	double GetHouseVar(int ����ֵ);
	void DeleteHouseVar(int ����ֵ);
	void ForEach_HouseVar(void* ������, void (CALLBACK* ������)(void* ������, int ����, double& ֵ));

	//ָ��Ƶ���Ĺ㲥����
	void FreshBroadcastPower(int Ƶ��, double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void PostBroadcast(int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	int GetBroadcastListenerCount(int Ƶ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	double GetBroadcastPower(int Ƶ��);
	void AddBroadcastListener(int Ƶ��, SIBuffClass* ����Buff);
	void RemoveBroadcastListener(int Ƶ��, SIBuffClass* ����Buff);
	void ForEach_Broadcast(void* ������, void (CALLBACK* ������)(void* ������, int Ƶ��, SIBroadcastClass* ֵ));
	SIBroadcastClass* Broadcast_FindOrAllocate(int Ƶ��);// ����ֵ����Ϊ�գ�Ϊ����δ�ҵ�
	SIBroadcastClass* Broadcast_Find(int Ƶ��);// ����ֵ����Ϊ�գ�Ϊ���ǳ��ִ������ϣ����������ʱ�������������Ƿ�Ϊ��
};

class SIBroadcastClass
{
public:
	static const SIConstVector<SIBroadcastClass*>& GetArray();
	// ���ܺ���
	void FreshBroadcastPower(double �㲥ǿ��, bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void PostBroadcast(bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	int GetBroadcastListenerCount(bool ������������, CoordStruct ��������, TechnoClass* ������λ, AbstractClass* ָ��Ŀ��);
	void AddListener(SIBuffClass* Ҫ��ӵļ���Buff);
	void RemoveListener(SIBuffClass* Ҫ�Ƴ��ļ���Buff);
	bool IsEmpty();
	HouseClass* OwnerObject();
	double GetBroadcastPower();
	const SIDataList<SIBuffClass*>& GetListenerBuffList();
};

// �뵥λһһ��Ӧ�Ĳ����ӿ�
class SIInterface_ExtendData
{
public:
	//��ȡ��Ӧ��λ��INIע�����������
	char* GetName(); // ���� ID ����

	//ͨ��SIInterface_ExtendData*�ҵ���Ӧ��AbstractClass*��
	//�����󲻴���/���Ϸ����򷵻�nullptr��
	AbstractClass* OwnerEntity(); // ����ֵ����Ϊ��

	//���ظ��ŵĶ�������
	AbstractType WhatAmI();

	//��ȡInterface�������е�������
	int  GetArrayIndex();

	//����Ƿ�ѡ�С�
	bool  IsSelected();

	//��ȡ�������ꡣ
	void GetCoords(CoordStruct* ��������);
	
	//���ֿ�����غ���
	void SetForceFireWeapon(WeaponTypeClass* ��������);
	void SetForceDeathWeapon(WeaponTypeClass* ��������);
	CoordStruct Calculate_FireCoord(CoordStruct ��������, double ���ű���);

	//���� & �ȼ�����غ�����
	int  GetStaticEXPLevel();
	double  GetLevelTotalEXPCost(double �̶�����ֵ����, bool ʹ���¾���ϵͳ);
	void EXP_Add(const double �ṩ�ľ���ֵ, SIEXPSourceType ����ֵ��Դ, SIValues_EXPMerge* �ϲ�����ֵ������); // �۳�����ʹ�ø���

	//�Զ���Ľ���Ⱦɫ������
	int GetTintColor(bool ��ĻȾɫ, bool ��ϮȾɫ, bool ��Ⱦɫ);
	void Calculate_CustomTintValues(int& Ⱦɫ��ɫ, int& Ⱦɫǿ��);
	int Calculate_CustomTintValues_Color();
	int Calculate_CustomTintValues_Intensity();
	int Property_Register_CustomTintValues(ColorStruct Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������);
	int Property_Register_CustomTintValues(int Ⱦɫ��ɫ, int Ⱦɫǿ��, int ��ս������);
	void Property_Unregister_CustomTintValues(int ע������);

	/*
	�����Ƕ�̬���ԵĲ����ӿڡ�
	Calculate����ԭʼֵ��������ֵ��
	Merge��Ѷ�̬���Ե����ֵ���úϲ����ϲ��󲻿����޸ģ����ء��µ���ֵ����û�гɹ��ϲ��Ĳ��֣���Ϊ0�ĳ�����
	Register�����һ���µĶ�̬���Լӳɣ�����һ��ע��������
	Unregister�ᰴ��ע������������Ӧ�Ķ�̬���Լӳɡ�

	���µĶ�̬���԰������·������㡣
	ע��EXP��EXPCost��Armor��Versus�ֱ���Ч���˷���Ч������
	    ��Armorע��һ��1.25��Versusע��һ��0.8����һ����Ч��
	Speed��Armor��Versus��Attack��ROF��
	EXP��EXPProvide��EXPCost��
	RangeWeapon��CellSpread
	���㹫ʽ��
	Multiplier������
	ExtraMultiplier���۳�
	Addon��׷��
	FinalAddon������ֵ
	ʵ��ֵ = ( ԭʼֵ * (1.0 + ����1 + ����2 + ��) + (׷��1 + ׷��2 + ��) ) * (1.0 * �۳�1 * �۳�2 * ��) + (����ֵ1 + ����ֵ2 + ��)

	Property_Merge_Ability_ImmuneXX�ĺ������У������0��ֵΪ�������ߡ�
	*/
	double Calculate_PropertyGroup_Speed(double ԭʼ�ƶ��ٶ�);
	double  Calculate_PropertyGroup_Defence(double ԭʼ�˺�);
	double  Calculate_PropertyGroup_Attack(double ԭʼ�˺�);
	double Calculate_PropertyGroup_ROF(double ԭʼ����);
	double Calculate_PropertyGroup_EXP(double ԭʼ��õľ���ֵ);
	double Calculate_PropertyGroup_EXPProvide(double ԭʼ�ṩ�ľ���ֵ);
	double Calculate_PropertyGroup_EXPCost(double ��λ��������ľ���ֵ);
	double Calculate_PropertyGroup_RangeWeapon(double ԭʼ�������);
	double Calculate_PropertyGroup_CellSpread(double ԭʼ��ͷ���䷶Χ);
	double Property_Merge_Multiplier_Speed(double �µ�������);
	double Property_Merge_Multiplier_Armor(double �µ�������);
	double Property_Merge_Multiplier_Versus(double �µ�������);
	double Property_Merge_Multiplier_Attack(double �µ�������);
	double Property_Merge_Multiplier_ROF(double �µ�������);
	double Property_Merge_Multiplier_EXP(double �µ�������);
	double Property_Merge_Multiplier_EXPProvide(double �µ�������);
	double Property_Merge_Multiplier_EXPCost(double �µ�������);
	double Property_Merge_Multiplier_RangeWeapon(double �µ�������);
	double Property_Merge_Multiplier_CellSpread(double �µ�������);
	double Property_Merge_ExtraMultiplier_Speed(double �µ�������);
	double Property_Merge_ExtraMultiplier_Armor(double �µ�������);
	double Property_Merge_ExtraMultiplier_Versus(double �µ�������);
	double Property_Merge_ExtraMultiplier_Attack(double �µ�������);
	double Property_Merge_ExtraMultiplier_ROF(double �µ�������);
	double Property_Merge_ExtraMultiplier_EXP(double �µ�������);
	double Property_Merge_ExtraMultiplier_EXPProvide(double �µ�������);
	double Property_Merge_ExtraMultiplier_EXPCost(double �µ�������);
	double Property_Merge_ExtraMultiplier_RangeWeapon(double �µ�������);
	double Property_Merge_ExtraMultiplier_CellSpread(double �µ�������);
	double Property_Merge_Addon_Speed(double �µ�������);
	double Property_Merge_Addon_Damage(double �µ�������);
	double Property_Merge_Addon_Attack(double �µ�������);
	double Property_Merge_Addon_ROF(double �µ�������);
	double Property_Merge_Addon_EXP(double �µ�������);
	double Property_Merge_Addon_EXPProvide(double �µ�������);
	double Property_Merge_Addon_EXPCost(double �µ�������);
	double Property_Merge_Addon_RangeWeapon(double �µ�������); // ��λ�Ǹ��
	double Property_Merge_Addon_CellSpread(double �µ�������);
	double Property_Merge_FinalAddon_Speed(double �µ�������);
	double Property_Merge_FinalAddon_Damage(double �µ�������);
	double Property_Merge_FinalAddon_Attack(double �µ�������);
	double Property_Merge_FinalAddon_ROF(double �µ�������);
	double Property_Merge_FinalAddon_EXP(double �µ�������);
	double Property_Merge_FinalAddon_EXPProvide(double �µ�������);
	double Property_Merge_FinalAddon_EXPCost(double �µ�������);
	double Property_Merge_FinalAddon_RangeWeapon(double �µ�������); // ��λ�Ǹ��
	double Property_Merge_FinalAddon_CellSpread(double �µ�������);
	int Property_Register_Multiplier_Speed(double ������);
	int Property_Register_Multiplier_Armor(double ������);
	int Property_Register_Multiplier_Versus(double ������);
	int Property_Register_Multiplier_Attack(double ������);
	int Property_Register_Multiplier_ROF(double ������);
	int Property_Register_Multiplier_EXP(double ������);
	int Property_Register_Multiplier_EXPProvide(double ������);
	int Property_Register_Multiplier_EXPCost(double ������);
	int Property_Register_Multiplier_RangeWeapon(double ������);
	int Property_Register_Multiplier_CellSpread(double ������);
	int Property_Register_ExtraMultiplier_Speed(double ������);
	int Property_Register_ExtraMultiplier_Armor(double ������);
	int Property_Register_ExtraMultiplier_Versus(double ������);
	int Property_Register_ExtraMultiplier_Attack(double ������);
	int Property_Register_ExtraMultiplier_ROF(double ������);
	int Property_Register_ExtraMultiplier_EXP(double ������);
	int Property_Register_ExtraMultiplier_EXPProvide(double ������);
	int Property_Register_ExtraMultiplier_EXPCost(double ������);
	int Property_Register_ExtraMultiplier_RangeWeapon(double ������);
	int Property_Register_ExtraMultiplier_CellSpread(double ������);
	int Property_Register_Addon_Speed(double ������);
	int Property_Register_Addon_Damage(double ������);
	int Property_Register_Addon_Attack(double ������);
	int Property_Register_Addon_ROF(double ������);
	int Property_Register_Addon_EXP(double ������);
	int Property_Register_Addon_EXPProvide(double ������);
	int Property_Register_Addon_EXPCost(double ������);
	int Property_Register_Addon_RangeWeapon(double ������); // ��λ�Ǹ��
	int Property_Register_Addon_CellSpread(double ������); // ��λ�Ǹ���
	int Property_Register_FinalAddon_Speed(double ������);
	int Property_Register_FinalAddon_Damage(double ������);
	int Property_Register_FinalAddon_Attack(double ������);
	int Property_Register_FinalAddon_ROF(double ������);
	int Property_Register_FinalAddon_EXP(double ������);
	int Property_Register_FinalAddon_EXPProvide(double ������);
	int Property_Register_FinalAddon_EXPCost(double ������);
	int Property_Register_FinalAddon_RangeWeapon(double ������); // ��λ�Ǹ��
	int Property_Register_FinalAddon_CellSpread(double ������);
	void Property_Unregister_Multiplier_Speed(int ע������);
	void Property_Unregister_Multiplier_Armor(int ע������);
	void Property_Unregister_Multiplier_Versus(int ע������);
	void Property_Unregister_Multiplier_Attack(int ע������);
	void Property_Unregister_Multiplier_ROF(int ע������);
	void Property_Unregister_Multiplier_EXP(int ע������);
	void Property_Unregister_Multiplier_EXPProvide(int ע������);
	void Property_Unregister_Multiplier_EXPCost(int ע������);
	void Property_Unregister_Multiplier_RangeWeapon(int ע������);
	void Property_Unregister_Multiplier_CellSpread(int ע������);
	void Property_Unregister_ExtraMultiplier_Speed(int ע������);
	void Property_Unregister_ExtraMultiplier_Armor(int ע������);
	void Property_Unregister_ExtraMultiplier_Versus(int ע������);
	void Property_Unregister_ExtraMultiplier_Attack(int ע������);
	void Property_Unregister_ExtraMultiplier_ROF(int ע������);
	void Property_Unregister_ExtraMultiplier_EXP(int ע������);
	void Property_Unregister_ExtraMultiplier_EXPProvide(int ע������);
	void Property_Unregister_ExtraMultiplier_EXPCost(int ע������);
	void Property_Unregister_ExtraMultiplier_RangeWeapon(int ע������);
	void Property_Unregister_ExtraMultiplier_CellSpread(int ע������);
	void Property_Unregister_Addon_Speed(int ע������);
	void Property_Unregister_Addon_Damage(int ע������);
	void Property_Unregister_Addon_Attack(int ע������);
	void Property_Unregister_Addon_ROF(int ע������);
	void Property_Unregister_Addon_EXP(int ע������);
	void Property_Unregister_Addon_EXPProvide(int ע������);
	void Property_Unregister_Addon_EXPCost(int ע������);
	void Property_Unregister_Addon_RangeWeapon(int ע������);
	void Property_Unregister_Addon_CellSpread(int ע������);
	void Property_Unregister_FinalAddon_Speed(int ע������);
	void Property_Unregister_FinalAddon_Damage(int ע������);
	void Property_Unregister_FinalAddon_Attack(int ע������);
	void Property_Unregister_FinalAddon_ROF(int ע������);
	void Property_Unregister_FinalAddon_EXP(int ע������);
	void Property_Unregister_FinalAddon_EXPProvide(int ע������);
	void Property_Unregister_FinalAddon_EXPCost(int ע������);
	void Property_Unregister_FinalAddon_RangeWeapon(int ע������);
	void Property_Unregister_FinalAddon_CellSpread(int ע������);
	void Property_Merge_Ability_ImmuneCrush(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmuneCrushAdvance(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmuneRadiation(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmunePsionicDamage(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmunePsionic(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmuneBerzerk(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmunePoison(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmuneWarp(int �µ�����ǿ��);
	void Property_Merge_Ability_ImmuneEMP(int �µ�����ǿ��);
	void Property_Merge_Ability_SelfHealing(int �µ�����ǿ��);
	void Property_Merge_Ability_SelfHealing_Percent(double �µ�����ǿ��);
	void Property_Merge_Ability_SelfHealing_Cap(double �µ�����ǿ��);
	void Property_Merge_Ability_SelfHealing_Delay(int �µ�����ǿ��);
	void Property_Merge_Ability_SelfHealing_CombatDelay(int �µ�����ǿ��);

	
	/*
	������Buff�Ĳ����ӿڡ�
	������ȡ������ı䣬������ɾ���Ⱥ�����
	*/
	//SIPack_BuffSetting_FromStatic�Ļ�ȡ���SIPackTypeClass_BuffSetting�ĺ���
	SIBuffClass* Buff_CreateOrMerge(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, AbstractClass* ��Դ, HouseClass* ��Դ������ս��);

	bool Buff_IsPowerOffline();
	bool Buff_IsNeedHide();
	int Buff_GetEffectDuration(SIBuffTypeClass* Buff����, int ԭʼ�Ĺ��س���ʱ��);
	double Buff_GetEffectHealth(SIBuffTypeClass* Buff����, double ԭʼ������ֵ);
	double Buff_GetEffectPower(SIBuffTypeClass* Buff����, double ԭʼ��ǿ��ֵ);
	void Buff_TryActive(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�);
	void Buff_TryActive(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void Buff_TryActive(void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)); // [�жϴ�����] ���� true ��� Buff ���д���
	void Buff_TryAfter(SIBuffTypeClass* Buff����);
	void Buff_TryAfter(SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void Buff_TryAfter(void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff)); // [�жϴ�����] ���� true ��� Buff ���д���
	void Buff_TryRemove(SIBuffTypeClass* Buff����);
	void Buff_TryRemove(SIBuffTypeClass* Buff����, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void Buff_TryRemove(void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff)); // [�жϴ�����] ���� true ��� Buff ���д���
	void Buff_TryMergeSetting(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�);
	void Buff_TryMergeSetting(SIBuffTypeClass* Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	void Buff_TryMergeSetting(void* ��Դ, bool (*�жϴ�����)(void* ��Դ, SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)); // [�жϴ�����] ���� true ��� Buff ���д���
	void Buff_TryChange(SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��);
	void Buff_TryChange(SIBuffTypeClass* ��Buff����, SIBuffTypeClass* ��Buff����, SIPack_BuffSetting_FromStatic* Buff�������ð�, int �������س���ʱ��������, double ǿ��ֵ������, AbstractClass* ��Դ, HouseClass* ��Դ������ս��, SIPackTypeClass_CheckTechno* ��鵥λ���Բ�����);
	bool Buff_HasBuffed();
	bool Buff_HasBuffed(SIBuffTypeClass* Buff����);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff����, SIBuffClass_State Buff״̬);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff����, AbstractClass* �ų�����Դ);
	bool Buff_HasBuffed(SIBuffTypeClass* Buff����, AbstractClass* �������Դ, bool û�õı�ʶ);
	bool Buff_HasBuffed(SIBuffTypeClass_EffectType Ч������);
	bool Buff_HasBuffed(SIBuffTypeClass_EffectType Ч������, SIBuffClass_State Buff״̬);
	void Buff_LookUp(void* ��Դ, void (*��������)(void* ��Դ, SIBuffClass* Buff)); // �˴����������Ƴ���Ч�� Buff
	void Buff_Update();
	bool Buff_TriggerFire(AbstractClass* Ŀ��, int ��������, WeaponStruct* ��������, CoordStruct ���忪������, bool ��������);
	double Buff_TriggerAttacker(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	double Buff_TriggerDefender(args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	double Buff_TriggerDefender_Final(args_ReceiveDamage* �˺�����, double ��ǰ�˺�);
	void Buff_TriggerDeath(args_ReceiveDamage* �˺�����);
	void Buff_TriggerKiller(TechnoClass* Ŀ�굥λ, args_ReceiveDamage* �˺�����);
	double Buff_TriggerSelf(double ��ǰ�˺�);
	void Buff_DoDamages(double ׼����ɵ��˺�);
	//Update 0815
	SIBuffClass* Buff_GetBuff(int Buff����); // ����ֵ����Ϊ��
	SIBuffClass* Buff_GetBuff(SIBuffTypeClass* Buff����); // ����ֵ����Ϊ��
	void AvailableEffect();
	void UnavailableEffect();

	//Ԫ�ؿ��� & Ԫ��������ء�
	void Element_Merge_ResistanceType(SIElementResistanceTypeClass* Ԫ�ؿ�������, int ��������);
	void Element_Merge_ImmuneElementType(SIElementTypeClass* Ԫ������, int ��������);

	//����״̬��ء�
	//�����ύ�����İ�ť��״̬��
	void Update();
	void UpdateBar_UpdateAllow();
	void UpdateDrawBar();
	bool UpdateClickBar_Hover(HouseClass* �������ս��, Point2D* �������ƫ��);
	void UpdateClickBar_TryClick(HouseClass* �������ս��, Point2D* �������ƫ��, int& Buff����, SIDigitalClickBarIndex& �������, double& ��ǰֵ, bool& ���ñ�ʶ);
	void UpdateClickBar_Effect(int Buff����, SIDigitalClickBarIndex �������, HouseClass* �������ս��, double ��ǰֵ);
	bool UpdateKeyExecuteBar_TryExecute(HouseClass* �������ս��, SICommandCode ��ݼ�����);
	void UpdateKeyExecuteBar_Effect(HouseClass* �������ս��, SICommandCode ��ݼ�����);
	void Init();
	void UnInit();

	//WIC �����߼�
	// ���ֻ�Ǹ��º����������ı��δ��벻����
	//����ʹ���������������WIC˵����
	void UpdateTechnoType(TechnoTypeClass* �ɵĵ�λ����);

	//��Ӧ�ڵ�λ��SetOwningHouse�����õ������ǲ�ͨ��techno->SetOwningHouse�����ı��˵�λ��������ս��
	void UpdateOwningHouse(const HouseClass* const �ɵĵ�λ������ս��, const HouseClass* const �µĵ�λ������ս��);

};

