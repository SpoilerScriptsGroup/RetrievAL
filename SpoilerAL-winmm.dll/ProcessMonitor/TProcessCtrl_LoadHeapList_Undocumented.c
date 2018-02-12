#include <windows.h>
#include <winternl.h>
#include "tlhelp32fix.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"

#pragma comment(lib, "ntdll.lib")

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
	PRTL_DEBUG_INFORMATION DebugBuffer;

	vector_clear(&this->heapList);
	DebugBuffer = RtlCreateQueryDebugBuffer(0, FALSE);
	if (DebugBuffer)
	{
		ULONG NumberOfHeaps;

		if (NT_SUCCESS(RtlQueryProcessDebugInformation(this->entry.th32ProcessID, RTL_DEBUG_QUERY_HEAPS, DebugBuffer)) &&
			DebugBuffer->Heaps &&
			(NumberOfHeaps = DebugBuffer->Heaps->NumberOfHeaps))
		{
			THeapListData         heapListData;
			PRTL_HEAP_INFORMATION Element, Last;

			heapListData.heapList.dwSize        = sizeof(HEAPLIST32);           // unused
			heapListData.heapList.th32ProcessID = this->entry.th32ProcessID;    // unused
			heapListData.heapList.th32HeapID    = 0;                            // unused
			heapListData.heapList.dwFlags       = 0;                            // unused
			heapListData.heapListSize           = 4096 - 1;                     // unused
			Last = (Element = DebugBuffer->Heaps->Heaps) + NumberOfHeaps;
			vector_reserve(&this->heapList, NumberOfHeaps);
			do
			{
				heapListData.heapListAddress = (DWORD)Element->BaseAddress;
				vector_push_back(&this->heapList, heapListData);
			} while (++Element != Last);
			qsort(this->heapList._M_start, NumberOfHeaps, sizeof(THeapListData), CompareHeapListData);
		}
		RtlDestroyQueryDebugBuffer(DebugBuffer);
	}
}
