#include <windows.h>
#include <mbstring.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

string * __cdecl TSSGScriptStruct_EscapeDistraction(
	string          *Result,
	TStringDivision *this,
	string          *Src,
	map             *ReplaceMap,
	unsigned long   Option)
{
	TStringDivision_RemoveByMap(Result, this, Src, ReplaceMap, Option);
	size_t n = string_length(Result) * 2;
	if (n >= (size_t)(Result->_M_end_of_storage - Result->_M_start))
		string_reserve(Result, n);
	for (char *p = Result->_M_start; p = _mbschr(p, '\\'); p += 2)
		memmove(p + 1, p, ++Result->_M_finish - p);
	for (char *p = Result->_M_start; p = _mbschr(p, ','); *p = '\\', p += 2)
		memmove(p + 1, p, ++Result->_M_finish - p);
	for (char *p = Result->_M_start; p = _mbschr(p, ':'); *p = '\\', p += 2)
		memmove(p + 1, p, ++Result->_M_finish - p);
	for (char *p = Result->_M_start; p = _mbschr(p, '='); *p = '\\', p += 2)
		memmove(p + 1, p, ++Result->_M_finish - p);
	return Result;
}
