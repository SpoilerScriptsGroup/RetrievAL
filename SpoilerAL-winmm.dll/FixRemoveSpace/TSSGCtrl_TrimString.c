#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

EXTERN_C char * __fastcall TrimPointer(const char **pfirst, const char *last);

EXTERN_C string * __cdecl TSSGCtrl_TrimString(string *Result, TStringDivision *StringDivision, string *Src, string Token, char *Dest, unsigned long Option)
{
	const char *first, *last;

	string_dtor(&Token);
	first = string_begin(Src);
	last = TrimPointer(&first, string_end(Src));
	return string_ctor_assign_cstr_with_length(Result, first, last - first);
}
