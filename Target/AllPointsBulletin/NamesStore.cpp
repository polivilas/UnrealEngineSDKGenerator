#include <windows.h>

#include "PatternFinder.hpp"
#include "NamesStore.hpp"

#include "EngineClasses.hpp"

class FNameEntry
{
public:
	uint32_t Flags;
	char pad_0x0004[0xC];
	union
	{
		char Name[1024];
		char* NamePtr;
	};

	const char* GetName() const
	{
		return Flags & 0x4000 ? NamePtr : Name;
	}
};

TArray<FNameEntry*>* GlobalNames = nullptr;

bool NamesStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x83\x3C\x81\x00"), "xx????xxxx");
	if (address == -1)
	{
		return false;
	}

	GlobalNames = reinterpret_cast<decltype(GlobalNames)>(*reinterpret_cast<uint32_t*>(address + 2));

	return true;
}

void* NamesStore::GetAddress()
{
	return GlobalNames;
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
