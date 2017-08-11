#include <windows.h>
#include <winternl.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "TProcessCtrl.h"

#pragma comment(lib, "ntdll.lib")

void __fastcall bcb6_std_vector_THeapListData_clear(vector *heapList);
void __fastcall bcb6_std_vector_THeapListData_push_back(vector *heapList, THeapListData *heapListData);

#ifdef USING_NAMESPACE_BCB6_STD
#define vector_THeapListData_clear     bcb6_std_vector_THeapListData_clear
#define vector_THeapListData_push_back bcb6_std_vector_THeapListData_push_back
#endif

extern HANDLE hHeap;

//
// RTL Debug Queries
//
#define RTL_DEBUG_QUERY_MODULES     0x01
#define RTL_DEBUG_QUERY_BACKTRACES  0x02
#define RTL_DEBUG_QUERY_HEAPS       0x04
#define RTL_DEBUG_QUERY_HEAP_TAGS   0x08
#define RTL_DEBUG_QUERY_HEAP_BLOCKS 0x10
#define RTL_DEBUG_QUERY_LOCKS       0x20

//
// Information Structures for RTL Debug Functions
//
typedef struct _RTL_HEAP_INFORMATION
{
	PVOID BaseAddress;
	ULONG Flags;
	USHORT EntryOverhead;
	USHORT CreatorBackTraceIndex;
	SIZE_T BytesAllocated;
	SIZE_T BytesCommitted;
	ULONG NumberOfTags;
	ULONG NumberOfEntries;
	ULONG NumberOfPseudoTags;
	ULONG PseudoTagGranularity;
	ULONG Reserved[5];
	LPVOID/*PRTL_HEAP_TAG*/ Tags;
	LPVOID/*PRTL_HEAP_ENTRY*/ Entries;
} RTL_HEAP_INFORMATION, *PRTL_HEAP_INFORMATION;

typedef struct _RTL_PROCESS_HEAPS
{
	ULONG NumberOfHeaps;
	RTL_HEAP_INFORMATION Heaps[1];
} RTL_PROCESS_HEAPS, *PRTL_PROCESS_HEAPS;

typedef struct _RTL_DEBUG_INFORMATION
{
	HANDLE SectionHandleClient;
	PVOID ViewBaseClient;
	PVOID ViewBaseTarget;
	ULONG ViewBaseDelta;
	HANDLE EventPairClient;
	PVOID EventPairTarget;
	HANDLE TargetProcessId;
	HANDLE TargetThreadHandle;
	ULONG Flags;
	ULONG OffsetFree;
	ULONG CommitSize;
	ULONG ViewSize;
	union
	{
		LPVOID/*PRTL_PROCESS_MODULES*/ Modules;
		LPVOID/*PRTL_PROCESS_MODULE_INFORMATION_EX*/ ModulesEx;
	};
	LPVOID/*PRTL_PROCESS_BACKTRACES*/ BackTraces;
	PRTL_PROCESS_HEAPS Heaps;
	LPVOID/*PRTL_PROCESS_LOCKS*/ Locks;
	HANDLE SpecificHeap;
	HANDLE TargetProcessHandle;
	LPVOID/*RTL_PROCESS_VERIFIER_OPTIONS*/ VerifierOptions;
	HANDLE ProcessHeap;
	HANDLE CriticalSectionHandle;
	HANDLE CriticalSectionOwnerThread;
	PVOID Reserved[4];
} RTL_DEBUG_INFORMATION, *PRTL_DEBUG_INFORMATION;

EXTERN_C
__declspec(dllimport)
NTSTATUS
NTAPI
RtlQueryProcessDebugInformation(
	IN     ULONG                  ProcessId,
	IN     ULONG                  DebugInfoClassMask,
	IN OUT PRTL_DEBUG_INFORMATION DebugBuffer);

EXTERN_C
__declspec(dllimport)
PRTL_DEBUG_INFORMATION
NTAPI
RtlCreateQueryDebugBuffer(
	IN ULONG   Size,
	IN BOOLEAN EventPair);

EXTERN_C
__declspec(dllimport)
NTSTATUS
NTAPI
RtlDestroyQueryDebugBuffer(
	IN PRTL_DEBUG_INFORMATION DebugBuffer);

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
	PRTL_DEBUG_INFORMATION DebugBuffer;

	vector_THeapListData_clear(&_this->heapList);
	DebugBuffer = RtlCreateQueryDebugBuffer(0, FALSE);
	if (DebugBuffer)
	{
		DWORD  dwNumberOfHeaps;
		LPVOID *lpHeapBases;

		if (NT_SUCCESS(RtlQueryProcessDebugInformation(_this->entry.th32ProcessID, RTL_DEBUG_QUERY_HEAPS, DebugBuffer)) &&
			DebugBuffer->Heaps &&
			(dwNumberOfHeaps = DebugBuffer->Heaps->NumberOfHeaps) &&
			(lpHeapBases = (LPVOID *)HeapAlloc(hHeap, 0, dwNumberOfHeaps * sizeof(LPVOID))))
		{
			PRTL_HEAP_INFORMATION HeapInformation;
			THeapListData         heapListData;

			HeapInformation = DebugBuffer->Heaps->Heaps;
			for (DWORD i = 0; i < dwNumberOfHeaps; i++)
			{
				lpHeapBases[i] = (LPVOID)HeapInformation[i].BaseAddress;
			}
			qsort(lpHeapBases, dwNumberOfHeaps, sizeof(LPVOID), CompareAddress);
			heapListData.heapList.dwSize        = sizeof(HEAPLIST32);           // unused
			heapListData.heapList.th32ProcessID = _this->entry.th32ProcessID;   // unused
			heapListData.heapList.th32HeapID    = 0;                            // unused
			heapListData.heapList.dwFlags       = 0;                            // unused
			heapListData.heapListSize           = 4096 - 1;                     // unused
			for (DWORD i = 0; i < dwNumberOfHeaps; i++)
			{
				heapListData.heapListAddress = (DWORD)lpHeapBases[i];
				vector_THeapListData_push_back(&_this->heapList, &heapListData);
			}
			HeapFree(hHeap, 0, lpHeapBases);
		}
		RtlDestroyQueryDebugBuffer(DebugBuffer);
	}
}
