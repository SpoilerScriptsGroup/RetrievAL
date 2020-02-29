#include "TSSGCtrl.h"
#include "TSSGAttributeElement.h"

__declspec(naked) void __cdecl TSSGActionListner_OnSubjectWriteError_ErrorSkip()
{
	__asm
	{
		#define _NULL 0
		#define SSGS  (ebp + 12)

		push    eax
		mov     ecx, dword ptr [SSGS]
		push    atON_ERROR
		push    ecx
		push    _NULL
		call    dword ptr [TSSGCtrl_GetAttribute]
		test    eax, eax
		jnz     L1
		add     esp, 12
		mov     ecx, 0057AEDCH
		pop     eax
		jmp     ecx

	L1:
		add     esp, 12 + 4
		ret

		#undef _NULL
		#undef SSGS
	}
}
