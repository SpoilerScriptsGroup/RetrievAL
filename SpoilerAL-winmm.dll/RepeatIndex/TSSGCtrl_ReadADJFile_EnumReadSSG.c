#include "TSSGCtrl.h"

extern DWORD RepeatDepth;

__declspec(naked) void __cdecl TSSGCtrl_ReadADJFile_EnumReadSSG()
{
	__asm
	{
		#define _this       esi
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
		push    _this
		push    004FD6F6H
		jmp     dword ptr [TSSGCtrl_EnumReadSSG]

		#undef _this
		#undef ADJElem
		#undef ADJ
		#undef ParentStack
	}
}
