#include <windows.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

unsigned long __cdecl TSSGCtrl_ByteArrayFind(
	TStringDivision *StringDivision,
	string          *Src,
	string          Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Reserved)
{
	#define TokenLength 2

	size_t  length;
	LPCBYTE p, end;
	size_t  nest;

	assert(string_length(&Token) == 2);
	assert(
		*(LPWORD)string_begin(&Token) == BSWAP16('*>') ||
		*(LPWORD)string_begin(&Token) == BSWAP16('*}') ||
		*(LPWORD)string_begin(&Token) == BSWAP16('*]') ||
		*(LPWORD)string_begin(&Token) == BSWAP16('_>') ||
		*(LPWORD)string_begin(&Token) == BSWAP16('::'));

	if (FromIndex >= ToIndex)
		return -1;
	length = string_length(Src);
	if (length <= FromIndex || length - FromIndex < TokenLength)
		return -1;
	p = string_c_str(Src) + FromIndex;
	end = string_c_str(Src) + ToIndex;
	if (end < string_c_str(Src))
		end = string_end(Src);
	end -= TokenLength - 1;
	nest = 0;
	do
	{
		LPCBYTE prev;
		BYTE    c, comparand;

		switch (c = *(prev = p++))
		{
		case '$':
			// "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$$"
			if (c < '1' || c > '8')
				continue;
			while (p < end)
			{
				BYTE c;

				switch (*(p++))
				{
				default:
					continue;
				case '$':
					if (p >= end)
						break;
					if (*p != '$')
						continue;
					p++;
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
		case '*':
			// "*<", "*[", "*{", "*>", "*]", "*}"
			if (p >= end)
				break;
			switch (p[0])
			{
			case '[':
				if (p + 1 >= end)
					break;
				switch (p[1])
				{
				case '.':
				case ':':
				case '_':
				case '~':
					p += 2;
					continue;
				}
				/* FALLTHROUGH */
			case '<':
			case '{':
				nest++;
				break;
			case '>':
			case ']':
			case '}':
				if (nest)
					nest--;
				else if (*(LPWORD)prev == *(LPWORD)string_begin(&Token))
					goto TOKEN_FOUND;
				break;
			default:
				continue;
			}
			p++;
			continue;
		case ':':
			// "::"
			if (p >= end)
				break;
			if (*p != ':' || nest || *(LPWORD)string_begin(&Token) != BSWAP16('::'))
				continue;
			goto TOKEN_FOUND;
		case '<':
			// "<#", "#>", "<@", "@>", "<_"
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
			case '_':
				if (++p >= end)
					break;
				nest++;
				continue;
			default:
				continue;
			}
			break;
		case '_':
			// "_>"
			if (p >= end)
				break;
			if (*p != '>')
				continue;
			if (nest)
				nest--;
			else if (*(LPWORD)string_begin(&Token) == BSWAP16('_>'))
				goto TOKEN_FOUND;
			p++;
			continue;
		TOKEN_FOUND:
			string_dtor(&Token);
			return prev - string_c_str(Src);
		case '\\':
			// escape-sequence
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
		default:
			continue;
		}
		break;
	} while (p < end);
	string_dtor(&Token);
	return -1;

	#undef TokenLength
}
