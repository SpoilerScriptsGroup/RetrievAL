#include <windows.h>

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_CommonList()
{
	__asm
	{
		#define CallAddress 0043CC08H
		#define tmpS        (esp +  4)
		#define begin       (esp +  8)
		#define end         (esp + 12)
		#define unknown     (esp + 16)
		#define prefix      (esp + 20)

		mov     al, byte ptr [prefix]
		mov     ecx, dword ptr [begin]
		cmp     al, '+'
		je      L1
		cmp     al, '*'
		je      L4
		mov     eax, CallAddress
		jmp     eax

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jz      L3
		cmp     al, '='
		jne     L1

		align   16
	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L2
	L3:
		dec     ecx
		mov     eax, CallAddress
		mov     dword ptr [begin], ecx
		jmp     eax

		align   16
	L4:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jz      L5
		cmp     al, '='
		jne     L4
		mov     edx, dword ptr [begin]
		sub     ecx, 2
		jmp     L6

	L5:
		dec     ecx
		jmp     L8

		align   16
	L6:
		cmp     ecx, edx
		jb      L7
		mov     al, byte ptr [ecx]
		dec     ecx
		cmp     al, ' '
		je      L6
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L6
		inc     ecx
	L7:
		inc     ecx
	L8:
		mov     eax, CallAddress
		mov     dword ptr [end], ecx
		jmp     eax
	}
}
