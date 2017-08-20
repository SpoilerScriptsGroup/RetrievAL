#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_allocator.h"

#pragma function(strlen)

#define _string_assign_range     _bcb6_std_string_assign_range
#define _string_ctor_assign_cstr _bcb6_std_string_ctor_assign_cstr
#define _string_append_range     _bcb6_std_string_append_range
#define _string_append           _bcb6_std_string_append

void(__cdecl *string_ctor)(string *s) = (LPVOID)0x00402590;
void(__cdecl *_string_assign_range)(string *dest, LPCSTR first, LPCSTR last, ULONG64 *outReserved) = (LPVOID)0x004159D4;
void(__cdecl *string_concat)(string *dest, const string *left, const string *right) = (LPVOID)0x00415C8C;
void(__cdecl *_string_ctor_assign_cstr)(string *dest, LPCSTR src, ULONG64 *outReserved) = (LPVOID)0x004165E8;
void(__cdecl *string_ctor_assign)(string *dest, const string *src) = (LPVOID)0x004166F0;
void(__cdecl *_string_append_range)(string *s, LPCSTR first, LPCSTR last, ULONG64 *outReserved) = (LPVOID)0x00418610;
void(__cdecl *string_allocate)(string *s, size_t n) = (LPVOID)0x004462DC;
string *(__cdecl *_string_append)(string *s, size_t n, char c) = (LPVOID)0x00462FFC;

extern const DWORD F005D54CC;

void __fastcall string_dtor(string *s)
{
	allocator_deallocate(string_begin(s), string_end_of_storage(s) - string_begin(s));
}

__declspec(naked) void __fastcall string_ctor_assign_cstr(string *dest, LPCSTR src)
{
	__asm
	{
		sub     esp, 8
		push    esp
		push    edx
		push    ecx
		call    dword ptr[_string_ctor_assign_cstr]
		add     esp, 12 + 8
		ret
	}
}

__declspec(naked) void __fastcall string_ctor_assign_cstr_with_length(string *dest, LPCSTR src, size_t length)
{
	__asm
	{
		sub     esp, 8
		mov     eax, 006076C8H
		push    esp
		push    edx
		push    ecx
		push    offset L1
		push    ebp
		mov     ebp, esp
		sub     esp, 36
		push    ebx
		push    esi
		push    edi
		mov     ebx, edx
		call    dword ptr [F005D54CC]
		mov     eax, dword ptr [ebp + 8H]
		xor     ecx, ecx
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [eax + 10H], ecx
		mov     word ptr [ebp - 14H], 8
		mov     edx, dword ptr [ebp - 8H]
		mov     ecx, dword ptr [ebp + 20H]
		add     edx, 3
		lea     esi, [ebx + ecx]
		mov     dword ptr [ebp - 8H], edx
		inc     ecx
		mov     edx, 0041668CH
		push    ecx
		push    eax
		push    00416643H
		jmp     edx
		align   16
	L1:
		add     esp, 20
		ret     4
	}
}

__declspec(naked) void __fastcall string_ctor_assign_range(string *dest, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     esp, 8
		mov     eax, 006076C8H
		push    esp
		push    edx
		push    ecx
		push    offset L1
		push    ebp
		mov     ebp, esp
		sub     esp, 36
		push    ebx
		push    esi
		push    edi
		mov     ebx, edx
		call    dword ptr [F005D54CC]
		mov     eax, dword ptr [ebp + 8H]
		xor     ecx, ecx
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4H], ecx
		mov     dword ptr [eax + 10H], ecx
		mov     word ptr [ebp - 14H], 8
		mov     edx, dword ptr [ebp - 8H]
		mov     esi, dword ptr [ebp + 20H]
		add     edx, 3
		lea     ecx, [esi + 1]
		mov     dword ptr [ebp - 8H], edx
		sub     ecx, ebx
		mov     edx, 0041668CH
		push    ecx
		push    eax
		push    00416643H
		jmp     edx
		align   16
	L1:
		add     esp, 20
		ret     4
	}
}

__declspec(naked) void __fastcall string_assign(string *dest, const string *src)
{
	__asm
	{
		sub     esp, 8
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		push    esp
		push    eax
		push    edx
		push    ecx
		call    dword ptr [_string_assign_range]
		add     esp, 24
		ret
	}
}

__declspec(naked) void __fastcall string_assign_range(string *dest, LPCSTR first, LPCSTR last)
{
	__asm
	{
		mov     eax, dword ptr[esp + 4]
		sub     esp, 8
		push    esp
		push    eax
		push    edx
		push    ecx
		call    dword ptr [_string_assign_range]
		add     esp, 16 + 8
		ret     4
	}
}

__declspec(naked) void __fastcall string_assign_cstr(string *dest, LPCSTR src)
{
	__asm
	{
		push    edx
		push    ecx
		push    edx
		call    strlen
		mov     edx, dword ptr [esp + 8]
		pop     ecx
		add     eax, edx
		mov     ecx, dword ptr [esp]
		push    esp
		push    eax
		push    edx
		push    ecx
		call    dword ptr [_string_assign_range]
		add     esp, 16 + 8
		ret
	}
}

__declspec(naked) void __fastcall string_assign_cstr_with_length(string *dest, LPCSTR src, size_t length)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]
		sub     esp, 8
		add     eax, edx
		push    esp
		push    eax
		push    edx
		push    ecx
		call    dword ptr [_string_assign_range]
		add     esp, 24
		ret     4
	}
}

static void __fastcall append_storage(string *s, size_t n)
{
	if (n && string_end(s) + n >= string_end_of_storage(s))
	{
		size_t length = string_length(s);
		size_t capacity = length + 1;
		capacity += max(capacity, n);
		char *p = (char *)allocator_reallocate(string_begin(s), string_end_of_storage(s) - string_begin(s), capacity);
		string_begin(s) = p;
		string_end(s) = p + length;
		string_end_of_storage(s) = p + capacity;
	}
}

string * __fastcall string_append(string *s, size_t n, char c)
{
	append_storage(s, n);
	memset(string_end(s), c, n);
	*(string_end(s) += n) = '\0';
	return s;
}

void __fastcall string_append_range(string *s, LPCSTR first, LPCSTR last)
{
	size_t n = last - first;
	append_storage(s, n);
	memcpy(string_end(s), first, n);
	*(string_end(s) += n) = '\0';
}

void __fastcall string_resize(string *s, size_t length)
{
	char *end = string_begin(s) + length;
	if (end > string_end(s))
		string_append(s, end - string_end(s), '\0');
	else
		*(string_end(s) = end) = '\0';
}

void __fastcall string_shrink_to_fit(string *s)
{
	size_t capacity = string_end_of_storage(s) - string_begin(s);
	size_t length = string_length(s);
	size_t size = length + 1;
	if (size != capacity)
		string_end_of_storage(s) = (string_end(s) = (char *)(string_begin(s) = allocator_reallocate(string_begin(s), capacity, size)) + length) + 1;
}
