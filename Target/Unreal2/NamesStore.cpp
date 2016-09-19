#include <windows.h>

#include "PatternFinder.hpp"
#include "NamesStore.hpp"

#include "EngineClasses.hpp"

struct FNameEntry
{
	char UnknownData00[0xC];
	wchar_t Data[0x10];
	
	std::string GetName()
	{
		auto length = std::wcslen(Data);
		auto neededLength = WideCharToMultiByte(CP_UTF8, 0, Data, length, nullptr, 0, nullptr, nullptr);
		std::string str(neededLength, 0);
		WideCharToMultiByte(CP_UTF8, 0, Data, length, &str[0], neededLength, nullptr, nullptr);
		return str;
	}
};

TArray<FNameEntry*>* GlobalNames = nullptr;

bool NamesStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(L"core.dll"), (const unsigned char*)"\xA1\x00\x00\x00\x00\x8B\x88", "x????xx");
	if (address == -1)
	{
		return false;
	}

	GlobalNames = (decltype(GlobalNames))*reinterpret_cast<uint32_t*>(address + 1);

	return true;
}

size_t NamesStore::GetNamesNum() const
{
	return GlobalNames->Num();
}

bool NamesStore::IsValid(size_t id) const
{
	return GlobalNames->IsValidIndex(id) && (*GlobalNames)[id] != nullptr;
}

std::string NamesStore::GetById(size_t id) const
{
	return (*GlobalNames)[id]->GetName();
}
