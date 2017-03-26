#pragma once

#include <windows.h>
#include "intrinsic.h"

typedef struct
{
	LPSTR  _M_start;
	LPSTR  _M_finish;
	LPVOID padding1;
	LPVOID padding2;
	LPSTR  _M_end_of_storage;
	LPVOID padding3;
} bcb6_std_string, *pbcb6_std_string;

typedef void (__cdecl *LPFN_BCB6_STD_STRING_CTOR)(bcb6_std_string *s);
#define bcb6_std_string_ctor ((LPFN_BCB6_STD_STRING_CTOR)0x00402590)

typedef void (__cdecl *BCB6_STD_STRING_CTOR_ASSIGN)(bcb6_std_string *dest, bcb6_std_string *src);
#define bcb6_std_string_ctor_assign ((BCB6_STD_STRING_CTOR_ASSIGN)0x004166F0)

EXTERN_C void __fastcall bcb6_std_string_ctor_assign_cstr(bcb6_std_string *dest, LPCSTR src);
EXTERN_C void __fastcall bcb6_std_string_ctor_assign_cstr_with_length(bcb6_std_string *dest, LPCSTR src, size_t length);
EXTERN_C void __fastcall bcb6_std_string_dtor(bcb6_std_string *s);

typedef void (__cdecl *BCB6_STD_STRING_ALLOCATOR)(bcb6_std_string *s, size_t n);
#define bcb6_std_string_allocator ((BCB6_STD_STRING_ALLOCATOR)0x004462DC)

__inline void bcb6_std_string_reserve(bcb6_std_string *s, size_t n)
{
	if (n >= (size_t)(s->_M_end_of_storage - s->_M_start))
	{
		if ((INT_PTR)n > 0)
		{
			unsigned long bits;

#ifndef _WIN64
			_BitScanReverse(&bits, n);
#else
			_BitScanReverse64(&bits, n);
#endif
			n = ((size_t)1 << (bits + 1)) - 1;
		}
		bcb6_std_string_allocator(s, n);
	}
}

typedef bcb6_std_string *(__cdecl *LPFN_BCB6_STD_STRING_APPEND)(bcb6_std_string *s, size_t n, char c);
#define bcb6_std_string_append ((LPFN_BCB6_STD_STRING_APPEND)0x00462FFC)

#define bcb6_std_string_c_str(s)  (LPCSTR)(s)->_M_start
#define bcb6_std_string_begin(s)  (s)->_M_start
#define bcb6_std_string_end(s)    (s)->_M_finish
#define bcb6_std_string_length(s) (size_t)((s)->_M_finish - (s)->_M_start)
#define bcb6_std_string_empty(s)  ((s)->_M_finish == (s)->_M_start)
#define bcb6_std_string_clear(s)  (*((s)->_M_finish = (s)->_M_start) = '\0')

EXTERN_C bcb6_std_string * __cdecl new_bcb6_std_string();
EXTERN_C void __fastcall delete_bcb6_std_string(bcb6_std_string *s);
EXTERN_C void __fastcall bcb6_std_string_assign(bcb6_std_string *dest, const bcb6_std_string *src);
EXTERN_C void __stdcall bcb6_std_string_assign_cstr(bcb6_std_string *dest, LPCSTR src);
EXTERN_C void __fastcall bcb6_std_string_assign_cstr_with_length(bcb6_std_string *dest, LPCSTR src, size_t length);
EXTERN_C void __fastcall bcb6_std_string_resize(bcb6_std_string *s, size_t length);

__inline void bcb6_std_string_assign_char(bcb6_std_string *s, char c)
{
	bcb6_std_string_reserve(s, 1);
	*s->_M_start = c;
	*(s->_M_finish = s->_M_start + 1) = '\0';
}

__inline BOOL bcb6_std_string_equals(bcb6_std_string *s1, bcb6_std_string *s2)
{
	size_t length;

	length = bcb6_std_string_length(s1);
	return
		bcb6_std_string_length(s2) == length &&
		memcmp(s1->_M_start, s2->_M_start, length) == 0;
}

__inline void bcb6_std_string_add_char(bcb6_std_string *s, char c)
{
	size_t pos;

	pos = bcb6_std_string_length(s);
	bcb6_std_string_reserve(s, pos + 1);
	*(s->_M_start + pos) = c;
	*(s->_M_finish = s->_M_start + pos + 1) = '\0';
}

__inline void bcb6_std_string_add_wide_char(bcb6_std_string *s, wchar_t c)
{
	size_t pos;

	pos = bcb6_std_string_length(s);
	bcb6_std_string_reserve(s, pos + 2);
	*(wchar_t *)(s->_M_start + pos) = c;
	*(s->_M_finish = s->_M_start + pos + 2) = '\0';
}

__inline void bcb6_std_string_add_string(bcb6_std_string *s1, char *s2, size_t length)
{
	size_t pos;

	pos = bcb6_std_string_length(s1);
	bcb6_std_string_reserve(s1, pos + length);
	memcpy(s1->_M_start + pos, s2, length);
	*(s1->_M_finish = s1->_M_start + pos + length) = '\0';
}

__inline void bcb6_std_string_trim(bcb6_std_string *s)
{
	char *begin, *end;

	begin = s->_M_start;
	while (__intrinsic_isspace(*begin))
		begin++;
	end = s->_M_finish;
	while (--end > begin && __intrinsic_isspace(*end));
	if (++end != s->_M_finish || begin != s->_M_start)
	{
		size_t length;

		length = end - begin;
		if (begin != s->_M_start)
			memcpy(s->_M_start, begin, length);
		*(s->_M_finish = s->_M_start + length) = '\0';
	}
}
