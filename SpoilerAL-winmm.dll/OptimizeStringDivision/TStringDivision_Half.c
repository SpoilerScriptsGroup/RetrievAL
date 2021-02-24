#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#pragma intrinsic(memcmp)

string * __stdcall TStringDivision_Half_WithoutTokenDtor(
	OUT    string          *Result,
	IN     TStringDivision *this,
	IN OUT string          *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	IN     unsigned long   Index,
	IN     unsigned long   Option)
{
	LPCBYTE lastFound, p;
	size_t  length;

	lastFound = NULL;
	if (Src && TokenLength && string_length(Src) >= TokenLength)
	{
		LPCBYTE end;
		size_t  nest;

		p = string_c_str(Src);
		end = string_end(Src) - TokenLength + 1;
		nest = 0;
		do
		{
			LPCBYTE prev;
			BYTE    c, comparand;

			switch (c = *(prev = p++))
			{
			case '\'':
				if (!nest && (TokenLength == 1 && *(LPBYTE)Token == ':' && Option & dtESCAPE ||
							  TokenLength == 2 && *(LPWORD)Token == BSWAP16('!]')))
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
			case '$':
				if (!(Option & dtBYTEARRAY)) goto DEFAULT;
				// "$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$$"
				switch (*p)
				{
				case '0':
				case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8':
					for (p++; p < end; )
					{
						switch (c = *(p++))
						{
						default:
							continue;
						case '$':
							if (*p != '$')
								continue;
							p++;
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
					break;
				default:
					goto DEFAULT;
				}
				break;
			case '!':
				// "!]"
				if (*p != ']')
					goto DEFAULT;
				if (TokenLength == 2 && *(LPWORD)Token == BSWAP16('!]'))
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
				if (memcmp(prev, Token, TokenLength) != 0)
					break;
			MATCHED:
				lastFound = prev;
				if (!nest && !Index--)
					goto SUCCESS;
				p = prev + TokenLength;
				break;
			}
		} while (p < end);
	}
	if (*(LPWORD)Token == BSWAP16(':\0') && lastFound)
		goto SUCCESS;
	string_ctor_assign_cstr_with_length(Result, Token, TokenLength);
	return Result;

SUCCESS:
	string_ctor_assign_cstr_with_length(Result, string_c_str(Src), lastFound - string_c_str(Src));
	string_end(Src) = string_begin(Src) + (length = string_end(Src) - (p = lastFound + TokenLength));
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
