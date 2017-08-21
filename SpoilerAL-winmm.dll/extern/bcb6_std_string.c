#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_allocator.h"
#include <stdbool.h>

#pragma function(strlen)

string *(__cdecl *string_concat)(string *s, const string *left, const string *right) = (LPVOID)0x00415C8C;

void __fastcall string_dtor(string *s)
{
	allocator_deallocate(string_begin(s), string_array_capacity(s));
}

string * __fastcall string_ctor(string *s)
{
	*(string_end(s) = string_begin(s) = (char *)allocator_allocate(8)) = '\0';
	string_end_of_storage(s) = string_begin(s) + 8;
	return s;
}

__declspec(naked) string * __fastcall string_ctor_assign(string *s, const string *src)
{
	__asm
	{
		pop     eax
		push    eax
		push    eax
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		sub     eax, edx
		mov     dword ptr [esp + 4], eax
		jmp     string_ctor_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_ctor_assign_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jae     L1
		mov     dword ptr [esp + 4], 0
	L1:
		jmp     string_ctor_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_ctor_assign_cstr(string *s, LPCSTR src)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    eax
		push    edx
		push    ecx
		push    edx
		call    strlen
		mov     dword ptr [esp + 16], eax
		pop     eax
		pop     ecx
		pop     edx
		jmp     string_ctor_assign_cstr_with_length
	}
}

string * __fastcall string_ctor_assign_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	char *p = (char *)allocator_allocate(length + 1);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + length + 1;
	memcpy(p, src, length);
	*string_end(s) = '\0';
	return s;
}

__declspec(naked) string * __fastcall string_assign(string *s, const string *src)
{
	__asm
	{
		pop     eax
		push    eax
		push    eax
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		sub     eax, edx
		mov     dword ptr [esp + 4], eax
		jmp     string_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_assign_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jae     L1
		mov     dword ptr [esp + 4], 0
	L1:
		jmp     string_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_assign_cstr(string *s, LPCSTR src)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    eax
		push    edx
		push    ecx
		push    edx
		call    strlen
		mov     dword ptr [esp + 16], eax
		pop     eax
		pop     ecx
		pop     edx
		jmp     string_assign_cstr_with_length
	}
}

string * __fastcall string_assign_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	size_t array_capacity = string_array_capacity(s);
	if (length >= array_capacity)
	{
		bool is_internal;
		if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
			src -= (size_t)string_begin(s);
		char *p = (char *)allocator_reallocate(string_begin(s), array_capacity, length + 1);
		string_begin(s) = p;
		string_end_of_storage(s) = p + length + 1;
		if (is_internal)
			src += (size_t)string_begin(s);
	}
	string_end(s) = string_begin(s) + length;
	memmove(string_begin(s), src, length);
	*string_end(s) = '\0';
	return s;
}

static void __fastcall append_storage(string *s, size_t n)
{
	if (string_end(s) + n >= string_end_of_storage(s))
	{
		size_t length = string_length(s);
		size_t array_capacity = length + 1;
		array_capacity += max(array_capacity, n);
		char *p = (char *)allocator_reallocate(string_begin(s), string_array_capacity(s), array_capacity);
		string_begin(s) = p;
		string_end(s) = p + length;
		string_end_of_storage(s) = p + array_capacity;
	}
}

string * __fastcall string_append(string *s, size_t n, char c)
{
	append_storage(s, n);
	char *p = string_end(s);
	string_end(s) = p + n;
	memset(p, c, n);
	*string_end(s) = '\0';
	return s;
}

string * __fastcall string_append_range(string *s, LPCSTR first, LPCSTR last)
{
	if (last > first)
	{
		size_t n = last - first;
		bool is_internal;
		if (is_internal = (first >= string_begin(s) && last <= string_end(s)))
			first -= (size_t)string_begin(s);
		append_storage(s, n);
		if (is_internal)
			first += (size_t)string_begin(s);
		char *p = string_end(s);
		string_end(s) = p + n;
		memmove(p, first, n);
		*string_end(s) = '\0';
	}
	return s;
}

void __fastcall string_reserve(string *s, size_t n)
{
	size_t array_capacity = string_array_capacity(s);
	size_t require = n + 1;
	if (require != array_capacity)
	{
		size_t length = string_length(s);
		if (length > n)
			*(string_begin(s) + (length = n)) = '\0';
		char *p = (char *)allocator_reallocate(string_begin(s), array_capacity, require);
		string_begin(s) = p;
		string_end(s) = p + length;
		string_end_of_storage(s) = p + require;
	}
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
	size_t array_capacity = string_array_capacity(s);
	size_t size = string_length(s) + 1;
	if (size != array_capacity)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), array_capacity, size--);
		string_begin(s) = p;
		string_end(s) = p += size;
		string_end_of_storage(s) = p + 1;
	}
}
