#include "TSSGCtrl.h"

extern DWORD RepeatDepth;

__declspec(naked) void __cdecl TSSGCtrl_ReadSSG_EnumReadSSG()
{
	__asm
	{
		#define _NULL       0
		#define _this       ebx
		#define SSGFile     (ebp + 0CH)
		#define ParentStack edi

		mov     dword ptr [RepeatDepth], 0
		mov     eax, dword ptr [SSGFile]
		push    -1
		push    0
		push    _NULL
		push    ParentStack
		push    eax
		push    _this
		push    004E4708H
		jmp     dword ptr [TSSGCtrl_EnumReadSSG]

		#undef _NULL
		#undef _this
		#undef SSGFile
		#undef ParentStack
	}
}
