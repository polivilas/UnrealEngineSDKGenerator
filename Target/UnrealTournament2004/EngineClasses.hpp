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
	char			UnknownData[20];
	UObject*		Outer;
	int				Flags;
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
			char			UnknownData00[0x08];
			UField*			Children;
			unsigned long	PropertySize;
			char			UnknownData01[0x3C];
		};

			class UScriptStruct : public UStruct
			{
			public:
				char			UnknownData00[0x1C];
			};

			class UFunction : public UStruct
			{
			public:
				int32_t			FunctionFlags;
				int16_t			iNative;
				char			UnknownData00[2];
				int8_t			OperPrecedence;
				int8_t			NumParms;
				int16_t			ParmsSize;
				int16_t			ReturnValueOffset;
				char			UnknownData01[2];
				void*			Func;
			};

			class UState : public UStruct
			{
			public:
				char			UnknownData00[0x48];
			};

				class UClass : public UState
				{
				public:
					char			UnknownData00[136];
					UObject*		DefaultObject;
					char			UnknownData01[112];
				};

		class UProperty : public UField
		{
		public:
			unsigned long	ArrayDim;
			unsigned long	ElementSize;
			FQWord			PropertyFlags;
			unsigned short	RepOffset;
			unsigned short	RepIndex;
			unsigned long	Offset;
			UProperty*		PropertyLinkNext;
			UProperty*		ConfigLinkNext;
			UProperty*		ConstructorLinkNext;
			UProperty*		RepOwner;
			char			UnknownData[0x10];
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
