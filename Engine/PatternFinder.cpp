#include "PatternFinder.hpp"

#include <cstring>
#include <psapi.h>

uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask)
{
	MODULEINFO info = { 0 };
	GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

	return FindPattern((uintptr_t)module, info.SizeOfImage, pattern, mask);
}

uintptr_t FindPattern(uintptr_t start, size_t length, const unsigned char* pattern, const char* mask)
{
	size_t pos = 0;
	auto maskLength = std::strlen(mask) - 1;

	auto startAdress = start;
	for (auto it = startAdress; it < startAdress + length; ++it)
	{
		if (*(unsigned char*)it == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return it - maskLength;
			}

			pos++;
		}
		else
		{
			pos = 0;
		}
	}

	return -1;
}
