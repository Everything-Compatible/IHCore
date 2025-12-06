#pragma once
#include <YRPP.h>
#include <BitFont.h>

void __cdecl AutoWrapTextEx(
    BitFont* _In_ Font,
    const wchar_t* _In_ Str,
    int _In_ MaxPixels,
    int _In_ MaxChars,
    bool _In_ WordWrap,
    int& _Out_ Len,
    const wchar_t*& _Out_ StrBefore,
    const wchar_t*& _Out_ StrAfter
);


std::wstring ParseStringForTest(const wchar_t* Str);
