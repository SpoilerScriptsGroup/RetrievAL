#include <windows.h>

EXTERN_C void __cdecl FixTraceAndCopySetting();
EXTERN_C void __cdecl FixTraceAndCopyWrite();

#define NOP_2BYTE  (WORD)0x9066
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_FixTraceAndCopy()
{
	// TSSCopy::Setting
	*(LPDWORD)(0x004C2A0E + 1) = (DWORD)FixTraceAndCopySetting - (0x004C2A0E + 1 + sizeof(DWORD));

	// TSSCopy::Write
	*(LPBYTE )0x004C2AA3 = CALL_REL32;
	*(LPDWORD)0x004C2AA4 = (DWORD)FixTraceAndCopyWrite - (0x004C2AA4 + sizeof(DWORD));

	// TSSTrace::Setting
	*(LPDWORD)(0x0052CE05 + 1) = (DWORD)FixTraceAndCopySetting - (0x0052CE05 + 1 + sizeof(DWORD));

	// TSSTrace::Write
	*(LPBYTE )0x0052CE8B = CALL_REL32;
	*(LPDWORD)0x0052CE8C = (DWORD)FixTraceAndCopyWrite - (0x0052CE8C + sizeof(DWORD));
}
