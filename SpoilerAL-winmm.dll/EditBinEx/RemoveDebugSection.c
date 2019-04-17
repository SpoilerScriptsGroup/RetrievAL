#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <limits.h>
#include <assert.h>
#include "crt\memset.h"
#include "Internal.h"

///////////////////////////////////////////////////////////////////////////////
// CodeView debug information structures

#define CV_SIGNATURE_NB10   '01BN'
#define CV_SIGNATURE_RSDS   'SDSR'

// CodeView header
typedef struct
{
	DWORD      CvSignature;     // NBxx
	LONG       Offset;          // Always 0 for NB10
} CV_HEADER;

// CodeView NB10 debug information
// (used when debug information is stored in a PDB 2.00 file)
typedef struct
{
	CV_HEADER  Header;
	DWORD      Signature;       // seconds since 01.01.1970
	DWORD      Age;             // an always-incrementing value
	BYTE       PdbFileName[1];  // zero terminated string with the name of the PDB file
} CV_INFO_PDB20;

// CodeView RSDS debug information
// (used when debug information is stored in a PDB 7.00 file)
typedef struct
{
	DWORD      CvSignature;
	GUID       Signature;       // unique identifier
	DWORD      Age;             // an always-incrementing value
	BYTE       PdbFileName[1];  // zero terminated string with the name of the PDB file
} CV_INFO_PDB70;

DWORD RemoveDebugSection(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum)
{
	PIMAGE_DATA_DIRECTORY DataDirectory;
	BOOL                  Update;

	if (IsBadReadPtr(IMAGE_FIRST_SECTION(NtHeaders), NtHeaders->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)))
	{
		return ERROR_ACCESS_DENIED;
	}

	/* Obtain the debug directory RVA and size */
	if (PE32Plus == FALSE)
	{
		DataDirectory = &((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
	}
	else
	{
		DataDirectory = &((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
	}

	Update = FALSE;
	do
	{
		DWORD                  DebugDirectoryOffset;
		PIMAGE_DEBUG_DIRECTORY DebugDirectory;
		int                    NumEntries;
		int                    i;

		if (DataDirectory->VirtualAddress == 0)
		{
			if (DataDirectory->Size != 0)
			{
				DataDirectory->Size = 0;
				Update = TRUE;
			}
			break;
		}
		else if (DataDirectory->Size < sizeof(IMAGE_DEBUG_DIRECTORY))
		{
			if (DataDirectory->Size == 0)
			{
				DataDirectory->VirtualAddress = 0;
				Update = TRUE;
				break;
			}
			return ERROR_BAD_EXE_FORMAT;
		}

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory->VirtualAddress, &DebugDirectoryOffset) == FALSE)
		{
			return ERROR_BAD_EXE_FORMAT;
		}

		DebugDirectory = (PIMAGE_DEBUG_DIRECTORY)((size_t)BaseAddress + (size_t)DebugDirectoryOffset);

		if (IsBadReadPtr(DebugDirectory, DataDirectory->Size))
		{
			return ERROR_ACCESS_DENIED;
		}

		// Determine the number of entries in the debug directory
		NumEntries = DataDirectory->Size / sizeof(IMAGE_DEBUG_DIRECTORY);

		for (i = 0; i < NumEntries; i++, DebugDirectory++)
		{
			LPBYTE  DebugInfo;
			DWORD   CvSignature;
			LPCSTR  PdbFileName;

			if (DebugDirectory->Type != IMAGE_DEBUG_TYPE_CODEVIEW)
			{
				continue;
			}
			if (IsBadReadPtr(&DebugDirectory->SizeOfData, sizeof(DebugDirectory->SizeOfData)) != FALSE)
			{
				break;
			}
			if (DebugDirectory->SizeOfData < sizeof(DWORD))
			{
				break;
			}
			if (IsBadWritePtr(&DebugDirectory->SizeOfData, sizeof(DebugDirectory->SizeOfData)) != FALSE)
			{
				break;
			}
			DebugInfo = (LPBYTE)BaseAddress + DebugDirectory->PointerToRawData;
			CvSignature = *(DWORD*)DebugInfo;
			if (CvSignature == CV_SIGNATURE_NB10)
			{
				PdbFileName = (LPCSTR)((CV_INFO_PDB20 *)DebugInfo)->PdbFileName;
			}
			else if (CvSignature == CV_SIGNATURE_RSDS)
			{
				PdbFileName = (LPCSTR)((CV_INFO_PDB70 *)DebugInfo)->PdbFileName;
			}
			else
			{
				break;
			}
			if (IsBadStringPtrA(PdbFileName, UINT_MAX) != FALSE)
			{
				break;
			}
			if (IsBadWritePtr(DebugDirectory, DebugDirectory->SizeOfData) != FALSE)
			{
				break;
			}
			memset(DebugInfo, 0, DebugDirectory->SizeOfData);
			memset((unsigned char *)DebugDirectory, 0, sizeof(*DebugDirectory));
			if (NumEntries == 1)
			{
				DataDirectory->VirtualAddress = 0;
				DataDirectory->Size = 0;
			}
			Update = TRUE;
		}
	}
	while (0);
	if (Update != FALSE && HasCheckSum != FALSE)
	{
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);
	}

	return ERROR_SUCCESS;
}

