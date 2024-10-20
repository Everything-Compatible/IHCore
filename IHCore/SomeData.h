#pragma once
#include <YRPP.h>
#include <CD.h>
#include <CRT.h>
#include <Wstring.h>
#include <Straws.h>

namespace GameData
{
	//OK

	//现在在读的是不是音频文件，是则为1
	static reference<bool, 0xB04BEC> IsReadingAudioFile;
	//自订战役中选中的地图,在选地图界面上可以查现在被选择的地图名字
	//如果没有进入过该界面，此为空串
	static reference<wchar_t, 0xAC0EC8, 45> MPMapSelPage_SelectedMapName;
	//选中的地图名字的存储区域（后台）
	// = SessionClass::Instance->Config.MapDescription
	static reference<wchar_t, 0xA8B322, 45> MPSelectedMapName;
	//最近一张随机地图的名字，默认为“随机地图”
	static reference<wchar_t, 0xABE050, 45> MPLatestRamdomMap;

	//if Unk_DontCheck==TRUE then skip some checks 
	//else check if sub_5E2CD0(StringTable::GetString("TXT_LAUNCHBROWSER", "D:\ra2mdpost\wonline.cpp", 1398), 4, (int)sub_77D710, 0) == 6 
	//return true if success
	static bool __fastcall OpenWebsite(const char* Website, BOOL Unk_DontCheck)
	{
		JMP_STD(0x77DC90);
	}

	//YR不再支持修改菜单页面的分辨率，此时改变会在选择应用设置之后IE
	static bool __fastcall SetDisplayMode(int Width, int Height)
	{
		JMP_STD(0x561180);
	}

	static Point2D* __fastcall DrawSWTimer(int TimerIdx, ColorScheme* pColor, int TotSeconds,
		const wchar_t* Title, int* BlinkTimer = nullptr, int* BlinkState = nullptr)//传nullptr则不闪烁
	{
		JMP_STD(0x6D4B50);
	}

	//一个随叫随到的公用缓冲区
	static reference<char, 0x8A3A08, 0x10000> RAPublicBufferI;

	//是否捕捉主循环的异常，仅在初始化时（WinMain中0x6BE02F位置）检查一次值
	static reference<const bool, 0xA8F7AC> MainGameLoopCatchExceptions;

	static INT_PTR __fastcall ShowInternalErrorDialog()//if OK return 1153
	{
		JMP_STD(0x4C84E0);
	}
	
	static void ReadYRMData(_In_ const char* FileName,
		_Out_ wchar_t* Desc,
		_Out_ char* Digest,
		_Out_ char* GameModes,
		_Out_ bool& IsOfficialMap,
		_Out_ int& MinPlayers,
		_Out_ int& MaxPlayers)
	{
		JMP_STD(0x6994F0);
	}
}


//---------------
//  ADDED IHS
//---------------






class CCINIExtBufferElem
{
public:
	DynamicVectorClass<DynamicVectorClass<char*>*> VectorC1;
	DynamicVectorClass<DynamicVectorClass<char*>*> VectorC2;
	DWORD unknown_30;
	DWORD unknown_34;
	DWORD unknown_38;
	DWORD unknown_3C;
	DWORD unknown_40;
	DWORD unknown_44;
	DWORD unknown_48;
};

class CCINIExtBuffer
{
public:
	DWORD dwUnk1;
	DWORD dwUnk2;
	DWORD dwUnk3;
	DWORD dwUnk4;
	CCINIExtBufferElem ElementVLAHeader;//这是VLA的第一个元素

	CCINIExtBufferElem& GetElem_Unchecked(int Idx)//不检查元素个数
	{
		CCINIExtBufferElem* Array = &ElementVLAHeader;
		return Array[Idx];
	}
};

class CCINIExtClass : public CCINIClass
{
public:
	static constexpr reference<CCINIExtClass, 0x89F518u> const INI_COOPCAMPMD{};

	CCINIExtBuffer* Buffer;
	DWORD BufferElemCount;
	bool Loaded;
	PROTECTED_PROPERTY(BYTE, padding_61[3]);

	CCINIExtBufferElem* GetElem(int Idx)
	{
		if (!Loaded)return nullptr;
		_ASSERT(Idx < BufferElemCount);
		return &Buffer->GetElem_Unchecked(Idx);
	}

	static void __fastcall LoadCoopMDIni(CCINIExtClass&)
	{
		JMP_STD(0x49DB00);
	}
};


struct RA2MDIniSetting
{
	static reference<RA2MDIniSetting, 0xA8EB60> Instance;

	int GameSpeed;
	int Difficulty;
	int CampDifficulty;
	int ScrollMethod;
	int ScrollRate;
	int IsAutoscrollOn;
	int DetailLevel;
	bool Unknown_1C;
	bool SidebarCameoText;
	bool UnitActionLines;
	bool ShowHidden;
	bool IsToolTipsOn;
	char Unknown_21[3];
	int ScreenWidth;
	int ScreenHeight;
	DWORD ResolutionX;
	DWORD ResolutionY;
	bool StretchMovies;
	bool AllowHiRes;
	bool AllowVRAMSidebar;
	char Unknown_37;
	int SoundVolume;
	int VoiceVolume;
	int ScoreVolume;
	bool IsScoreRepeat;
	bool InGameMusic;
	bool IsScoreShuffle;
	bool Unknown_47;
	USHORT SoundLatency;
	USHORT Socket;
	int NetID;
	int Unknown_50;
	int NetCard;
	char DestNet[64];
};

/*
SessionClass::SessionClass()
{
	JMP_THIS(0x697AB0);
}

SessionClass::~SessionClass()
{
	JMP_THIS(0x697840);
}*/


struct WString__Impl
{
	char* Buffer;
	inline bool operator==(const WString__Impl& rhs)
	{
		return !strcmp(Buffer, rhs.Buffer);
	}
};

enum class DiskID :int
{
	NotInDisk = -2//不在光盘上
};

class MultiMission
{
public:
	//无PKT的YRM地图的构造函数
	MultiMission(
		const char* FileName,
		const wchar_t* Desc,
		const char* Digest,
		bool IsOfficialMap,
		const char* GameModes,
		int MinPlayers,
		int MaxPlayers)
	{
		JMP_THIS(0x89F518);
	}

	//MapName没有后缀名，由构造函数内部加上".MAP"
	//有PKT地图的构造函数
	MultiMission(CCINIClass* pPKT, const char* MapName)
	{
		JMP_THIS(0x69A3B0);
	}
	//没找着析构

	MultiMission() = delete;

	wchar_t Description[44];
	char FileFullName[260];
	char Digest[32];
	bool IsOfficialMap;
	int MinPlayers;
	int MaxPlayers;
	TypeList<DiskID> DiskList;
	DynamicVectorClass<Wstring> GameModes;
};







struct LightingData
{
	BYTE Data[256];
};

struct VplRemapData
{
	float BrightNess[16];
	float MaxBrightNess;
};

struct NormalLighting
{
	LightingData Lighting[32];
	DWORD Block1[2048];
	DWORD Block2[262];
	DWORD UnknownData1[256];
	int64_t UnknownData2[31];
	DWORD Unknown_4910;
	BYTE Unknown_4914;
	BYTE Unknown_4915;
	__int16 Unknown_4916;
	VplRemapData RemapData;
	DWORD Block4[1449];
	DWORD Block5[2048];
};

struct VplHeader
{
	DWORD unknown_0;
	DWORD unknown_1;
	DWORD BlockCount;//always 128?
	DWORD unknown_3;
};

class VplClass
{
public:
	VplHeader Header;
	BytePalette* VplPalette;
	NormalLighting* Lighting;
	bool OwnsVplPalette;
	bool OwnsNormalLighting;

	VplClass(const BytePalette* Palette = nullptr, const NormalLighting* Lighting = nullptr)//传入nullptr则创建一个新的
	{
		JMP_THIS(0x758950);
	}

	~VplClass()
	{
		JMP_THIS(0x7589C0);
	}

	BOOL LoadFromFile(CCFileClass* pFile)
	{
		JMP_THIS(0x758A30);
	}

	//Operates VplManager::VplNormalLighting
	int VplManager_RemapLighting(VplRemapData* pRemapData, int SectionCount)
	{
		JMP_THIS(0x758B70);
	}

	BYTE RemapIndex(float R, float G, float B, char IsRemapColor)
	{
		JMP_THIS(0x758EA0);
	}
};


class VplManager
{
public:
	static reference<BytePalette, 0xB2FB78> VplPalette;
	static reference<NormalLighting, 0xB41178> VplNormalLighting;
	static reference<int, 0xB2FB58> VplLightingGroupCount;
	
	//About VoxelAnim Operating VplNormalLighting::UnknownData1
	static bool __fastcall ProcessUnknownData1(DWORD a1, DWORD a2, float* a3)
	{
		JMP_STD(0x756590);
	}

	//IDK
	static int __fastcall sub_756860(DWORD a1, int a2, float* a3)
	{
		JMP_STD(0x756860);
	}

	//Operating VplNormalLighting::UnknownData2
	static bool __fastcall ProcessUnknownData2_A(float* a1, int a2)
	{
		JMP_STD(0x758670);
	}
	static bool __fastcall ProcessUnknownData2_B(float* a1, float* a2, float a3, int a4)
	{
		JMP_STD(0x7586F0);
	}
};