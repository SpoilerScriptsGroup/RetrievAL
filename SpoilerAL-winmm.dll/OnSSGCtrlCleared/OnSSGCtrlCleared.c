#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TProcessCtrl.h"
#include "SSGSubjectProperty.h"

#define ALLOCATE_SUPPORT 1

typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;

typedef struct {
	size_t SizeOfReplace;
	LPBYTE Replace;
	LPSTR  Source;
	size_t NumberOfMarkup;
	LPVOID Markup;
	size_t NumberOfPostfix;
	LPVOID Postfix;
	size_t Next;
} CODECACHE, *PCODECACHE;

extern HANDLE             hHeap;
extern HANDLE             hPrivateHeap;
extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;
extern FILETIME           ftProcessCreationTime;
extern size_t             nNumberOfCodeCache;
extern CODECACHE          *lpCodeCache;

void __cdecl OnSSGCtrlCleared(IN TSSGCtrl *SSGCtrl)
{
	ClearSubjectProperty();

#if ALLOCATE_SUPPORT
	if (lpProcessMemory)
	{
		if (nNumberOfProcessMemory)
		{
			size_t i = nNumberOfProcessMemory;
			do
				if (!lpProcessMemory[--i].Protect && lpProcessMemory[i].Address)
					HeapFree(hPrivateHeap, HEAP_NO_SERIALIZE, lpProcessMemory[i].Address);
			while (i);
			nNumberOfProcessMemory = 0;
		}
		HeapFree(hHeap, 0, lpProcessMemory);
		lpProcessMemory = NULL;
	}
#endif
	HeapCompact(hPrivateHeap, 0);
	if (lpCodeCache)
	{
		if (nNumberOfCodeCache)
		{
			CODECACHE *lpCache;

			lpCache = lpCodeCache + nNumberOfCodeCache;
			nNumberOfCodeCache = 0;
			do
			{
				lpCache--;
				HeapFree(hHeap, 0, lpCache->Postfix);
				HeapFree(hHeap, 0, lpCache->Markup);
				HeapFree(hHeap, 0, lpCache->Source);
				if (lpCache->Replace)
					HeapFree(hHeap, 0, lpCache->Replace);
			} while (lpCache != lpCodeCache);
		}
		HeapFree(hHeap, 0, lpCodeCache);
		lpCodeCache = NULL;
	}

	TProcessCtrl_Clear(&SSGCtrl->processCtrl);
}
