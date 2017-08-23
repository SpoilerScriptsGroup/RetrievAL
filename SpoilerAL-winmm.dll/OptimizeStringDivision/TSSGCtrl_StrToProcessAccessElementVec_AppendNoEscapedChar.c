#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"

__declspec(naked) void __cdecl TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar()
{
	__asm
	{
#if CODEPAGE_SUPPORT
		movzx   eax, byte ptr [esi]
		push    005072DCH
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     dl, byte ptr [esi]
#else
		mov     dl, byte ptr [esi]
		push    005072DCH
		cmp     dl, 81H
		jb      L2
		cmp     dl, 9FH
		jbe     L1
		cmp     dl, 0E0H
		jb      L2
		cmp     dl, 0FCH
		ja      L2
	L1:
#endif
		mov     ecx, dword ptr [ebp - 618H]
		call    vector_byte_push_back
		inc     esi
	L2:
		ret
	}
}
