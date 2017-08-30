#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_allocator.h"
#include <stdbool.h>

#pragma function(strlen)

string * __cdecl new_string()
{
	string *s = (string *)operator_new(sizeof(string));
	string_ctor(s);
	return s;
}

void __fastcall delete_string(string *s)
{
	if (s)
	{
		string_dtor(s);
		operator_delete(s);
	}
}

void __fastcall string_dtor(string *s)
{
	allocator_deallocate(string_begin(s), string_storage_capacity(s));
}

string * __fastcall string_ctor(string *s)
{
	char *p = (char *)allocator_allocate(8);
	string_begin(s) = p;
	*(string_end(s) = p) = '\0';
	string_end_of_storage(s) = p + 8;
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
	size_t capacity = string_storage_capacity(s);
	if (length >= capacity)
	{
		bool is_internal;
		if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
			src -= (size_t)string_begin(s);
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, length + 1);
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

static void __fastcall storage_append(string *s, size_t n)
{
	size_t length = string_length(s);
	size_t capacity = length + 1;
	capacity += max(capacity, n);
	char *p = (char *)allocator_reallocate(string_begin(s), string_storage_capacity(s), capacity);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + capacity;
}

string * __fastcall string_append_repeat_char(string *s, size_t n, char c)
{
	if (string_end(s) + n >= string_end_of_storage(s))
		storage_append(s, n);
	char *p = string_end(s);
	string_end(s) = p + n;
	memset(p, c, n);
	*string_end(s) = '\0';
	return s;
}

__declspec(naked) string * __fastcall string_append_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jbe     L1
		jmp     string_append_cstr_with_length
	L1:
		mov     eax, ecx
		ret
	}
}

string * __fastcall string_append_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	bool is_internal;
	if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
		src -= (size_t)string_begin(s);
	if (string_end(s) + length >= string_end_of_storage(s))
		storage_append(s, length);
	if (is_internal)
		src += (size_t)string_begin(s);
	char *p = string_end(s);
	string_end(s) = p + length;
	memmove(p, src, length);
	*string_end(s) = '\0';
	return s;
}

string * __fastcall string_concat(string *s, const string *left, const string *right)
{
	size_t leftLength = string_length(left);
	size_t rightLength = string_length(right);
	size_t length = leftLength + rightLength;
	char *p = (char *)allocator_allocate(length + 1);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + length + 1;
	memcpy(p, string_begin(left), leftLength);
	memcpy(p + leftLength, string_begin(right), rightLength);
	*string_end(s) = '\0';
	return s;
}

void __fastcall string_reserve(string *s, size_t n)
{
	size_t capacity = string_storage_capacity(s);
	size_t require = n + 1;
	if (require != capacity)
	{
		size_t length = string_length(s);
		if (length > n)
			*(string_begin(s) + (length = n)) = '\0';
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, require);
		string_begin(s) = p;
		string_end(s) = p + length;
		string_end_of_storage(s) = p + require;
	}
}

void __fastcall string_resize(string *s, size_t n)
{
	char *end = string_begin(s) + n;
	if (end > string_end(s))
		string_append_repeat_char(s, end - string_end(s), '\0');
	else
		*(string_end(s) = end) = '\0';
}

void __fastcall string_storage_append(string *s, size_t n)
{
	if (string_end(s) + n >= string_end_of_storage(s))
		storage_append(s, n);
}

void __fastcall string_storage_resize(string *s, size_t n)
{
	char *p = string_begin(s) + n;
	if (p >= string_end_of_storage(s))
		storage_append(s, p - string_end(s));
}

void __fastcall string_shrink_to_fit(string *s)
{
	size_t capacity = string_storage_capacity(s);
	size_t size = string_length(s) + 1;
	if (size != capacity)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, size--);
		string_begin(s) = p;
		string_end(s) = p += size;
		string_end_of_storage(s) = p + 1;
	}
}

void __fastcall string_trim(string *s)
{
	char *begin, *end;

	begin = string_begin(s);
	while (__intrinsic_isspace(*begin))
		begin++;
	end = string_end(s);
	while (--end > begin && __intrinsic_isspace(*end));
	if (++end != string_end(s) || begin != string_begin(s))
	{
		size_t length;

		length = end - begin;
		if (begin != string_begin(s))
			memcpy(string_begin(s), begin, length);
		*(string_end(s) = string_begin(s) + length) = '\0';
	}
}
