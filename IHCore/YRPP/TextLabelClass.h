#pragma once

#include <GadgetClass.h>

class NOVTABLE TextLabelClass : public GadgetClass
{
public:
	//Non virtual

	//Statics

	//Constructors
	TextLabelClass(wchar_t* pText, int X, int Y, int ColorSchemeIndex, TextPrintType style) noexcept
		: GadgetClass(noinit_t()) { JMP_THIS(0x72A440); }
protected:
	explicit __forceinline TextLabelClass(noinit_t)  noexcept
		: GadgetClass(noinit_t())
	{
	}

	//Properties
public:

	void* UserData1;
	void* UserData2;
	DWORD Style;
	const wchar_t* Text;
	int ColorSchemeIndex;
	DWORD PixWidth;
	DWORD anim_dword3C;//动态显示时标记显示到了哪个位置
	bool SkipDraw;
	bool Animate;
	DWORD AnimPos;
	DWORD AnimTiming;
};
