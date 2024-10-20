#include "MyLoader.h"
#include <Helpers/Macro.h>
#include <WIC.h>
#include <WWMessageBox.h>
#include <BitFont.h>

void DbgLog(const char* pFormat, ...)
{
	JMP_STD(0x4068E0);
}

/*
UNIT TEST:

HARD CODE:
RegisterRoutine : OK
ActiveRoutine(Members) & DeleteRoutine : OK
MyGetFunc & GetFuncFromLib : OK
IHCore LibFunc & DoNotCheckVersion: OK

*/

/*
Ext::ActiveRoutine SwingR{ noinit_t() };
int __cdecl Swing(RoutineParam*)
{
	::MessageBoxA(NULL, "AA", "fjdskfds", MB_OK);
	return 30;
}

int __cdecl KillSwing(RoutineParam*)
{
	bool HasSel = false;
	for (auto const& curTechno : *TechnoClass::Array)
	{
		if (curTechno->IsSelected)
		{
			HasSel = true;
			break;
		}
	}
	if (HasSel)
	{
		SwingR.Pause();
	}
	else
	{
		SwingR.Resume();
	}
	return 10;
}*/
/*
EXPORT_FUNC(Get00)
{
	GET_STACK(DWORD, chk, 0);
	DbgLog("\nPtr is %X\n", chk);
	return 0;
}*/

void PrintBitMap(size_t sz, const unsigned char* Data)
{
	char ss[120];
	DbgLog("Width=%d\n", (int)Data[0]);
	for (size_t i = 0; i < sz; i++)
	{
		_itoa(Data[i], ss, 2);
		DbgLog("%08s", ss);
		if (i == 0)DbgLog("\n");
		if (i % 3 == 0)DbgLog("\n");
	}
	DbgLog("\n");
}

void PrintBitMap(size_t sz, wchar_t Char)
{
	auto Data = BitFont::Instance->GetCharacterBitmap(Char);
	char ss[120];
	DbgLog("Width=%d\n", (int)Data[0]);
	for (size_t i = 0; i < sz; i++)
	{
		_itoa(Data[i], ss, 2);
		DbgLog("%08s", ss);
		if (i == 0)DbgLog("\n");
		if (i % 3 == 0)DbgLog("\n");
	}
	DbgLog("\n");
}

int __cdecl YouShouldNotSelect(RoutineParam*)
{
	static bool First = true;
	if (First)
	{
		/*
			Width = 20
			Height = 16
			Stride = 3
			Lines = 17
			Size = 49
			Count = 载入字形数
			ValidCount = 载入符号数
		*/
		
		DbgLog("Width = %d Height = %d\n", BitFont::Instance->InternalPTR->FontWidth, BitFont::Instance->InternalPTR->FontHeight);
		DbgLog("Stride = %d Lines = %d\n", BitFont::Instance->InternalPTR->Stride, BitFont::Instance->InternalPTR->Lines);
		DbgLog("Count = %d ValidCount = %d Size = %d\n", BitFont::Instance->InternalPTR->SymbolCount, BitFont::Instance->InternalPTR->ValidSymbolCount, BitFont::Instance->InternalPTR->SymbolDataSize);
		auto sz = BitFont::Instance->InternalPTR->SymbolDataSize;
		PrintBitMap(sz, L'a');
		PrintBitMap(sz, L'C');
		PrintBitMap(sz, L'。');
		PrintBitMap(sz, L'D');
		First = false;
	}
	
	bool HasSel = false;
	std::vector <TechnoClass*> vt;
	for (auto const& curTechno : *TechnoClass::Array)
	{
		if (curTechno->IsSelected)
		{
			HasSel = true;
			vt.push_back(curTechno);
			//curTechno->Deselect();
		}
	}
	if (HasSel)
	{
		for (auto& p : vt)
		{
			try
			{

				auto ty = SIClassManager::BuffType_Find("BuffType1");
				DbgLog("ExampleSIBuff Type Ptr at %X\n", ty);
				auto Interface = SIClassManager::GetExtendData(p);
				DbgLog("%s has an SIInterface at %s\n", p->GetType()->Name, Interface->GetName());
				auto Pack = SIClassManager::MakePack_Empty();
				if (!Interface->Buff_HasBuffed(ty))
				{
					DbgLog("ExampleSIBuff will be created\n");
					Interface->Buff_CreateOrMerge(ty, &Pack, Interface->OwnerEntity(), p->GetOwningHouse());
					//WWMessageBox::Instance->Process(L"我测你的马");
					DbgLog("ExampleSIBuff has created\n");
				}
				else
				{
					DbgLog("ExampleSIBuff is on the Unit\n");
				}
			}
			catch (std::exception& e)
			{
				DbgLog("WIC EXCEPTION: %s\n", e.what());
			}
			//Interface->UpdateTechnoType(TechnoTypeClass::Find("E2"));
			//p->GetTechnoType()
			//Interface->Property_Register_ExtraMultiplier_Attack(1.5);
			//DbgLog("%s has an SIInterface at %s\n", p->GetType()->Name, Interface->GetName());
		}
		//auto p = Ext::GetFuncFromLib("IHCore", "ShowIEDialog", DoNotCheckVersion);
		//if (p)Ext::GeneralCall(*p);
		/*
		auto Lib = Ext::GetLib("SIWinterIsComing");
		if (Lib.Available())
		{
			DbgLog("WIC Available\n");
			DbgLog(
				"Name = %s\n"
				"Version = %d\n"
				"FnTable = %X\n"
				"FnTable.size = %d\n"
				"Fn = %X\n",

				Lib.LibName(),
				Lib.Version(),
				Lib.LibFnTable(),
				Lib.LibFnTable()->Size,
				//Lib.QueryFunction("SIInterface_ExtendData::GetInterface", DoNotCheckVersion)
				Lib.QueryFunction("WIC.IExt.GetInterface", DoNotCheckVersion)
			);
			DbgLog(
				"Function = %X\n",
				Lib.GetRaw()->Out->GetFunc("WIC.IExt.GetInterface", Lib.Version())
			);
		}//*/

	}
	return 30;
}

const FuncInfo* __cdecl MyGetFunc(const char* Name, int Version)
{
	DbgLog("Call MyGetFunc Name %s, Version %d CurVer %d\n", Name, Version, PRODUCT_VERSION);
	if (PRODUCT_VERSION < Version)return nullptr;
	auto it = Funcs.find(Name);
	DbgLog("Try to find\n");
	if (it == Funcs.end())return nullptr;
	else return &it->second;
}

/*
__declspec(noinline) void* __cdecl operator_new(size_t Size)
{
	//char* p = nullptr;
	//p[5] = 6;
	//return p;
	return 0;
}

struct ccf
{
	char cc[2000];
};

void* OutR()
{
	char ss[1000];
	sprintf(ss, "%p", OutR);
	MessageBoxA(NULL, ss, "Test", MB_OK);
	sprintf(ss, "%p", operator_new);
	MessageBoxA(NULL, ss, "Test", MB_OK);
	//void* pp=operator_new(5000);
	auto ccs = GameCreate<ccf>();
	return 0;// pp;
}
*/
InitDependency MyDependency[1] =
{
	{"SIWinterIsComing",DoNotCheckVersion,WIC_LIBRARY_VERSION,true}
};

void __cdecl DependentInit()
{
	try
	{
		auto bufs = SIClassManager::RegisterBuff<ExampleSIBuff>("ExampleSIBuff");
	}
	catch (SIException& e)
	{
		DbgLog("IHTest: Error \"%s\"\n", e.what());
	}
}

void MyInit(InitResult& Result)
{
	static LibVersionInfo info;
	info.LibName = "IHTest";
	info.Version = PRODUCT_VERSION;
	info.Description = u8"IH测试组件";
	Result.Info = &info;
	Result.GetFunc = (GetFunc_t)MyGetFunc;
	Result.Dependencies.N = 1;
	Result.Dependencies.Data = MyDependency;
	Result.OrderedInit = DependentInit;

	if (0)
	{
		Ext::CSFClass Ra2MD;
		Ra2MD.LoadAsCCFile("ra2md.csf");

		MessageBoxW(NULL, Ra2MD.GetStringDefault("Name:HTNK").Value, L"WTF", MB_OK);
		MessageBoxW(NULL, Ra2MD.GetStringDefault("nAME:htnk").Value, L"WTF", MB_OK);
		MessageBoxW(NULL, Ra2MD.GetStringDefault("GUI:OK").Value, L"WTF", MB_OK);
		MessageBoxW(NULL, Ra2MD.GetStringDefault("Name:MISSING").Value, L"WTF", MB_OK);

		DbgLog("NOOOOOOOOOOOOOOOOOOOOOO\n");

		auto exe = SyringeData::GetExeData();
		DbgLog(
			"Addr1 = \"%08X\"\n"
			"Addr = \"%08X\"\n"
			"Path = \"%s\"\n"
			"FileName = \"%s\"\n"
			"Syringe = \"%s\"\n",
			SyringeData::BaseAddress(), &exe,
			exe.AbsPath,
			exe.FileName,
			exe.SyringeVersionStr
		);

		for (auto& p : SyringeData::LibMap)
		{
			DbgLog(
				"Library: \n"
				"\tName = \"%s\"\n"
				"\tAbsPath = \"%s\"\n"
				"\tLibID = %08X\n"
				,
				p.second->LibName,
				p.second->AbsPath,
				p.second->LibID
			);
		}
		for (auto& p : SyringeData::AddrMap)
		{
			auto parr = SyringeData::GetOpCode(p.second->Addr);
			DbgLog(
				"Addr %08X Has %d Hooks:\nOpCode %08X NValidHook %d OriginalOpCode %d Bytes at %08X \nID : ",
				p.second->Addr,
				p.second->HookCount,
				p.second->HookDataAddr,
				SyringeData::GetValidHookCount(p.second->Addr),
				parr.N,
				parr.Data
			);
			for (int i = 0; i < p.second->HookCount; i++)
			{
				DbgLog("%08X ", p.second->HookIdx(i));
			}
			DbgLog("\n");
		}
		for (auto& p : SyringeData::HookMap)
		{
			auto pLib = SyringeData::GetLibData(p.second->LibID);
			DbgLog(
				"Hook At %08X ID %08X LibID %08X-\"%s\"  \nProcName \"%s\" Overriding %d Bytes \n",
				p.second->HookAddress,
				p.second->HookID,
				p.second->LibID,
				pLib ? pLib->LibName : "NO LIBRARY",
				p.second->ProcName,
				p.second->OverrideLength
			);
		}
		for (auto& p : SyringeData::MemMap)
		{
			DbgLog(
				"Copy Mem \"%s\" At %08X \n",
				p.second->Name,
				p.second->Addr
			);
		}

	}
	//OutR();
	//SwingR.Init("Swing",Swing);//Ext::RegisterRoutine("Swing", Swing);
	//Ext::RegisterRoutine("KillSwing", KillSwing);
	Ext::RegisterRoutine("YouShouldNotSelect", YouShouldNotSelect, NullRoutineParam);
}


void __cdecl NOOOOOOO(void)
{
	::MessageBoxA(NULL, "AA", "NOOOOOOOOOOOOOOOOOOOOOO", MB_OK);
}


void __cdecl WEX(void)
{
	::MessageBoxA(NULL, "AA", "YEEEEEEEEEEE", MB_OK);
}

const std::unordered_map<std::string, FuncInfo>Funcs
{
	{"NOOOOOOO",FuncInfo(NOOOOOOO, FuncType::Procedure)},
	{"IHCore::Exit",FuncInfo(WEX, FuncType::Procedure)},
};

