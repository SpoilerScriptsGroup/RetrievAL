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
	char *p = (char *)node_alloc_allocate(8);
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
	char *p = (char *)internal_allocate(length + 1);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + length + 1;
	memcpy(p, src, length);
	*string_end(s) = '\0';
	return s;
}

bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_char(bcb6_std_string *s, const char c)
{
	char *p = (char *)node_alloc_allocate(2);
	*(string_begin(s) = p) = c;
	*(string_end(s) = p + 1) = '\0';
	string_end_of_storage(s) = p + 2;
	return s;
}

bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_wchar(bcb6_std_string *s, const wchar_t c)
{
	char *p = (char *)node_alloc_allocate(3);
	*(wchar_t *)(string_begin(s) = p) = c;
	*(string_end(s) = p + 2) = '\0';
	string_end_of_storage(s) = p + 3;
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

string * __fastcall string_assign_char(string *s, const char c)
{
	size_t capacity = string_storage_capacity(s);
	if (capacity < 2)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, 2);
		string_begin(s) = p;
		string_end_of_storage(s) = p + 2;
	}
	*string_begin(s) = c;
	*(string_end(s) = string_begin(s) + 1) = '\0';
	return s;
}

string * __fastcall string_assign_wchar(string *s, const wchar_t c)
{
	size_t capacity = string_storage_capacity(s);
	if (capacity < 3)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, 3);
		string_begin(s) = p;
		string_end_of_storage(s) = p + 3;
	}
	*(wchar_t *)string_begin(s) = c;
	*(string_end(s) = string_begin(s) + 2) = '\0';
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

__declspec(naked) string * __fastcall string_append(string *s, const string *src)
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
		jmp     string_append_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_append_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jz      L1
		jmp     string_append_cstr_with_length
	L1:
		mov     eax, ecx
		ret     4
	}
}

string * __fastcall string_append_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	if (string_end(s) + length >= string_end_of_storage(s))
	{
		bool is_internal;
		if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
			src -= (size_t)string_begin(s);
		storage_append(s, length);
		if (is_internal)
			src += (size_t)string_begin(s);
	}
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
	char *p = (char *)internal_allocate(length + 1);
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

#ifndef _M_IX86
string * __fastcall string_trim(string *s)
{
	char *begin, *end;

	begin = string_begin(s);
	end = string_end(s);
	if (end > begin)
	{
		char c;

		for (; ; )
		{
			c = *(--end);
			if (!__intrinsic_isspace(c))
				break;
			if (end == begin)
				goto TRIMED;
		}
		do
			c = *(begin++);
		while (__intrinsic_isspace(c));
		--begin;
		++end;
		if (begin != string_begin(s))
		{
			size_t length;

			length = end - begin;
			end = string_begin(s) + length;
			memcpy(string_begin(s), begin, length);
		}
	TRIMED:
		*(string_end(s) = end) = '\0';
	}
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall string_trim(string *s)
{
	__asm
	{
		#define s ecx

		push    ebx
		mov     edx, dword ptr [ecx]
		mov     ebx, dword ptr [ecx + 4]
		cmp     ebx, edx
		jbe     L6
	L1:
		mov     al, byte ptr [ebx - 1]
		dec     ebx
		cmp     al, ' '
		je      L2
		cmp     al, '\r'
		ja      L3
		cmp     al, '\t'
		jb      L3
	L2:
		cmp     ebx, edx
		jne     L1
		jmp     L5

		align   16
	L3:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\r'
		ja      L4
		cmp     al, '\t'
		jae     L3
	L4:
		dec     edx
		inc     ebx
		mov     eax, dword ptr [ecx]
		cmp     eax, edx
		je      L5
		sub     ebx, edx
		push    ecx
		push    ebx
		push    edx
		push    eax
		add     ebx, eax
		call    memcpy
		mov     ecx, dword ptr [esp + 12]
		add     esp, 16
	L5:
		mov     byte ptr [ebx], '\0'
		mov     dword ptr [ecx + 4], ebx
	L6:
		mov     eax, ecx
		pop     ebx
		ret

		#undef s
	}
}
#endif
