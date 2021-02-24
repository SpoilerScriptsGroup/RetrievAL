#include <windows.h>
#include "bcb6_std_string.h"

void __stdcall ByteArrayReplaceDefine(void *SSGS, void *line);

extern BOOL EnableParserFix;

__declspec(naked) char*__cdecl TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine()
{
	__asm
	{
		#define SSGS (ebp + 10H)
		#define Code (ebp + 14H)

		cmp     EnableParserFix, FALSE
		je      L1
		lea     edx, [Code]
		push    edx
		push    dword ptr [SSGS]
		call    ByteArrayReplaceDefine
	L1:
		mov     eax, [Code]bcb6_std_string._M_start
		ret

		#undef SSGS
		#undef Code
	}
}
