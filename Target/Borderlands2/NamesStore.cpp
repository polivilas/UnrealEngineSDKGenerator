#include <windows.h>

#include "PatternFinder.hpp"
#include "NamesStore.hpp"

#include "EngineClasses.hpp"

class FNameEntry
{
public:
	char UnknownData00[0x10];
	char Name[1024];

	std::string GetName() const
	{
		return Name;
	}
};

TArray<FNameEntry*>* GlobalNames = nullptr;

bool NamesStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x83\x3C\x81\x00\x74"), "xx????xxxxx");
	if (address == -1)
	{
		return false;
	}

	GlobalNames = reinterpret_cast<decltype(GlobalNames)>(*reinterpret_cast<uint32_t*>(address + 2));

	/*char bla[100];
	sprintf(bla, "Names: %X", GlobalNames);
	MessageBoxA(0, bla, bla, 0);*/

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
