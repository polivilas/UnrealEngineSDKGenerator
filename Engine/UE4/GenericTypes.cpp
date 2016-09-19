#include "GenericTypes.hpp"
#include "../NameValidator.hpp"

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
			else if (className == "Object")
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
		if (IsA<UEByteProperty>())
		{
			return Cast<UEByteProperty>().GetInfo();
		}
		if (IsA<UEUInt16Property>())
		{
			return Cast<UEUInt16Property>().GetInfo();
		}
		if (IsA<UEUInt32Property>())
		{
			return Cast<UEUInt32Property>().GetInfo();
		}
		if (IsA<UEUInt64Property>())
		{
			return Cast<UEUInt64Property>().GetInfo();
		}
		if (IsA<UEInt8Property>())
		{
			return Cast<UEInt8Property>().GetInfo();
		}
		if (IsA<UEInt16Property>())
		{
			return Cast<UEInt16Property>().GetInfo();
		}
		if (IsA<UEIntProperty>())
		{
			return Cast<UEIntProperty>().GetInfo();
		}
		if (IsA<UEInt64Property>())
		{
			return Cast<UEInt64Property>().GetInfo();
		}
		if (IsA<UEFloatProperty>())
		{
			return Cast<UEFloatProperty>().GetInfo();
		}
		if (IsA<UEDoubleProperty>())
		{
			return Cast<UEDoubleProperty>().GetInfo();
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
		if (IsA<UEWeakObjectProperty>())
		{
			return Cast<UEWeakObjectProperty>().GetInfo();
		}
		if (IsA<UELazyObjectProperty>())
		{
			return Cast<UELazyObjectProperty>().GetInfo();
		}
		if (IsA<UEAssetObjectProperty>())
		{
			return Cast<UEAssetObjectProperty>().GetInfo();
		}
		if (IsA<UEAssetClassProperty>())
		{
			return Cast<UEAssetClassProperty>().GetInfo();
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
		if (IsA<UETextProperty>())
		{
			return Cast<UETextProperty>().GetInfo();
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
		if (IsA<UEMulticastDelegateProperty>())
		{
			return Cast<UEMulticastDelegateProperty>().GetInfo();
		}
	}
	return { PropertyType::Unknown };
}