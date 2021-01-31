#include <windows.h>

void __stdcall ByteArrayReplaceDefine(void *SSGS, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_StrToProcessAccessElementVec_ByteArrayReplaceDefine()
{
	__asm
	{
		#define SSGS (ebp + 10H)
		#define Code (ebp + 14H)

		cmp     EnableParserFix, FALSE
		je      L1
		mov     eax, dword ptr [SSGS]
		lea     ecx, [Code]
		push    ecx
		push    eax
		call    ByteArrayReplaceDefine
	L1:
		mov     eax, dword ptr [Code]
		movzx   eax, byte  ptr [eax]
		ret

		#undef SSGS
		#undef Code
	}
}
