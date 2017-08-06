#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

bcb6_std_string * __cdecl TStringDivision_RemoveByMap(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_map    *ReplaceMap,
	unsigned long   Option);

bcb6_std_string * __cdecl TStringDivision_Editing(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	unsigned long   Option)
{
	if (Option & ET_TRIM)
	{
		if (Option & ET_REPLACE)
		{
			bcb6_std_string s;

			TStringDivision_RemoveByMap(Result, _this, TStringDivision_TrimDefault(&s, _this, Src, NULL, Option), NULL, Option);
			bcb6_std_string_dtor(&s);
		}
		else
		{
			return TStringDivision_TrimDefault(Result, _this, Src, NULL, Option);
		}
	}
	else if (Option & ET_REPLACE)
	{
		return TStringDivision_RemoveByMap(Result, _this, Src, NULL, Option);
	}
	else
	{
		bcb6_std_string_ctor_assign(Result, Src);
	}
	return Result;
}
