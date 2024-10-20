#include <YRPP.h>
#include "InitialLoad.h"
#include "SyringeEx.h"
#include "Global.h"
#include <vector>
#include "ToolFunc.h"
#include <unordered_set>
#include "sha1.h"
#include "Debug.h"
#include "SomeData.h"

void InitMix();
void UninitMix();

std::vector<MixFileClass*> Mixes;
std::unordered_set<std::string, UpperHash, UpperEqualPred> BlackList;
std::unordered_set<std::string, UpperHash, UpperEqualPred> WhiteList;


void InitMix()
{
	UninitMix();
	Service_MixFile.RefreshAndProcess([](const auto& Param)
		{
			Mixes.push_back(GameCreate<MixFileClass>(Param.FileName));
		});
	char Name[MAX_PATH];
	GetModuleFileNameA(hInstDLL, Name, MAX_PATH);
	auto pIH = SyringeData::GetLibData(PathFindFileNameA(Name));
	if (pIH)
	{
		JsonFile F; F.Parse(SyringeData::GetSettingText(*pIH));
		if (!F.Available())return;
		auto Obj = F.GetObj().GetObjectItem("CustomMix");
		if (Obj.Available())
			for (auto& s : Obj.GetArrayString())
				Mixes.push_back(GameCreate<MixFileClass>(s.c_str()));
		/*
		Obj = F.GetObj().GetObjectItem("WhiteList");
		if (Obj.Available() && Obj.IsNotEmptyArray())
		{
			for (auto& s : Obj.GetArrayString())
				WhiteList.insert(s);
		}
		
		else
		{*/
			Obj = F.GetObj().GetObjectItem("BlackList");
			if (Obj.Available())
				for (auto& s : Obj.GetArrayString())
					BlackList.insert(s);
		//}
		Obj = F.GetObj().GetObjectItem("Mixfiles");
		if (Obj.Available())
			for (auto& s : Obj.GetPVObject())
			{
				int addon = 0;
				auto it = s.second.GetObjectItem("add-on");
				if (it.Available() && it.IsTypeBool())
				{
					addon = it.GetBool() ? 1 : 0;
				}
				bool Success = true;
				CCFileClass* pCC = GameCreate<CCFileClass>(s.first.c_str());
				if (pCC->Open(FileAccessMode::Read))
				{
					it = s.second.GetObjectItem("size");
					if (it.Available() && it.IsTypeNumber() && it.GetInt() != pCC->GetFileSize())
					{
						Debug::Log("IHCore : ERROR! Size Expected = %d Actual = %d\n", it.GetInt(), pCC->GetFileSize());
						Success = false;
						if (addon == 0)
						{
							GameData::ShowInternalErrorDialog();
							Game::RaiseError(0xEA114514);
						}
						//校验失败。寄。
					}
					it = s.second.GetObjectItem("hash");
					if (it.Available() && it.IsTypeString())
					{
						auto pFile = pCC->ReadWholeFile();
						auto SHA1 = GetSHA1(pFile, pCC->GetFileSize());
						if (SHA1 != it.GetString())
						{
							Debug::Log("IHCore : ERROR! Hash Expected = %s Actual = %s\n", it.GetCString(), SHA1.c_str());
							Success = false;
							if (addon == 0)
							{
								GameData::ShowInternalErrorDialog();
								Game::RaiseError(0xEA114514);
							}
							//校验失败。寄。
						}
						GameDelete(pFile);
					}
					pCC->Close();
				}
				else
				{
					Success = false;
					Debug::Log("IHCore : ERROR! %s Does not Exist\n", s.first.c_str());
					if (addon == 0)
					{
						GameData::ShowInternalErrorDialog();
						Game::RaiseError(0xEA114514);
					}
					//校验失败。寄。
				}
				GameDelete(pCC);
				if(Success)Mixes.push_back(GameCreate<MixFileClass>(s.first.c_str()));
			}
	}
}

void UninitMix()
{
	for (auto& p : Mixes)
	{
		if (p)GameDelete(p);
	}
	Mixes.clear();
}

DEFINE_HOOK(0x53029E, LoadCustomMix, 5)
{
	InitMix();
	return 0;
}

DEFINE_HOOK(0x6BE9BD, ReleaseCustomMix, 6)
{
	UninitMix();
	return 0;
}