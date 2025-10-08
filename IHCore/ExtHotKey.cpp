
#include <YRPP.h>
#include "HotKeyClass.h"
#include "Patch.h"
#include "ToolFunc.h"
#include "../Common/IHLoader/IH.h"
#include <unordered_map>

std::string GetWWKeyName(WWKey k)
{
	static wchar_t wkbuf[1000]{};
	wkbuf[0] = L'\0';
	GetWWKeyDesc(k, wkbuf);
	return UnicodetoUTF8(wkbuf);
}


std::unordered_map<std::string, std::string> HotKeyDescMap;
UTF8_CString __cdecl TextDrawRouter_HotKey(const char* Key)
{
	auto it = HotKeyDescMap.find(Key);
	if (it == HotKeyDescMap.end())return nullptr;
	else return conv it->second.c_str();
}

void UpdateHotKeyVars()
{
	//MessageBoxA(NULL, "HotKeyExt: UpdateHotKeyVars called", "HotKeyExt", MB_OK);
	HotKeyDescMap.clear();
	for (auto& hk : HotKeyListClass::Instance.get())
	{
		if (!hk.Command)continue;
		auto Name = std::string(hk.Command->GetName());
		//Key.xxx.KeyName
		HotKeyDescMap[Name + ".KeyName"] = GetWWKeyName(hk.KeyID);
		//Key.xxx.Name
		HotKeyDescMap[Name + ".Name"] = UnicodetoUTF8(hk.Command->GetUIName());
		//Key.xxx.Desc
		HotKeyDescMap[Name + ".Desc"] = UnicodetoUTF8(hk.Command->GetUIDescription());
		//Key.xxx.Category
		HotKeyDescMap[Name + ".Category"] = UnicodetoUTF8(hk.Command->GetUICategory());
	}
}

DEFINE_HOOK(0x533EC0, ReadKeyboardMD, 0x8)
{
	UpdateHotKeyVars();
	IH::SetTextDrawRouter("Key", TextDrawRouter_HotKey);
	return 0;
}

struct HotKeySaveClass
{
	int sub_474430(int a2, char a3)
	{
		JMP_THIS(0x474430);
	}

	int sub_474430_new(int a2, char a3)
	{
		UpdateHotKeyVars();
		return sub_474430(a2, a3);
	}
};

void HotKey_InitBeforeEverything()
{
	Patch::Apply_CALL(0x5FB9CF, union_cast<void*>(&HotKeySaveClass::sub_474430_new));
}