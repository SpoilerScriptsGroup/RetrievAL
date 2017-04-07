#include <windows.h>
#include "tlhelp32fix.h"
#include "bcb6_std_vector.h"
#include "TProcessCtrl.h"

void __fastcall bcb6_std_vector_THeapListData_clear(bcb6_std_vector *heapList);
void __fastcall bcb6_std_vector_THeapListData_push_back(bcb6_std_vector *heapList, THeapListData *heapListData);

__declspec(naked) int __cdecl CompareHeapListData(const void *elem1, const void *elem2)
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

void __cdecl TProcessCtrl_LoadHeapList(TProcessCtrl *_this)
{
	HANDLE hSnapshot;

	bcb6_std_vector_THeapListData_clear(&_this->heapList);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, _this->entry.th32ProcessID);
	if (hSnapshot)
	{
		HEAPLIST32 hl;

		hl.dwSize = sizeof(HEAPLIST32);
		if (Heap32ListFirst(hSnapshot, &hl))
		{
			THeapListData heapListData;

			heapListData.heapList.dwSize        = sizeof(HEAPLIST32);           // unused
			heapListData.heapList.th32ProcessID = _this->entry.th32ProcessID;   // unused
			heapListData.heapList.th32HeapID    = 0;                            // unused
			heapListData.heapList.dwFlags       = 0;                            // unused
			heapListData.heapListSize           = 4096 - 1;                     // unused
			do
			{
				// internal specification
				heapListData.heapListAddress = hl.th32HeapID;
				bcb6_std_vector_THeapListData_push_back(&_this->heapList, &heapListData);
			} while (Heap32ListNext(hSnapshot, &hl));
			qsort(_this->heapList._M_start, bcb6_std_vector_size(&_this->heapList, THeapListData), sizeof(THeapListData), CompareHeapListData);
		}
		CloseHandle(hSnapshot);
	}
}
