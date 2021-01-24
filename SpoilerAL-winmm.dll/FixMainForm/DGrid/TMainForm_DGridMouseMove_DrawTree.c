#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_DGridMouseMove_DrawTree()
{
	__asm
	{
		#define _NULL         0
		#define this          ebx

		cmp     [this]TMainForm.userMode, 4
		sbb     eax, eax
		inc     eax
#if DRAWTREE_CONVENTION
		push    eax
		push    0
#else
		ror     eax, 1
		push    eax
#endif
		push    0
		push    _NULL
		push    this
		call    TMainForm_DrawTree
#if !DRAWTREE_CONVENTION
		add     esp, 16
#endif
		ret

		#undef _NULL
		#undef this
	}
}
