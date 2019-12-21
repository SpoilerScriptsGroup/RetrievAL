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
					if (!(bMatches = hl.th32HeapID == (he.dwAddress & -PAGE_SIZE)))
						break;
			while (Heap32ListNext(hSnapshot, &hl));
		}
		CloseHandle(hSnapshot);
	}
	return bMatches;
}

static int __cdecl CompareHeapListData(const void *elem1, const void *elem2)
{
	return
		((THeapListData *)elem1)->heapListAddress > ((THeapListData *)elem2)->heapListAddress ? 1 :
		((THeapListData *)elem1)->heapListAddress < ((THeapListData *)elem2)->heapListAddress ? -1 :
		0;
}

void __cdecl LoadHeapList(TProcessCtrl *this)
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
