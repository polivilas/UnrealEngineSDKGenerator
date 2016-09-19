#pragma once

#include <set>
#include <string>
#include <windows.h>

struct FPointer
{
	uintptr_t Dummy;
};

struct FQWord
{
	int A;
	int B;
};

struct FName
{
	int32_t Index;
	int32_t Number;
};

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

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : public TArray<wchar_t>
{
	std::string ToString() const
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, Data, Count - 1, nullptr, 0, nullptr, nullptr);
		std::string str(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, Data, Count - 1, &str[0], size, nullptr, nullptr);
		return str;
	}
};

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
};

class UClass;

class UObject
{
public:
	FPointer		VTableObject;
	char			UnknownData00[0x1C];
	uint32_t		InternalIndex;
	char			UnknownData01[0x04];
	UObject*		Outer;
	FName			Name;
	UObject*		Class;
	UObject*		ObjectArchetype;
};

class UField : public UObject
{
public:
	UField*			Next;
};

class UEnum : public UField
{
public:
	TArray<FName>	Names;
};

class UConst : public UField
{
public:
	FString			Value;
};

class UStruct : public UField
{
public:
	char			UnknownData00[0x08];
	UField*			SuperField;
	UField*			Children;
	uint32_t		PropertySize;
	char			UnknownData01[0x30];
};

class UScriptStruct : public UStruct
{
public:
	char			UnknownData00[0x1C];
};

class UFunction : public UStruct
{
public:
	uint32_t		FunctionFlags;
	uint16_t		iNative;
	uint16_t		RepOffset;
	FName			FriendlyName;
	uint8_t			OperPrecendence;
	uint8_t			NumParms;
	uint16_t		ParmsSize;
	uint16_t		ReturnValueOffset;
	void*			Func;
};

class UState : public UStruct
{
public:
	char			UnknownData00[0x48];
};

class UClass : public UStruct
{
public:
	char			UnknownData00[0x88];
	UObject*		ClassDefaultObject;
	char			UnknownData01[0x70];
};

class UProperty : public UField
{
public:
	uint32_t		ArrayDim;
	uint32_t		ElementSize;
	FQWord			PropertyFlags;
	uint32_t		PropertySize;
	char			UnknownData00[0x0C];
	uint32_t		Offset;
	char			UnknownData01[0x0C];
};

class UByteProperty : public UProperty
{
public:
	UEnum*			Enum;
};

class UIntProperty : public UProperty
{
public:

};

class UFloatProperty : public UProperty
{
public:

};

class UDoubleProperty : public UProperty
{
public:

};

class UBoolProperty : public UProperty
{
public:
	unsigned long	BitMask;
};

class UObjectProperty : public UProperty
{
public:
	UClass*			PropertyClass;
};

class UComponentProperty : public UObjectProperty
{
public:

};

class UClassProperty : public UObjectProperty
{
public:
	UClass*			MetaClass;
};

class UInterfaceProperty : public UProperty
{
public:
	UClass*			InterfaceClass;
};

class UNameProperty : public UProperty
{
public:

};

class UStructProperty : public UProperty
{
public:
	UStruct*		Struct;
};

class UStrProperty : public UProperty
{
public:

};

class UArrayProperty : public UProperty
{
public:
	UProperty*		Inner;
};

class UMapProperty : public UProperty
{
public:
	UProperty*		KeyProp;
	UProperty*		ValueProp;
};

class UDelegateProperty : public UProperty
{
public:
	UFunction*		SignatureFunction;
};
