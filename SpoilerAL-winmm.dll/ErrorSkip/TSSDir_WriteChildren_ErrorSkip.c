#include "TSSGCtrl.h"
#include "TSSGAttributeElement.h"

__declspec(naked) void __fastcall TSSDir_WriteChildren_ErrorSkip(LPCVOID vtable)
{
	__asm
	{
		#define ReturnAddress 004C32A4H
		#define SSGC          edi
		#define VIt           ebx

		mov     dword ptr [esp], offset L1
		jmp     dword ptr [ecx + 12]
	L1:
		test    eax, eax
		jz      L4
		mov     dword ptr [esp], eax
		push    atON_ERROR
		push    dword ptr [VIt]
		push    SSGC
		call    dword ptr [TSSGCtrl_GetAttribute]
		add     esp, 12
		neg     eax
		sbb     eax, eax
		inc     eax
		cmovnz  eax, dword ptr [esp]
	L4:
		mov     dword ptr [esp], ReturnAddress
		ret     8

		#undef VIt
		#undef SSGC
		#undef ReturnAddress
	}
}
