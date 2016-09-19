#include <windows.h>

#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <filesystem>
#include <bitset>
namespace fs = std::experimental::filesystem;
#include "cpplinq.hpp"

#include "Logger.hpp"

#include "IGenerator.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"
#include "Package.hpp"
#include "NameValidator.hpp"
#include "PrintHelper.hpp"

extern IGenerator* generator;

/// <summary>
/// Dumps the objects and names to files.
/// </summary>
/// <param name="path">The path where to create the dumps.</param>
void Dump(const fs::path& path)
{
	{
		std::ofstream o(path / "ObjectsDump.txt");

		for (auto obj : ObjectsStore())
		{
			tfm::format(o, "[%06i] %-100s 0x%P\n", obj.Index, obj.Object.GetFullName(), obj.Object.GetAddress());
		}
	}

	{
		std::ofstream o(path / "NamesDump.txt");

		for (auto name : NamesStore())
		{
			tfm::format(o, "[%06i] %s\n", name.Index, name.Name);
		}
	}
}

/// <summary>
/// Generates the sdk header.
/// </summary>
/// <param name="path">The path where to create the sdk header.</param>
/// <param name="definedClasses">The defined classes info.</param>
/// <param name="packageOrder">The package order info.</param>
void SaveSDKHeader(const fs::path& path, const std::unordered_map<UEObject, bool>& definedClasses, const std::vector<UEObject>& packageOrder)
{
	std::ofstream os(path / "SDK.hpp");

	os << "#pragma once\n\n"
		<< tfm::format("// %s (%s) SDK\n\n", generator->GetGameName(), generator->GetGameVersion());

	//Includes
	os << "#include <set>\n";
	os << "#include <string>\n";
	for (auto&& i : generator->GetIncludes())
	{
		os << "#include " << i << "\n";
	}

	//include the basics
	{
		{
			std::ofstream os2(path / "SDK" / tfm::format("%s_Basic.hpp", generator->GetGameNameShort()));

			PrintFileHeader(os2);
			
			os2 << generator->GetBasicDeclarations() << "\n";

			PrintFileFooter(os2);

			os << "\n#include \"SDK/" << tfm::format("%s_Basic.hpp", generator->GetGameNameShort()) << "\"\n";
		}
		{
			std::ofstream os2(path / "SDK" / tfm::format("%s_Basic.cpp", generator->GetGameNameShort()));

			PrintFileHeader(os2, { "\"../SDK.hpp\"" });

			os2 << generator->GetBasicDefinitions() << "\n";

			PrintFileFooter(os2);
		}
	}

	using namespace cpplinq;

	//check for missing structs
	auto missing = from(definedClasses) >> where([](auto&& kv) { return kv.second == false; });
	if (missing >> any())
	{
		std::ofstream os2(path / "SDK" / tfm::format("%s_MISSING.hpp", generator->GetGameNameShort()));

		PrintFileHeader(os2);

		for (auto&& s : missing >> select([](auto&& kv) { return kv.first.Cast<UEStruct>(); }) >> experimental::container())
		{
			os2 << "// " << s.GetFullName() << "\n// ";
			os2 << tfm::format("0x%04X\n", s.GetPropertySize());

			os2 << "struct " << MakeValidName(s.GetNameCPP()) << "\n{\n";
			os2 << "\tunsigned char UnknownData[0x" << tfm::format("%X", s.GetPropertySize()) << "];\n};\n\n";
		}

		PrintFileFooter(os2);

		os << "\n#include \"SDK/" << tfm::format("%s_MISSING.hpp", generator->GetGameNameShort()) << "\"\n";
	}

	os << "\n";

	for (auto&& package : packageOrder)
	{
		os << R"(#include "SDK/)" << tfm::format("%s_%s_structs.hpp", generator->GetGameNameShort(), package.GetName()) << "\"\n";
		os << R"(#include "SDK/)" << tfm::format("%s_%s_classes.hpp", generator->GetGameNameShort(), package.GetName()) << "\"\n";
	}
}

/// <summary>
/// Process the packages.
/// </summary>
/// <param name="path">The path where to create the package files.</param>
void ProcessPackages(const fs::path& path)
{
	auto sdkPath = path / "SDK";
	fs::create_directories(sdkPath);
	
	std::unordered_set<UEObject> uniquePackages;
	std::unordered_set<UEObject> excludePackage;
	std::vector<UEObject> packageOrder;

	std::unordered_map<UEObject, bool> definedClasses;

	for (auto obj : ObjectsStore())
	{
		auto packageObj = obj.Object.GetPackageObject();
		if (packageObj.IsValid())
		{
			if (uniquePackages.find(packageObj) == std::end(uniquePackages))
			{
				uniquePackages.insert(packageObj);

				Package package(packageObj, packageOrder, definedClasses);
				package.Process();
				if (!package.Save(sdkPath))
				{
					excludePackage.insert(packageObj);
				}
			}
		}
	}

	//remove excluded (empty) packages
	for (auto&& package : excludePackage)
	{
		packageOrder.erase(std::remove(std::begin(packageOrder), std::end(packageOrder), package), std::end(packageOrder));
	}

	SaveSDKHeader(path, definedClasses, packageOrder);
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	if (!ObjectsStore::Initialize())
	{
		MessageBoxA(0, "ObjectsStore::Initialize failed", "Error", 0);
		return -1;
	}
	if (!NamesStore::Initialize())
	{
		MessageBoxA(0, "NamesStore::Initialize failed", "Error", 0);
		return -1;
	}

	if (!generator->Initialize(lpParameter))
	{
		MessageBoxA(0, "Initialize failed", "Error", 0);
		return -1;
	}

	fs::path outputDirectory(generator->GetOutputDirectory());
	if (!outputDirectory.is_absolute())
	{
		char buffer[2048];
		if (GetModuleFileNameA((HMODULE)lpParameter, buffer, sizeof(buffer)) == 0)
		{
			MessageBoxA(0, "GetModuleFileName failed", "Error", 0);
			return -1;
		}

		outputDirectory = fs::path(buffer).remove_filename() / outputDirectory;
	}

	outputDirectory /= generator->GetGameNameShort();
	fs::create_directories(outputDirectory);
	
	std::ofstream log(outputDirectory / "Generator.log");
	Logger::SetStream(&log);

	if (generator->ShouldDumpArrays())
	{
		Dump(outputDirectory);
	}

	fs::create_directories(outputDirectory);

	auto begin = std::chrono::system_clock::now();

	ProcessPackages(outputDirectory);

	Logger::Log("Finished, took %d seconds.", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - begin).count());

	Logger::SetStream(nullptr);

	MessageBoxA(0, "Finished!", "Info", 0);

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);

		return TRUE;
	}

	return FALSE;
}
