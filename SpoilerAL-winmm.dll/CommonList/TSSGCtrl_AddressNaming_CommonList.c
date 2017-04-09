#include <windows.h>

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_CommonList()
{
	__asm
	{
		#define Src     (esp + 4)
		#define Default (esp + 8)

		mov     eax, dword ptr [Src]
		lea     ecx, [Default]
		mov     eax, dword ptr [eax]
		push    0
		push    ecx
		push    eax
		call    strtoul
		mov     ecx, dword ptr [Src     + 12]
		mov     edx, dword ptr [Default + 12]
		mov     ecx, dword ptr [ecx]
		add     esp, 12
		cmp     ecx, edx
		jne     L1
		add     esp, 4 + 8
		push    1
		push    005059D0H
	L1:
		ret

		#undef Src
		#undef Default
	}
}
