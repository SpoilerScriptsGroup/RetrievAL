#include "TSSToggle.h"

#define MakeSubjectClass_SwitchSubjectTypeTable 0x004EC41F

__declspec(naked) TSSToggle * __cdecl new_TSSToggle()
{
	extern const DWORD __InitExceptBlockLDTC;

	__asm
	{
		push    ebp
		mov     eax, 006354ACH
		mov     ebp, esp
		sub     esp, 136
		push    ebx
		push    esi
		mov     ecx, dword ptr ds:[MakeSubjectClass_SwitchSubjectTypeTable + stTOGGLE * 4]
		lea     ebx, [ebp - 124]
		push    edi
		push    ecx
		jmp     dword ptr [__InitExceptBlockLDTC]
	}
}

