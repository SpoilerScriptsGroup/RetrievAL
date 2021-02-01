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
	LPCBYTE lastFound, p;
	size_t  srcLength, length;

	lastFound = NULL;
	if (Src && TokenLength && (srcLength = string_length(Src)) >= TokenLength)
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
				nest++;
				continue;
			case ')':
			case '}':
				// ")", "}"
				if (nest)
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
			case '!':
				// "!]"
				if (p >= end)
					break;
				if (*p == ']' && TokenLength == 2 && *(LPWORD)Token == BSWAP16('!]'))
					goto MATCHED;
				/* FALLTHROUGH */
			default:
			DEFAULT:
				if ((size_t)(end - prev) < TokenLength || memcmp(prev, Token, TokenLength) != 0)
					goto CHECK_LEADBYTE;
				lastFound = prev;
				if (!nest)
					goto MATCHED;
			CHECK_LEADBYTE:
				if (!__intrinsic_isleadbyte(c))
					continue;
				if (p >= end)
					break;
				p++;
				continue;
			MATCHED:
				if (Index--)
					if ((p = prev + TokenLength) < end)
						continue;
					else
						break;
				lastFound = prev;
				goto SUCCESS;
			}
			break;
		} while (p < end);
	}
	if (*(LPWORD)Token == BSWAP16(':\0') && lastFound)
		goto SUCCESS;
	string_ctor_assign_cstr_with_length(Result, Token, TokenLength);
	return Result;

SUCCESS:
	string_ctor_assign_cstr_with_length(Result, string_c_str(Src), (p = lastFound) - string_c_str(Src));
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
