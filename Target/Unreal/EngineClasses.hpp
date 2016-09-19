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
};

template<class T>
struct TArray
{
	friend struct FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	size_t Num() const
	{
		return Count;
	};

	T& operator[](size_t i)
	{
		return Data[i];
	};

	const T& operator[](size_t i) const
	{
		return Data[i];
	};

	bool IsValidIndex(size_t i) const
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

struct FScriptDelegate
{
	unsigned char UnknownData[0x0C];
};

class UClass;

class UObject
{
public:
	FPointer		VfTableObject;
	int				InternalIndex;
	char			UnknownData00[0x10];
	UObject*		Outer;
	char			UnknownData01[0x4];
	FName			Name;
	UClass*			Class;
};

	class UField : public UObject
	{
	public:
		UField*			SuperField;
		UField*			Next;
		UField*			HashNext;
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
			char			UnknownData00[0x04];
			UField*			Children;
			unsigned long	PropertySize;
			char			UnknownData01[0x2C];
		};

			class UScriptStruct : public UStruct
			{
			public:

			};

			class UFunction : public UStruct
			{
			public:
				unsigned long	FunctionFlags;
				unsigned short	iNative;
				unsigned short	RepOffset;
				FName			FriendlyName;
				unsigned short	NumParms;
				unsigned short	ParmsSize;
				unsigned short	ReturnValueOffset;
				char			UnknownData00[0x04];
				void*			Func;
			};

			class UState : public UStruct
			{
			public:
				char			UnknownData00[0x418];
			};

				class UClass : public UState
				{
				public:
					char			UnknownData00[0x64];
				};

		class UProperty : public UField
		{
		public:
			unsigned long	ArrayDim;
			unsigned long	ElementSize;
			unsigned long	PropertyFlags;
			unsigned long	PropertySize;
			char			UnknownData01[0x4];
			unsigned long	Offset;
			char			UnknownData02[0x10];
		};

			class UPointerProperty : public UProperty
			{
			public:

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

			class UBoolProperty : public UProperty
			{
			public:
				unsigned long		BitMask;
			};

			class UObjectProperty : public UProperty
			{
			public:
				UClass*				PropertyClass;
				UObjectProperty*	NextReference;
			};

				class UClassProperty : public UObjectProperty
				{
				public:
					UClass*				MetaClass;
				};

			class UInterfaceProperty : public UProperty
			{
			public:
				UClass*				InterfaceClass;
			};

			class UNameProperty : public UProperty
			{
			public:

			};

			class UStructProperty : public UProperty
			{
			public:
				UStruct*			Struct;
			};

			class UStrProperty : public UProperty
			{
			public:

			};

			class UArrayProperty : public UProperty
			{
			public:
				UProperty*			Inner;
			};

			class UMapProperty : public UProperty
			{
			public:
				UProperty*			KeyProp;
				UProperty*			ValueProp;
			};

			class UDelegateProperty : public UProperty
			{
			public:
				UFunction*			SignatureFunction;
			};
