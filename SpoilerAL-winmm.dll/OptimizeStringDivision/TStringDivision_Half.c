#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#pragma function(memcmp)

bcb6_std_string * __cdecl TStringDivision_Half(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   Index,
	unsigned long   Option)
{
	size_t srcLength;
	size_t tokenLength;
	char   *end, *p;
	size_t nest;
	size_t length;

	if (!Src)
		goto FAILED;
	tokenLength = bcb6_std_string_length(&Token);
	if (!tokenLength)
		goto FAILED;
	srcLength = bcb6_std_string_length(Src);
	if (srcLength < tokenLength)
		goto FAILED;
	p = Src->_M_start;
	end = Src->_M_finish - tokenLength + 1;
	nest = 0;
	do
	{
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
			if (!(Option & DT_ESCAPE))
				break;
			if (++p < end)
				goto CHECK_LEADBYTE;
			else
				goto FAILED;
		case '<':
			if (*(p + 1) != '#')
				goto DEFAULT;
			if ((p += 2) >= end)
				goto FAILED;
			while (*p != '#' || *(p + 1) != '>')
			{
				if (*p == '\\' && (Option & DT_ESCAPE) && ++p >= end)
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
		case '[':
			if (*(p + 1) != '!')
				goto DEFAULT;
			if (tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('[!'))
				goto MATCHED;
			if ((p += 2) >= end)
				goto FAILED;
			while (*p != '!' || *(p + 1) != ']')
			{
				if (*p == '\\' && (Option & DT_ESCAPE) && ++p >= end)
					goto FAILED;
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto FAILED;
				if (++p >= end)
					goto FAILED;
			}
			if (tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('!]'))
				goto MATCHED;
			if ((p += 2) < end)
				continue;
			else
				goto FAILED;
		case '!':
			if (*(p + 1) != ']')
				goto DEFAULT;
			if (tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('!]'))
				goto MATCHED;
		default:
		DEFAULT:
			if (nest)
				goto CHECK_LEADBYTE;
			if (memcmp(p, Token._M_start, tokenLength) != 0)
				goto CHECK_LEADBYTE;
		MATCHED:
			if (!Index)
				goto SUCCESS;
			Index--;
			if ((p += tokenLength) < end)
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
	bcb6_std_string_ctor_assign(Result, &Token);
	bcb6_std_string_dtor(&Token);
	return Result;

SUCCESS:
	bcb6_std_string_ctor_assign_range(Result, Src->_M_start, p);
	p += tokenLength;
	length = Src->_M_finish - p;
	Src->_M_finish = Src->_M_start + length;
	memcpy(Src->_M_start, p, length + 1);
	if (Option & ET_SOME_EDIT)
	{
		bcb6_std_string r;

		TStringDivision_Editing(&r, _this, Src, Option);
		bcb6_std_string_assign(Src, &r);
		bcb6_std_string_dtor(&r);
		TStringDivision_Editing(&r, _this, Result, Option);
		bcb6_std_string_assign(Result, &r);
		bcb6_std_string_dtor(&r);
	}
	bcb6_std_string_dtor(&Token);
	return Result;
}
