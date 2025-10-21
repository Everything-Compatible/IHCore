#include "ECInterprocess.h"
#include "Debug.h"
#include <filesystem>
#include "ExtFile.h"

namespace ReadRemoteComponents_ErrorContext
{
	std::u8string CurrentFileName{};
}

/*

Examples as JSON:

"Components" : [
	{
	  "Name": "calculator",
	  "Run": "C:/services/calculator.exe",
	  "CommType": "Pipe",
	  "Location": "\\\\.\\pipe\\calculator_service"
	},
	{
	  "Name": "file_processor",
	  "Run": "C:/services/file_processor.py",
	  "CommType": "TCP",
	  "Location": "127.0.0.1:8081"
	}
]

*/
void RemoteComponentNameType::Load(JsonObject obj)
{
	CommType = Unknown;

	if (!obj.HasItem("Name"))
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] RemoteComponentNameType::Load: Missing Field : \"Name\"\n");
		return;
	}
	if (!obj.HasItem("Run"))
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] RemoteComponentNameType::Load: Missing Field : \"Run\"");
		return;
	}
	if (!obj.HasItem("CommType"))
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] RemoteComponentNameType::Load: Missing Field : \"CommType\"");
		return;
	}
	if (!obj.HasItem("Location"))
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] RemoteComponentNameType::Load: Missing Field : \"Location\"");
		return;
	}

	std::u8string commTypeStr = ~obj.ItemString("CommType");
	if (commTypeStr == u8"Pipe")
	{
		CommType = Pipe;
	}
	else if (commTypeStr == u8"TCP")
	{
		CommType = TCP;
	}
	else
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] RemoteComponentNameType::Load: Unknown CommType : \"%s\"\n", commTypeStr.c_str());
		return;
	}
	
	RegName = ~obj.ItemString("Name");
	RunCommand = ~obj.ItemString("Run");
	Location = ~obj.ItemString("Location");
}

/*

Target :
*.json

*/
std::vector<RemoteComponentNameType> ReadRemoteComponents(const std::filesystem::path& file)
{
	ReadRemoteComponents_ErrorContext::CurrentFileName = file.u8string();
	JsonFile F;
	ExtFileClass E;
	E.Open(file.wstring().c_str(), ExtFileClass::GetReadSignW());
	auto PA = E.ReadWholeFile();
	std::string Content(PA.AsStringView());
	auto ParseResult = F.ParseTmpChecked(std::move(Content), conv u8"[ERROR POSITION]");
	PA.Delete();
	if (!ParseResult.empty())
	{
		Debug::Log("[EC] On Reading Json File \"%s\"\n", ReadRemoteComponents_ErrorContext::CurrentFileName.c_str());
		Debug::Log("[EC] JSON Parse Error: %s\n", ParseResult.c_str());
		return {};
	}
	
	std::vector<RemoteComponentNameType> result;
	JsonObject root = F.GetObj();
	if (!root.HasItem("Components"))
	{
		//Consider as empty
		return {};
	}

	auto ComponentsArr = root.ItemArrayObject("Components");
	result.resize(ComponentsArr.size());
	for (auto i = 0; i < ComponentsArr.size(); i++)
	{
		result[i].Load(ComponentsArr[i]);
	}
	
	return result;

}

std::vector<RemoteComponentNameType> ReadRemoteComponents(const std::filesystem::path& dir)
{
	Debug::Log("[EC] Reading Remote Components from Directory: \"%s\"\n", dir.u8string().c_str());
	std::vector<RemoteComponentNameType> result;
	for (auto& p : std::filesystem::directory_iterator(dir))
	{
		//filter *.json file
		if (p.is_regular_file() && p.path().extension() == ".json")
		{
			Debug::Log("[EC] Found Remote Component Definition File: \"%s\"\n", p.path().u8string().c_str());
			auto content = ReadRemoteComponents(p.path());
			result.insert(result.end(), content.begin(), content.end());
		}
	}
}


/*

Read From :
Game Root Dir (Current Dir)
&
Root Dir/Components

Target :
*.json

*/
std::vector<RemoteComponentNameType> ReadRemoteComponents()
{
	std::filesystem::path rootPath = std::filesystem::current_path();

	auto components1 = ReadRemoteComponents(rootPath);
	auto components2 = ReadRemoteComponents(rootPath / "Components");

	components1.insert(components1.end(), components2.begin(), components2.end());
	return components1;
}