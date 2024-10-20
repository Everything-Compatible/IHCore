
#include "Command.h"
#include "MiscFunc.h"
#include "Version.h"
#include <YRPP.h>

DEFINE_HOOK(0x531413, IHGameStart, 5)
{
	DSurface::Hidden->DrawText(PRODUCT_FULLNAME_STR L" is active!", 10, 480, COLOR_RED | COLOR_GREEN);
	return 0;
}

template <typename T>
void MakeCommand()
{
	T* command = GameCreate<T>();
	CommandClass::Array->AddItem(command);
};


void RegisterCommands()
{
	MakeCommand<AttachScriptClass>();
}


DEFINE_HOOK(0x533062, IHCommandClassRegister, 7)
{
	RegisterCommands();

	DWORD* D = GameCreate<DWORD>();
	R->EAX(D);	//Allocate SetUnitTabCommandClass
	return 0x533066;
}


namespace Test
{
	int CurrentFrame();
}

//extern int QuGs;

void AttachScriptClass::Execute(WWKey dwUnk) const
{
	
	
	//MessageListClass::Instance->PrintMessage(L"Test Function executed!");
	//This will be called when the command gets executed.
	//MessageListClass::Instance->PrintMessage(StringTable::LoadString("TXT_DEBUG1"));
	//MessageListClass::Instance->PrintMessage((L"Frame = " + std::to_wstring(Test::CurrentFrame())).c_str());
	/*
	for (auto const& curTechno : *TechnoClass::Array)
	{
		if (curTechno->IsSelected)
		{
			MessageListClass::Instance->PrintMessage(curTechno->GetUIName());
			PlayAnimOnObject("NUKEANIM", curTechno, 0x01u);
		}
	}
	//MessageListClass::Instance->PrintMessage((L"QuGs = " + std::to_wstring(QuGs)).c_str());
	//*/
	//*
	auto pAnimType = AnimTypeClass::Find("NUKEANIM");
	if(pAnimType==nullptr) MessageListClass::Instance->PrintMessage(L"No such Anim");
	else for (auto const& curTechno : *TechnoClass::Array)
	{
		if (curTechno->IsSelected)
		{
			//MessageListClass::Instance->PrintMessage(curTechno->GetUIName());
			AnimClass* pAnim = GameCreate<AnimClass>(pAnimType, curTechno->Location);
			pAnim->SetOwnerObject(curTechno);
			pAnim->RemainingIterations = 0x5u;//0xFFu;
			AnimClass::Array->AddItem(pAnim);
		}
	}
	//*/
	//VoxClass::Play("EVA_NewTechnologyAcquired");
}
const wchar_t* AttachScriptClass::GetUICategory() const
{
	return StringTable::LoadString("TXT_IHDEBUGTYPE");
}
const wchar_t* AttachScriptClass::GetUIName() const
{
	return StringTable::LoadString("TXT_IHDEBUG");
}

const wchar_t* AttachScriptClass::GetUIDescription() const
{
	return StringTable::LoadString("TXT_IHDEBUG_DESC");
}

