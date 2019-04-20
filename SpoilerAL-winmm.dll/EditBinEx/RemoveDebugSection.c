#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "crt\memset.h"
#include "Internal.h"

DWORD RemoveDebugSection(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum)
{
	PIMAGE_DATA_DIRECTORY DataDirectory;

	if (IsBadReadPtr(IMAGE_FIRST_SECTION(NtHeaders), NtHeaders->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)))
		return ERROR_ACCESS_DENIED;

	DataDirectory = (!PE32Plus ?
		((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory :
		((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory) + IMAGE_DIRECTORY_ENTRY_DEBUG;

	do
	{
		DWORD                  Offset;
		PIMAGE_DEBUG_DIRECTORY DebugDirectory;
		DWORD                  NumEntries;

		if (!DataDirectory->VirtualAddress)
		{
			if (!DataDirectory->Size)
				return ERROR_SUCCESS;
			DataDirectory->Size = 0;
			break;
		}
		else if (!DataDirectory->Size)
		{
			DataDirectory->VirtualAddress = 0;
			break;
		}

		if (!GetFileOffsetFromRVA(NtHeaders, DataDirectory->VirtualAddress, &Offset))
			return ERROR_BAD_EXE_FORMAT;

		if (IsBadWritePtr(DebugDirectory = (PIMAGE_DEBUG_DIRECTORY)((size_t)BaseAddress + (size_t)Offset), DataDirectory->Size))
			return ERROR_ACCESS_DENIED;

		if (NumEntries = DataDirectory->Size / sizeof(IMAGE_DEBUG_DIRECTORY))
		{
			PIMAGE_DEBUG_DIRECTORY Entry, EndOfEntries;
			LPBYTE                 DebugInfo;

			EndOfEntries = (Entry = DebugDirectory) + NumEntries;
			do
				if (!IsBadWritePtr(DebugInfo = (LPBYTE)BaseAddress + Entry->PointerToRawData, sizeof(Entry->SizeOfData)))
					memset(DebugInfo, 0, Entry->SizeOfData);
			while (++Entry != EndOfEntries);
		}
		memset(DebugDirectory, 0, DataDirectory->Size);

		DataDirectory->VirtualAddress = 0;
		DataDirectory->Size = 0;

	} while (0);

	if (HasCheckSum)
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);

	return ERROR_SUCCESS;
}

