#include <windows.h>
#include "tlhelp32fix.h"
#include "bcb6_std_vector.h"
#include "TProcessCtrl.h"

#define PSAPI_VERSION 1
#include <psapi.h>	// using GetModuleInformation (required Windows NT 4.0)
#pragma comment(lib, "psapi.lib")

void __fastcall bcb6_std_vector_THeapListData_clear(bcb6_std_vector *heapList);
void __fastcall bcb6_std_vector_THeapListData_push_back(bcb6_std_vector *heapList, THeapListData *heapListData);

extern HANDLE hHeap;

typedef HANDLE(WINAPI *LPFN_CREATEREMOTETHREAD)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);

// prevention for the misdetection of security software
__inline HMODULE InitializeKernel32Clone(OUT LPFN_CREATEREMOTETHREAD *lplpfnCreateRemoteThread)
{
	HMODULE hModule;

	hModule = GetModuleHandleA("kernel32.dll");
	if (hModule)
	{
		MODULEINFO modInfo;

		if (GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO)))
		{
			HMODULE hKernel32Clone;

			hKernel32Clone = (HMODULE)VirtualAlloc(NULL, modInfo.SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (hKernel32Clone)
			{
				size_t diff;
				LPBYTE lpAddress, lpEndOfAddress;

				diff = (size_t)hKernel32Clone - (size_t)modInfo.lpBaseOfDll;
				lpAddress = modInfo.lpBaseOfDll;
				lpEndOfAddress = (LPBYTE)modInfo.lpBaseOfDll + modInfo.SizeOfImage;
				while (lpAddress < lpEndOfAddress)
				{
					MEMORY_BASIC_INFORMATION mbi;

					if (VirtualQuery(lpAddress, &mbi, sizeof(mbi)))
					{
						if (mbi.Protect & (
							PAGE_READONLY          |
							PAGE_READWRITE         |
							PAGE_WRITECOPY         |
							PAGE_EXECUTE           |
							PAGE_EXECUTE_READ      |
							PAGE_EXECUTE_READWRITE |
							PAGE_EXECUTE_WRITECOPY))
						{
							memcpy((LPBYTE)mbi.BaseAddress + diff, mbi.BaseAddress, mbi.RegionSize);
						}
						lpAddress += mbi.RegionSize;
					}
					else
					{
						lpAddress += 0x1000;
					}
				}
				*lplpfnCreateRemoteThread = (LPFN_CREATEREMOTETHREAD)((LPBYTE)CreateRemoteThread + diff);
				return hKernel32Clone;
			}
		}
	}
	return NULL;
}

#define DeleteKernel32Clone(hKernel32Clone) \
	VirtualFree(hKernel32Clone, 0, MEM_RELEASE)

__inline LPTHREAD_START_ROUTINE InitializeRemoteCode(IN HANDLE hProcess)
{
	LPTHREAD_START_ROUTINE lpfnEnumrateHeapAddress;

	/*
	LPVOID __stdcall EnumrateHeapAddress(IN LPVOID lpThreadParameter)
	{
		DWORD dwNumberOfHeaps;

		dwNumberOfHeaps = GetProcessHeaps(0, NULL);
		if (dwNumberOfHeaps)
		{
			LPBYTE lpBuffer;

			lpBuffer = (LPBYTE)VirtualAlloc(NULL, sizeof(DWORD) + dwNumberOfHeaps * sizeof(LPVOID), MEM_COMMIT, PAGE_READWRITE);
			if (lpBuffer)
			{
				PHANDLE lpHeap;

				lpHeap = (PHANDLE)(lpBuffer + sizeof(DWORD));
				if (GetProcessHeaps(dwNumberOfHeaps, lpHeap) == dwNumberOfHeaps)
				{
					PHANDLE lpEndOfHeap;

					*(LPDWORD)lpBuffer = dwNumberOfHeaps;
					lpEndOfHeap = lpHeap + dwNumberOfHeaps;
					do
					{
						HANDLE             hHeap;
						PROCESS_HEAP_ENTRY entry;

						hHeap = *lpHeap;
						*lpHeap = NULL;
						if (!HeapLock(hHeap))
							continue;
						entry.lpData = NULL;
						if (HeapWalk(hHeap, &entry))
							*lpHeap = entry.lpData;
						HeapUnlock(hHeap);
					} while (++lpHeap != lpEndOfHeap);
					return lpBuffer;
				}
				VirtualFree(lpBuffer, 0, MEM_RELEASE);
			}
		}
		return NULL;
	}
	*/
	static BYTE lpCode[] = {
		0x6A, 0x00,                                 // 00     push    NULL
		0x6A, 0x00,                                 // 02     push    0
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 04     call    GetProcessHeaps
		0x85, 0xC0,                                 // 09     test    eax, eax
		0x74, 0x7C,                                 // 0B     jz      L5
		0x53,                                       // 0D     push    ebx
		0x56,                                       // 0E     push    esi
		0x8B, 0xD8,                                 // 0F     mov     ebx, eax
		0x8D, 0x04, 0x85, 0x04, 0x00, 0x00, 0x00,   // 11     lea     eax, [eax * 4 + 4H]
		0x6A, 0x04,                                 // 18     push    PAGE_READWRITE
		0x68, 0x00, 0x10, 0x00, 0x00,               // 1A     push    MEM_COMMIT
		0x50,                                       // 1F     push    eax
		0x6A, 0x00,                                 // 20     push    NULL
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 22     call    VirtualAlloc
		0x85, 0xC0,                                 // 27     test    eax, eax
		0x74, 0x5C,                                 // 29     jz      L4
		0x57,                                       // 2B     push    edi
		0x55,                                       // 2C     push    ebp
		0x8B, 0xF0,                                 // 2D     mov     esi, eax
		0x8D, 0x78, 0x04,                           // 2F     lea     edi, [eax + 4H]
		0x57,                                       // 32     push    edi
		0x53,                                       // 33     push    ebx
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 34     call    GetProcessHeaps
		0x3B, 0xC3,                                 // 39     cmp     eax, ebx
		0x75, 0x53,                                 // 3B     jne     L6
		0x83, 0xEC, 0x1C,                           // 3D     sub     esp, 28
		0x8D, 0x2C, 0x87,                           // 40     lea     ebp, [edi + eax * 4]
		0x89, 0x06,                                 // 43     mov     dword ptr [esi], eax
		0x89, 0x74, 0x24, 0x30,                     // 45     mov     dword ptr [esp + 30H], esi
		0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00,   // 49     align   16
		0x33, 0xDB,                                 // 50 L1: xor     ebx, ebx
		0x8B, 0x37,                                 // 52     mov     esi, dword ptr [edi]
		0x89, 0x1C, 0x24,                           // 54     mov     dword ptr [esp], ebx
		0x56,                                       // 57     push    esi
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 58     call    HeapLock
		0x85, 0xC0,                                 // 5D     test    eax, eax
		0x74, 0x14,                                 // 5F     jz      L3
		0x54,                                       // 61     push    esp
		0x56,                                       // 62     push    esi
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 63     call    HeapWalk
		0x85, 0xC0,                                 // 68     test    eax, eax
		0x74, 0x03,                                 // 6A     jz      L2
		0x8B, 0x1C, 0x24,                           // 6C     mov     ebx, dword ptr [esp]
		0x56,                                       // 6F L2: push    esi
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 70     call    HeapUnlock
		0x89, 0x1F,                                 // 75 L3: mov     dword ptr [edi], ebx
		0x83, 0xC7, 0x04,                           // 77     add     edi, 4
		0x3B, 0xFD,                                 // 7A     cmp     edi, ebp
		0x75, 0xD2,                                 // 7C     jne     L1
		0x8B, 0x44, 0x24, 0x30,                     // 7E     mov     eax, dword ptr [esp + 30H]
		0x83, 0xC4, 0x1C,                           // 82     add     esp, 28
		0x5D,                                       // 85     pop     ebp
		0x5F,                                       // 86     pop     edi
		0x5E,                                       // 87 L4: pop     esi
		0x5B,                                       // 88     pop     ebx
		0xC2, 0x04, 0x00,                           // 89 L5: ret     4
		0xCC,                                       // 8C     int     3
		0xCC,                                       // 8D     int     3
		0xCC,                                       // 8E     int     3
		0xCC,                                       // 8F     int     3
		0x68, 0x00, 0x80, 0x00, 0x00,               // 90 L6: push    MEM_RELEASE
		0x6A, 0x00,                                 // 95     push    0
		0x56,                                       // 97     push    esi
		0xE8, 0x00, 0x00, 0x00, 0x00,               // 98     call    VirtualFree
		0x5D,                                       // 9D     pop     ebp
		0x5F,                                       // 9E     pop     edi
		0x5E,                                       // 9F     pop     esi
		0x5B,                                       // A0     pop     ebx
		0x33, 0xC0,                                 // A1     xor     eax, eax
		0xC2, 0x04, 0x00,                           // A3     ret     4
		0xCC,                                       // A6     int     3
		0xCC,                                       // A7     int     3
		0xCC,                                       // A8     int     3
		0xCC,                                       // A9     int     3
		0xCC,                                       // AA     int     3
		0xCC,                                       // AB     int     3
		0xCC,                                       // AC     int     3
		0xCC,                                       // AD     int     3
		0xCC,                                       // AE     int     3
		0xCC,                                       // AF     int     3
	};

	lpfnEnumrateHeapAddress = (LPTHREAD_START_ROUTINE)VirtualAllocEx(hProcess, NULL, sizeof(lpCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpfnEnumrateHeapAddress)
	{
		*(LPDWORD)(lpCode + 0x05) = (DWORD)GetProcessHeaps - ((DWORD)lpfnEnumrateHeapAddress + 0x05 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x23) = (DWORD)VirtualAlloc    - ((DWORD)lpfnEnumrateHeapAddress + 0x23 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x35) = (DWORD)GetProcessHeaps - ((DWORD)lpfnEnumrateHeapAddress + 0x35 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x59) = (DWORD)HeapLock        - ((DWORD)lpfnEnumrateHeapAddress + 0x59 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x64) = (DWORD)HeapWalk        - ((DWORD)lpfnEnumrateHeapAddress + 0x64 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x71) = (DWORD)HeapUnlock      - ((DWORD)lpfnEnumrateHeapAddress + 0x71 + sizeof(DWORD));
		*(LPDWORD)(lpCode + 0x99) = (DWORD)VirtualFree     - ((DWORD)lpfnEnumrateHeapAddress + 0x99 + sizeof(DWORD));

		if (WriteProcessMemory(hProcess, lpfnEnumrateHeapAddress, lpCode, sizeof(lpCode), NULL))
			return lpfnEnumrateHeapAddress;
		VirtualFreeEx(hProcess, lpfnEnumrateHeapAddress, 0, MEM_RELEASE);
	}
	return NULL;
}

#define DeleteRemoteCode(hProcess, lpfnEnumrateHeapAddress) \
	VirtualFreeEx(hProcess, lpfnEnumrateHeapAddress, 0, MEM_RELEASE)

__inline LPVOID * ThreadFunction_EnumrateHeapAddress(IN HANDLE hProcess, OUT LPDWORD lpdwNumberOfHeaps, IN LPFN_CREATEREMOTETHREAD lpfnCreateRemoteThread, IN LPTHREAD_START_ROUTINE lpfnEnumrateHeapAddress)
{
	LPVOID *lpBuffer;
	HANDLE  hThread;

	lpBuffer = NULL;
	hThread = lpfnCreateRemoteThread(hProcess, NULL, 0, lpfnEnumrateHeapAddress, NULL, 0, NULL);
	if (hThread)
	{
		LPBYTE lpRemoteBuffer;

		WaitForSingleObject(hThread, INFINITE);
		if (GetExitCodeThread(hThread, (LPDWORD)&lpRemoteBuffer) && lpRemoteBuffer)
		{
			if (ReadProcessMemory(hProcess, lpRemoteBuffer, lpdwNumberOfHeaps, sizeof(DWORD), NULL))
			{
				size_t nSize;

				nSize = (size_t)*lpdwNumberOfHeaps * sizeof(LPVOID);
				lpBuffer = (LPVOID *)HeapAlloc(hHeap, 0, nSize);
				if (lpBuffer)
				{
					if (!ReadProcessMemory(hProcess, lpRemoteBuffer + sizeof(DWORD), lpBuffer, nSize, NULL))
					{
						HeapFree(hHeap, 0, lpBuffer);
						lpBuffer = NULL;
					}
				}
			}
			VirtualFreeEx(hProcess, lpRemoteBuffer, 0, MEM_RELEASE);
		}
		CloseHandle(hThread);
	}
	return lpBuffer;
}

__inline LPVOID * EnumrateRemoteHeapAddress(IN HANDLE hProcess, OUT LPDWORD lpdwNumberOfHeaps)
{
	LPVOID                  *lpHeapAddress;
	HMODULE                 hKernel32Clone;
	LPFN_CREATEREMOTETHREAD lpfnCreateRemoteThread;

	lpHeapAddress = NULL;
	hKernel32Clone = InitializeKernel32Clone(&lpfnCreateRemoteThread);
	if (hKernel32Clone)
	{
		LPTHREAD_START_ROUTINE lpfnEnumrateHeapAddress;

		lpfnEnumrateHeapAddress = InitializeRemoteCode(hProcess);
		if (lpfnEnumrateHeapAddress)
		{
			lpHeapAddress = ThreadFunction_EnumrateHeapAddress(hProcess, lpdwNumberOfHeaps, lpfnCreateRemoteThread, lpfnEnumrateHeapAddress);
			DeleteRemoteCode(hProcess, lpfnEnumrateHeapAddress);
		}
		DeleteKernel32Clone(hKernel32Clone);
	}
	return lpHeapAddress;
}

static __declspec(naked) int __cdecl CompareAddress(const void *elem1, const void *elem2)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [ecx]
		mov     edx, dword ptr [edx]
		xor     eax, eax
		cmp     ecx, edx
		seta    al
		sbb     eax, 0
		ret
	}
}

void __cdecl TProcessCtrl_LoadHeapList(TProcessCtrl *_this)
{
	HANDLE hProcess;

	bcb6_std_vector_THeapListData_clear(&_this->heapList);
	hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, _this->entry.th32ProcessID);
	if (hProcess)
	{
		LPVOID *lpHeapAddress;
		DWORD   dwNumberOfHeaps;

		lpHeapAddress = EnumrateRemoteHeapAddress(hProcess, &dwNumberOfHeaps);
		if (lpHeapAddress)
		{
			THeapListData heapListData;

			qsort(lpHeapAddress, dwNumberOfHeaps, sizeof(LPVOID), CompareAddress);
			heapListData.heapList.dwSize        = sizeof(HEAPLIST32);           // unused
			heapListData.heapList.th32ProcessID = _this->entry.th32ProcessID;   // unused
			heapListData.heapList.th32HeapID    = 0;                            // unused
			heapListData.heapList.dwFlags       = 0;                            // unused
			heapListData.heapListSize           = 4096 - 1;                     // unused
			for (DWORD i = 0; i < dwNumberOfHeaps; i++)
			{
				if (!lpHeapAddress[i])
					continue;
				heapListData.heapListAddress = (DWORD)lpHeapAddress[i];
				bcb6_std_vector_THeapListData_push_back(&_this->heapList, &heapListData);
			}
			HeapFree(hHeap, 0, lpHeapAddress);
		}
		CloseHandle(hProcess);
	}
}
