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

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_string  string;
typedef pbcb6_std_string pstring;
#define string_c_str                        bcb6_std_string_c_str
#define string_begin                        bcb6_std_string_begin
#define string_end                          bcb6_std_string_end
#define string_end_of_storage               bcb6_std_string_end_of_storage
#define string_length                       bcb6_std_string_length
#define string_size                         bcb6_std_string_size
#define string_capacity                     bcb6_std_string_capacity
#define string_array_length                 bcb6_std_string_array_length
#define string_array_capacity               bcb6_std_string_array_capacity
#define string_empty                        bcb6_std_string_empty
#define string_clear                        bcb6_std_string_clear
#define new_string                          new_bcb6_std_string
#define delete_string                       delete_bcb6_std_string
#define string_concat                       bcb6_std_string_concat
#define string_dtor                         bcb6_std_string_dtor
#define string_ctor                         bcb6_std_string_ctor
#define string_ctor_assign_range            bcb6_std_string_ctor_assign_range
#define string_ctor_assign                  bcb6_std_string_ctor_assign
#define string_ctor_assign_cstr             bcb6_std_string_ctor_assign_cstr
#define string_ctor_assign_cstr_with_length bcb6_std_string_ctor_assign_cstr_with_length
#define string_assign                       bcb6_std_string_assign
#define string_assign_range                 bcb6_std_string_assign_range
#define string_assign_cstr                  bcb6_std_string_assign_cstr
#define string_assign_cstr_with_length      bcb6_std_string_assign_cstr_with_length
#define string_append                       bcb6_std_string_append
#define string_append_range                 bcb6_std_string_append_range
#define string_reserve                      bcb6_std_string_reserve
#define string_resize                       bcb6_std_string_resize
#define string_shrink_to_fit                bcb6_std_string_shrink_to_fit
#define string_assign_char                  bcb6_std_string_assign_char
#define string_equals                       bcb6_std_string_equals
#define string_append_char                  bcb6_std_string_append_char
#define string_append_wide_char             bcb6_std_string_append_wide_char
#define string_append_string                bcb6_std_string_append_string
#define string_trim                         bcb6_std_string_trim
#endif

#define bcb6_std_string_c_str(s)          (LPCSTR)(s)->_M_start
#define bcb6_std_string_begin(s)          (s)->_M_start
#define bcb6_std_string_end(s)            (s)->_M_finish
#define bcb6_std_string_end_of_storage(s) (s)->_M_end_of_storage
#define bcb6_std_string_length(s)         (size_t)((s)->_M_finish - (s)->_M_start)
#define bcb6_std_string_size              bcb6_std_string_length
#define bcb6_std_string_capacity(s)       (bcb6_std_string_array_capacity(s) - 1)
#define bcb6_std_string_array_length(s)   (bcb6_std_string_length(s) + 1)
#define bcb6_std_string_array_capacity(s) (size_t)((s)->_M_end_of_storage - (s)->_M_start)
#define bcb6_std_string_empty(s)          ((s)->_M_finish == (s)->_M_start)
#define bcb6_std_string_clear(s)          (*((s)->_M_finish = (s)->_M_start) = '\0')

EXTERN_C bcb6_std_string *(__cdecl *bcb6_std_string_concat)(bcb6_std_string *s, const bcb6_std_string *left, const bcb6_std_string *right);

EXTERN_C bcb6_std_string * __cdecl new_bcb6_std_string();
EXTERN_C void __fastcall delete_bcb6_std_string(bcb6_std_string *s);
EXTERN_C void __fastcall bcb6_std_string_dtor(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign(bcb6_std_string *s, const bcb6_std_string *src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_cstr(bcb6_std_string *s, LPCSTR src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_cstr_with_length(bcb6_std_string *s, LPCSTR src, size_t length);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign(bcb6_std_string *s, const bcb6_std_string *src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_cstr(bcb6_std_string *s, LPCSTR src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_cstr_with_length(bcb6_std_string *s, LPCSTR src, size_t length);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append(bcb6_std_string *s, size_t n, char c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C void __fastcall bcb6_std_string_reserve(bcb6_std_string *s, size_t n);
EXTERN_C void __fastcall bcb6_std_string_resize(bcb6_std_string *s, size_t length);
EXTERN_C void __fastcall bcb6_std_string_shrink_to_fit(bcb6_std_string *s);

__inline void bcb6_std_string_assign_char(bcb6_std_string *s, char c)
{
	bcb6_std_string_resize(s, 1);
	*s->_M_start = c;
	*(s->_M_finish = s->_M_start + 1) = '\0';
}

__inline BOOL bcb6_std_string_equals(bcb6_std_string *s1, bcb6_std_string *s2)
{
	size_t length = bcb6_std_string_length(s1);
	return
		bcb6_std_string_length(s2) == length &&
		memcmp(s1->_M_start, s2->_M_start, length) == 0;
}

__inline void bcb6_std_string_append_char(bcb6_std_string *s, char c)
{
	size_t pos = bcb6_std_string_length(s);
	bcb6_std_string_resize(s, pos + 1);
	*(s->_M_start + pos) = c;
}

__inline void bcb6_std_string_append_wide_char(bcb6_std_string *s, wchar_t c)
{
	size_t pos = bcb6_std_string_length(s);
	bcb6_std_string_resize(s, pos + 2);
	*(wchar_t *)(s->_M_start + pos) = c;
}

__inline void bcb6_std_string_append_string(bcb6_std_string *s1, char *s2, size_t length)
{
	size_t pos = bcb6_std_string_length(s1);
	bcb6_std_string_resize(s1, pos + length);
	memcpy(s1->_M_start + pos, s2, length);
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
