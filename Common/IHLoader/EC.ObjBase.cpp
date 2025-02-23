#include "EC.ObjBase.h"


const char* Internal_GetRTTIClassName(const ECRTTIInfo* pInfo);

const ECRTTIInfo::ImplNode& ECRTTIInfo::GetBaseInfo(size_t Index) const 
{
    // 计算内存偏移：跳过Count，按节点大小索引
    return *reinterpret_cast<const ImplNode*>(
        reinterpret_cast<const char*>(this) +
        sizeof(Count) +
        Index * sizeof(ImplNode)
        );
}

const char* ECRTTIInfo::GetName() const
{
    return Internal_GetRTTIClassName(this);
}

std::vector<const char*> ECRTTIInfo::GetBaseClassName() const
{
    std::vector<const char*> Result;
    Result.resize(Count);
    for (size_t i = 0; i < Count; i++)
        Result[i] = Internal_GetRTTIClassName(GetBaseInfo(i).pBaseInfo);
    return Result;
}
std::vector<const ECRTTIInfo*> ECRTTIInfo::GetBaseClassInfo() const
{
    std::vector<const ECRTTIInfo*> Result;
    Result.resize(Count);
    for (size_t i = 0; i < Count; i++)
        Result[i] = GetBaseInfo(i).pBaseInfo;
    return Result;
}

const char* ECRTTIInfo::GetFirstBaseClassName() const
{
    return Internal_GetRTTIClassName(GetBaseInfo(0).pBaseInfo);
}

const ECRTTIInfo* ECRTTIInfo::GetFirstBaseClassInfo() const
{
    return GetBaseInfo(0).pBaseInfo;
}

size_t ECRTTIInfo::FindTypeOffset(const ECRTTIInfo* TypeToFind) const
{
    if (TypeToFind == this)return 0;
    for (size_t i = 0; i < Count; i++)
        if (GetBaseInfo(i).pBaseInfo == TypeToFind)
            return GetBaseInfo(i).Offset;
    return FindTypeOffset_Failure;
}

int Internal_GenerateID();


ECUniqueID::ECUniqueID() noexcept :UniqueID(Internal_GenerateID()) {};
ECUniqueID::ECUniqueID(const ECUniqueID&)  noexcept :UniqueID(Internal_GenerateID()) {};
ECUniqueID::ECUniqueID(ECUniqueID&&) noexcept :UniqueID(Internal_GenerateID()) {};
