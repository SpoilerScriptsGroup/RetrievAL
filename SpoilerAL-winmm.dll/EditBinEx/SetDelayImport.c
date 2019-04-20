#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

DWORD SetDelayImport(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum, LPWSTR lpParameter)
{
	DWORD                 VirtualAddress;
	DWORD                 Size;
	LPWSTR                p;
	PIMAGE_DATA_DIRECTORY DataDirectory;

	Size = 0;
	for (p = lpParameter; *p; p++)
	{
		if (*p != L',')
			continue;
		*(p++) = L'\0';
		if (!GetDwordNumber(p, &Size))
			return ERROR_INVALID_PARAMETER;
		break;
	}
	if (!GetDwordNumber(lpParameter, &VirtualAddress))
		return ERROR_INVALID_PARAMETER;

	if (IsBadReadPtr(IMAGE_FIRST_SECTION(NtHeaders), NtHeaders->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)))
		return ERROR_ACCESS_DENIED;

	DataDirectory = (!PE32Plus ?
		((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory :
		((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory) + IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT;

	DataDirectory->VirtualAddress = VirtualAddress;
	DataDirectory->Size = Size;

	if (HasCheckSum)
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);

	return ERROR_SUCCESS;
}

