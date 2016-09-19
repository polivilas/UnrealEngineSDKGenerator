#include "../Package.hpp"

bool Package::Method::Parameter::MakeType(UEPropertyFlags flags, Type& type)
{
	if (flags & UEPropertyFlags::CPF_ReturnParm)
	{
		type = Type::Return;
	}
	else if (flags & UEPropertyFlags::CPF_OutParm)
	{
		//if it is a const parameter make it a default parameter
		if (flags & UEPropertyFlags::CPF_ConstParm)
		{
			type = Type::Default;
		}
		else
		{
			type = Type::Out;
		}
	}
	else if (flags & UEPropertyFlags::CPF_Parm)
	{
		type = Type::Default;
	}
	else
	{
		return false;
	}

	return true;
}