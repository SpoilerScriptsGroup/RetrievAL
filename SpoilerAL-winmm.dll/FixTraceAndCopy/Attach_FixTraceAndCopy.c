#include <windows.h>
#include "bcb6_operator.h"
#include "TSSGCtrl.h"

EXTERN_C void __cdecl FixTraceAndCopySetting();
EXTERN_C void __cdecl FixTraceAndCopyWrite();

static __declspec(naked) __fastcall TSSCopy_Write_delete_Data(BYTE *Data) {
	__asm {
		mov edi, [ecx]// Data = *(LPDWORD)Data;
		jmp bcb6_operator_delete
	}
}

static __declspec(naked) void __cdecl TSSCopy_Write_CheckFunnel(TSSGCtrl* this, TSSGSubject* SSGS, unsigned long Val) {
	extern BOOL FixTheProcedure;
	__asm {
		cmp FixTheProcedure, 0
		je  FUNNEL
		mov ecx, 4
		sub ecx, [ebx + 0x78]// size
		jbe ASSIGN
		shl ecx, 3
		or  eax, -1
		shr eax, cl
		and edi, eax
	ASSIGN:
		mov [esp + 0x0C], edi
	FUNNEL:
		jmp TSSGCtrl_CheckFunnel
	}
}

#define NOP_2BYTE  (WORD)0x9066
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_FixTraceAndCopy()
{
	// TSSCopy::Setting
	*(LPDWORD)(0x004C2A0E + 1) = (DWORD)FixTraceAndCopySetting - (0x004C2A0E + 1 + sizeof(DWORD));

	// TSSCopy::Write
	*(LPBYTE )0x004C2AA3 = CALL_REL32;
	*(LPDWORD)0x004C2AA4 = (DWORD)FixTraceAndCopyWrite - (0x004C2AA4 + sizeof(DWORD));
	
	*(LPBYTE )0x004C2BE7 =         0x8B;// mov ecx, edi
	*(LPWORD )0x004C2BE8 = BSWAP16(0xCF << 8 | CALL_REL32);
	*(LPDWORD)0x004C2BEA = (DWORD)TSSCopy_Write_delete_Data - (0x004C2BEA + sizeof(DWORD));

	*(LPDWORD)(0x004C2C4A + 1) = (DWORD)TSSCopy_Write_CheckFunnel - (0x004C2C4A + 1 + sizeof(DWORD));

	// TSSTrace::Setting
	*(LPDWORD)(0x0052CE05 + 1) = (DWORD)FixTraceAndCopySetting - (0x0052CE05 + 1 + sizeof(DWORD));

	// TSSTrace::Write
	*(LPBYTE )0x0052CE8B = CALL_REL32;
	*(LPDWORD)0x0052CE8C = (DWORD)FixTraceAndCopyWrite - (0x0052CE8C + sizeof(DWORD));
}
