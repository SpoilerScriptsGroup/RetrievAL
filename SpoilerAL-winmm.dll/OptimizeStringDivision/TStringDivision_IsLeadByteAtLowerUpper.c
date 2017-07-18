#include <windows.h>
#include "intrinsic.h"

__declspec(naked) void __cdecl TStringDivision_IsLeadByteAtLowerUpper()
{
	__asm
	{
#if CODEPAGE_SUPPORT
		movzx   edx, byte ptr [ebx]
		pop     eax
		push    edx
		push    CP_THREAD_ACP
		push    eax
		jmp     IsDBCSLeadByteEx
#else
		mov     dl, byte ptr [ebx]
		xor     eax, eax
		cmp     dl, 81H
		jb      L2
		cmp     dl, 9FH
		jbe     L1
		cmp     dl, 0E0H
		jb      L2
		cmp     dl, 0FCH
		ja      L2
	L1:
		inc     eax
	L2:
		ret
#endif
	}
}
