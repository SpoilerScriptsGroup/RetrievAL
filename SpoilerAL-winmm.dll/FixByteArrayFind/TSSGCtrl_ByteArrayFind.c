#include <windows.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

#define TokenLength 2
unsigned long __cdecl TSSGCtrl_ByteArrayFind(
	TStringDivision *StringDivision,
	string          *Src,
	string          Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Reserved)
{
	size_t length;
	LPCSTR p, end;
	size_t nest;

	assert(string_length(&Token) == 2);
	assert(
		*(LPWORD)Token._M_start == BSWAP16('*}') ||
		*(LPWORD)Token._M_start == BSWAP16('*]') ||
		*(LPWORD)Token._M_start == BSWAP16('_>') ||
		*(LPWORD)Token._M_start == BSWAP16('::'));

	if (FromIndex >= ToIndex)
		return SIZE_MAX;
	length = string_length(Src);
	if (length <= FromIndex || length - FromIndex < TokenLength)
		return SIZE_MAX;
	p = Src->_M_start + FromIndex;
	end = Src->_M_start + ToIndex;
	if (end < Src->_M_start)
		end = (LPCSTR)SIZE_MAX;
	end -= TokenLength - 1;
	nest = 0;
	do
	{
		char c;

		switch (*p)
		{
		case '"':
			while (++p < end && *p != '"')
			{
				if (*p == '\\')
					p++;
				if (__intrinsic_isleadbyte(*p))
					p++;
			}
			break;
		case '$':
			if (*(++p) >= '1' && *p <= '4')
			{
				while (++p < end - 1 && (*p != '$' || *(p + 1) != '$'))
				{
					if (*p == '\\')
						p++;
					if (__intrinsic_isleadbyte(*p))
						p++;
				}
				p += 2;
			}
			continue;
		case '(':
			nest++;
			break;
		case ')':
			if (nest)
				nest--;
			break;
		case '*':
			switch (p[1])
			{
			case '[':
				switch (p[2])
				{
				case '_':
				case '.':
				case '~':
				case ':':
					p += 3;
					continue;
				}
			case '<':
			case '{':
				nest++;
				break;
			case ']':
			case '}':
				if (!nest && *(LPWORD)p == *(LPWORD)Token._M_start)
					goto TOKEN_FOUND;
			case '>':
				if (nest)
					nest--;
				break;
			default:
				p++;
				continue;
			}
			p += 2;
			continue;
		case '<':
			switch (c = *(++p))
			{
			case '#':
			case '@':
				while (++p < end - 1 && ((*p != c) || *(p + 1) != '>'))
				{
					if (*p == '\\')
						p++;
					if (__intrinsic_isleadbyte(*p))
						p++;
				}
				p += 2;
				continue;
			case '_':
				nest++;
				break;
			default:
				continue;
			}
			break;
		case '\\':
			p++;
			goto CHECK_LEADBYTE;
		case '_':
			if (p[1] != '>')
				break;
			if (!nest && *(LPWORD)Token._M_start == BSWAP16('_>'))
				goto TOKEN_FOUND;
			if (nest)
				nest--;
			p += 2;
			continue;
		default:
			if (!nest && *(LPWORD)p == *(LPWORD)Token._M_start)
			{
			TOKEN_FOUND:
				string_dtor(&Token);
				return p - Src->_M_start;
			}
		CHECK_LEADBYTE:
			if (__intrinsic_isleadbyte(*p))
				p++;
			break;
		}
		p++;
	} while (p < end);
	string_dtor(&Token);
	return SIZE_MAX;
}
#undef TokenLength
