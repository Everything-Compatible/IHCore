#pragma once
#include <cstdint>
#include <concepts>
#include <YRPPCore.h>
#include <vector>
#include <string>
#include <CRC.h>

struct ECRTTIInfo;

void Internal_RegisterExportRTTI(const char* ClassName, const ECRTTIInfo* pInfo);
const ECRTTIInfo* Internal_GetRTTIInfo (const char* ClassName);

struct ECRTTIInfo {
    // ������Ϣ�ڵ�
    struct ImplNode {
        const ECRTTIInfo* pBaseInfo; // ����RTTI��Ϣ
        size_t Offset;            // ���������е�ƫ����
    };

    size_t Count; // ��������

    // ͨ��VLA��ʽ���ʻ�����Ϣ
    const ImplNode& GetBaseInfo(size_t Index) const;

#define FindTypeOffset_Failure 0xFFFFFFFF
    size_t FindTypeOffset(const ECRTTIInfo* TypeToFind)const;

    const char* GetName() const;

    std::vector<const char*> GetBaseClassName() const;

    std::vector<const ECRTTIInfo*> GetBaseClassInfo() const;

    const char* GetFirstBaseClassName() const;

    const ECRTTIInfo* GetFirstBaseClassInfo() const;
};



template<typename T>
concept CanUseECRTTI = requires
{
    { T::StaticGetRTTIInfo() } -> std::same_as<const ECRTTIInfo*>;
};


class ECRTTI
{
    void* DynamicCast_Impl(const void* Ptr, const ECRTTIInfo* TargetType)const
    {
        if (auto Offset = WhatAmI()->FindTypeOffset(TargetType); Offset != FindTypeOffset_Failure)
            return reinterpret_cast<void*>(reinterpret_cast<char*>(const_cast<void*>(Ptr)) + Offset);
        return nullptr;
    }
public:
    virtual const ECRTTIInfo* WhatAmI() const = 0;

    template<CanUseECRTTI T, CanUseECRTTI Derived>
    T* DynamicCast_Impl(Derived* Ptr)const
    {
        return reinterpret_cast<T*>(DynamicCast_Impl(Ptr, T::StaticGetRTTIInfo()));
    }
    template<CanUseECRTTI T, CanUseECRTTI Derived>
    const T* DynamicCast_Impl(const Derived* Ptr)const
    {
        return reinterpret_cast<const T*>(DynamicCast_Impl(const_cast<Derived*>(Ptr), T::StaticGetRTTIInfo()));
    }
    template<CanUseECRTTI T, CanUseECRTTI Derived>
    bool HasSameActualType(const Derived* p1, const T* p2)
    {
        return p1->WhatAmI() == p2->WhatAmI();
    }
};


// ================ ����ƫ�Ƽ��� ================
template <typename Base, typename Derived>
constexpr size_t GetOffset() {
    // ͨ��ָ��ƫ�Ƽ��㼼����ȡ����λ��
    // ԭ����ָ������ת����ĵ�ַ�Ϊƫ����
    return reinterpret_cast<size_t>(
        static_cast<Base*>(
            reinterpret_cast<Derived*>(0x1000) // ��������ַ�����ָ������
            )
        ) - 0x1000; // ��ȥ��׼��ַ�õ���ƫ��
}

template <typename... Args>
struct CountHelper {
    static constexpr size_t Value = sizeof...(Args);
};

template <CanUseECRTTI Base, CanUseECRTTI Derived>
struct DeriveClassData {
    ECRTTIInfo::ImplNode Node;

    // ���캯������RTTI��Ϣ��ƫ����
    DeriveClassData()
        : Node{ Base::StaticGetRTTIInfo(), GetOffset<Base, Derived>() } // ����Derivedʵ��GetRTTIInfoImpl
    {}

    // ������������concept���
    using BaseType = Base;
    using DerivedType = Derived;
};

template <typename T>
concept CountHelperType = requires {
    // ����Ƿ�ΪDeriveClassData�ػ�����
        requires std::is_same_v<T, DeriveClassData<typename T::BaseType, typename T::DerivedType>>;
};

template <CountHelperType... Args> // ֻ���ܺϷ�����������
struct ECRTTIInfoDef {
    // ���������������ڼ��㣩
    size_t Count{ CountHelper<Args...>::Value };
    const static constexpr size_t Static_Count{ CountHelper<Args...>::Value };

    // �洢���л������ݽڵ�
    std::tuple<Args...> BaseClasses;

    const ECRTTIInfo* GetImpl() const {
        return reinterpret_cast<const ECRTTIInfo*>(this);
    }
};

template <typename Derived, typename... Bases>
using ECRTTIInfoType = ECRTTIInfoDef<DeriveClassData<Bases, Derived>...>;

template <typename... Args>
struct ECRTTIExportHelper
{
    ECRTTIExportHelper(const char* ClassName)
    {
        if (!Internal_GetRTTIInfo(ClassName))
        {
            auto pNewInfo = new ECRTTIInfoType<Args...>{};
            Internal_RegisterExportRTTI(ClassName, pNewInfo->GetImpl());
        }
        else MessageBoxA(NULL, (std::string("����������RTTI�ࣺ") + ClassName).c_str(), "EC SDK", MB_OK);
    }
};


class ECUniqueID
{
    int UniqueID{ 0 };
public:
    ECUniqueID() noexcept;
    ECUniqueID(const ECUniqueID&) noexcept;
    ECUniqueID(ECUniqueID&&) noexcept;
    int GetUniqueID() const{ return UniqueID; }
};

class ECStreamReader;
class ECStreamWriter;
class AbstractClass;
class ECGameClass;
class SIBuffClass;

//�Զ�����
class ECLoadSaveable
{
    virtual void LoadGame(ECStreamReader& Stream) = 0;
    virtual void SaveGame(ECStreamWriter& Stream) = 0;
    virtual size_t GetSizeMax() = 0;
    virtual void FinalSwizzle() = 0;
    virtual void PointerGotInvalid(AbstractClass* const pObject, bool bRemoved) = 0;
    virtual void PointerGotInvalid(ECGameClass* const pObject, bool bRemoved) = 0;
    virtual void PointerGotInvalid(SIBuffClass* const pObject, bool bRemoved) = 0;
};

//�Զ�����
class ECSync
{
    virtual void ComputeCRC(CRCEngine& crc) = 0;
};



#define ECObjType class

#define EC_USERTTI virtual public ECRTTI
#define ECRTTI_DefineRTTIFunction(Class) \
private:\
mutable const ECRTTIInfo* MyType{ nullptr };\
public:\
static const ECRTTIInfo* StaticGetRTTIInfo() { return Internal_GetRTTIInfo(#Class); }\
virtual const ECRTTIInfo* WhatAmI() const { if(!MyType)MyType = StaticGetRTTIInfo(); return MyType; }\
template<typename T>\
T* DynamicCast(){return DynamicCast_Impl<T, Class>(this); }\
template<typename T>\
const T* DynamicCast()const{return DynamicCast_Impl<T, Class>(this); }\
template<CanUseECRTTI T>\
bool HasSameActualType(const T* p) { return this->WhatAmI() == p->WhatAmI(); }
//ע�⣡��ֻ�д���������DLL���в���Ҫ�������������������õ�ʱ��Ҫ���������
#define ECRTTI_ExportRTTI(Derived, ...) \
class Derived;\
namespace ECRTTIHelperNamespace{ ::ECRTTIExportHelper<Derived, ##__VA_ARGS__> Derived ## ExportHelperObject{ #Derived }; }

#define EC_USEUNIQUEID virtual public ECUniqueID

#define EC_SYNC public ECSync

#define EC_USELOADSAVE public ECLoadSaveable
//ǰ�᣺�Ѿ�ʵ����Serialize����
#define ECRTTI_DefineLoadSaveFunction \
virtual void LoadGame(ECStreamReader& Stream){ this->Serialize(Stream);}\
virtual void SaveGame(ECStreamWriter& Stream){ this->Serialize(Stream);}
