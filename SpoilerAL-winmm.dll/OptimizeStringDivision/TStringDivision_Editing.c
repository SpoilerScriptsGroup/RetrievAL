#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_RemoveByMap(
	string          *Result,
	TStringDivision *_this,
	string          *Src,
	map             *ReplaceMap,
	unsigned long   Option);

string * __cdecl TStringDivision_Editing(
	string          *Result,
	TStringDivision *_this,
	string          *Src,
	unsigned long   Option)
{
	if (Option & ET_TRIM)
	{
		if (Option & ET_REPLACE)
		{
			string s;

			TStringDivision_RemoveByMap(Result, _this, TStringDivision_TrimDefault(&s, _this, Src, NULL, Option), NULL, Option);
			string_dtor(&s);
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
		string_ctor_assign(Result, Src);
	}
	return Result;
}
