#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSToggle.h"
#include "bcb6_operator.h"
#include "SSGSubjectProperty.h"

#define MakeSubjectClass_JumpBySubjectTypeTable 0x004EC41F

__declspec(naked) TSSToggle * __cdecl new_TSSToggle()
{
	extern const DWORD F005D54CC;

	__asm
	{
		push    ebp
		mov     eax, 006354ACH
		mov     ebp, esp
		sub     esp, 136
		push    ebx
		push    esi
		mov     ecx, dword ptr cs:[MakeSubjectClass_JumpBySubjectTypeTable + stTOGGLE * 4]
		lea     ebx, [ebp - 124]
		push    edi
		push    ecx
		jmp     dword ptr [F005D54CC]
	}
}

