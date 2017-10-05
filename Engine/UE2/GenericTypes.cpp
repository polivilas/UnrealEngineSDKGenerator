#include "GenericTypes.hpp"

void* UEObject::GetAddress() const
{
	return object;
}

UEObject UEObject::GetPackageObject() const
{
	UEObject package(nullptr);

	for (auto outer = GetOuter(); outer.IsValid(); outer = outer.GetOuter())
	{
		package = outer;
	}

	return package;
}

std::string UEObject::GetFullName() const
{
	if (GetClass().IsValid())
	{
		std::string temp;

		for (auto outer = GetOuter(); outer.IsValid(); outer = outer.GetOuter())
		{
			temp = outer.GetName() + "." + temp;
		}

		std::string name = GetClass().GetName();
		name += " ";
		name += temp;
		name += GetName();

		return name;
	}

	return std::string("(null)");
}

std::string UEObject::GetNameCPP() const
{
	std::string name;

	if (IsA<UEClass>())
	{
		auto c = Cast<UEClass>();
		while (c.IsValid())
		{
			auto className = c.GetName();
			if (className == "Actor")
			{
				name += "A";
				break;
			}
			if (className == "Object")
			{
				name += "U";
				break;
			}

			c = c.GetSuper().Cast<UEClass>();
		}
	}
	else
	{
		name += "F";
	}

	name += GetName();

	return name;
}

UEProperty::Info UEProperty::GetInfo() const
{
	if (IsValid())
	{
		if (IsA<UEPointerProperty>())
		{
			return Cast<UEPointerProperty>().GetInfo();
		}
		if (IsA<UEByteProperty>())
		{
			return Cast<UEByteProperty>().GetInfo();
		}
		if (IsA<UEIntProperty>())
		{
			return Cast<UEIntProperty>().GetInfo();
		}
		if (IsA<UEFloatProperty>())
		{
			return Cast<UEFloatProperty>().GetInfo();
		}
		if (IsA<UEBoolProperty>())
		{
			return Cast<UEBoolProperty>().GetInfo();
		}
		if (IsA<UEObjectProperty>())
		{
			return Cast<UEObjectProperty>().GetInfo();
		}
		if (IsA<UEClassProperty>())
		{
			return Cast<UEClassProperty>().GetInfo();
		}
		if (IsA<UEInterfaceProperty>())
		{
			return Cast<UEInterfaceProperty>().GetInfo();
		}
		if (IsA<UENameProperty>())
		{
			return Cast<UENameProperty>().GetInfo();
		}
		if (IsA<UEStructProperty>())
		{
			return Cast<UEStructProperty>().GetInfo();
		}
		if (IsA<UEStrProperty>())
		{
			return Cast<UEStrProperty>().GetInfo();
		}
		if (IsA<UEArrayProperty>())
		{
			return Cast<UEArrayProperty>().GetInfo();
		}
		if (IsA<UEMapProperty>())
		{
			return Cast<UEMapProperty>().GetInfo();
		}
		if (IsA<UEDelegateProperty>())
		{
			return Cast<UEDelegateProperty>().GetInfo();
		}
	}
	return { PropertyType::Unknown };
}

//---------------------------------------------------------------------------
//UEByteProperty
//---------------------------------------------------------------------------
bool UEByteProperty::IsEnum() const
{
	return GetEnum().IsValid();
}
//---------------------------------------------------------------------------
//UEScriptStruct
//---------------------------------------------------------------------------
UEClass UEScriptStruct::StaticClass()
{
	//Unreal Engine 2 doesn't have the ScriptStruct class
	return nullptr;
}
//---------------------------------------------------------------------------
//UEEnumProperty
//---------------------------------------------------------------------------
UEEnum UEEnumProperty::GetEnum() const
{
	return UEEnum(nullptr);
}
//---------------------------------------------------------------------------
UEClass UEEnumProperty::StaticClass()
{
	//Unreal Engine 2 doesn't have the EnumProperty class
	return nullptr;
}
//---------------------------------------------------------------------------
//UEBoolProperty
//---------------------------------------------------------------------------
int GetBitPosition(uint32_t value)
{
	int i16 = !(value & 0xffff) << 4;
	value >>= i16;

	int i8 = !(value & 0xff) << 3;
	value >>= i8;

	int i4 = !(value & 0xf) << 2;
	value >>= i4;

	int i2 = !(value & 0x3) << 1;
	value >>= i2;

	int i1 = !(value & 0x1);

	int i0 = (value >> i1) & 1 ? 0 : -32;

	return i16 + i8 + i4 + i2 + i1 + i0;
}
//---------------------------------------------------------------------------
std::array<int, 2> UEBoolProperty::GetMissingBitsCount(const UEBoolProperty& other) const
{
	// If there is no previous bitfield member, just calculate the missing bits.
	if (!other.IsValid())
	{
		return { GetBitPosition(GetBitMask()), -1 };
	}

	// If both bitfield member belong to the same int, calculate the bit position difference.
	if (GetOffset() == other.GetOffset())
	{
		return { GetBitPosition(GetBitMask()) - GetBitPosition(other.GetBitMask()) - 1, -1 };
	}

	// If they have different offsets, we need two distances
	// |0000...1000|0010...0000|
	// 1.      ^---^
	// 2.          ^--^

	return { std::numeric_limits<uint32_t>::digits - GetBitPosition(other.GetBitMask()) - 1, GetBitPosition(GetBitMask()) };
}
//---------------------------------------------------------------------------