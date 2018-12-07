#include <windows.h>
#include "intrinsic.h"
#include "TSSGCtrl.h"

EXTERN_C void __cdecl TSSBitList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleList_WriteOne_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleToggle_Write_CheckFunnel();
EXTERN_C void __cdecl TSSFloatCalc_Write_CheckFunnel();
EXTERN_C void __cdecl TSSString_Write_CheckFunnel();
EXTERN_C void __cdecl TSSBundleFloatCalc_Write_CheckFunnel();

static __declspec(naked) BOOLEAN __cdecl TSSCalc_Write_CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, unsigned long Val) {
	extern BOOL FixTheProcedure;
	__asm {
		cmp FixTheProcedure, 0
		je  SKIP
		mov ecx, [ebp + 0x10]// Arg
		mov edx, [ecx + 0x08]// TSSArgLong*->value
		mov [esp + 0x0C], edx// Val
	SKIP:
		jmp TSSGCtrl_CheckFunnel
	}
}

#define JZ_REL32   (WORD)0x840F
#define JNZ_REL32  (WORD)0x850F
#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_ForceFunnel()
{
	// TSSBitList::Write - CheckFunnel
	*(LPDWORD)0x004BB703 = JNZ_REL32;
	*(LPDWORD)0x004BB705 = (DWORD)TSSBitList_Write_CheckFunnel - (0x004BB705 + sizeof(DWORD));

	// TSSBundleCalc::Write - CheckFunnel
	*(LPDWORD)0x004BE6B4 = CALL_REL32;
	*(LPDWORD)0x004BE6B5 = (DWORD)TSSCalc_Write_CheckFunnel - (0x004BE6B5 + sizeof(DWORD));

	// TSSCalc::Write - CheckFunnel
	*(LPDWORD)0x004C20F1 = CALL_REL32;
	*(LPDWORD)0x004C20F2 = (DWORD)TSSCalc_Write_CheckFunnel - (0x004C20F2 + sizeof(DWORD));

	// TSSDoubleList::Write - CheckFunnel
	*(LPDWORD)0x004C5507 = JNZ_REL32;
	*(LPDWORD)0x004C5509 = (DWORD)TSSDoubleList_WriteOne_CheckFunnel - (0x004C5509 + sizeof(DWORD));

	/*
		call    TSSDoubleList_Write_CheckFunnel         ; 004C6280 _ E8, ????????
		mov     eax, dword ptr [ebp - 4H]               ; 004C6285 _ 8B. 45, FC
		test    eax, eax                                ; 004C6288 _ 85. C0
		jz      004C6292H                               ; 004C628A _ 74, 06
		push    eax                                     ; 004C628C _ 50
	*/
	*(LPBYTE )0x004C6280 = CALL_REL32;
	*(LPDWORD)0x004C6281 = (DWORD)TSSDoubleList_Write_CheckFunnel - (0x004C6281 + sizeof(DWORD));
	*(LPDWORD)0x004C6285 = BSWAP32(0x8B45FC85);
	*(LPDWORD)0x004C6289 = BSWAP32(0xC0740650);

	// TSSDoubleToggle::Write - CheckFunnel
	*(LPBYTE )0x004CD90B = CALL_REL32;
	*(LPDWORD)0x004CD90C = (DWORD)TSSDoubleToggle_Write_CheckFunnel - (0x004CD90C + sizeof(DWORD));
	*(LPBYTE )0x004CD910 = NOP;

	// TSSFloatCalc::Write - CheckFunnel
	/*
		call    TSSFloatCalc_Write_CheckFunnel          ; 004CE697 _ E8, ????????
		mov     eax, dword ptr [ebp - 4H]               ; 004CE69C _ 8B. 45, FC
		test    eax, eax                                ; 004CE69F _ 85. C0
		jz      004CE6A9H                               ; 004CE6A1 _ 74, 06
		push    eax                                     ; 004CE6A3 _ 50
	*/
	*(LPBYTE )0x004CE697 = CALL_REL32;
	*(LPDWORD)0x004CE698 = (DWORD)TSSFloatCalc_Write_CheckFunnel - (0x004CE698 + sizeof(DWORD));
	*(LPDWORD)0x004CE69C = BSWAP32(0x8B45FC85);
	*(LPDWORD)0x004CE6A0 = BSWAP32(0xC0740650);

	// TSSString::Write - CheckFunnel
	*(LPDWORD)0x0052B558 = JZ_REL32;
	*(LPDWORD)0x0052B55A = (DWORD)TSSString_Write_CheckFunnel - (0x0052B55A + sizeof(DWORD));

	// TSSBundleFloatCalc::Write - CheckFunnel
	/*
		call    TSSBundleFloatCalc_Write_CheckFunnel    ; 0052EAFA _ E8, ????????
		mov     eax, dword ptr [ebp - 4H]               ; 0052EAFF _ 8B. 45, FC
		test    eax, eax                                ; 0052EB02 _ 85. C0
		jz      0052EB0CH                               ; 0052EB04 _ 74, 06
		push    eax                                     ; 0052EB06 _ 50
	*/
	*(LPBYTE )0x0052EAFA = CALL_REL32;
	*(LPDWORD)0x0052EAFB = (DWORD)TSSBundleFloatCalc_Write_CheckFunnel - (0x0052EAFB + sizeof(DWORD));
	*(LPDWORD)0x0052EAFF = BSWAP32(0x8B45FC85);
	*(LPDWORD)0x0052EB03 = BSWAP32(0xC0740650);
}
