#include <windows.h>

EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_TrimString();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile();

#define PUSH_ECX                   (BYTE)0x51
#define PUSH_IMM8                  (BYTE)0x6A
#define MOV_EAX_DWORD_PTR_EBP_IMM8 (WORD)0x458B
#define LEA_ECX_EBP_ADD_IMM8       (WORD)0x4D8D
#define NOP                        (BYTE)0x90
#define NOP_X2                     (WORD)0x9090
#define JMP_REL8                   (BYTE)0xEB

EXTERN_C void Attach_NocachedMemoryList()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPBYTE )0x004EE11B = PUSH_IMM8;
	*(LPBYTE )0x004EE11C = 0x00;
	*(LPBYTE )0x004EE11D = JMP_REL8;
	*(LPBYTE )0x004EE11E = 0x004EE121 - (0x004EE11E + sizeof(BYTE));
	*(LPWORD )0x004EE11F = NOP_X2;

	*(LPDWORD)(0x004EE180 + 1) = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam - (0x004EE180 + 1 + sizeof(DWORD));

	*(LPBYTE )(0x004EE19E + 2) = 0x07;
	*(LPDWORD)(0x004EE1B9 + 2) = 0x000000A8;
	*(LPDWORD)(0x004EE23F + 1) = 0x00000007;

	*(LPDWORD)(0x004EE3B5 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_TrimString - (0x004EE3B5 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004EF423 = JMP_REL8;
	*(LPBYTE )0x004EF424 = 0x004EF43B - (0x004EF424 + sizeof(BYTE));
	*(LPBYTE )0x004EF425 = NOP;

	*(LPWORD )0x004EF43B = MOV_EAX_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004EF43D = (char)0x08;
	*(LPWORD )0x004EF43E = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF440 = (char)0x10;
	*(LPBYTE )0x004EF441 = PUSH_ECX;
	*(LPWORD )0x004EF442 = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF444 = (char)-0x30;
	*(LPBYTE )0x004EF445 = PUSH_ECX;

	*(LPDWORD)(0x004EF447 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile - (0x004EF447 + 1 + sizeof(DWORD));
}
