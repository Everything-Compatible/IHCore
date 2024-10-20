#include "WIC.Buff.h"

// ====================================================================================================
// ==== BuffClass �ӿ� - ���̺��� =====================================================================
// ====================================================================================================

void SIBuffClass::TryRemark( SIPack_BuffSetting_FromStatic* Buff�������ð�)
{
	SIBuff::TryRemark(this, Buff�������ð�);
}

void SIBuffClass::TryActive( SIPack_BuffSetting_FromStatic* Buff�������ð�)
{
	SIBuff::TryActive(this, Buff�������ð�);
}

void SIBuffClass::TryAfter()
{
	SIBuff::TryAfter(this);
}

void SIBuffClass::TryDamage( double ׼����ɵ��˺�)
{
	SIBuff::TryDamage(this, ׼����ɵ��˺�);
}

// ====================================================================================================
// ==== BuffClass �ӿ� - ���Ժ��� =====================================================================
// ====================================================================================================

void SIBuffClass::EnterState( SIBuffClass_State ״̬)
{
	SIBuff::EnterState(this, ״̬);
}

void SIBuffClass::MergeSetting( SIPack_BuffSetting_FromStatic* Buff�������ð�)
{
	SIBuff::MergeSetting(this, Buff�������ð�);
}

CoordStruct SIBuffClass::GetCenterCoords()
{
	return SIBuff::GetCenterCoords(this);
}

// ����ֵ����Ϊ��
TechnoClass* SIBuffClass::GetOwnerTechno()
{
	return SIBuff::GetOwnerTechno(this);
}

// ����ֵ����Ϊ��
TechnoClass* SIBuffClass::GetSourceTechno()
{
	return SIBuff::GetSourceTechno(this);
}

// ����ֵ����Ϊ��
TechnoTypeClass* SIBuffClass::GetOwnerTechnoType()
{
	return SIBuff::GetOwnerTechnoType(this);
}

// ����ֵ����Ϊ��
TechnoTypeClass* SIBuffClass::GetSourceTechnoType()
{
	return SIBuff::GetSourceTechnoType(this);
}

HouseClass* SIBuffClass::GetActiveOwnerHouse()
{
	return SIBuff::GetActiveOwnerHouse(this);
}

HouseClass* SIBuffClass::GetActiveSourceHouse()
{
	return SIBuff::GetActiveSourceHouse(this);
}

// ====================================================================================================
// ==== BuffClass �ӿ� - ���ߺ��� =====================================================================
// ====================================================================================================

int SIBuffClass::GetEffectMode( int ����������, int ���ֵ)
{
	return SIBuff::GetEffectMode_A(this, ����������, ���ֵ);
}

int SIBuffClass::GetEffectMode( int ����������, int ���ֵ, int ��Сֵ)
{
	return SIBuff::GetEffectMode_B(this, ����������, ���ֵ, ��Сֵ);
}

double SIBuffClass::GetEffectPower( int ����������)
{
	return SIBuff::GetEffectPower_A(this, ����������);
}

double SIBuffClass::GetEffectPower( int ����������, double ��Сֵ)
{
	return SIBuff::GetEffectPower_B(this, ����������, ��Сֵ);
}

double SIBuffClass::GetEffectTotal( int ����������, double �ϲ����ʵ��Ч��ǿ��)
{
	return SIBuff::GetEffectTotal_A(this, ����������, �ϲ����ʵ��Ч��ǿ��);
}

double SIBuffClass::GetEffectTotal( int ����������, double �ϲ����ʵ��Ч��ǿ��, double ��Сֵ)
{
	return SIBuff::GetEffectTotal_B(this, ����������, �ϲ����ʵ��Ч��ǿ��, ��Сֵ);
}

double SIBuffClass::GetEffectTotalMax( int ����������)
{
	return SIBuff::GetEffectTotalMax(this, ����������);
}

double SIBuffClass::GetEffectTotalMin( int ����������)
{
	return SIBuff::GetEffectTotalMin(this, ����������);
}

bool SIBuffClass::Effect_NotPassCommonCheck()
{
	return SIBuff::Effect_NotPassCommonCheck_A(this);
}

bool SIBuffClass::Effect_NotPassCommonCheck(SIBuffClass_EffectData* ��Ч����)
{
	return SIBuff::Effect_NotPassCommonCheck_B(this, ��Ч����);
}

bool SIBuffClass::Effect_NotPassCommonCheck( args_ReceiveDamage* �˺�����)
{
	return SIBuff::Effect_NotPassCommonCheck_C(this, �˺�����);
}

bool SIBuffClass::Effect_NotPassCommonCheck( args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
{
	return SIBuff::Effect_NotPassCommonCheck_D(this, �˺�����, ��ǰ�˺�);
}

bool SIBuffClass::Effect_NotPassCommonCheck( double ��ǰ�˺�)
{
	return SIBuff::Effect_NotPassCommonCheck_E(this, ��ǰ�˺�);
}

bool SIBuffClass::Effect_NotPassCommonCheck_NoIronCurtain()
{
	return SIBuff::Effect_NotPassCommonCheck_NoIronCurtain(this);
}

bool SIBuffClass::Effect_PassCheck_TechnoTypeList( TechnoTypeClass* Ŀ�굥λ����)
{
	return SIBuff::Effect_PassCheck_TechnoTypeList(this, Ŀ�굥λ����);
}

bool SIBuffClass::Effect_NotPassCheck_TechnoTypeList( TechnoTypeClass* Ŀ�굥λ����)
{
	return SIBuff::Effect_NotPassCheck_TechnoTypeList(this, Ŀ�굥λ����);
}

bool SIBuffClass::Effect_Match_TechnoTypeList( TechnoTypeClass* Ŀ�굥λ����)
{
	return SIBuff::Effect_Match_TechnoTypeList(this, Ŀ�굥λ����);
}

bool SIBuffClass::Effect_NotMatch_TechnoTypeList( TechnoTypeClass* Ŀ�굥λ����)
{
	return SIBuff::Effect_NotMatch_TechnoTypeList(this, Ŀ�굥λ����);
}

// ������ SIDamageLeft ����
void SIBuffClass::ReceiveWarheadDamage( TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����)
{
	SIBuff::ReceiveWarheadDamage_A(this, Ŀ�굥λ, ������λ, �˺�, �˺���ͷ����);
}

// ������ SIDamageLeft ����
void SIBuffClass::ReceiveWarheadDamage( TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����, HouseClass* ������λ������ս��)
{
	SIBuff::ReceiveWarheadDamage_B(this, Ŀ�굥λ, ������λ, �˺�, �˺���ͷ����, ������λ������ս��);
}

// ������ SIDamageLeft ����
void SIBuffClass::ReceiveHealthDamage( TechnoClass* Ŀ�굥λ, double ����ֵ���, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������)
{
	SIBuff::ReceiveHealthDamage_A(this, Ŀ�굥λ, ����ֵ���, ���㻤�׵�ͷ����, �����˺���ͷ����, ִ�����������);
}

// ������ SIDamageLeft ����
void SIBuffClass::ReceiveHealthDamage( TechnoClass* Ŀ�굥λ, double ����ֵ���, double ����ֵ�������, double ����ֵ�������, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������)
{
	SIBuff::ReceiveHealthDamage_B(this, Ŀ�굥λ, ����ֵ���, ����ֵ�������, ����ֵ�������, ���㻤�׵�ͷ����, �����˺���ͷ����, ִ�����������);
}

bool SIBuffClass::FreshOrPostBroadcast( double ��Ҫ���µĹ㲥ǿ��, AbstractClass* ָ��Ŀ��)
{
	return SIBuff::FreshOrPostBroadcast(this, ��Ҫ���µĹ㲥ǿ��, ָ��Ŀ��);
}

void SIBuffClass::ResetBroadcastPower( double ��Ҫ���µĹ㲥ǿ��)
{
	SIBuff::ResetBroadcastPower(this, ��Ҫ���µĹ㲥ǿ��);
}

int SIBuffClass::GetBroadcastListenerCount( bool ������������, AbstractClass* ָ��Ŀ��)
{
	return SIBuff::GetBroadcastListenerCount(this, ������������, ָ��Ŀ��);
}

bool SIBuffClass::AddAsBroadcastListener()
{
	return SIBuff::AddAsBroadcastListener(this);
}

void SIBuffClass::RemoveAsBroadcastListener()
{
	SIBuff::RemoveAsBroadcastListener(this);
}

double SIBuffClass::GetBroadcastPower_Total()
{
	return SIBuff::GetBroadcastPower_Total(this);
}

// ����ֵ����Ϊ��
TechnoClass* SIBuffClass::GetFireOwner()
{
	return SIBuff::GetFireOwner(this);
}

bool SIBuffClass::GetFireOwnerRawOffset( CoordStruct& ��������)
{
	return SIBuff::GetFireOwnerRawOffset(this, ��������);
}

bool SIBuffClass::GetFireTargetRawOffset( AbstractClass* Ŀ��, CoordStruct& Ŀ������)
{
	return SIBuff::GetFireTargetRawOffset(this, Ŀ��, Ŀ������);
}

double SIBuffClass::GetFacingRotate( SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������)
{
	return SIBuff::GetFacingRotate(this, ������ת����, ��������, Ŀ������);
}

double SIBuffClass::GetFacingRotate_Same( SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������, SIRotateType ����ͬ���ĸ�����ת����, double ����ͬ������ת�Ƕ�_����)
{
	return SIBuff::GetFacingRotate_Same(this, ������ת����, ��������, Ŀ������, ����ͬ���ĸ�����ת����, ����ͬ������ת�Ƕ�_����);
}

void SIBuffClass::FreshFacingAndAimingRadius_Common( CoordStruct ��������, CoordStruct ���忪������, CoordStruct Ŀ������, CoordStruct& ��������, CoordStruct& ��������)
{
	SIBuff::FreshFacingAndAimingRadius_Common(this, ��������, ���忪������, Ŀ������, ��������, ��������);
}

void SIBuffClass::OffsetMotion_Init()
{
	SIBuff::OffsetMotion_Init(this);
}

void SIBuffClass::OffsetMotion_AI()
{
	SIBuff::OffsetMotion_AI(this);
}

void SIBuffClass::DigitalButtonData_Init( int ��λ����)
{
	SIBuff::DigitalButtonData_Init(this, ��λ����);
}

namespace SIBuff
{

	// ====================================================================================================
	// ==== BuffClass �ӿ� - ���̺��� =====================================================================
	// ====================================================================================================

	void SI_API TryRemark(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)
	{
		SIBuffDecl_void(TryRemark)(Buff, Buff�������ð�);
	}

	void SI_API TryActive(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)
	{
		SIBuffDecl_void(TryActive)(Buff, Buff�������ð�);
	}

	void SI_API TryAfter(SIBuffClass* Buff)
	{
		SIBuffDecl_void(TryAfter)(Buff);
	}

	void SI_API TryDamage(SIBuffClass* Buff, double ׼����ɵ��˺�)
	{
		SIBuffDecl_void(TryDamage)(Buff, ׼����ɵ��˺�);
	}

	// ====================================================================================================
	// ==== BuffClass �ӿ� - ���Ժ��� =====================================================================
	// ====================================================================================================

	void SI_API EnterState(SIBuffClass* Buff, SIBuffClass_State ״̬)
	{
		SIBuffDecl_void(EnterState)(Buff, ״̬);
	}

	void SI_API MergeSetting(SIBuffClass* Buff, SIPack_BuffSetting_FromStatic* Buff�������ð�)
	{
		SIBuffDecl_void(MergeSetting)(Buff, Buff�������ð�);
	}

	CoordStruct SI_API GetCenterCoords(SIBuffClass* Buff)
	{
		SIBuffDecl(GetCenterCoords)(Buff);
	}

	// ����ֵ����Ϊ��
	TechnoClass* SI_API GetOwnerTechno(SIBuffClass* Buff)
	{
		SIBuffDecl(GetOwnerTechno)(Buff);
	}

	// ����ֵ����Ϊ��
	TechnoClass* SI_API GetSourceTechno(SIBuffClass* Buff)
	{
		SIBuffDecl(GetSourceTechno)(Buff);
	}

	// ����ֵ����Ϊ��
	TechnoTypeClass* SI_API GetOwnerTechnoType(SIBuffClass* Buff)
	{
		SIBuffDecl(GetOwnerTechnoType)(Buff);
	}

	// ����ֵ����Ϊ��
	TechnoTypeClass* SI_API GetSourceTechnoType(SIBuffClass* Buff)
	{
		SIBuffDecl(GetSourceTechnoType)(Buff);
	}

	HouseClass* SI_API GetActiveOwnerHouse(SIBuffClass* Buff)
	{
		SIBuffDecl(GetActiveOwnerHouse)(Buff);
	}

	HouseClass* SI_API GetActiveSourceHouse(SIBuffClass* Buff)
	{
		SIBuffDecl(GetActiveSourceHouse)(Buff);
	}

	// ====================================================================================================
	// ==== BuffClass �ӿ� - ���ߺ��� =====================================================================
	// ====================================================================================================

	int SI_API GetEffectMode_A(SIBuffClass* Buff, int ����������, int ���ֵ)
	{
		SIBuffDecl(GetEffectMode_A)(Buff, ����������, ���ֵ);
	}

	int SI_API GetEffectMode_B(SIBuffClass* Buff, int ����������, int ���ֵ, int ��Сֵ)
	{
		SIBuffDecl(GetEffectMode_B)(Buff, ����������, ���ֵ, ��Сֵ);
	}

	double SI_API GetEffectPower_A(SIBuffClass* Buff, int ����������)
	{
		SIBuffDecl(GetEffectPower_A)(Buff, ����������);
	}

	double SI_API GetEffectPower_B(SIBuffClass* Buff, int ����������, double ��Сֵ)
	{
		SIBuffDecl(GetEffectPower_B)(Buff, ����������, ��Сֵ);
	}

	double SI_API GetEffectTotal_A(SIBuffClass* Buff, int ����������, double �ϲ����ʵ��Ч��ǿ��)
	{
		SIBuffDecl(GetEffectTotal_A)(Buff, ����������, �ϲ����ʵ��Ч��ǿ��);
	}

	double SI_API GetEffectTotal_B(SIBuffClass* Buff, int ����������, double �ϲ����ʵ��Ч��ǿ��, double ��Сֵ)
	{
		SIBuffDecl(GetEffectTotal_B)(Buff, ����������, �ϲ����ʵ��Ч��ǿ��, ��Сֵ);
	}

	double SI_API GetEffectTotalMax(SIBuffClass* Buff, int ����������)
	{
		SIBuffDecl(GetEffectTotalMax)(Buff, ����������);
	}

	double SI_API GetEffectTotalMin(SIBuffClass* Buff, int ����������)
	{
		SIBuffDecl(GetEffectTotalMin)(Buff, ����������);
	}

	bool SI_API Effect_NotPassCommonCheck_A(SIBuffClass* Buff)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_A)(Buff);
	}

	bool SI_API Effect_NotPassCommonCheck_B(SIBuffClass* Buff, SIBuffClass_EffectData* ��Ч����)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_B)(Buff, ��Ч����);
	}

	bool SI_API Effect_NotPassCommonCheck_C(SIBuffClass* Buff, args_ReceiveDamage* �˺�����)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_C)(Buff, �˺�����);
	}

	bool SI_API Effect_NotPassCommonCheck_D(SIBuffClass* Buff, args_ReceiveDamage* �˺�����, double ��ǰ�˺�)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_D)(Buff, �˺�����, ��ǰ�˺�);
	}

	bool SI_API Effect_NotPassCommonCheck_E(SIBuffClass* Buff, double ��ǰ�˺�)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_E)(Buff, ��ǰ�˺�);
	}

	bool SI_API Effect_NotPassCommonCheck_NoIronCurtain(SIBuffClass* Buff)
	{
		SIBuffDecl(Effect_NotPassCommonCheck_NoIronCurtain)(Buff);
	}

	bool SI_API Effect_PassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����)
	{
		SIBuffDecl(Effect_PassCheck_TechnoTypeList)(Buff, Ŀ�굥λ����);
	}

	bool SI_API Effect_NotPassCheck_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����)
	{
		SIBuffDecl(Effect_NotPassCheck_TechnoTypeList)(Buff, Ŀ�굥λ����);
	}

	bool SI_API Effect_Match_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����)
	{
		SIBuffDecl(Effect_Match_TechnoTypeList)(Buff, Ŀ�굥λ����);
	}

	bool SI_API Effect_NotMatch_TechnoTypeList(SIBuffClass* Buff, TechnoTypeClass* Ŀ�굥λ����)
	{
		SIBuffDecl(Effect_NotMatch_TechnoTypeList)(Buff, Ŀ�굥λ����);
	}

	// ������ SIDamageLeft ����
	void SI_API ReceiveWarheadDamage_A(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����)
	{
		SIBuffDecl_void(ReceiveWarheadDamage_A)(Buff, Ŀ�굥λ, ������λ, �˺�, �˺���ͷ����);
	}

	// ������ SIDamageLeft ����
	void SI_API ReceiveWarheadDamage_B(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, TechnoClass* ������λ, double �˺�, WarheadTypeClass* �˺���ͷ����, HouseClass* ������λ������ս��)
	{
		SIBuffDecl_void(ReceiveWarheadDamage_B)(Buff, Ŀ�굥λ, ������λ, �˺�, �˺���ͷ����, ������λ������ս��);
	}

	// ������ SIDamageLeft ����
	void SI_API ReceiveHealthDamage_A(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, double ����ֵ���, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������)
	{
		SIBuffDecl_void(ReceiveHealthDamage_A)(Buff, Ŀ�굥λ, ����ֵ���, ���㻤�׵�ͷ����, �����˺���ͷ����, ִ�����������);
	}

	// ������ SIDamageLeft ����
	void SI_API ReceiveHealthDamage_B(SIBuffClass* Buff, TechnoClass* Ŀ�굥λ, double ����ֵ���, double ����ֵ�������, double ����ֵ�������, WarheadTypeClass* ���㻤�׵�ͷ����, WarheadTypeClass* �����˺���ͷ����, bool ִ�����������)
	{
		SIBuffDecl_void(ReceiveHealthDamage_B)(Buff, Ŀ�굥λ, ����ֵ���, ����ֵ�������, ����ֵ�������, ���㻤�׵�ͷ����, �����˺���ͷ����, ִ�����������);
	}

	bool SI_API FreshOrPostBroadcast(SIBuffClass* Buff, double ��Ҫ���µĹ㲥ǿ��, AbstractClass* ָ��Ŀ��)
	{
		SIBuffDecl(FreshOrPostBroadcast)(Buff, ��Ҫ���µĹ㲥ǿ��, ָ��Ŀ��);
	}

	void SI_API ResetBroadcastPower(SIBuffClass* Buff, double ��Ҫ���µĹ㲥ǿ��)
	{
		SIBuffDecl_void(ResetBroadcastPower)(Buff, ��Ҫ���µĹ㲥ǿ��);
	}

	int SI_API GetBroadcastListenerCount(SIBuffClass* Buff, bool ������������, AbstractClass* ָ��Ŀ��)
	{
		SIBuffDecl(GetBroadcastListenerCount)(Buff, ������������, ָ��Ŀ��);
	}

	bool SI_API AddAsBroadcastListener(SIBuffClass* Buff)
	{
		SIBuffDecl(AddAsBroadcastListener)(Buff);
	}

	void SI_API RemoveAsBroadcastListener(SIBuffClass* Buff)
	{
		SIBuffDecl_void(RemoveAsBroadcastListener)(Buff);
	}

	double SI_API GetBroadcastPower_Total(SIBuffClass* Buff)
	{
		SIBuffDecl(GetBroadcastPower_Total)(Buff);
	}

	// ����ֵ����Ϊ��
	TechnoClass* SI_API GetFireOwner(SIBuffClass* Buff)
	{
		SIBuffDecl(GetFireOwner)(Buff);
	}

	bool SI_API GetFireOwnerRawOffset(SIBuffClass* Buff, CoordStruct& ��������)
	{
		SIBuffDecl(GetFireOwnerRawOffset)(Buff, ��������);
	}

	bool SI_API GetFireTargetRawOffset(SIBuffClass* Buff, AbstractClass* Ŀ��, CoordStruct& Ŀ������)
	{
		SIBuffDecl(GetFireTargetRawOffset)(Buff, Ŀ��, Ŀ������);
	}

	double SI_API GetFacingRotate(SIBuffClass* Buff, SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������)
	{
		SIBuffDecl(GetFacingRotate)(Buff, ������ת����, ��������, Ŀ������);
	}

	double SI_API GetFacingRotate_Same(SIBuffClass* Buff, SIRotateType ������ת����, CoordStruct ��������, CoordStruct Ŀ������, SIRotateType ����ͬ���ĸ�����ת����, double ����ͬ������ת�Ƕ�_����)
	{
		SIBuffDecl(GetFacingRotate_Same)(Buff, ������ת����, ��������, Ŀ������, ����ͬ���ĸ�����ת����, ����ͬ������ת�Ƕ�_����);
	}

	void SI_API FreshFacingAndAimingRadius_Common(SIBuffClass* Buff, CoordStruct ��������, CoordStruct ���忪������, CoordStruct Ŀ������, CoordStruct& ��������, CoordStruct& ��������)
	{
		SIBuffDecl_void(FreshFacingAndAimingRadius_Common)(Buff, ��������, ���忪������, Ŀ������, ��������, ��������);
	}

	void SI_API OffsetMotion_Init(SIBuffClass* Buff)
	{
		SIBuffDecl_void(OffsetMotion_Init)(Buff);
	}

	void SI_API OffsetMotion_AI(SIBuffClass* Buff)
	{
		SIBuffDecl_void(OffsetMotion_AI)(Buff);
	}

	void SI_API DigitalButtonData_Init(SIBuffClass* Buff, int ��λ����)
	{
		SIBuffDecl_void(DigitalButtonData_Init)(Buff, ��λ����);
	}
}


// ���ܺ���
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack(HouseClass* ��ǰ��ս��)
{
	return SIClassManager::MakePack(this, ��ǰ��ս��);
}
// ��������
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_Empty()
{
	return SIClassManager::MakePack_Empty();
}
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_DurationOnly(int ����ʱ��, bool ���ǳ���ʱ��, bool ���ܳ���ʱ�俹��)
{
	return SIClassManager::MakePack_DurationOnly(����ʱ��, ���ǳ���ʱ��, ���ܳ���ʱ�俹��);
}
SIPack_BuffSetting_FromStatic SIPackTypeClass_BuffSetting::MakePack_PowerOnly(double ǿ��ֵ, bool ����ǿ��ֵ, bool ����ǿ��ֵ����)
{
	return SIClassManager::MakePack_PowerOnly(ǿ��ֵ, ����ǿ��ֵ, ����ǿ��ֵ����);
}