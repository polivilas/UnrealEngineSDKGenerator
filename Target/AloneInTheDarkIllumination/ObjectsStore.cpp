#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

class FUObjectArray
{
public:
	__int32 ObjFirstGCIndex;
	__int32 ObjLastNonGCIndex;
	__int32 OpenForDisregardForGC;

	TArray<UObject*> ObjObjects;
};

FUObjectArray* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x48\x8D\x15\x00\x00\x00\x00\x41\x8B\xF9"), "xxx????xxx");
	if (address == -1)
	{
		return false;
	}
	auto offset = *reinterpret_cast<uint32_t*>(address + 3);
	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(address + 7 + offset);

	return true;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.Num();
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects[id];
}
