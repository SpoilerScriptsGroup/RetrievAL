#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector *v);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector *v, size_t size);

unsigned long TStringDivision_List(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	bcb6_std_vector *List,
	unsigned long   Option)
{
	size_t          listSize;
	const char      *split;
	size_t          tokenLength;
	bcb6_std_string *elem;

	listSize = 0;
	bcb6_std_vector_string_clear(List);
	split = Src->_M_start;
	if ((tokenLength = bcb6_std_string_length(&Token)) && bcb6_std_string_length(Src) >= tokenLength)
	{
		const char *end, *p;
		size_t     nest;

		end = Src->_M_finish - tokenLength + 1;
		p = split;
		nest = 0;
		do
		{
			switch (*p)
			{
			case '(':
				// '\x2B' == '+'
				if (tokenLength != 3 || *(LPDWORD)Token._M_start != (DWORD)'\'\x2B\'')
					nest++;
				break;
			case ')':
				// '\x2B' == '+'
				if ((tokenLength != 3 || *(LPDWORD)Token._M_start != (DWORD)'\'\x2B\'') && nest)
					nest--;
				break;
			case '\\':
				if (!(Option & DT_ESCAPE))
					break;
				if (++p < end)
					goto CHECK_LEADBYTE;
				else
					goto NESTED_BREAK;
			case '"':
				if (!nest)
					break;
				if (++p >= end)
					goto NESTED_BREAK;
				while (*p != '"')
				{
					if (*p == '\\' && ++p >= end)
						goto NESTED_BREAK;
					if (__intrinsic_isleadbyte(*p) && ++p >= end)
						goto NESTED_BREAK;
					if (++p >= end)
						goto NESTED_BREAK;
				}
				break;
			case '<':
				if (*(p + 1) != '#')
					goto DEFAULT;
				if ((p += 2) >= end)
					goto NESTED_BREAK;
				while (*p != '#' || *(p + 1) != '>')
				{
					if (*p == '[' && *(p + 1) == '!' && tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('[!'))
						goto MATCHED;
					if (*p == '\\' && (Option & DT_ESCAPE) && ++p >= end)
						goto NESTED_BREAK;
					if (__intrinsic_isleadbyte(*p) && ++p >= end)
						goto NESTED_BREAK;
					if (++p >= end)
						goto NESTED_BREAK;
				}
				if ((p += 2) < end)
					continue;
				else
					goto NESTED_BREAK;
			case '[':
				if (*(p + 1) == '!' && tokenLength == 2 && *(LPWORD)Token._M_start == BSWAP16('[!'))
					goto MATCHED;
			default:
			DEFAULT:
				if (nest)
					goto CHECK_LEADBYTE;
				if (memcmp(p, Token._M_start, tokenLength) != 0)
					goto CHECK_LEADBYTE;
			MATCHED:
				bcb6_std_vector_string_resize(List, ++listSize);
				elem = (bcb6_std_string *)List->_M_finish - 1;
				bcb6_std_string_assign_range(elem, split, p);
				if (Option & ET_SOME_EDIT)
				{
					bcb6_std_string s;

					s = *elem;
					TStringDivision_Editing(elem, _this, &s, Option);
					bcb6_std_string_dtor(&s);
				}
				split = (p += tokenLength);
				continue;
			CHECK_LEADBYTE:
				if (__intrinsic_isleadbyte(*p) && ++p >= end)
					goto NESTED_BREAK;
				break;
			}
			p++;
		} while (p < end);
	}
NESTED_BREAK:
	bcb6_std_vector_string_resize(List, ++listSize);
	elem = (bcb6_std_string *)List->_M_finish - 1;
	bcb6_std_string_assign_range(elem, split, Src->_M_finish);
	if (Option & ET_SOME_EDIT)
	{
		bcb6_std_string s;

		s = *elem;
		TStringDivision_Editing(elem, _this, &s, Option);
		bcb6_std_string_dtor(&s);
	}
	bcb6_std_string_dtor(&Token);
	return listSize;
}
