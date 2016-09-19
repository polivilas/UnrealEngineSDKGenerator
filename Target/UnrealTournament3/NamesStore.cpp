#include <windows.h>

#include "PatternFinder.hpp"
#include "NamesStore.hpp"

#include "EngineClasses.hpp"

class FNameEntry
{
public:
	uint32_t Index;
	char UnknownData00[0x0C];
	wchar_t WideName[1024];

	inline const int32_t GetIndex() const
	{
		return Index;
	}

	inline const wchar_t* GetWideName() const
	{
		return WideName;
	}

	std::string GetName() const
	{
		auto length = std::wcslen(WideName);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(WideName, WideName + length, '?', &str[0]);

		return str;
	}
};

TArray<FNameEntry*>* GlobalNames = nullptr;

bool NamesStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), (const unsigned char*)"\x8B\x0D\x00\x00\x00\x00\x83\x3C\x81\x00\x74", "xx????xxxxx");
	if (address == -1)
	{
		return false;
	}

	GlobalNames = (decltype(GlobalNames))*reinterpret_cast<uint32_t*>(address + 2);

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
