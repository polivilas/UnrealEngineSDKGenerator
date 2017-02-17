#include "IGenerator.hpp"
#include "GenericTypes.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"
#include "NameValidator.hpp"

#include "EngineClasses.hpp"

//---------------------------------------------------------------------------
//UEObject
//---------------------------------------------------------------------------
size_t UEObject::GetIndex() const
{
	return object->ObjectInternalInteger;
}
//---------------------------------------------------------------------------
UEClass UEObject::GetClass() const
{
	return UEClass(object->Class);
}
//---------------------------------------------------------------------------
UEObject UEObject::GetOuter() const
{
	return UEObject(object->Outer);
}
//---------------------------------------------------------------------------
std::string UEObject::GetName() const
{
	auto name = NamesStore().GetById(object->Name.Index);
	if (object->Name.Number > 0)
	{
		name += '_' + std::to_string(object->Name.Number);
	}
	return name;
}
//---------------------------------------------------------------------------
UEClass UEObject::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Object");
	return c;
}
//---------------------------------------------------------------------------
//UEField
//---------------------------------------------------------------------------
UEField UEField::GetNext() const
{
	return UEField(static_cast<UField*>(object)->Next);
}
//---------------------------------------------------------------------------
UEClass UEField::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Field");
	return c;
}
//---------------------------------------------------------------------------
//UEEnum
//---------------------------------------------------------------------------
std::vector<std::string> UEEnum::GetNames() const
{
	std::vector<std::string> buffer;
	auto& names = static_cast<UEnum*>(object)->Names;

	for (auto i = 0u; i < names.Num(); ++i)
	{
		buffer.push_back(NamesStore().GetById(names[i].Index));
	}

	return buffer;
}
//---------------------------------------------------------------------------
UEClass UEEnum::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Enum");
	return c;
}
//---------------------------------------------------------------------------
//UEConst
//---------------------------------------------------------------------------
std::string UEConst::GetValue() const
{
	return static_cast<UConst*>(object)->Value.ToString();
}
//---------------------------------------------------------------------------
UEClass UEConst::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Const");
	return c;
}
//---------------------------------------------------------------------------
//UEStruct
//---------------------------------------------------------------------------
UEStruct UEStruct::GetSuper() const
{
	return UEStruct(static_cast<UStruct*>(object)->SuperField);
}
//---------------------------------------------------------------------------
UEField UEStruct::GetChildren() const
{
	return UEField(static_cast<UStruct*>(object)->Children);
}
//---------------------------------------------------------------------------
size_t UEStruct::GetPropertySize() const
{
	return static_cast<UStruct*>(object)->PropertySize;
}
//---------------------------------------------------------------------------
UEClass UEStruct::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Struct");
	return c;
}
//---------------------------------------------------------------------------
//UEScriptStruct
//---------------------------------------------------------------------------
UEClass UEScriptStruct::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ScriptStruct");
	return c;
}
//---------------------------------------------------------------------------
//UEFunction
//---------------------------------------------------------------------------
UEFunctionFlags UEFunction::GetFunctionFlags() const
{
	return static_cast<UEFunctionFlags>(static_cast<UFunction*>(object)->FunctionFlags);
}
//---------------------------------------------------------------------------
UEClass UEFunction::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Function");
	return c;
}
//---------------------------------------------------------------------------
//UEClass
//---------------------------------------------------------------------------
UEClass UEClass::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Class");
	return c;
}
//---------------------------------------------------------------------------
//UEProperty
//---------------------------------------------------------------------------
size_t UEProperty::GetArrayDim() const
{
	return static_cast<UProperty*>(object)->ArrayDim;
}
//---------------------------------------------------------------------------
size_t UEProperty::GetElementSize() const
{
	return static_cast<UProperty*>(object)->ElementSize;
}
//---------------------------------------------------------------------------
UEPropertyFlags UEProperty::GetPropertyFlags() const
{
	return static_cast<UEPropertyFlags>(static_cast<UProperty*>(object)->PropertyFlags.A);
}
//---------------------------------------------------------------------------
size_t UEProperty::GetOffset() const
{
	return static_cast<UProperty*>(object)->Offset;
}
//---------------------------------------------------------------------------
UEClass UEProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.Property");
	return c;
}
//---------------------------------------------------------------------------
//UEByteProperty
//---------------------------------------------------------------------------
UEEnum UEByteProperty::GetEnum() const
{
	return UEEnum(static_cast<UByteProperty*>(object)->Enum);
}
//---------------------------------------------------------------------------
UEProperty::Info UEByteProperty::GetInfo() const
{
	auto enumObj = GetEnum();
	if (enumObj.IsValid())
	{
		return Info::Create(PropertyType::Primitive, sizeof(uint8_t), false, "TEnumAsByte<enum class " + MakeUniqueCppName(enumObj) + ">");
	}
	return Info::Create(PropertyType::Primitive, sizeof(uint8_t), false, "unsigned char");
}
//---------------------------------------------------------------------------
UEClass UEByteProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ByteProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEIntProperty
//---------------------------------------------------------------------------
UEProperty::Info UEIntProperty::GetInfo() const
{
	return Info::Create(PropertyType::Primitive, sizeof(int), false, "int");
}
//---------------------------------------------------------------------------
UEClass UEIntProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.IntProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEFloatProperty
//---------------------------------------------------------------------------
UEProperty::Info UEFloatProperty::GetInfo() const
{
	return Info::Create(PropertyType::Primitive, sizeof(float), false, "float");
}
//---------------------------------------------------------------------------
UEClass UEFloatProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.FloatProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEBoolProperty
//---------------------------------------------------------------------------
size_t UEBoolProperty::GetBitMask() const
{
	return static_cast<UBoolProperty*>(object)->BitMask;
}
//---------------------------------------------------------------------------
UEProperty::Info UEBoolProperty::GetInfo() const
{
	return Info::Create(PropertyType::Primitive, sizeof(unsigned long), false, "unsigned long");
}
//---------------------------------------------------------------------------
UEClass UEBoolProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.BoolProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEObjectProperty
//---------------------------------------------------------------------------
UEClass UEObjectProperty::GetPropertyClass() const
{
	return static_cast<UObjectProperty*>(object)->PropertyClass;
}
//---------------------------------------------------------------------------
UEProperty::Info UEObjectProperty::GetInfo() const
{
	return Info::Create(PropertyType::Primitive, sizeof(void*), false, "class " + MakeValidName(GetPropertyClass().GetNameCPP()) + "*");
}
//---------------------------------------------------------------------------
UEClass UEObjectProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ObjectProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEComponentProperty
//---------------------------------------------------------------------------
UEProperty::Info UEComponentProperty::GetInfo() const
{
	return UEObjectProperty::GetInfo();
}
//---------------------------------------------------------------------------
UEClass UEComponentProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ComponentProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEClassProperty
//---------------------------------------------------------------------------
UEClass UEClassProperty::GetMetaClass() const
{
	return UEClass(static_cast<UClassProperty*>(object)->MetaClass);
}
//---------------------------------------------------------------------------
UEProperty::Info UEClassProperty::GetInfo() const
{
	return Info::Create(PropertyType::Primitive, sizeof(void*), false, "class " + MakeValidName(GetMetaClass().GetNameCPP()) + "*");
}
//---------------------------------------------------------------------------
UEClass UEClassProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ClassProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEInterfaceProperty
//---------------------------------------------------------------------------
UEClass UEInterfaceProperty::GetInterfaceClass() const
{
	return UEClass(static_cast<UInterfaceProperty*>(object)->InterfaceClass);
}
//---------------------------------------------------------------------------
UEProperty::Info UEInterfaceProperty::GetInfo() const
{
	return Info::Create(PropertyType::PredefinedStruct, sizeof(FScriptInterface), true, "TScriptInterface<class " + MakeValidName(GetInterfaceClass().GetNameCPP()) + ">");
}
//---------------------------------------------------------------------------
UEClass UEInterfaceProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.InterfaceProperty");
	return c;
}
//---------------------------------------------------------------------------
//UENameProperty
//---------------------------------------------------------------------------
UEProperty::Info UENameProperty::GetInfo() const
{
	return Info::Create(PropertyType::PredefinedStruct, sizeof(FName), true, "struct FName");
}
//---------------------------------------------------------------------------
UEClass UENameProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.NameProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEStructProperty
//---------------------------------------------------------------------------
UEScriptStruct UEStructProperty::GetStruct() const
{
	return UEScriptStruct(static_cast<UStructProperty*>(object)->Struct);
}
//---------------------------------------------------------------------------
UEProperty::Info UEStructProperty::GetInfo() const
{
	return Info::Create(PropertyType::CustomStruct, GetElementSize(), true, "struct " + MakeUniqueCppName(GetStruct()));
}
//---------------------------------------------------------------------------
UEClass UEStructProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.StructProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEStrProperty
//---------------------------------------------------------------------------
UEProperty::Info UEStrProperty::GetInfo() const
{
	return Info::Create(PropertyType::PredefinedStruct, sizeof(FString), true, "struct FString");
}
//---------------------------------------------------------------------------
UEClass UEStrProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.StrProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEArrayProperty
//---------------------------------------------------------------------------
UEProperty UEArrayProperty::GetInner() const
{
	return UEProperty(static_cast<UArrayProperty*>(object)->Inner);
}
//---------------------------------------------------------------------------
UEProperty::Info UEArrayProperty::GetInfo() const
{
	auto inner = GetInner().GetInfo();
	if (inner.Type != PropertyType::Unknown)
	{
		extern IGenerator* generator;

		return Info::Create(PropertyType::Container, sizeof(TArray<void*>), false, "TArray<" + generator->GetOverrideType(inner.CppType) + ">");
	}
	
	return { PropertyType::Unknown };
}
//---------------------------------------------------------------------------
UEClass UEArrayProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.ArrayProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEMapProperty
//---------------------------------------------------------------------------
UEProperty UEMapProperty::GetKeyProperty() const
{
	return UEProperty(static_cast<UMapProperty*>(object)->KeyProp);
}
//---------------------------------------------------------------------------
UEProperty UEMapProperty::GetValueProperty() const
{
	return UEProperty(static_cast<UMapProperty*>(object)->ValueProp);
}
//---------------------------------------------------------------------------
UEProperty::Info UEMapProperty::GetInfo() const
{
	auto key = GetKeyProperty().GetInfo();
	auto value = GetValueProperty().GetInfo();
	if (key.Type != PropertyType::Unknown && value.Type != PropertyType::Unknown)
	{
		extern IGenerator* generator;

		return Info::Create(PropertyType::Container, 0x3C, false, "TMap<" + generator->GetOverrideType(key.CppType) + ", " + generator->GetOverrideType(value.CppType) + ">");
	}

	return { PropertyType::Unknown };
}
//---------------------------------------------------------------------------
UEClass UEMapProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.MapProperty");
	return c;
}
//---------------------------------------------------------------------------
//UEDelegateProperty
//---------------------------------------------------------------------------
UEFunction UEDelegateProperty::GetSignatureFunction() const
{
	return UEFunction(static_cast<UDelegateProperty*>(object)->SignatureFunction);
}
//---------------------------------------------------------------------------
UEProperty::Info UEDelegateProperty::GetInfo() const
{
	return Info::Create(PropertyType::PredefinedStruct, sizeof(FScriptDelegate), true, "struct FScriptDelegate");
}
//---------------------------------------------------------------------------
UEClass UEDelegateProperty::StaticClass()
{
	static auto c = ObjectsStore().FindClass("Class Core.DelegateProperty");
	return c;
}
//---------------------------------------------------------------------------
