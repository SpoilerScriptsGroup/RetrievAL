#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

DWORD SetOperatingSystemVersion(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL HasCheckSum, LPWSTR lpParameter)
{
	DWORD  Number;
	WORD   MajorOperatingSystemVersion;
	WORD   MinorOperatingSystemVersion;
	LPWSTR p;

	MinorOperatingSystemVersion = 0;
	for (p = lpParameter; *p; p++)
	{
		if (*p != L'.')
			continue;
		*(p++) = L'\0';
		if (!GetDwordNumber(p, &Number))
			return ERROR_INVALID_PARAMETER;
		MinorOperatingSystemVersion = (WORD)Number;
		break;
	}
	if (!GetDwordNumber(lpParameter, &Number))
		return ERROR_INVALID_PARAMETER;
	MajorOperatingSystemVersion = (WORD)Number;

	assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) == offsetof(IMAGE_NT_HEADERS64, OptionalHeader));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MajorOperatingSystemVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MajorOperatingSystemVersion));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MinorOperatingSystemVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MinorOperatingSystemVersion));

	NtHeaders->OptionalHeader.MajorOperatingSystemVersion = MajorOperatingSystemVersion;
	NtHeaders->OptionalHeader.MinorOperatingSystemVersion = MinorOperatingSystemVersion;

	if (HasCheckSum)
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);

	return ERROR_SUCCESS;
}

