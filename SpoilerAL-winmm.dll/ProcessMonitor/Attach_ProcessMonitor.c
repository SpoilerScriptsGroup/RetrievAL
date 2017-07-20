#include <windows.h>
#include "TProcessCtrl.h"

#define USE_INTERNAL_SPECIFICATION_OF_HEAP_ID 1

#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
EXTERN_C BOOL __cdecl VerifyInternalSpecificationOfHeapID();
#endif

#define JMP_REL32 (BYTE )0xE9
#define NOP_X4    (DWORD)0x90909090

EXTERN_C void __cdecl Attach_ProcessMonitor()
{
	// TProcessCtrl::LoadHeapList
#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
	if (VerifyInternalSpecificationOfHeapID())
#endif
	{
		*(LPBYTE )0x004A3980 = JMP_REL32;
		*(LPDWORD)0x004A3981 = (DWORD)TProcessCtrl_LoadHeapList - (0x004A3981 + sizeof(DWORD));
		*(LPDWORD)0x004A3985 = NOP_X4;
	}

	// TProcessCtrl::FindProcess
	*(LPBYTE )0x004A5ACC = JMP_REL32;
	*(LPDWORD)0x004A5ACD = (DWORD)TProcessCtrl_FindProcess - (0x004A5ACD + sizeof(DWORD));
	*(LPDWORD)0x004A5AD1 = NOP_X4;
}
