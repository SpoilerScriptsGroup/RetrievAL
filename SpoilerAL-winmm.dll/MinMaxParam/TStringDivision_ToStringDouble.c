#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <float.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

string * __cdecl TStringDivision_ToStringDouble(string *Result, double Src, const char *Format)
{
	char   buffer[512];
	size_t length;

	length = _snprintf(buffer, _countof(buffer), !Format || _isnan(Src) ? "%f" : Format, Src);
	if ((unsigned int)length >= _countof(buffer))
		length = 0;
	string_ctor_assign_cstr_with_length(Result, buffer, length);
	return Result;
}
