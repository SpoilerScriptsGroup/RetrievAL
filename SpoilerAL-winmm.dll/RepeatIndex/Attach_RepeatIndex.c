#include <windows.h>
#include "TSSGCtrl.h"

EXTERN_C void __cdecl TSSGCtrl_ReadSSG_EnumReadSSG(LPVOID this, LPVOID SSGFile, LPVOID ParentStack, LPVOID ADJElem);
#if 0
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty1();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_replace_EnumReadSSG();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG_SetSubjectProperty2();
#endif
EXTERN_C void __cdecl TSSGCtrl_ReadADJFile_EnumReadSSG();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopParamAtRepeat();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopParamAtLoopSSRFile();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_ReserveLoopIndex();
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopIndex();

#define MOV_DWORD_PTR_ECX_EAX      (WORD )0x0189
#define MOV_ECX_DWORD_PTR_EBP_IMM8 (WORD )0x4D8B
#define SEG_SS                     (BYTE )0x36
#define PUSH_ECX                   (BYTE )0x51
#define PUSH_EDX                   (BYTE )0x52
#define PUSH_IMM8                  (BYTE )0x6A
#define NOP                        (BYTE )0x90
#define NOP_X2                     (WORD )0x9090
#define NOP_X4                     (DWORD)0x90909090
#define CALL_REL32                 (BYTE )0xE8
#define JMP_REL32                  (BYTE )0xE9

DWORD RepeatDepth;

EXTERN_C void __cdecl Attach_RepeatIndex()
{
	// TSSGCtrl::ReadSSG
	*(LPDWORD)(0x004E4703 + 1) = (DWORD)TSSGCtrl_ReadSSG_EnumReadSSG - (0x004E4703 + 1 + sizeof(DWORD));

#if 0
	// TSSGCtrl::EnumReadSSG
	*(LPDWORD)(0x004E6228 + 1) = (DWORD)TSSGCtrl_EnumReadSSG_SetSubjectProperty1 - (0x004E6228 + 1 + sizeof(DWORD));

	*(LPBYTE )0x004E66B7 = JMP_REL32;
	*(LPDWORD)0x004E66B8 = (DWORD)TSSGCtrl_EnumReadSSG_replace_EnumReadSSG - (0x004E66B8 + sizeof(DWORD));
	*(LPWORD )0x004E66BC = NOP_X2;
	*(LPBYTE )(0x004E66CF + 2) = 24;

	*(LPBYTE )0x004E70B9 = JMP_REL32;
	*(LPDWORD)0x004E70BA = (DWORD)TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile - (0x004E70BA + sizeof(DWORD));
	*(LPWORD )0x004E70BE = NOP_X2;

	*(LPDWORD)(0x004EB3EF + 1) = (DWORD)TSSGCtrl_EnumReadSSG_SetSubjectProperty2 - (0x004EB3EF + 1 + sizeof(DWORD));
#endif

	// TSSGCtrl::ReadADJFile
#ifdef ADJ_USE_JMP
	*(LPBYTE )0x004FD6DE = JMP_REL32;
	*(LPDWORD)0x004FD6DF = (DWORD)TSSGCtrl_ReadADJFile_EnumReadSSG - (0x004FD6DF + sizeof(DWORD));
	*(LPDWORD)0x004FD6E3 = NOP_X4;
#else
	*(LPBYTE )(0x004FD6E7 + 0) = PUSH_IMM8;
	*(LPBYTE )(0x004FD6E7 + 1) = -1;
	*(LPBYTE )(0x004FD6E9 + 0) = PUSH_IMM8;
	*(LPBYTE )(0x004FD6E9 + 1) = 0;
	*(LPBYTE ) 0x004FD6EB      = PUSH_EDX;
	*(LPBYTE ) 0x004FD6EC      = PUSH_ECX;
	*(LPBYTE ) 0x004FD6ED      =         0xFF   ;// push dword ptr 
	*(LPWORD ) 0x004FD6EE      = BSWAP16(0x71A8);// [ecx - 0x58]
	*(LPDWORD)(0x004FD6F1 + 1) = (DWORD)TSSGCtrl_EnumReadSSG - (0x004FD6F1 + 1 + sizeof(DWORD));
#endif
	*(LPBYTE )(0x004FD6F6 + 2) = 24;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )(0x004FF2D0 + 0) = SEG_SS;
	*(LPBYTE )(0x004FF2D0 + 1) = PUSH_ECX;
	*(LPDWORD)(0x004FF2D7 + 1) = (DWORD)TSSGCtrl_ReadSSRFile_ReserveLoopIndex - (0x004FF2D7 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004FF2DC + 2) = 12;

	*(LPDWORD)(0x004FF2FB + 1) = (DWORD)TSSGCtrl_ReadSSRFile_StoreLoopIndex - (0x004FF2FB + 1 + sizeof(DWORD));

	*(LPBYTE )(0x004FF33F + 0) = SEG_SS;
	*(LPBYTE )(0x004FF33F + 1) = PUSH_EDX;
	*(LPDWORD)(0x004FF346 + 1) = (DWORD)TSSGCtrl_ReadSSRFile_ReserveLoopIndex - (0x004FF346 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004FF34B + 2) = 12;

	*(LPDWORD)(0x004FF391 + 1) = (DWORD)TSSGCtrl_ReadSSRFile_StoreLoopIndex - (0x004FF391 + 1 + sizeof(DWORD));
}
