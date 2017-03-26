#include <windows.h>
#include "intrinsic.h"

EXTERN_C LPVOID __stdcall GetSectionAddress(HANDLE hProcess, HMODULE hModule, LPCSTR lpSectionName, LPDWORD lpdwSectionSize)
{
	do	/* do { ... } while (0); */
	{
		typedef struct {
			WORD Machine;
			WORD NumberOfSections;
		} PARTIAL_FILE_HEADER;

		typedef struct {
			DWORD               Signature;
			PARTIAL_FILE_HEADER FileHeader;
		} PARTIAL_NT_HEADERS;

		size_t             nSectionNameSize;
		LPBYTE             lpAddress;
		IMAGE_DOS_HEADER   DosHeader;
		PARTIAL_NT_HEADERS NtHeader;
		WORD               OptionalHdrMagic;
		LPBYTE             EndOfSectionHeader;

		if (!hProcess || !hModule || !lpSectionName)
			break;
		nSectionNameSize = strlen(lpSectionName);
		if (!nSectionNameSize)
			break;
		if (++nSectionNameSize > max(10 + 1, IMAGE_SIZEOF_SHORT_NAME))
			break;
		lpAddress = (LPBYTE)hModule;
		if (!ReadProcessMemory(hProcess, lpAddress, &DosHeader, sizeof(DosHeader), NULL))
			break;
		if (DosHeader.e_magic != IMAGE_DOS_SIGNATURE)
			break;
		lpAddress += DosHeader.e_lfanew;
		if (!ReadProcessMemory(hProcess, lpAddress, &NtHeader, sizeof(NtHeader), NULL))
			break;
		if (NtHeader.Signature != IMAGE_NT_SIGNATURE)
			break;
		lpAddress += offsetof(IMAGE_NT_HEADERS, OptionalHeader);
		if (!ReadProcessMemory(hProcess, lpAddress, &OptionalHdrMagic, sizeof(OptionalHdrMagic), NULL))
			break;
		if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		{
			switch (*lpSectionName)
			{
			case 'I':
				if (nSectionNameSize == 9 + 1 &&
					*(LPDWORD)(lpSectionName + 1) == BSWAP32('mage') &&
					*(LPDWORD)(lpSectionName + 5) == BSWAP32('Base'))
				{
					if (lpdwSectionSize)
						if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, SizeOfImage), lpdwSectionSize, sizeof(DWORD), NULL))
							return NULL;
					return (LPVOID)hModule;
				}
				break;
			case 'C':
				if (nSectionNameSize == 4 + 1 && *(LPDWORD)lpSectionName == BSWAP32('Code'))
				{
					DWORD BaseOfCode;

					if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, BaseOfCode), &BaseOfCode, sizeof(DWORD), NULL))
						return NULL;
					if (lpdwSectionSize)
						if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, SizeOfCode), lpdwSectionSize, sizeof(DWORD), NULL))
							return NULL;
					return (LPBYTE)hModule + BaseOfCode;
				}
				break;
			case 'D':
				if (nSectionNameSize == 4 + 1 && *(LPDWORD)lpSectionName == BSWAP32('Data'))
				{
					DWORD BaseOfData;

					if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, BaseOfData), &BaseOfData, sizeof(DWORD), NULL))
						return NULL;
					if (lpdwSectionSize)
						if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, SizeOfInitializedData), lpdwSectionSize, sizeof(DWORD), NULL))
							return NULL;
					return (LPBYTE)hModule + BaseOfData;
				}
				break;
			case 'E':
				if (nSectionNameSize == 10 + 1 &&
					*(LPDWORD)(lpSectionName + 1) == BSWAP32('ntry') &&
					*(LPDWORD)(lpSectionName + 5) == BSWAP32('Poin') &&
					lpSectionName[9] == 't')
				{
					DWORD AddressOfEntryPoint;

					if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER32, AddressOfEntryPoint), &AddressOfEntryPoint, sizeof(DWORD), NULL))
						return NULL;
					if (lpdwSectionSize)
						*lpdwSectionSize = 0;
					return (LPBYTE)hModule + AddressOfEntryPoint;
				}
				break;
			}
			lpAddress += sizeof(IMAGE_OPTIONAL_HEADER32);
		}
#ifdef _WIN64
		else if (OptionalHdrMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		{
			switch (*lpSectionName)
			{
			case 'I':
				if (nSectionNameSize == 9 + 1 &&
					*(LPDWORD)(lpSectionName + 1) == BSWAP32('mage') &&
					*(LPDWORD)(lpSectionName + 5) == BSWAP32('Base'))
				{
					if (lpdwSectionSize)
						if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER64, SizeOfImage), lpdwSectionSize, sizeof(DWORD), NULL))
							return NULL;
					return (LPVOID)hModule;
				}
				break;
			case 'C':
				if (nSectionNameSize == 4 + 1 && *(LPDWORD)lpSectionName == BSWAP32('Code'))
				{
					DWORD BaseOfCode;

					if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER64, BaseOfCode), &BaseOfCode, sizeof(DWORD), NULL))
						return NULL;
					if (lpdwSectionSize)
						if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER64, SizeOfCode), lpdwSectionSize, sizeof(DWORD), NULL))
							return NULL;
					return (LPBYTE)hModule + BaseOfCode;
				}
				break;
			case 'E':
				if (nSectionNameSize == 10 + 1 &&
					*(LPDWORD)(lpSectionName + 1) == BSWAP32('ntry') &&
					*(LPDWORD)(lpSectionName + 5) == BSWAP32('Poin') &&
					lpSectionName[9] == 't')
				{
					DWORD AddressOfEntryPoint;

					if (!ReadProcessMemory(hProcess, lpAddress + offsetof(IMAGE_OPTIONAL_HEADER64, AddressOfEntryPoint), &AddressOfEntryPoint, sizeof(DWORD), NULL))
						return NULL;
					if (lpdwSectionSize)
						*lpdwSectionSize = 0;
					return (LPBYTE)hModule + AddressOfEntryPoint;
				}
				break;
			}
			lpAddress += sizeof(IMAGE_OPTIONAL_HEADER64);
		}
#endif
		else
			break;
		if (nSectionNameSize > IMAGE_SIZEOF_SHORT_NAME)
			break;
		EndOfSectionHeader = lpAddress + NtHeader.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
		for (; lpAddress < EndOfSectionHeader; lpAddress += sizeof(IMAGE_SECTION_HEADER))
		{
			typedef struct {
				BYTE  Name[IMAGE_SIZEOF_SHORT_NAME];
				union {
					DWORD PhysicalAddress;
					DWORD VirtualSize;
				} Misc;
				DWORD VirtualAddress;
			} PARTIAL_SECTION_HEADER;

			PARTIAL_SECTION_HEADER SectionHeader;

			if (!ReadProcessMemory(hProcess, lpAddress, &SectionHeader, sizeof(SectionHeader), NULL))
				break;
			if (memcmp(SectionHeader.Name, lpSectionName, nSectionNameSize) != 0)
				continue;
			lpAddress = (LPBYTE)hModule + SectionHeader.VirtualAddress;
			if (lpdwSectionSize)
				*lpdwSectionSize = SectionHeader.Misc.VirtualSize;
			return lpAddress;
		}
	} while (0);
	return NULL;
}
