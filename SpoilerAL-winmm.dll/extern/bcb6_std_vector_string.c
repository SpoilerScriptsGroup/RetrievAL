#include "bcb6_std_vector_string.h"
#include "bcb6_std_allocator.h"

void(__cdecl *_bcb6_std_vector_string_deallocate)(bcb6_std_string *first, bcb6_std_string *last, ULONG64 *outReserved) = (LPVOID)0x00415F90;
void(__cdecl *_bcb6_std_vector_string_destroy)(bcb6_std_string *last, bcb6_std_string *vectorEnd, bcb6_std_string *first, ULONG64 *outReserved, DWORD zeroReserved) = (LPVOID)0x00449D18;
void(__cdecl *bcb6_std_vector_string_reserve)(bcb6_std_vector_string *s, size_t n) = (LPVOID)0x00473564;

__declspec(naked) void __fastcall bcb6_std_vector_string_dtor(bcb6_std_vector_string *v)
{
	__asm
	{
		mov     edx, dword ptr [ecx + 4]
		mov     ecx, dword ptr [ecx]
		test    ecx, ecx
		jz      L1
		push    edx
		push    ecx
		sub     esp, 8
		push    esp
		push    edx
		push    ecx
		call    dword ptr [_bcb6_std_vector_string_deallocate]
		mov     ecx, dword ptr [esp + 12 + 8]
		mov     edx, dword ptr [esp + 12 + 8 + 4]
		add     esp, 12 + 8 + 8
		sub     edx, ecx
		jmp     bcb6_std_allocator_deallocate
	L1:
		ret
	}
}
