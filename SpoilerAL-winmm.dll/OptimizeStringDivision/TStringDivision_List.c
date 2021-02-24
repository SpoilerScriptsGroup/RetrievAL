#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#pragma intrinsic(memcmp)

unsigned long __cdecl TStringDivision_List(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     string          Token,
	OUT    vector_string   *List,
	IN     unsigned long   Option)
{
	LPCBYTE split;
	size_t  tokenLength;

	vector_string_clear(List);
	split = string_c_str(Src);
	if ((tokenLength = string_length(&Token)) && string_length(Src) >= tokenLength)
	{
		LPCBYTE token, end, p;
		size_t  nest;

		token = string_c_str(&Token);
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
				if (tokenLength == 3 && (*(LPDWORD)prev & 0x00FFFFFF) == *(LPDWORD)token)
					goto MATCHED;// "'+'"
				if (!nest && (tokenLength == 1 && *(LPBYTE )token == '/' && Option & dtESCAPE ||
							  tokenLength == 2 && *(LPWORD )token == BSWAP16('[!') ||
							  tokenLength == 3 && *(LPDWORD)token == '\'\x2B\''))// "'+'"
					break;// treat as apostrophe
				/* FALLTHROUGH */
			case '"':
				// string literals, character literals
				for (comparand = c; p < end; )
				{
					switch (c = *(p++))
					{
					default:
						continue;
					case '"':
					case '\'':
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
				break;
			case '(':
			case '{':
				// "(", "{"
				nest++;
				break;
			case ')':
			case '}':
				// ")", "}"
				if (nest)
					nest--;
				break;
			case '<':
				// "<#", "<@", "#>", "@>", "[!"
				switch (c = *p)
				{
				case '#':
				case '@':
					for (p++, comparand = c; p < end; )
					{
						switch (c = *(p++))
						{
						default:
							continue;
						case '#':
						case '@':
							if (c != comparand || *p != '>')
								continue;
							p++;
							break;
						case '[':
							if (*p != '!')
								continue;
							if (tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
								goto MATCHED;
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
					break;
				default:
					goto DEFAULT;
				}
				break;
			case '[':
				// "[!"
				if (*p != '!')
					break;
				if (tokenLength == 2 && *(LPWORD)token == BSWAP16('[!'))
					goto MATCHED;
				p++;
				break;
			case '\\':
				// escape-sequence
				if (!(Option & dtESCAPE))
					break;
				c = *(p++);
				if (!__intrinsic_isleadbyte(c))
					break;
				/* FALLTHROUGH */
			case_unsigned_leadbyte:
				// lead byte
				p++;
				break;
			default:
			DEFAULT:
				if (nest || memcmp(prev, token, tokenLength) != 0)
					break;
			MATCHED:
				vector_string_push_back_range(List, split, prev);
				if (Option & etSOME_EDIT)
				{
					string *it, s;

					s = *(it = string_end(List) - 1);
					TStringDivision_Editing(it, this, &s, Option);
					string_dtor(&s);
				}
				split = p = prev + tokenLength;
				break;
			}
		} while (p < end);
	}
	vector_string_push_back_range(List, split, string_end(Src));
	if (Option & etSOME_EDIT)
	{
		string *it, s;

		s = *(it = string_end(List) - 1);
		TStringDivision_Editing(it, this, &s, Option);
		string_dtor(&s);
	}
	string_dtor(&Token);
	return vector_size(List);
}
