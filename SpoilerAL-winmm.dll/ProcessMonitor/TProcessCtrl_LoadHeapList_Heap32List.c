#include <windows.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"
#include "PageSize.h"

BOOL __cdecl VerifyInternalSpecificationOfHeapID()
{
	BOOL   bMatches;
	HANDLE hSnapshot;

	bMatches = FALSE;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, GetCurrentProcessId());
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		HEAPLIST32 hl;

		hl.dwSize = sizeof(HEAPLIST32);
		if (Heap32ListFirst(hSnapshot, &hl))
		{
			HEAPENTRY32 he;

			he.dwSize = sizeof(HEAPENTRY32);
			do
				if (Heap32First(&he, hl.th32ProcessID, hl.th32HeapID))
					if (!(bMatches = hl.th32HeapID == (he.dwAddress & -(LONG_PTR)PAGE_SIZE)))
						break;
			while (Heap32ListNext(hSnapshot, &hl));
		}
		CloseHandle(hSnapshot);
	}
	return bMatches;
}

__declspec(naked) static int __cdecl CompareHeapListData(const void *elem1, const void *elem2)
{
	__asm
	{
		#define offsetof_THeapListData_heapListAddress 16

		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [ecx + offsetof_THeapListData_heapListAddress]
		mov     edx, dword ptr [edx + offsetof_THeapListData_heapListAddress]
		xor     eax, eax
		cmp     ecx, edx
		seta    al
		sbb     eax, 0
		ret

		#undef offsetof_THeapListData_heapListAddress
	}
}

void __cdecl TProcessCtrl_LoadHeapList(TProcessCtrl *this)
{
	extern BOOL FixTheProcedure;
	HANDLE hSnapshot;

	vector_clear(&this->heapList);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, this->entry.th32ProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		HEAPLIST32 hl;

		hl.dwSize = sizeof(HEAPLIST32);
		if (Heap32ListFirst(hSnapshot, &hl))
		{
			THeapListData heapListData;

			heapListData.heapList.dwSize        = sizeof(HEAPLIST32);           // unused
			heapListData.heapList.th32ProcessID = this->entry.th32ProcessID;    // unused
			heapListData.heapList.th32HeapID    = 0;                            // unused
			heapListData.heapList.dwFlags       = 0;                            // unused
			heapListData.heapListSize           = 4096 - 1;                     // unused
			do
			{
				heapListData.heapListAddress = hl.th32HeapID;
				vector_push_back(&this->heapList, heapListData);
			} while (Heap32ListNext(hSnapshot, &hl));
			if (!FixTheProcedure)
				qsort(vector_begin(&this->heapList), vector_size(&this->heapList), sizeof(THeapListData), CompareHeapListData);
		}
		CloseHandle(hSnapshot);
	}
}
