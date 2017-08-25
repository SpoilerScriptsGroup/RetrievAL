#include <windows.h>

EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_CheckSignedParam();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_DestReserve();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_Format();

#define POP_ECX     (BYTE)0x59
#define POP_EBX     (BYTE)0x5B
#define POP_EBP     (BYTE)0x5D
#define POP_ESI     (BYTE)0x5E
#define POP_EDI     (BYTE)0x5F
#define MOV_ESP_EBP (WORD)0xE58B
#define NOP         (BYTE)0x90
#define NOP_X2      (WORD)0x9090
#define RET         (BYTE)0xC3
#define JMP_REL32   (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixRepeat()
{
	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FEBCA = JMP_REL32;
	*(LPDWORD)0x004FEBCB = (DWORD)TSSGCtrl_ReadSSRFile_CheckSignedParam - (0x004FEBCB + sizeof(DWORD));
	*(LPBYTE )0x004FEBCF = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF2BA = JMP_REL32;
	*(LPDWORD)0x004FF2BB = (DWORD)TSSGCtrl_ReadSSRFile_DestReserve - (0x004FF2BB + sizeof(DWORD));
	*(LPBYTE )0x004FF2BF = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF2E5 = JMP_REL32;
	*(LPDWORD)0x004FF2E6 = (DWORD)TSSGCtrl_ReadSSRFile_CompareLoopCounter - (0x004FF2E6 + sizeof(DWORD));
	*(LPBYTE )0x004FF2EA = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF309 = JMP_REL32;
	*(LPDWORD)0x004FF30A = (DWORD)TSSGCtrl_ReadSSRFile_CompareLoopCounter - (0x004FF30A + sizeof(DWORD));
	*(LPBYTE )0x004FF30E = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF5E0 = POP_ECX;
	*(LPBYTE )0x004FF5E1 = POP_EDI;
	*(LPBYTE )0x004FF5E2 = POP_ESI;
	*(LPBYTE )0x004FF5E3 = POP_EBX;
	*(LPWORD )0x004FF5E4 = MOV_ESP_EBP;
	*(LPBYTE )0x004FF5E6 = POP_EBP;
	*(LPBYTE )0x004FF5E7 = RET;

	// TSSGCtrl::EnumReadSSR
	*(LPBYTE )0x004FFDBD = JMP_REL32;
	*(LPDWORD)0x004FFDBE = (DWORD)TSSGCtrl_EnumReadSSR_SwitchTmpS_0 - (0x004FFDBE + sizeof(DWORD));
	*(LPWORD )0x004FFDC2 = NOP_X2;
	*(LPBYTE )0x004FFDC4 = NOP;

	// TSSGCtrl::LoopSSRFile
	*(LPBYTE )0x0050221D = JMP_REL32;
	*(LPDWORD)0x0050221E = (DWORD)TSSGCtrl_LoopSSRFile_FixWordRepeat - (0x0050221E + sizeof(DWORD));
	*(LPBYTE )0x00502222 = NOP;

	// TSSGCtrl::LoopSSRFile
	*(LPBYTE )0x00502676 = JMP_REL32;
	*(LPDWORD)0x00502677 = (DWORD)TSSGCtrl_LoopSSRFile_Format - (0x00502677 + sizeof(DWORD));
	*(LPBYTE )0x0050267B = NOP;
}
