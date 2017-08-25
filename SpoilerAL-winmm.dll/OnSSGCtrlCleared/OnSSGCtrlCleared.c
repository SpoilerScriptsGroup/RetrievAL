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

__declspec(naked) void __cdecl OnSSGCtrlCleared()
{
	static void __cdecl InternalOnSSGCtrlCleared(IN TSSGCtrl *SSGCtrl);

	__asm
	{
		pop     ebx
		pop     ebp
		jmp     InternalOnSSGCtrlCleared
	}
}

static void __cdecl InternalOnSSGCtrlCleared(IN TSSGCtrl *SSGCtrl)
{
	ClearSubjectProperty();

#if ALLOCATE_SUPPORT
	if (lpProcessMemory)
	{
		if (nNumberOfProcessMemory)
		{
			HANDLE hProcess;
			size_t i;

			hProcess = TProcessCtrl_Open(&SSGCtrl->processCtrl, PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION);
			if (hProcess)
			{
				i = nNumberOfProcessMemory;
				do
				{
					if (lpProcessMemory[--i].Protect && lpProcessMemory[i].Address)
						VirtualFreeEx(hProcess, lpProcessMemory[i].Address, 0, MEM_RELEASE);
				} while (i);
				CloseHandle(hProcess);
			}
			i = nNumberOfProcessMemory;
			do
			{
				if (!lpProcessMemory[--i].Protect && lpProcessMemory[i].Address)
					HeapFree(hHeap, 0, lpProcessMemory[i].Address);
			} while (i);
			nNumberOfProcessMemory = 0;
		}
		HeapFree(hHeap, 0, lpProcessMemory);
		lpProcessMemory = NULL;
	}
#endif
}
