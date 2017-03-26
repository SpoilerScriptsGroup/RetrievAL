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

extern HANDLE             hHeap;
extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;

EXTERN_C void __cdecl OnSSGCtrlCleared(IN TSSGCtrl *SSGCtrl)
{
	ClearSubjectProperty();

#if defined(ALLOCATE_SUPPORT) && ALLOCATE_SUPPORT
	if (lpProcessMemory)
	{
		if (nNumberOfProcessMemory)
		{
			HANDLE hProcess;

			hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION);
			if (hProcess)
			{
				size_t i;

				i = nNumberOfProcessMemory;
				do
				{
					if (lpProcessMemory[--i].Address)
						VirtualFreeEx(hProcess, lpProcessMemory[i].Address, 0, MEM_RELEASE);
				} while (i);
				CloseHandle(hProcess);
			}
			nNumberOfProcessMemory = 0;
		}
		HeapFree(hHeap, 0, lpProcessMemory);
		lpProcessMemory = NULL;
	}
#endif
}
