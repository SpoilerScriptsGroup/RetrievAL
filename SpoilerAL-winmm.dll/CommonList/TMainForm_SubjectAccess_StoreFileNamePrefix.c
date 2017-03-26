#include <windows.h>

EXTERN_C void * __cdecl A_memcpy(void *dest, const void *src, size_t count);

__declspec(naked) void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix()
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]
		mov     edx, dword ptr [esp + 16]
		mov     cl, byte ptr [eax]
		push    004EDFB8H
		cmp     cl, '+'
		je      L1
		mov     byte ptr [esp + 4 + 64], cl
		ret
	L1:
		sub     edx, eax
		mov     byte ptr [esp + 4 + 64], cl
		push    edx
		lea     ecx, [eax + 1]
		add     edx, eax
		push    ecx
		dec     edx
		push    eax
		mov     dword ptr [esp + 16 + 16], edx
		call    A_memcpy
		add     esp, 12
		ret
	}
}
