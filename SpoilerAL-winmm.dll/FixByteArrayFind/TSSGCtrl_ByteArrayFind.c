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
//		*(LPWORD)string_c_str(&Token) == BSWAP16('$$') ||
		*(LPWORD)string_c_str(&Token) == BSWAP16('::') ||
		*(LPWORD)string_c_str(&Token) == BSWAP16('*>') ||
		*(LPWORD)string_c_str(&Token) == BSWAP16('_>') ||
		*(LPWORD)string_c_str(&Token) == BSWAP16('*]') ||
		*(LPWORD)string_c_str(&Token) == BSWAP16('*}'));

	if (FromIndex >= ToIndex)
		return -1;
	length = string_size(Src);
	if (length <= FromIndex || length - FromIndex < TokenLength)
		return -1;
	p = string_begin(Src) + FromIndex;
	end = string_begin(Src) + ToIndex;
	if (end < string_begin(Src))
		end = string_end(Src);
	end -= TokenLength - 1;
	nest = 0;
	do
	{
		LPCBYTE prev;
		BYTE    c, comparand;

		switch (c = *(prev = p++))
		{
#if 0// Address coding must be preceded by '$'
		case '\'':
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
#endif
		case '$':
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
#if 0
		case '(':
#endif
		case '{':// unresolved {define}
			// "(", "{"
			nest++;
			break;
#if 0
		case ')':
#endif
		case '}':// unresolved {define}
			// ")", "}"
			if (nest)
				nest--;
			break;
		case '*':
			// "*<", "*[", "*{", "*>", "*]", "*}"
			switch (*p)
			{
			case '[':
#if 0
				switch (p[1])
				{
				case '.':
				case ':':
				case '_':
				case '~':
					p += 2;
					break;
				}
				/* FALLTHROUGH */
#endif
			case '<':
			case '{':
				nest++;
				p++;
				break;
			case '>':
			case ']':
			case '}':
				if (p++, nest)
					nest--;
				else if (*(LPWORD)prev == *(LPWORD)string_c_str(&Token))
					goto TOKEN_FOUND;
				break;
			}
			break;
		case ':':
			// "::"
			if (*p == ':' && (p++, !nest) && *(LPWORD)string_c_str(&Token) == BSWAP16('::'))
				goto TOKEN_FOUND;
			break;
		case '<':
			// "<#", "<@", "<_", "#>", "@>"
			switch (*p)
			{
			case '#': case '@':
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
			case '_':
				nest++;
				p++;
				break;
			}
			break;
		case '\\':
			// escape-sequence
			c = *(p++);
			if (__intrinsic_isleadbyte(c))
				p++;
			break;
		case '_':
			// "_>"
			if (*p != '>')
				break;
			if (p++, nest)
				nest--;
			else if (*(LPWORD)string_c_str(&Token) == BSWAP16('_>'))
				goto TOKEN_FOUND;
			break;
		case_unsigned_leadbyte:
			// lead byte
			p++;
			break;
		TOKEN_FOUND:
			string_dtor(&Token);
			return prev - string_begin(Src);
		}
	} while (p < end);
	string_dtor(&Token);
	return -1;

	#undef TokenLength
}
