#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

EXTERN_C void __fastcall TrimString(string *s);

EXTERN_C string * __cdecl TSSGCtrl_TrimString(string *Result, TStringDivision *StringDivision, string *Src, string Token, char *Dest, unsigned long Option)
{
	string_dtor(&Token);
	string_ctor_assign(Result, Src);
	TrimString(Result);
	return Result;
}
