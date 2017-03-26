#include <windows.h>

EXTERN_C void __cdecl AddressNamingFromFloat();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

EXTERN_C void Attach_AddressNamingFromFloat()
{
	// TSSGCtrl::AddressNaming
	*(LPBYTE )0x0050624E = JMP_REL32;
	*(LPDWORD)0x0050624F = (DWORD)AddressNamingFromFloat - (0x0050624F + sizeof(DWORD));
	*(LPBYTE )0x00506253 = NOP;
	*(LPBYTE )0x00506254 = NOP;
	*(LPBYTE )0x00506255 = NOP;
}
