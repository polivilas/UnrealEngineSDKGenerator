#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

TArray<UObject*>* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(L"core.dll"), reinterpret_cast<const unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x8B\x04\x81\xC3\x33\xC0"), "xx????xxxxxx");
	if (address == -1)
	{
		return false;
	}

	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(*reinterpret_cast<uint32_t*>(address + 2));

	return true;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->Num();
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return (*GlobalObjects)[id];
}
