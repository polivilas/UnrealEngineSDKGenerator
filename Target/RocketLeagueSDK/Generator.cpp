#include "IGenerator.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"

class Generator : public IGenerator
{
public:

	virtual bool Initialize(void* module) override
	{
		overrideTypes = {};

		predefinedStaticMembers["Class Core.Object"] = {
			{ "TArray<UObject*>*", "GObjects" }
		};

		predefinedMembers["Class Core.Field"] = {
			{ "class UField*", "Next" }
		};
		predefinedMembers["Class Core.Struct"] = {
			{ "unsigned char", "UnknownData00[0x08]" },
			{ "class UField*", "SuperField" },
			{ "class UField*", "Children" },
			{ "unsigned long", "PropertySize" },
			{ "unsigned char", "UnknownData01[0x30]" }
		};
		predefinedMembers["Class Core.Function"] = {
			{ "unsigned long", "FunctionFlags" },
			{ "unsigned long", "iNative" },
			{ "unsinged long", "RepOffset" },
			{ "FName", "FriendlyName" },
			{ "unsigned short", "NumParms" },
			{ "unsigned short", "ParmsSize" },
			{ "unsigned long", "ReturnValueOffset" },
			{ "unsigned char", "UnknownData00[0x4"},
			{ "void*", "Func" }
		};
		predefinedMembers["Class Core.Class"] = {
			{ "char", "UnknownData00[0x88]" },
			{ "class UObject*", "ClassDefaultObject" },
			{ "char", "UnknownData01[0x70]" }
		};

		predefinedMethods["ScriptStruct Core.Object.Color"] = {
			PredefinedMethod::Inline(R"(	inline FColor()
		: R(0), G(0), B(0), A(0)
	{ })"),
			PredefinedMethod::Inline(R"(	inline FColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: R(r),
		  G(g),
		  B(b),
		  A(a)
	{ })")
		};

		virtualFunctionPattern["Class Core.Object"] = {
			{ "\x74\x00\x83\xC0\x07\x83\xE0\xF8\xE8\x00\x00\x00\x00\x8B\xC4", "x?xxxxxxx????xx", R"(	inline void ProcessEvent(class UFunction* function, void* parms)
	{
		return GetVFunction<void(__thiscall *)(UObject*, class UFunction*, void*)>(this, %d)(this, function, parms);
	})" }
		};
		predefinedMethods["Class Core.Object"] = {
			PredefinedMethod::Inline(R"(	static inline TArray<UObject*>& GetGlobalObjects()
	{
		return *GObjects;
	})"),
			PredefinedMethod::Inline(R"(	inline std::string UObject::GetName() const
	{
		auto name = Name.GetName();
		if (Name.Number > 0)
		{
			name += '_' + std::to_string(Name.Number);
		}
		return name;
	})"),
			PredefinedMethod::Default("std::string GetFullName() const", R"(std::string UObject::GetFullName() const
{
	std::string name;

	if (Class != nullptr)
	{
		std::string temp;
		for (auto p = Outer; p; p = p->Outer)
		{
			temp = p->GetName() + "." + temp;
		}

		name = Class->GetName();
		name += " ";
		name += temp;
		name += GetName();
	}

	return name;
})"),
			PredefinedMethod::Inline(R"(	template<typename T>
	static T* FindObject(const std::string& name)
	{
		for (auto i = 0u; i < GetGlobalObjects().Num(); ++i)
		{
			auto object = GetGlobalObjects().GetByIndex(i);
	
			if (object == nullptr)
			{
				continue;
			}
	
			if (object->GetFullName() == name)
			{
				return static_cast<T*>(object);
			}
		}
		return nullptr;
	})"),
			PredefinedMethod::Inline(R"(	static UClass* FindClass(const std::string& name)
	{
		return FindObject<UClass>(name);
	})"),
			PredefinedMethod::Default("bool IsA(UClass* cmp) const", R"(bool UObject::IsA(UClass* cmp) const
{
	for (auto super = Class; super; super = (UClass*)super->SuperField)
	{
		if (super == cmp)
		{
			return true;
		}
	}

	return false;
})")
		};
		predefinedMethods["Class Core.Class"] = {
			PredefinedMethod::Inline(R"(	template<typename T>
	inline T* CreateDefaultObject()
	{
		return static_cast<T*>(CreateDefaultObject());
	})"),
			PredefinedMethod::Inline(R"(	inline UObject* CreateDefaultObject()
	{
		using Fn = UObject*(__thiscall *)(UClass*, unsigned int);

		//UClass::GetDefaultObject can be found with the sig
		//74 ?? F7 86 ?? 00 00 00 00 00 00 10 75 ?? F6 05 ?? ?? ?? ?? 02 0F 84
		static Fn fn = nullptr;
		
		return fn(this, 0);

		//or use the default object (WARNING: may be null)
		//return ClassDefaultObject;
	})")
		};

		return true;
	}

	virtual std::string GetGameName() const override
	{
		return "Rocket League";
	}
	virtual std::string GetGameNameShort() const override
	{
		return "RL";
	}
	virtual std::string GetGameVersion() const override
	{
		return "1.1 - 2/12/2017";
	}

	virtual std::string GetNamespaceName() const override
	{
		return "SDK";
	}

	virtual std::vector<std::string> GetIncludes() const override
	{
		return {

		};
	}

	virtual std::string GetBasicDeclarations() const override
	{
		return R"(template<typename Fn>
inline Fn GetVFunction(const void *instance, std::size_t index)
{
	auto vtable = *reinterpret_cast<const void***>(const_cast<void*>(instance));
	return reinterpret_cast<Fn>(vtable[index]);
}

template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline size_t Num() const
	{
		return Count;
	};

	inline T& operator[](size_t i)
	{
		return Data[i];
	};

	inline const T& operator[](size_t i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(size_t i) const
	{
		return i < Num();
	}

	inline T& GetByIndex(size_t i)
	{
		return Data[i];
	}

	inline const T& GetByIndex(size_t i) const
	{
		return Data[i];
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	}

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

class FNameEntry
{
	static const auto NAME_WIDE_MASK = 0x1;
	static const auto NAME_INDEX_SHIFT = 1;

public:
	uint64_t Flags;
	uint32_t Index;
	FNameEntry* HashNext;
	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};

	inline const int32_t GetIndex() const
	{
		return Index >> NAME_INDEX_SHIFT;
	}

	inline bool IsWide() const
	{
		return Index & NAME_WIDE_MASK;
	}

	inline const char* GetAnsiName() const
	{
		return AnsiName;
	}

	inline const wchar_t* GetWideName() const
	{
		return WideName;
	}

	std::string GetName() const
	{
		if (IsWide())
		{
			auto length = std::wcslen(WideName);

			std::string str(length, '\0');

			std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(WideName, WideName + length, '?', &str[0]);

			return str;
		}
		else
		{
			return AnsiName;
		}
	}
};

struct FName
{
	int32_t Index;
	int32_t Number;

	inline FName()
		: Index(0),
		  Number(0)
	{
	};

	inline FName(int32_t i)
		: Index(i),
		  Number(0)
	{
	};

	FName(const char* nameToFind)
		: Index(0),
		  Number(0)
	{
		static std::set<size_t> cache;

		for (auto i : cache)
		{
			if (GetGlobalNames()[i]->GetName() == nameToFind)
			{
				Index = i;
				
				return;
			}
		}

		for (auto i = 0u; i < GetGlobalNames().Num(); ++i)
		{
			if (GetGlobalNames()[i] != nullptr)
			{
				if (GetGlobalNames()[i]->GetName() == nameToFind)
				{
					cache.insert(i);

					Index = i;

					return;
				}
			}
		}
	};

	static TArray<FNameEntry*>* GNames;
	static inline TArray<FNameEntry*>& GetGlobalNames()
	{
		return *GNames;
	};

	inline std::string GetName() const
	{
		return GetGlobalNames()[Index]->GetName();
	};

	inline bool operator==(const FName& other) const
	{
		return Index == other.Index;
	};
};

class UObject;

class FScriptInterface
{
private:
	UObject* ObjectPointer;
	void* InterfacePointer;

public:
	inline UObject* GetObject() const
	{
		return ObjectPointer;
	}

	inline UObject*& GetObjectRef()
	{
		return ObjectPointer;
	}

	inline void* GetInterface() const
	{
		return ObjectPointer != nullptr ? InterfacePointer : nullptr;
	}
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	inline InterfaceType* operator->() const
	{
		return (InterfaceType*)GetInterface();
	}

	inline InterfaceType& operator*() const
	{
		return *((InterfaceType*)GetInterface());
	}

	inline operator bool() const
	{
		return GetInterface() != nullptr;
	}
};

struct FScriptDelegate
{
	unsigned char UnknownData[0x0C];
};)";
	}

	virtual std::string GetBasicDefinitions() const override
	{
		return R"(TArray<FNameEntry*>* FName::GNames = nullptr;
 TArray<UObject*>* UObject::GObjects = nullptr;)";
	}
};

Generator _generator;
IGenerator* generator = &_generator;
