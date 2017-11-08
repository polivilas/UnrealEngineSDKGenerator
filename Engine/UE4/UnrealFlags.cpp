#include "UnrealFlags.hpp"

#include <vector>
#include <sstream>
#include <iterator>

std::string StringifyFlags(const UEPropertyFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEPropertyFlags::Edit) { buffer.push_back("Edit"); }
	if (flags & UEPropertyFlags::ConstParm) { buffer.push_back("ConstParm"); }
	if (flags & UEPropertyFlags::BlueprintVisible) { buffer.push_back("BlueprintVisible"); }
	if (flags & UEPropertyFlags::ExportObject) { buffer.push_back("ExportObject"); }
	if (flags & UEPropertyFlags::BlueprintReadOnly) { buffer.push_back("BlueprintReadOnly"); }
	if (flags & UEPropertyFlags::Net) { buffer.push_back("Net"); }
	if (flags & UEPropertyFlags::EditFixedSize) { buffer.push_back("EditFixedSize"); }
	if (flags & UEPropertyFlags::Parm) { buffer.push_back("Parm"); }
	if (flags & UEPropertyFlags::OutParm) { buffer.push_back("OutParm"); }
	if (flags & UEPropertyFlags::ZeroConstructor) { buffer.push_back("ZeroConstructor"); }
	if (flags & UEPropertyFlags::ReturnParm) { buffer.push_back("ReturnParm"); }
	if (flags & UEPropertyFlags::DisableEditOnTemplate) { buffer.push_back("DisableEditOnTemplate"); }
	if (flags & UEPropertyFlags::Transient) { buffer.push_back("Transient"); }
	if (flags & UEPropertyFlags::Config) { buffer.push_back("Config"); }
	if (flags & UEPropertyFlags::DisableEditOnInstance) { buffer.push_back("DisableEditOnInstance"); }
	if (flags & UEPropertyFlags::EditConst) { buffer.push_back("EditConst"); }
	if (flags & UEPropertyFlags::GlobalConfig) { buffer.push_back("GlobalConfig"); }
	if (flags & UEPropertyFlags::InstancedReference) { buffer.push_back("InstancedReference"); }
	if (flags & UEPropertyFlags::DuplicateTransient) { buffer.push_back("DuplicateTransient"); }
	if (flags & UEPropertyFlags::SubobjectReference) { buffer.push_back("SubobjectReference"); }
	if (flags & UEPropertyFlags::SaveGame) { buffer.push_back("SaveGame"); }
	if (flags & UEPropertyFlags::NoClear) { buffer.push_back("NoClear"); }
	if (flags & UEPropertyFlags::ReferenceParm) { buffer.push_back("ReferenceParm"); }
	if (flags & UEPropertyFlags::BlueprintAssignable) { buffer.push_back("BlueprintAssignable"); }
	if (flags & UEPropertyFlags::Deprecated) { buffer.push_back("Deprecated"); }
	if (flags & UEPropertyFlags::IsPlainOldData) { buffer.push_back("IsPlainOldData"); }
	if (flags & UEPropertyFlags::RepSkip) { buffer.push_back("RepSkip"); }
	if (flags & UEPropertyFlags::RepNotify) { buffer.push_back("RepNotify"); }
	if (flags & UEPropertyFlags::Interp) { buffer.push_back("Interp"); }
	if (flags & UEPropertyFlags::NonTransactional) { buffer.push_back("NonTransactional"); }
	if (flags & UEPropertyFlags::EditorOnly) { buffer.push_back("EditorOnly"); }
	if (flags & UEPropertyFlags::NoDestructor) { buffer.push_back("NoDestructor"); }
	if (flags & UEPropertyFlags::AutoWeak) { buffer.push_back("AutoWeak"); }
	if (flags & UEPropertyFlags::ContainsInstancedReference) { buffer.push_back("ContainsInstancedReference"); }
	if (flags & UEPropertyFlags::AssetRegistrySearchable) { buffer.push_back("AssetRegistrySearchable"); }
	if (flags & UEPropertyFlags::SimpleDisplay) { buffer.push_back("SimpleDisplay"); }
	if (flags & UEPropertyFlags::AdvancedDisplay) { buffer.push_back("AdvancedDisplay"); }
	if (flags & UEPropertyFlags::Protected) { buffer.push_back("Protected"); }
	if (flags & UEPropertyFlags::BlueprintCallable) { buffer.push_back("BlueprintCallable"); }
	if (flags & UEPropertyFlags::BlueprintAuthorityOnly) { buffer.push_back("BlueprintAuthorityOnly"); }
	if (flags & UEPropertyFlags::TextExportTransient) { buffer.push_back("TextExportTransient"); }
	if (flags & UEPropertyFlags::NonPIEDuplicateTransient) { buffer.push_back("NonPIEDuplicateTransient"); }
	if (flags & UEPropertyFlags::ExposeOnSpawn) { buffer.push_back("ExposeOnSpawn"); }
	if (flags & UEPropertyFlags::PersistentInstance) { buffer.push_back("PersistentInstance"); }
	if (flags & UEPropertyFlags::UObjectWrapper) { buffer.push_back("UObjectWrapper"); }
	if (flags & UEPropertyFlags::HasGetValueTypeHash) { buffer.push_back("HasGetValueTypeHash"); }
	if (flags & UEPropertyFlags::NativeAccessSpecifierPublic) { buffer.push_back("NativeAccessSpecifierPublic"); }
	if (flags & UEPropertyFlags::NativeAccessSpecifierProtected) { buffer.push_back("NativeAccessSpecifierProtected"); }
	if (flags & UEPropertyFlags::NativeAccessSpecifierPrivate) { buffer.push_back("NativeAccessSpecifierPrivate"); }

	switch (buffer.size())
	{
		case 0:
			return std::string();
		case 1:
			return std::string(buffer[0]);
		default:
			std::ostringstream os;
			std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
			os << *buffer.rbegin();
			return os.str();
	}
}

std::string StringifyFlags(const UEFunctionFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEFunctionFlags::Final) { buffer.push_back("Final"); }
	if (flags & UEFunctionFlags::RequiredAPI) { buffer.push_back("RequiredAPI"); }
	if (flags & UEFunctionFlags::BlueprintAuthorityOnly) { buffer.push_back("BlueprintAuthorityOnly"); }
	if (flags & UEFunctionFlags::BlueprintCosmetic) { buffer.push_back("BlueprintCosmetic"); }
	if (flags & UEFunctionFlags::Net) { buffer.push_back("Net"); }
	if (flags & UEFunctionFlags::NetReliable) { buffer.push_back("NetReliable"); }
	if (flags & UEFunctionFlags::NetRequest) { buffer.push_back("NetRequest"); }
	if (flags & UEFunctionFlags::Exec) { buffer.push_back("Exec"); }
	if (flags & UEFunctionFlags::Native) { buffer.push_back("Native"); }
	if (flags & UEFunctionFlags::Event) { buffer.push_back("Event"); }
	if (flags & UEFunctionFlags::NetResponse) { buffer.push_back("NetResponse"); }
	if (flags & UEFunctionFlags::Static) { buffer.push_back("Static"); }
	if (flags & UEFunctionFlags::NetMulticast) { buffer.push_back("NetMulticast"); }
	if (flags & UEFunctionFlags::MulticastDelegate) { buffer.push_back("MulticastDelegate"); }
	if (flags & UEFunctionFlags::Public) { buffer.push_back("Public"); }
	if (flags & UEFunctionFlags::Private) { buffer.push_back("Private"); }
	if (flags & UEFunctionFlags::Protected) { buffer.push_back("Protected"); }
	if (flags & UEFunctionFlags::Delegate) { buffer.push_back("Delegate"); }
	if (flags & UEFunctionFlags::NetServer) { buffer.push_back("NetServer"); }
	if (flags & UEFunctionFlags::HasOutParms) { buffer.push_back("HasOutParms"); }
	if (flags & UEFunctionFlags::HasDefaults) { buffer.push_back("HasDefaults"); }
	if (flags & UEFunctionFlags::NetClient) { buffer.push_back("NetClient"); }
	if (flags & UEFunctionFlags::DLLImport) { buffer.push_back("DLLImport"); }
	if (flags & UEFunctionFlags::BlueprintCallable) { buffer.push_back("BlueprintCallable"); }
	if (flags & UEFunctionFlags::BlueprintEvent) { buffer.push_back("BlueprintEvent"); }
	if (flags & UEFunctionFlags::BlueprintPure) { buffer.push_back("BlueprintPure"); }
	if (flags & UEFunctionFlags::Const) { buffer.push_back("Const"); }
	if (flags & UEFunctionFlags::NetValidate) { buffer.push_back("NetValidate"); }

	switch (buffer.size())
	{
	case 0:
		return std::string();
	case 1:
		return std::string(buffer[0]);
	default:
		std::ostringstream os;
		std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
		os << *buffer.rbegin();
		return os.str();
	}
}