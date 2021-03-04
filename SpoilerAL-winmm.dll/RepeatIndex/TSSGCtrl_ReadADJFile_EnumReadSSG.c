#include "TSSGCtrl.h"

extern DWORD RepeatDepth;

#ifdef ADJ_USE_JMP
__declspec(naked) void __cdecl TSSGCtrl_ReadADJFile_EnumReadSSG()
{
	__asm
	{
		#define this        esi
		#define ADJElem     (ebp +  0CH)
		#define ADJ         (ebp - 110H)
		#define ParentStack (ebp - 0B8H)

		mov     edx, dword ptr [ADJElem]
		lea     ecx, [ParentStack]
		mov     eax, dword ptr [ADJ]
		push    -1
		push    0
		push    edx
		push    ecx
		push    eax
		push    this
		push    004FD6F6H
		jmp     TSSGCtrl_EnumReadSSG

		#undef this
		#undef ADJElem
		#undef ADJ
		#undef ParentStack
	}
}
#endif
