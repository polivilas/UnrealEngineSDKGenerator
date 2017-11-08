#include "../Package.hpp"

bool Package::Method::Parameter::MakeType(UEPropertyFlags flags, Type& type)
{
	if (flags & UEPropertyFlags::ReturnParm)
	{
		type = Type::Return;
	}
	else if (flags & UEPropertyFlags::OutParm)
	{
		type = Type::Out;
	}
	else if (flags & UEPropertyFlags::Parm)
	{
		type = Type::Default;
	}
	else
	{
		return false;
	}

	return true;
}