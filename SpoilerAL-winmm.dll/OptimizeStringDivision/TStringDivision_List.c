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
				// "'+'"
				if (!nest && tokenLength == 3 && (*(LPDWORD)prev & 0x00FFFFFF) == *(LPDWORD)token)
					goto MATCHED;
				/* FALLTHROUGH */
			case '"':
				// string literals, character literals
				for (comparand = c; p < end; )
				{
					switch (c = *(p++))
					{
					default:
						if (c != comparand)
							continue;
						break;
					case '\\':
						c = *(p++);
						if (!__intrinsic_isleadbyte(c))
							continue;
						/* FALLTHROUGH */
					case_unsigned_leadbyte:
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
				// "<#", "<@", "#>", "@>"
				switch (c = *p)
				{
				case '#':
				case '@':
					for (p++, comparand = c; p < end; )
					{
						switch (c = *(p++))
						{
						default:
							if (c != comparand || *p != '>')
								continue;
							p++;
							break;
						case '\\':
							if (!(Option & dtESCAPE))
								continue;
							c = *(p++);
							if (!__intrinsic_isleadbyte(c))
								continue;
							/* FALLTHROUGH */
						case_unsigned_leadbyte:
							p++;
							continue;
						}
						break;
					}
					continue;
				case_unsigned_leadbyte:
					goto LEADBYTE;
				default:
					goto DEFAULT;
				}
				break;
			case '\\':
				// escape-sequence
				if (!(Option & dtESCAPE))
					continue;
				c = *(p++);
				if (!__intrinsic_isleadbyte(c))
					continue;
#if MULTIBYTE_TOKEN
				goto LEADBYTE_INCREMENT;
			case_unsigned_leadbyte:
			LEADBYTE:
				// lead byte
				if (!nest && memcmp(prev, token, tokenLength) == 0)
					goto MATCHED;
			LEADBYTE_INCREMENT:
#else
				/* FALLTHROUGH */
			case_unsigned_leadbyte:
			LEADBYTE:
				// lead byte
#endif
				p++;
				continue;
			case '[':
				// "[!"
				if (*p == '!' && tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
					goto MATCHED;
				/* FALLTHROUGH */
			default:
			DEFAULT:
				if (nest || memcmp(prev, token, tokenLength) != 0)
					continue;
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
