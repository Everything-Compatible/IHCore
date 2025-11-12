#pragma once

#include <CommandClass.h>
#include <CRT.h>

//get the description of WWKey
//same as the setting menu
void __fastcall GetWWKeyDesc(WWKey Key, wchar_t* DescBuf)
{
	JMP_STD(0x61EF70);
}

struct HotKeyClass
{
	WWKey KeyID;
	CommandClass* Command;
};

static_assert(sizeof(HotKeyClass) == 8);

auto KeyCompare = reinterpret_cast<int(__cdecl *)(const void* a1, const void* a2)>(0x48BC20);

struct HotKeyListClass
{
	static constexpr reference<HotKeyListClass, 0x87F680u> const Instance{};

	HotKeyClass* HotKeys;
	int HotKeyCount;
	int Capacity;
	bool Sorted;
	HotKeyClass* LastPressedKey;

	bool ExpandCapacity(int CapacityIncrement)
	{
		JMP_THIS(0x538B80);
	}

	bool Sort()
	{
		if (!Sorted)
		{
			CRT::qsort(HotKeys, HotKeyCount, sizeof(HotKeyClass), KeyCompare);
			LastPressedKey = nullptr;
			Sorted = true;
		}
	}

	//return nullptr if not found
	HotKeyClass* FindKey(WWKey& Key)
	{
		JMP_THIS(0x55F6E0);
	}

	HotKeyClass* begin()
	{
		return HotKeys;
	}

	HotKeyClass* end()
	{
		return HotKeys + HotKeyCount;
	}


};