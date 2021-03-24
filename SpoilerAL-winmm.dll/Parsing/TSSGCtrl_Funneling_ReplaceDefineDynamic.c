#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

void __stdcall ByteArrayReplaceDefine(void *SSGS, void *line);

extern BOOL EnableParserFix;

#if 0
__declspec(naked) void __cdecl TSSGCtrl_Funneling_ReplaceDefineDynamic()
{
	__asm
	{
		#define Src  eax
		#define SSGS (ebp + 0CH)

		cmp     dword ptr [EnableParserFix], 0
		je      L1
		sub     esp, 24
		mov     edx, Src
		mov     ecx, esp
		call    string_ctor_assign
		push    esp
		push    dword ptr [SSGS]
		call    ByteArrayReplaceDefine
		mov     eax, esp
		mov     edx, dword ptr [eax + 24 + 4]
		movups xmm0, [eax + 24 + 12]
		movups xmm1, [eax + 24 + 12 + 16]
		sub     esp, 16
		movups[esp], xmm1
		sub     esp, 16
		movups[esp], xmm0
		push    eax
		push    edx
		call    TStringDivision_List
		add     esp, 40
		mov     ecx, esp
		push    eax
		call    string_dtor
		mov     eax, dword ptr [esp]
		add     esp, 4 + 24
		ret

		align   16
	L1:
		jmp     TStringDivision_List

		#undef Src
		#undef SSGS
	}
}
#endif
