#include <windows.h>

EXTERN_C void __cdecl TSSGCtrl_ReadSSG_EnumReadSSG();
EXTERN_C void __cdecl Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty1();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_replace_EnumReadSSG();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile();
EXTERN_C void __cdecl Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty2();
EXTERN_C void __cdecl TSSGCtrl_ReadADJFile_EnumReadSSG();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopParam();

#define MOV_DWORD_PTR_ECX_EAX      (WORD )0x0189
#define MOV_ECX_DWORD_PTR_EBP_IMM8 (WORD )0x4D8B
#define NOP                        (BYTE )0x90
#define NOP_X2                     (WORD )0x9090
#define NOP_X4                     (DWORD)0x90909090
#define CALL_REL32                 (BYTE )0xE8
#define JMP_REL32                  (BYTE )0xE9

DWORD RepeatDepth;

EXTERN_C void Attach_RepeatIndex()
{
	// TSSGCtrl::ReadSSG
	*(LPBYTE )0x004E46FB = JMP_REL32;
	*(LPDWORD)0x004E46FC = (DWORD)TSSGCtrl_ReadSSG_EnumReadSSG - (0x004E46FC + sizeof(DWORD));
	*(LPBYTE )0x004E4700 = NOP;
	*(LPBYTE )(0x004E4708 + 2) = 24;

	// TSSGCtrl::EnumReadSSG
	*(LPDWORD)(0x004E6228 + 1) = (DWORD)Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty1 - (0x004E6228 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004E66B7 = JMP_REL32;
	*(LPDWORD)0x004E66B8 = (DWORD)TSSGCtrl_EnumReadSSG_replace_EnumReadSSG - (0x004E66B8 + sizeof(DWORD));
	*(LPWORD )0x004E66BC = NOP_X2;
	*(LPBYTE )(0x004E66CF + 2) = 24;

	*(LPBYTE )0x004E70B9 = JMP_REL32;
	*(LPDWORD)0x004E70BA = (DWORD)TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile - (0x004E70BA + sizeof(DWORD));
	*(LPWORD )0x004E70BE = NOP_X2;

	*(LPDWORD)(0x004EB3EF + 1) = (DWORD)Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty2 - (0x004EB3EF + 1 + sizeof(DWORD));

	// TSSGCtrl::ReadADJFile
	*(LPBYTE )0x004FD6DE = JMP_REL32;
	*(LPDWORD)0x004FD6DF = (DWORD)TSSGCtrl_ReadADJFile_EnumReadSSG - (0x004FD6DF + sizeof(DWORD));
	*(LPDWORD)0x004FD6E3 = NOP_X4;
	*(LPBYTE )(0x004FD6F6 + 2) = 24;

	// TSSGCtrl::ReadSSRFile
	*(LPWORD )0x004FEB75 = MOV_ECX_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004FEB77 = 0x14;
	*(LPWORD )0x004FEB78 = MOV_DWORD_PTR_ECX_EAX;
	*(LPBYTE )0x004FEB7A = NOP;

	*(LPBYTE )0x004FF2AC = CALL_REL32;
	*(LPDWORD)0x004FF2AD = (DWORD)TSSGCtrl_ReadSSRFile_StoreLoopParam - (0x004FF2AD + sizeof(DWORD));
	*(LPBYTE )0x004FF2B1 = NOP;
}
