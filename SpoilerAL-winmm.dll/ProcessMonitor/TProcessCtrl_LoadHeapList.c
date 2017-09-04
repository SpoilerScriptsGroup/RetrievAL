#include <windows.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

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
			if (!vector_empty(&this->heapList))
			{
				qsort(this->heapList._M_start, vector_size(&this->heapList, THeapListData), sizeof(THeapListData), CompareHeapListData);
			}
		}
		CloseHandle(hSnapshot);
	}
}
