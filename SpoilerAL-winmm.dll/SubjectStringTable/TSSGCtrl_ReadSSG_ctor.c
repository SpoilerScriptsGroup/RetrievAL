#include "SubjectStringTable.h"

extern const DWORD __InitExceptBlockLDTC;
#if 0
__declspec(naked) void __cdecl TSSGCtrl_ReadSSG_ctor()
{
	__asm
	{
		call    SubjectStringTable_clear
		mov     eax, 00633D68H
		jmp     dword ptr [__InitExceptBlockLDTC]
	}
}
#endif
