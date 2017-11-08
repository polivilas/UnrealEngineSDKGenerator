#pragma once

#include <type_traits>
#include <string>

enum class UEFunctionFlags : uint32_t
{
	Final = 0x00000001,
	Defined = 0x00000002,
	Iterator = 0x00000004,
	Latent = 0x00000008,
	PreOperator = 0x00000010,
	Singular = 0x00000020,
	Net = 0x00000040,
	NetReliable = 0x00000080,
	Simulated = 0x00000100,
	Exec = 0x00000200,
	Native = 0x00000400,
	Event = 0x00000800,
	Operator = 0x00001000,
	Static = 0x00002000,
	HasOptionalParms = 0x00004000,
	Const = 0x00008000,
	Public = 0x00020000,
	Private = 0x00040000,
	Protected = 0x00080000,
	Delegate = 0x00100000,
	NetServer = 0x00200000,
	HasOutParms = 0x00400000,
	HasDefaults = 0x00800000,
	NetClient = 0x01000000,
	DLLImport = 0x02000000,
	K2Call = 0x04000000,
	K2Override = 0x08000000,
	K2Pure = 0x10000000
};

inline bool operator&(UEFunctionFlags lhs, UEFunctionFlags rhs)
{
	return (static_cast<std::underlying_type_t<UEFunctionFlags>>(lhs) & static_cast<std::underlying_type_t<UEFunctionFlags>>(rhs)) == static_cast<std::underlying_type_t<UEFunctionFlags>>(rhs);
}

std::string StringifyFlags(const UEFunctionFlags flags);
