#include <windows.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

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

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, this->entry.th32ProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		HEAPLIST32 hl;

		hl.dwSize = sizeof(HEAPLIST32);
		if (Heap32ListFirst(hSnapshot, &hl))
		{
			HEAPENTRY32   he;
			THeapListData heapListData;

			he.dwSize = sizeof(HEAPENTRY32);
			vector_clear(&this->heapList);
			heapListData.heapListSize = 4096 - 1;
			do
			{
				heapListData.heapList = hl;
				if (Heap32First(&he, hl.th32ProcessID, hl.th32HeapID))
				{
					heapListData.heapListAddress = he.dwAddress & ~(4096 - 1);
					vector_push_back(&this->heapList, heapListData);
				}
			} while (Heap32ListNext(hSnapshot, &hl));
			if (!FixTheProcedure && !vector_empty(&this->heapList))
				qsort(vector_begin(&this->heapList), vector_size(&this->heapList, THeapListData), sizeof(THeapListData), CompareHeapListData);
		}
		CloseHandle(hSnapshot);
	}
}
