#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

EXTERN_C string * __cdecl TSSGCtrl_TrimString(string *Result, TStringDivision *StringDivision, string *Src, string Token, char *Dest, unsigned long Option)
{
	char *first, *last;

	string_dtor(&Token);
	first = string_begin(Src);
	last = TrimBlank(&first, string_end(Src));
	return string_ctor_assign_cstr_with_length(Result, first, last - first);
}
