#include <windows.h>
#include <mbstring.h>
#include "intrinsic.h"
#include "TStringDivision.h"

bcb6_std_string * __cdecl TSSGScriptStruct_EscapeDistraction(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_map    *ReplaceMap,
	unsigned long   Option)
{
	TStringDivision_RemoveByMap(Result, _this, Src, ReplaceMap, Option);
	size_t n = bcb6_std_string_length(Result) * 2;
	if (n >= (size_t)(Result->_M_end_of_storage - Result->_M_start))
		bcb6_std_string_allocate(Result, n);
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
