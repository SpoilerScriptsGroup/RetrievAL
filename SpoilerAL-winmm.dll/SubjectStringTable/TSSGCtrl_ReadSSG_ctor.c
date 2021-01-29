#include <Windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "ToolTip/ToolTip.h"
#include "SubjectStringTable.h"

HANDLE hPrivateHeap = NULL;

void __cdecl TSSGCtrl_OpenSSG_scriptDir_M_append_dispatch(
	string const *const scriptDir,// been appended
	char   const *const __f,
	char   const *const __l,
	void   const *const __false_type)
{
	LPSTR lpBuffer;
	DWORD HeapInformation = 2;// Low-fragmentation Heap.
	SubjectStringTable_clear();
	if ((hPrivateHeap ||
		 !(hPrivateHeap = HeapCreate(0, 0, 0)) ||
		 !HeapSetInformation(hPrivateHeap, HeapCompatibilityInformation, &HeapInformation, sizeof(HeapInformation))) &&
		FormatMessageA(
			 FORMAT_MESSAGE_MAX_WIDTH_MASK * !USE_TOOLTIP |
			 FORMAT_MESSAGE_ALLOCATE_BUFFER |
			 FORMAT_MESSAGE_IGNORE_INSERTS |
			 FORMAT_MESSAGE_FROM_SYSTEM,
			 NULL,
			 GetLastError(),
			 0,
			 (LPSTR)&lpBuffer,
			 sizeof(double),
			 NULL)
		)
	{
#if USE_TOOLTIP
		ShowToolTip(lpBuffer, (HICON)TTI_ERROR);
#else
		if (TMainForm_GetUserMode(MainForm) != 1)
			TMainForm_Guide(lpBuffer, 0);
#endif
		LocalFree(lpBuffer);
	}
}
