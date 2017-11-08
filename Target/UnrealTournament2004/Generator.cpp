#include "IGenerator.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"

class Generator : public IGenerator
{
public:
	virtual bool Initialize(void* module) override
	{
		virtualFunctionPattern["Class Core.Object"] = {
			{ "\xA1\x00\x00\x00\x00\x85\xC0\x53\x56\x57\x8B", "x????xxxxxx", 0x200, R"(	inline void ProcessEvent(class UFunction* function, void* parms)
	{
		return GetVFunction<void(__thiscall *)(UObject*, class UFunction*, void*, void*)>(this, %d)(this, function, parms, nullptr);
	})" }
		};

		predefinedStaticMembers["Class Core.Object"] = {
			{ "TArray<UObject*>*", "GObjects" }
		};
		predefinedMembers["Class Core.Field"] = {
			{ "class UField*", "SuperField" },
			{ "class UField*", "Next" },
			{ "class UField*", "HashNext" }

		};
		predefinedMembers["Class Core.Struct"] = {
			{ "unsigned char", "UnknownData00[0x08]" },
			{ "class UField*", "Children" },
			{ "unsigned long", "PropertySize" },
			{ "unsigned char", "UnknownData01[0x3C]" }
		};
		predefinedMembers["Class Core.Function"] = {
			{ "uint32_t", "FunctionFlags" },
			{ "uint16_t", "iNative" },
			{ "uint16_t", "RepOffset" },
			{ "uint8_t", "OperPrecedence" },
			{ "uint8_t", "NumParms" },
			{ "uint16_t", "ParmsSize" },
			{ "uint16_t", "ReturnValueOffset" },
			{ "char", "UnknownData00[0x02]"},
			{ "void*", "Func" }
		};

		predefinedMethods["Struct Core.Object.Color"] = {
			PredefinedMethod::Inline(R"(	FColor()
		: R(0), G(0), B(0), A(0)
	{ })"),
			PredefinedMethod::Inline(R"(	FColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		: R(r),
		  G(g),
		  B(b),
		  A(a)
	{ })")
		};

		predefinedMethods["Class Core.Object"] = {
			PredefinedMethod::Default("static TArray<UObject*>& GetGlobalObjects()", R"(TArray<UObject*>& UObject::GetGlobalObjects()
{
	return *GObjects;
})"),
			PredefinedMethod::Inline(R"(	inline std::string UObject::GetName() const
	{
		return Name.GetName();
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
			PredefinedMethod::Inline(R"(	template<typename T>
	static T* GetObjectCasted(std::size_t index)
	{
		return static_cast<T*>(GetGlobalObjects().GetByIndex(index));
	})"),
			PredefinedMethod::Default("bool IsA(UClass* cmp) const", R"(bool UObject::IsA(UClass* cmp) const
{
	for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
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
	T* CreateDefaultObject()
	{
		return static_cast<T*>(CreateDefaultObject());
	})"),
			PredefinedMethod::Inline(R"(	UObject* CreateDefaultObject()
	{
		using Fn = UObject*(__thiscall *)(UClass*);
		//UClass::GetDefaultObject can be found with the sig
		//56 8B F1 8B 06 57 8B BE ?? ?? ?? ?? FF
		static Fn fn = nullptr;
		
		return fn(this);
	})")
		};

		return true;
	}

	virtual std::string GetGameName() const override
	{
		return "Unreal Tournament 2004";
	}
	virtual std::string GetGameNameShort() const override
	{
		return "UT2004";
	}
	virtual std::string GetGameVersion() const override
	{
		return "3369";
	}

	virtual std::string GetNamespaceName() const override
	{
		return "Classes";
	}

	virtual std::vector<std::string> GetIncludes() const override
	{
		return { };
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

struct FNameEntry
{
	char UnknownData00[0xC];
	wchar_t Data[0x10];
	
	std::string GetName()
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct FName
{
	int32_t Index;

	FName()
		: Index(0)
	{
	};

	FName(int32_t i)
		: Index(i)
	{
	};

	FName(const char* nameToFind)
		: Index(0)
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

struct FPointer
{
	int Dummy;
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
