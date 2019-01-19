#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

unsigned long __cdecl TStringDivision_List(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     string          Token,
	OUT    vector_string   *List,
	IN     unsigned long   Option)
{
	const char *split;
	string     *elem;
	const char *token;
	size_t     tokenLength;

	vector_string_clear(List);
	split = string_c_str(Src);
	token = string_c_str(&Token);
	tokenLength = string_length(&Token);
	if (tokenLength && string_length(Src) >= tokenLength)
	{
		const char *end, *p;
		size_t     nest;

		end = string_end(Src) - tokenLength + 1;
		p = split;
		nest = 0;
		do
		{
			char c;

			switch (*p)
			{
			case '(':
				// '\x2B' == '+'
				if (tokenLength != 3 || *(LPDWORD)token != (DWORD)'\'\x2B\'')
					nest++;
				break;
			case ')':
				// '\x2B' == '+'
				if ((tokenLength != 3 || *(LPDWORD)token != (DWORD)'\'\x2B\'') && nest)
					nest--;
				break;
			case '\\':
				if (!(Option & dtESCAPE))
					break;
				if (++p < end)
					goto CHECK_LEADBYTE;
				else
					goto NESTED_BREAK;
			case '"':
				if (++p >= end)
					goto NESTED_BREAK;
				while (*p != '"')
				{
					if (*p == '\\' &&  ++p >= end)
						goto NESTED_BREAK;
					if (__intrinsic_isleadbyte(*p) && ++p >= end)
						goto NESTED_BREAK;
					if (++p >= end)
						goto NESTED_BREAK;
				}
				break;
			case '<':
				if ((c = *(p + 1)) != '#' && c != '@')
					goto DEFAULT;
				if ((p += 2) >= end)
					goto NESTED_BREAK;
				while (*p != c || *(p + 1) != '>')
				{
					if (*p == '[' && *(p + 1) == '!' && tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
						goto MATCHED;
					if (*p == '\\' && (Option & dtESCAPE) && ++p >= end)
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
				if (*(p + 1) == '!' && tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
					goto MATCHED;
			default:
			DEFAULT:
				if (nest)
					goto CHECK_LEADBYTE;
				if (memcmp(p, token, tokenLength) != 0)
					goto CHECK_LEADBYTE;
			MATCHED:
				vector_string_push_back_range(List, split, p);
				elem = string_end(List) - 1;
				if (Option & etSOME_EDIT)
				{
					string s;

					s = *elem;
					TStringDivision_Editing(elem, this, &s, Option);
					string_dtor(&s);
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
	vector_string_push_back_range(List, split, string_end(Src));
	elem = string_end(List) - 1;
	if (Option & etSOME_EDIT)
	{
		string s;

		s = *elem;
		TStringDivision_Editing(elem, this, &s, Option);
		string_dtor(&s);
	}
	string_dtor(&Token);
	return vector_size(List);
}
