#include <windows.h>

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSR_SwitchCommonList()
{
	__asm
	{
		mov     ecx, 00500C13H
		mov     edx, 005009AFH
		cmp     al, '+' - 'R'
		je      L1
		cmp     al, '*' - 'R'
		je      L1
		jmp     ecx

		align   16
	L1:
		push    -1
		push    0
		jmp     edx
	}
}
