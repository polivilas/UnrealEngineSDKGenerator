#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

class FUObjectItem
{
public:
	UObject* Object; //0x0000
	__int32 Flags; //0x0008
	__int32 ClusterIndex; //0x000C
	__int32 SerialNumber; //0x0010
};

class TUObjectArray
{
public:
	FUObjectItem* Objects;
	int32_t MaxElements;
	int32_t NumElements;
};

class FUObjectArray
{
public:
	__int32 ObjFirstGCIndex; //0x0000
	__int32 ObjLastNonGCIndex; //0x0004
	__int32 MaxObjectsNotConsideredByGC; //0x0008
	__int32 OpenForDisregardForGC; //0x000C

	TUObjectArray ObjObjects; //0x0010
};

FUObjectArray* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	const auto address = FindPattern(GetModuleHandleW(L"FortniteClient-Win64-Shipping.exe"), reinterpret_cast<const unsigned char*>("\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x33\xC9\x84\xD2\x41\x8B\x40\x08\x49\x89\x48\x10\x0F\x45\x05\x00\x00\x00\x00\xFF\xC0\x49\x89\x48\x10\x41\x89\x40\x08"), "xxx????xxxxxxxxxxxxxxxxxx????xxxxxxxxxx");
	if (address == -1)
	{
		return false;
	}

	const auto offset = *reinterpret_cast<uint32_t*>(address + 3);
	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(address + 7 + offset);

	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.NumElements;
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects.Objects[id].Object;
}
