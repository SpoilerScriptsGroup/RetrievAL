#include "TSSGCtrl.h"
#include "TSSArg.h"

__declspec(naked) unsigned long __stdcall TSSDir_WriteChildren_ErrorSkip(TSSGSubject *SSGS, TSSGCtrl *SSGC, TSSArg *Arg)
{
	__asm
	{
		#define SSGC          edi
		#define VIt           ebx

		mov     edx, dword ptr [VIt]
		cmp     [edx]TSSGSubject.type, stSPLIT
		je      SPLIT

		mov     ecx, eax
		neg     ecx
		sbb     ecx, ecx
		inc     ecx
		jnz     REVERT

		push    eax
		push    atON_ERROR
		push    edx
		push    SSGC
		call    TSSGCtrl_GetAttribute
		add     esp, 12
		mov     ecx, eax
		pop     eax
		jmp     REVERT

	SPLIT:
		push    atCHILD_RW
		push    edx
		push    SSGC
		call    TSSGCtrl_GetAttribute
		add     esp, 12// eax is never null here
		movzx   ecx, [eax]TChildRWAttribute.spIgnore
		mov     eax, reNO_ERROR
	REVERT:
		ret     12// break if ecx is 0


		#undef VIt
		#undef SSGC
	}
}
