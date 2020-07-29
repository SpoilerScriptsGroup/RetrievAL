#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __stdcall TStringDivision_Half_WithoutTokenDtor(
	OUT    string          *Result,
	IN     TStringDivision *this,
	IN OUT string          *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	IN     unsigned long   Index,
	IN     unsigned long   Option)
{
	size_t     srcLength;
	const char *lastFound, *end, *p;
	size_t     nest;
	size_t     length;

	lastFound = NULL;
	if (!Src)
		goto FAILED;
	if (!TokenLength)
		goto FAILED;
	srcLength = string_length(Src);
	if (srcLength < TokenLength)
		goto FAILED;
	p = string_c_str(Src);
	end = string_end(Src) - TokenLength + 1;
	nest = 0;
	do
	{
		char c;

		switch (*p)
		{
		case '(':
			nest++;
			break;
		case ')':
			if (nest)
				nest--;
			break;
		case '\\':
			if (!(Option & dtESCAPE))
				break;
			if (++p < end)
				goto CHECK_LEADBYTE;
			else
				goto FAILED;
		case '"':
			if (++p >= end)
				goto FAILED;
			while (*p != '"')
			{
				if (*p == '\\' && ++p >= end)
					goto FAILED;
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto FAILED;
				if (++p >= end)
					goto FAILED;
			}
			break;
		case '<':
			if ((c = *(p + 1)) != '#' && c != '@')
				goto DEFAULT;
			if ((p += 2) >= end)
				goto FAILED;
			while (*p != c || *(p + 1) != '>')
			{
				if (*p == '\\' && (Option & dtESCAPE) && ++p >= end)
					goto FAILED;
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto FAILED;
				if (++p >= end)
					goto FAILED;
			}
			if ((p += 2) < end)
				continue;
			else
				goto FAILED;
		case '!':
			if (*(p + 1) == ']' && TokenLength == 2 && *(LPWORD)Token == BSWAP16('!]'))
				goto MATCHED;
		default:
		DEFAULT:
			if (memcmp(p, Token, TokenLength) != 0)
				goto CHECK_LEADBYTE;
			lastFound = p;
			if (nest)
				goto CHECK_LEADBYTE;
		MATCHED:
			if (!Index)
				goto SUCCESS;
			Index--;
			if ((p += TokenLength) < end)
				continue;
			else
				goto FAILED;
		CHECK_LEADBYTE:
			if (__intrinsic_isleadbyte(*p) && ++p >= end)
				goto FAILED;
			break;
		}
		p++;
	} while (p < end);
FAILED:
	if (*(LPWORD)Token == BSWAP16(':\0') && lastFound)
	{
		p = lastFound;
		goto SUCCESS;
	}
	string_ctor_assign_cstr_with_length(Result, Token, TokenLength);
	return Result;

SUCCESS:
	string_ctor_assign_cstr_with_length(Result, string_c_str(Src), p - string_c_str(Src));
	p += TokenLength;
	length = string_end(Src) - p;
	string_end(Src) = string_begin(Src) + length;
	memcpy(string_begin(Src), p, length + 1);
	if (Option & etSOME_EDIT)
	{
		string s;

		s = *Src;
		TStringDivision_Editing(Src, this, &s, Option);
		string_dtor(&s);
		s = *Result;
		TStringDivision_Editing(Result, this, &s, Option);
		string_dtor(&s);
	}
	return Result;
}

string * __cdecl TStringDivision_Half(
	OUT    string          *Result,
	IN     TStringDivision *this,
	IN OUT string          *Src,
	IN     string          Token,
	IN     unsigned long   Index,
	IN     unsigned long   Option)
{
	TStringDivision_Half_WithoutTokenDtor(
		Result,
		this,
		Src,
		string_c_str(&Token),
		string_length(&Token),
		Index,
		Option);
	string_dtor(&Token);
	return Result;
}
