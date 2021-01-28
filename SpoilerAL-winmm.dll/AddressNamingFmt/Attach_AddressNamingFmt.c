#include <windows.h>

EXTERN_C BOOLEAN __fastcall TSSGCtrl_AddressNaming_OneRead(LPVOID tmpV_0, DWORD);

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

	*(LPWORD )0x0050512F = PUSH_DWORD_PTR_EBP;
	*(LPWORD )0x00505135 = PUSH_DWORD_PTR_EBP;
	*(LPDWORD)0x00505137 = 0xFFFFFF44;// -0xBC
	*(LPBYTE )0x0050513B = 0x8B;// mov ecx,
	*(LPBYTE )0x0050513C = 0x0E;// dword ptr [esi]
	*(LPDWORD)0x0050513E = (DWORD)TSSGCtrl_AddressNaming_OneRead - (0x0050513E + sizeof(DWORD));
}
