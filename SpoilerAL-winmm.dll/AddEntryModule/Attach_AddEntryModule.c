#include <windows.h>

EXTERN_C void __cdecl AddEntryModule();

#define CALL_REL32 (BYTE)0xE8
#define NOP        (BYTE)0x90

EXTERN_C void __cdecl Attach_AddEntryModule()
{
	// TProcessCtrl::LoadModuleList
	*(LPBYTE )0x004A50D7 = CALL_REL32;
	*(LPDWORD)0x004A50D8 = (DWORD)AddEntryModule - (0x004A50D8 + sizeof(DWORD));
	*(LPBYTE )0x004A50DC = NOP;
}
