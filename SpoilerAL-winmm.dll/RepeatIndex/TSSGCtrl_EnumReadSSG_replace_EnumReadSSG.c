#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_replace_EnumReadSSG()
{
	__asm
	{
		#define this         (ebp +   8H)
		#define ParentStack  (ebp +  10H)
		#define ADJElem      (ebp +  14H)
		#define RepeatIndex  (ebp +  18H)
		#define ParentRepeat (ebp +  1CH)
		#define tmpL         (ebp - 904H)

		mov     ecx, dword ptr [ParentRepeat]
		mov     eax, dword ptr [RepeatIndex]
		push    ecx
		push    eax
		mov     ecx, dword ptr [ADJElem]
		mov     eax, dword ptr [ParentStack]
		push    ecx
		push    eax
		mov     ecx, dword ptr [tmpL]
		mov     eax, dword ptr [this]
		push    ecx
		push    eax
		push    004E66CFH
		jmp     TSSGCtrl_EnumReadSSG

		#undef this
		#undef ParentStack
		#undef ADJElem
		#undef RepeatIndex
		#undef ParentRepeat
		#undef tmpL
	}
}
