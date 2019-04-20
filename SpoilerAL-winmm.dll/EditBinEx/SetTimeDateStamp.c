#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stddef.h>
#include <assert.h>
#include "Internal.h"

static void SetResourcesTimeDateStamp(LPBYTE Section, PIMAGE_RESOURCE_DIRECTORY Directory, DWORD TimeDateStamp)
{
	size_t NumberOfEntries;

	if (Directory->TimeDateStamp)
		Directory->TimeDateStamp = TimeDateStamp;
	if (NumberOfEntries = (size_t)Directory->NumberOfNamedEntries + (size_t)Directory->NumberOfIdEntries)
	{
		PIMAGE_RESOURCE_DIRECTORY_ENTRY Entry, End;

		End = (Entry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(Directory + 1)) + NumberOfEntries;
		do
			if (Entry->DataIsDirectory)
				SetResourcesTimeDateStamp(Section, (PIMAGE_RESOURCE_DIRECTORY)(Section + Entry->OffsetToDirectory), TimeDateStamp);
		while (++Entry != End);
	}
}

DWORD SetTimeDateStamp(PVOID BaseAddress, DWORD FileSize, PIMAGE_NT_HEADERS NtHeaders, BOOL PE32Plus, BOOL HasCheckSum, LPWSTR lpParameter)
{
	DWORD                 TimeDateStamp;
	PIMAGE_DATA_DIRECTORY DataDirectory;
	DWORD                 Offset;

	if (!GetDwordNumber(lpParameter, &TimeDateStamp))
		return ERROR_INVALID_PARAMETER;

	if (!BaseAddress)
		return ERROR_INVALID_PARAMETER;

	if (NtHeaders->FileHeader.TimeDateStamp)
		NtHeaders->FileHeader.TimeDateStamp = TimeDateStamp;
	DataDirectory = !PE32Plus ?
		((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory :
		((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory;
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size)
	{
		PIMAGE_EXPORT_DIRECTORY ExportDirectory;

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)BaseAddress + Offset);
		if (ExportDirectory->TimeDateStamp)
			ExportDirectory->TimeDateStamp = TimeDateStamp;
	}
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor;

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)BaseAddress + Offset);
		if (ImportDescriptor->TimeDateStamp)
			ImportDescriptor->TimeDateStamp = TimeDateStamp;
	}
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size)
	{
		PIMAGE_RESOURCE_DIRECTORY ResourceDirectory;

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)((LPBYTE)BaseAddress + Offset);
		SetResourcesTimeDateStamp((LPBYTE)ResourceDirectory, ResourceDirectory, TimeDateStamp);
	}
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size)
	{
		PIMAGE_DEBUG_DIRECTORY DebugDirectory;

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		DebugDirectory = (PIMAGE_DEBUG_DIRECTORY)((LPBYTE)BaseAddress + Offset);
		if (DebugDirectory->TimeDateStamp)
			DebugDirectory->TimeDateStamp = TimeDateStamp;
	}
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size)
	{
		PIMAGE_LOAD_CONFIG_DIRECTORY LoadConfigDirectory;

#if _MSC_VER > 1310
		assert(offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32, TimeDateStamp) == offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64, TimeDateStamp));
#endif

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		LoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)((LPBYTE)BaseAddress + Offset);
		if (LoadConfigDirectory->TimeDateStamp)
			LoadConfigDirectory->TimeDateStamp = TimeDateStamp;
	}
	if (DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress && DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size)
	{
		PIMAGE_BOUND_IMPORT_DESCRIPTOR Current, Next, End;

		if (GetFileOffsetFromRVA(NtHeaders, DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress, &Offset) == FALSE)
			return ERROR_BAD_EXE_FORMAT;
		Current = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((LPBYTE)BaseAddress + Offset);
		End = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((LPBYTE)Current + DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
		while ((Next = Current + 1) <= End)
		{
			WORD i;

			if (!Current->OffsetModuleName)
				break;
			if (Current->TimeDateStamp)
				Current->TimeDateStamp = TimeDateStamp;
			i = Current->NumberOfModuleForwarderRefs;
			Current = Next;
			if (!i)
				continue;
			do
			{
				Next = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((PIMAGE_BOUND_FORWARDER_REF)Current + 1);
				if (Next > End)
					goto NESTED_BREAK;
				if (((PIMAGE_BOUND_FORWARDER_REF)Current)->TimeDateStamp)
					((PIMAGE_BOUND_FORWARDER_REF)Current)->TimeDateStamp = TimeDateStamp;
				Current = Next;
			} while (--i);
		}
	NESTED_BREAK:;
	}

	if (HasCheckSum)
		UpdateCheckSum(BaseAddress, FileSize, NtHeaders);

	return ERROR_SUCCESS;
}

