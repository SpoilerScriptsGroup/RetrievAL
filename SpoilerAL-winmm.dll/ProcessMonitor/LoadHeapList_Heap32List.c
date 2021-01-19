#include <windows.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"
#include "page.h"

extern BOOL FixTheProcedure;

BOOL __cdecl VerifyInternalSpecificationOfHeapID()
{
	BOOL         bMatches  = FALSE;
	HANDLE const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		HEAPLIST32 hl = { .dwSize = sizeof(HEAPLIST32) };

		if (Heap32ListFirst(hSnapshot, &hl))
		{
			HEAPENTRY32 he = { .dwSize = sizeof(HEAPENTRY32) };

			while ((bMatches = Heap32First(&he, hl.th32ProcessID, hl.th32HeapID) &&
					hl.th32HeapID == (he.dwAddress & -PAGE_SIZE)) &&
				   Heap32ListNext(hSnapshot, &hl));
		}
		CloseHandle(hSnapshot);
	}
	return bMatches;
}

static ptrdiff_t __cdecl CompareHeapListData(const THeapListData *const heap1, const THeapListData *const heap2)
{
	return heap1->heapListAddress - heap2->heapListAddress;
}

void __cdecl LoadHeapList(TProcessCtrl *this)
{
	HANDLE const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, this->entry.th32ProcessID);

	vector_clear(&this->heapList);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		THeapListData tmpHLD = {
			.heapList = {
				.dwSize = sizeof(tmpHLD.heapList),
			},
			.heapListSize = PAGE_SIZE - 1,
		};

		if (Heap32ListFirst(hSnapshot, &tmpHLD.heapList))
		{
			DWORD const dwErrCode = GetLastError();

			do
			{
				tmpHLD.heapListAddress = tmpHLD.heapList.th32HeapID;
				vector_push_back(&this->heapList, tmpHLD);
			} while (Heap32ListNext(hSnapshot, &tmpHLD.heapList));
			SetLastError(dwErrCode);// discard ERROR_NO_MORE_FILES
			if (!FixTheProcedure)
				qsort(vector_begin(&this->heapList), vector_size(&this->heapList), sizeof(THeapListData), CompareHeapListData);
		}
		CloseHandle(hSnapshot);
	}
}
