#include <wctype.h>

#ifdef __BORLANDC__
#pragma warn -8098
#define _UPPER   0x01
#define _LOWER   0x02
#define _DIGIT   0x04
#define _SPACE   0x08
#define _PUNCT   0x10
#define _CONTROL 0x20
#define _BLANK   0x40
#define _HEX     0x80
#define _ALPHA   (0x0100 | _UPPER | _LOWER)
#endif

#define BSWAP16(value) (unsigned short)( \
    ((unsigned short)(value) >> 8) |     \
    ((unsigned short)(value) << 8))

#define BSWAP32(value) (unsigned long)(             \
    ( (unsigned long)(value) >> 24              ) | \
    (((unsigned long)(value) >>  8) & 0x0000FF00) | \
    (((unsigned long)(value) <<  8) & 0x00FF0000) | \
    ( (unsigned long)(value) << 24              ))

wctype_t __cdecl wctype(const char *property)
{
	/*
		"alnum"  _ALPHA | _DIGIT
		"alpha"  _ALPHA
		"blank"  _BLANK
		"cntrl"  _CONTROL
		"digit"  _DIGIT
		"graph"  _ALPHA | _DIGIT | _PUNCT
		"lower"  _LOWER
		"print"  _ALPHA | _DIGIT | _PUNCT | _BLANK
		"punct"  _PUNCT
		"space"  _SPACE
		"upper"  _UPPER
		"xdigit" _HEX
	*/
	if (property[0] && property[1] && property[2] && property[3] && property[4])
	{
		switch (*(unsigned long *)property)
		{
		case BSWAP32('alnu'):
			// alnum
			if (*(unsigned short *)&property[4] == BSWAP16('m\0'))
				return _ALPHA | _DIGIT;
			break;
		case BSWAP32('alph'):
			// alpha
			if (*(unsigned short *)&property[4] == BSWAP16('a\0'))
				return _ALPHA;
			break;
		case BSWAP32('blan'):
			// blank
			if (*(unsigned short *)&property[4] == BSWAP16('k\0'))
				return _BLANK;
			break;
		case BSWAP32('cntr'):
			// cntrl
			if (*(unsigned short *)&property[4] == BSWAP16('l\0'))
				return _CONTROL;
			break;
		case BSWAP32('digi'):
			// digit
			if (*(unsigned short *)&property[4] == BSWAP16('t\0'))
				return _DIGIT;
			break;
		case BSWAP32('grap'):
			// graph
			if (*(unsigned short *)&property[4] == BSWAP16('h\0'))
				return _ALPHA | _DIGIT | _PUNCT;
			break;
		case BSWAP32('lowe'):
			// lower
			if (*(unsigned short *)&property[4] == BSWAP16('r\0'))
				return _LOWER;
			break;
		case BSWAP32('prin'):
			// print
			if (*(unsigned short *)&property[4] == BSWAP16('t\0'))
				return _ALPHA | _DIGIT | _PUNCT | _BLANK;
			break;
		case BSWAP32('punc'):
			// punct
			if (*(unsigned short *)&property[4] == BSWAP16('t\0'))
				return _PUNCT;
			break;
		case BSWAP32('spac'):
			// space
			if (*(unsigned short *)&property[4] == BSWAP16('e\0'))
				return _SPACE;
			break;
		case BSWAP32('uppe'):
			// upper
			if (*(unsigned short *)&property[4] == BSWAP16('r\0'))
				return _UPPER;
			break;
		case BSWAP32('xdig'):
			// xdigit
			if (*(unsigned short *)&property[4] == BSWAP16('it') && property[6] == '\0')
				return _HEX;
			break;
		}
	}
	return 0;
}
