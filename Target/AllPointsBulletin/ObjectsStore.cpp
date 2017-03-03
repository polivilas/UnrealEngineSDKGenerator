#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

TArray<UObject*>* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\xA1\x00\x00\x00\x00\x8B\x34\xB0\x85\xF6"), "x????xxxxx");
	if (address == -1)
	{
		return false;
	}

	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(*reinterpret_cast<uint32_t*>(address + 1));

	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->Num();
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return (*GlobalObjects)[id];
}
