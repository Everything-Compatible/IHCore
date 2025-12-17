#include <Windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include "LocalData.h"
#include <WIC.Define.h>

DWORD GetExportedFunctionCount(HMODULE hModule) {
    if (!hModule) return 0;

    // 获取DOS头
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        return 0;
    }

    // 获取NT头
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
        return 0;
    }

    // 获取导出表RVA
    DWORD exportRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (exportRVA == 0) {
        return 0;  // 没有导出表
    }

    // 获取导出表
    PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + exportRVA);

    // 返回导出函数数量
    return exportDir->NumberOfFunctions;
}

HMODULE GetModuleFromAddressTH32(void* funcAddress) {
    if (!funcAddress) return NULL;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
    if (hSnapshot == INVALID_HANDLE_VALUE) return NULL;

    MODULEENTRY32 me32 = { 0 };
    me32.dwSize = sizeof(MODULEENTRY32);

    HMODULE hFound = NULL;

    if (Module32First(hSnapshot, &me32)) {
        do {
            // 检查地址是否在模块的地址范围内
            if ((DWORD_PTR)funcAddress >= (DWORD_PTR)me32.modBaseAddr &&
                (DWORD_PTR)funcAddress < (DWORD_PTR)me32.modBaseAddr + me32.modBaseSize) {
                hFound = me32.hModule;
                break;
            }
        } while (Module32Next(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return hFound;
}

//在已知WIC版本为10的第二重检查
bool CheckSIWICNoLessThan197(Local::LibType& WIC)
{
    LibFuncHandle hd = nullptr;
    if (WIC.Out->GetFunc)hd = WIC.Out->GetFunc;
    if (WIC.Out->OrderedInit)hd = WIC.Out->OrderedInit;
    if (!hd)return false;//啥都没有的版本不会出问题

    HMODULE hm = GetModuleFromAddressTH32(hd);
    if (!hm)return false;//敢情WIC是冒牌的

    auto cnt = GetExportedFunctionCount(hm);

    return cnt >= 230;
}

bool HasHighWIC()
{
    if (!Local::LibMap.contains(WIC_LibName))
    {
        return false;
    }
    else
    {
        auto& lib = Local::LibMap[WIC_LibName];
        if (lib->Out->Info->Version <= 9)
        {
            return false;
        }
        else if (lib->Out->GetFunc && lib->Out->GetFunc("GetLTSVer", DoNotCheckVersion))
        {
            return false;
        }
        else if (!CheckSIWICNoLessThan197(*lib))
        {
            return false;
        }
    }
    return true;
}