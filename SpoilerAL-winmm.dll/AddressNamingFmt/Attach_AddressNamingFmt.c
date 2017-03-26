#include <windows.h>

EXTERN_C BOOLEAN __stdcall TSSGCtrl_AddressNaming_OneRead(LPVOID NameStr, HANDLE SHandle, DWORD Address, char *tmpC, DWORD DataSize);

#define NOP                (BYTE)0x90
#define CALL_REL32         (BYTE)0xE8
#define JMP_REL32          (BYTE)0xE9
#define PUSH_DWORD_PTR_EBP (WORD)0x75FF

EXTERN_C void Attach_AddressNamingFmt()
{
	// TSSGCtrl::AddressNaming
	*(LPBYTE )0x00504CA8 = JMP_REL32;
	*(LPDWORD)0x00504CA9 = 0x00504DF0 - (0x00504CA9 + sizeof(DWORD));
	*(LPBYTE )0x00504CAD = NOP;
	*(LPBYTE )0x00504CAE = NOP;

	*(LPWORD )0x0050513D = PUSH_DWORD_PTR_EBP;
	*(LPBYTE )0x0050513F = 0x14;
	*(LPBYTE )0x00505140 = CALL_REL32;
	*(LPDWORD)0x00505141 = (DWORD)TSSGCtrl_AddressNaming_OneRead - (0x00505141 + sizeof(DWORD));
}
