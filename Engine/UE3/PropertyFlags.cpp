#include "PropertyFlags.hpp"

#include <vector>
#include <sstream>
#include <iterator>

std::string StringifyFlags(const UEPropertyFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEPropertyFlags::Edit) { buffer.push_back("Edit"); }
	if (flags & UEPropertyFlags::Const) { buffer.push_back("Const"); }
	if (flags & UEPropertyFlags::Input) { buffer.push_back("Input"); }
	if (flags & UEPropertyFlags::ExportObject) { buffer.push_back("ExportObject"); }
	if (flags & UEPropertyFlags::OptionalParm) { buffer.push_back("OptionalParm"); }
	if (flags & UEPropertyFlags::Net) { buffer.push_back("Net"); }
	if (flags & UEPropertyFlags::EditFixedSize) { buffer.push_back("EditFixedSize"); }
	if (flags & UEPropertyFlags::Parm) { buffer.push_back("Parm"); }
	if (flags & UEPropertyFlags::OutParm) { buffer.push_back("OutParm"); }
	if (flags & UEPropertyFlags::SkipParm) { buffer.push_back("SkipParm"); }
	if (flags & UEPropertyFlags::ReturnParm) { buffer.push_back("ReturnParm"); }
	if (flags & UEPropertyFlags::CoerceParm) { buffer.push_back("CoerceParm"); }
	if (flags & UEPropertyFlags::Native) { buffer.push_back("Native"); }
	if (flags & UEPropertyFlags::Transient) { buffer.push_back("Transient"); }
	if (flags & UEPropertyFlags::Config) { buffer.push_back("Config"); }
	if (flags & UEPropertyFlags::Localized) { buffer.push_back("Localized"); }
	if (flags & UEPropertyFlags::EditConst) { buffer.push_back("EditConst"); }
	if (flags & UEPropertyFlags::GlobalConfig) { buffer.push_back("GlobalConfig"); }
	if (flags & UEPropertyFlags::Component) { buffer.push_back("Component"); }
	if (flags & UEPropertyFlags::AlwaysInit) { buffer.push_back("AlwaysInit"); }
	if (flags & UEPropertyFlags::DuplicateTransient) { buffer.push_back("DuplicateTransient"); }
	if (flags & UEPropertyFlags::NeedCtorLink) { buffer.push_back("NeedCtorLink"); }
	if (flags & UEPropertyFlags::NoExport) { buffer.push_back("NoExport"); }
	if (flags & UEPropertyFlags::NoImport) { buffer.push_back("NoImport"); }
	if (flags & UEPropertyFlags::NoClear) { buffer.push_back("NoClear"); }
	if (flags & UEPropertyFlags::EditInline) { buffer.push_back("EditInline"); }
	if (flags & UEPropertyFlags::EditInlineUse) { buffer.push_back("EditInlineUse"); }
	if (flags & UEPropertyFlags::Deprecated) { buffer.push_back("Deprecated"); }
	if (flags & UEPropertyFlags::DataBinding) { buffer.push_back("DataBinding"); }
	if (flags & UEPropertyFlags::SerializeText) { buffer.push_back("SerializeText"); }
	if (flags & UEPropertyFlags::RepNotify) { buffer.push_back("RepNotify"); }
	if (flags & UEPropertyFlags::Interp) { buffer.push_back("Interp"); }
	if (flags & UEPropertyFlags::NonTransactional) { buffer.push_back("NonTransactional"); }
	if (flags & UEPropertyFlags::EditorOnly) { buffer.push_back("EditorOnly"); }
	if (flags & UEPropertyFlags::NotForConsole) { buffer.push_back("NotForConsole"); }
	if (flags & UEPropertyFlags::RepRetry) { buffer.push_back("RepRetry"); }
	if (flags & UEPropertyFlags::PrivateWrite) { buffer.push_back("PrivateWrite"); }
	if (flags & UEPropertyFlags::ProtectedWrite) { buffer.push_back("ProtectedWrite"); }
	if (flags & UEPropertyFlags::ArchetypeProperty) { buffer.push_back("ArchetypeProperty"); }
	if (flags & UEPropertyFlags::EditHide) { buffer.push_back("EditHide"); }
	if (flags & UEPropertyFlags::EditTextBox) { buffer.push_back("EditTextBox"); }
	if (flags & UEPropertyFlags::CrossLevelPassive) { buffer.push_back("CrossLevelPassive"); }
	if (flags & UEPropertyFlags::CrossLevelActive) { buffer.push_back("CrossLevelActive"); }

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