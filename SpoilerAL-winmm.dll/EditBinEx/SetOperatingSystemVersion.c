#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

DWORD SetOperatingSystemVersion(PVOID BaseAddress, DWORD SizeOfImage, PIMAGE_NT_HEADERS NtHeaders, LPWSTR lpParameter)
{
	DWORD  Number;
	WORD   MajorOperatingSystemVersion;
	WORD   MinorOperatingSystemVersion;
	LPWSTR p;

	MinorOperatingSystemVersion = 0;
	for (p = lpParameter; *p != L'\0'; p++)
	{
		if (*p == L'.')
		{
			*(p++) = L'\0';
			if (GetDwordNumber(p, &Number) == FALSE)
			{
				return ERROR_INVALID_PARAMETER;
			}
			MinorOperatingSystemVersion = (WORD)Number;
			break;
		}
	}
	if (GetDwordNumber(lpParameter, &Number) == FALSE)
	{
		return ERROR_INVALID_PARAMETER;
	}
	MajorOperatingSystemVersion = (WORD)Number;

	assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) == offsetof(IMAGE_NT_HEADERS64, OptionalHeader));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MajorOperatingSystemVersion));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MinorOperatingSystemVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MinorOperatingSystemVersion));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, CheckSum) == offsetof(IMAGE_OPTIONAL_HEADER64, CheckSum));

	NtHeaders->OptionalHeader.MajorOperatingSystemVersion = MajorOperatingSystemVersion;
	NtHeaders->OptionalHeader.MinorOperatingSystemVersion = MinorOperatingSystemVersion;

	UpdateCheckSum(BaseAddress, SizeOfImage, &NtHeaders->OptionalHeader.CheckSum);

	return ERROR_SUCCESS;
}

