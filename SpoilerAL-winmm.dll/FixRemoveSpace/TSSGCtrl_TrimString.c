#include "TStringDivision.h"

EXTERN_C void __fastcall TrimString(bcb6_std_string *s);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_TrimString(bcb6_std_string *Result, TStringDivision *StringDivision, bcb6_std_string *Src, bcb6_std_string Token, char *Dest, unsigned long Option)
{
	bcb6_std_string_dtor(&Token);
	bcb6_std_string_ctor_assign(Result, Src);
	TrimString(Result);
	return Result;
}
