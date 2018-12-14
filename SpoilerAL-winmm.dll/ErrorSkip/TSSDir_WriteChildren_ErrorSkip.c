#include "TSSGCtrl.h"
#include "TSSGAttributeElement.h"

__declspec(naked) void __cdecl TSSDir_WriteChildren_ErrorSkip()
{
	__asm
	{
		#define ReturnAddress 004C32A4H
		#define SSGC          edi
		#define this          (ebp + 8)

		mov     eax, dword ptr [ecx + 12]
		mov     dword ptr [esp], offset L1
		jmp     eax
	L1:
		mov     dword ptr [esp + 8], ReturnAddress
		add     esp, 8
		test    eax, eax
		jz      L4
		push    eax
		mov     ecx, dword ptr [esp - 4]
		push    atON_ERROR
		push    ecx
		push    SSGC
		call    dword ptr [TSSGCtrl_GetAttribute]
		test    eax, eax
		jz      L2
		xor     eax, eax
		jmp     L3
	L2:
		mov     eax, dword ptr [esp + 12]
	L3:
		add     esp, 16
	L4:
		ret

		#undef ReturnAddress
		#undef SSGC
		#undef this
	}
}
