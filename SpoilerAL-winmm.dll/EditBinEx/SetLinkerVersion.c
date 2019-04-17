#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

DWORD SetLinkerVersion(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL HasCheckSum, LPWSTR lpParameter)
{
	DWORD  Number;
	BYTE   MajorLinkerVersion;
	BYTE   MinorLinkerVersion;
	LPWSTR p;

	MinorLinkerVersion = 0;
	for (p = lpParameter; *p != L'\0'; p++)
	{
		if (*p == L'.')
		{
			*(p++) = L'\0';
			if (GetDwordNumber(p, &Number) == FALSE)
			{
				return ERROR_INVALID_PARAMETER;
			}
			MinorLinkerVersion = (BYTE)Number;
			break;
		}
	}
	if (GetDwordNumber(lpParameter, &Number) == FALSE)
	{
		return ERROR_INVALID_PARAMETER;
	}
	MajorLinkerVersion = (BYTE)Number;

	assert(offsetof(IMAGE_NT_HEADERS32, OptionalHeader) == offsetof(IMAGE_NT_HEADERS64, OptionalHeader));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MajorLinkerVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MajorLinkerVersion));
	assert(offsetof(IMAGE_OPTIONAL_HEADER32, MajorLinkerVersion) == offsetof(IMAGE_OPTIONAL_HEADER64, MajorLinkerVersion));

	NtHeaders->OptionalHeader.MajorLinkerVersion = MajorLinkerVersion;
	NtHeaders->OptionalHeader.MinorLinkerVersion = MinorLinkerVersion;

	if (HasCheckSum != FALSE)
	{
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);
	}

	return ERROR_SUCCESS;
}

