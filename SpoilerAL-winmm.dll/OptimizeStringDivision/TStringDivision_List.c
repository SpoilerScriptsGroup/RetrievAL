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
	LPCBYTE split;
	string  *elem;
	LPCBYTE token;
	size_t  tokenLength;

	vector_string_clear(List);
	split = string_c_str(Src);
	token = string_c_str(&Token);
	tokenLength = string_length(&Token);
	if (tokenLength && string_length(Src) >= tokenLength)
	{
		LPCBYTE end, p;
		size_t  nest;

		end = string_end(Src) - tokenLength + 1;
		p = split;
		nest = 0;
		do
		{
			LPCBYTE prev;
			BYTE    c, comparand;

			switch (c = *(prev = p++))
			{
			case '\'':
			case '"':
				// character literals, string literals
				for (comparand = c; p < end; )
				{
					switch (c = *(p++))
					{
					default:
						if (c != comparand)
							continue;
						break;
					case '\\':
						if (p >= end)
							break;
						c = *(p++);
						if (!__intrinsic_isleadbyte(c))
							continue;
						/* FALLTHROUGH */
					case_unsigned_leadbyte:
						if (p >= end)
							break;
						p++;
						continue;
					}
					break;
				}
				continue;
			case '(':
			case '{':
				// "(", "{"
				// '\x2B' == '+'
				if (tokenLength != 3 || *(LPDWORD)token != (DWORD)'\'\x2B\'')
					nest++;
				continue;
			case ')':
			case '}':
				// ")", "}"
				// '\x2B' == '+'
				if ((tokenLength != 3 || *(LPDWORD)token != (DWORD)'\'\x2B\'') && nest)
					nest--;
				continue;
			case '<':
				// "<#", "#>", "<@", "@>"
				if (p >= end)
					break;
				switch (c = *p)
				{
				case '#':
				case '@':
					if (++p >= end)
						break;
					for (comparand = c; ; )
					{
						switch (c = *(p++))
						{
						default:
							if (p >= end)
								break;
							if (c != comparand || *p != '>')
								continue;
							p++;
							break;
						case '\\':
							if (p >= end)
								break;
							if (!(Option & dtESCAPE))
								continue;
							c = *(p++);
							if (p >= end)
								break;
							if (!__intrinsic_isleadbyte(c) || ++p < end)
								continue;
							break;
						case_unsigned_leadbyte:
							if (p < end && ++p < end)
								continue;
							break;
						}
						break;
					}
					continue;
				default:
					goto DEFAULT;
				}
				break;
			case '\\':
				// escape-sequence
				if (!(Option & dtESCAPE))
					continue;
				if (p >= end)
					break;
				c = *(p++);
				goto CHECK_LEADBYTE;
			case '[':
				// "[!"
				if (p >= end)
					break;
				if (*p == '!' && tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
					goto MATCHED;
				/* FALLTHROUGH */
			default:
			DEFAULT:
				if (nest)
					goto CHECK_LEADBYTE;
				if ((size_t)(end - prev) < tokenLength || memcmp(prev, token, tokenLength) != 0)
					goto CHECK_LEADBYTE;
			MATCHED:
				vector_string_push_back_range(List, split, prev);
				elem = string_end(List) - 1;
				if (Option & etSOME_EDIT)
				{
					string s;

					s = *elem;
					TStringDivision_Editing(elem, this, &s, Option);
					string_dtor(&s);
				}
				split = (p = prev + tokenLength);
				continue;
			CHECK_LEADBYTE:
				if (!__intrinsic_isleadbyte(c))
					continue;
				if (p >= end)
					break;
				p++;
				continue;
			}
			break;
		} while (p < end);
	}
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
