#include <windows.h>
#include "TProcessCtrl.h"

#define JMP_REL32  (BYTE )0xE9
#define NOP_X4     (DWORD)0x90909090

EXTERN_C void Attach_ProcessMonitor()
{
	// TProcessCtrl::LoadHeapList
	*(LPBYTE )0x004A3980 = JMP_REL32;
	*(LPDWORD)0x004A3981 = (DWORD)TProcessCtrl_LoadHeapList - (0x004A3981 + sizeof(DWORD));
	*(LPDWORD)0x004A3985 = NOP_X4;

	// TProcessCtrl::FindProcess
	*(LPBYTE )0x004A5ACC = JMP_REL32;
	*(LPDWORD)0x004A5ACD = (DWORD)TProcessCtrl_FindProcess - (0x004A5ACD + sizeof(DWORD));
	*(LPDWORD)0x004A5AD1 = NOP_X4;
}
