#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_DGridMouseMove_DrawTree()
{
	__asm
	{
		#define _NULL                       0
		#define this                        ebx
		#define offsetof_TMainForm_userMode 448H
		#define ReturnAddress               00446C6DH

		mov     ecx, dword ptr [this + offsetof_TMainForm_userMode]
		xor     eax, eax
		cmp     ecx, 4
		sete    al
		push    eax
		push    0
		push    0
		push    _NULL
		push    this
		push    ReturnAddress
		jmp     TMainForm_DrawTree

		#undef _NULL
		#undef this
		#undef offsetof_TMainForm_userMode
		#undef ReturnAddress
	}
}
