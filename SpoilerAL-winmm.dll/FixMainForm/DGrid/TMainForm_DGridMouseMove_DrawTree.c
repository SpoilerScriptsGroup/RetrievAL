#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_DGridMouseMove_DrawTree()
{
	__asm
	{
		#define _NULL         0
		#define this          ebx
		#define ReturnAddress 00446C6DH

		mov     ecx, [this]TMainForm.userMode
		cmp     ecx, 4
		setae   al
		movzx   eax, al
		push    eax
		push    0
		push    0
		push    _NULL
		push    this
		push    ReturnAddress
		jmp     TMainForm_DrawTree

		#undef _NULL
		#undef this
		#undef ReturnAddress
	}
}
