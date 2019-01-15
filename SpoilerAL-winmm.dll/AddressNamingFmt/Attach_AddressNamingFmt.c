#include <windows.h>

EXTERN_C BOOLEAN __fastcall TSSGCtrl_AddressNaming_OneRead(LPVOID SSGC, LPVOID tmpV_0, HANDLE SHandle, DWORD Address, char *tmpC, DWORD DataSize);

#define NOP                (BYTE)0x90
#define CALL_REL32         (BYTE)0xE8
#define JMP_REL32          (BYTE)0xE9
#define PUSH_DWORD_PTR_EBP (WORD)0xB5FF

EXTERN_C void __cdecl Attach_AddressNamingFmt()
{
	// TSSGCtrl::AddressNaming
	*(LPDWORD)0x00504CA8 = 0x00841F0F;
	*(LPDWORD)0x00504CAC = 0;// nop
	*(LPBYTE )0x00504CB0 = JMP_REL32;

	*(LPWORD )0x00505136 = PUSH_DWORD_PTR_EBP;
	*(LPDWORD)0x0050513C = 0xCF8B168B;// mov edx, dword ptr [esi]
	*(LPBYTE )0x00505140 = CALL_REL32;// mov ecx, edi
	*(LPDWORD)0x00505141 = (DWORD)TSSGCtrl_AddressNaming_OneRead - (0x00505141 + sizeof(DWORD));
}
