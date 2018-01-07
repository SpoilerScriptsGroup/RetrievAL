#include <wctype.h>

#ifdef __BORLANDC__
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
	switch (property[0])
	{
	case 'a':
		// alnum, alpha
		if (property[1] == 'l')
			if (property[2] == 'n')
			{
				if (property[3] == 'u' &&
					property[4] == 'm' &&
					property[5] == '\0')
					return _ALPHA | _DIGIT;
			}
			else if (property[2] == 'p')
			{
				if (property[3] == 'h' &&
					property[4] == 'a' &&
					property[5] == '\0')
					return _ALPHA;
			}
		break;
	case 'b':
		// blank
		if (property[1] == 'l' &&
			property[2] == 'a' &&
			property[3] == 'n' &&
			property[4] == 'k' &&
			property[5] == '\0')
			return _BLANK;
		break;
	case 'c':
		// cntrl
		if (property[1] == 'n' &&
			property[2] == 't' &&
			property[3] == 'r' &&
			property[4] == 'l' &&
			property[5] == '\0')
			return _CONTROL;
		break;
	case 'd':
		// digit
		if (property[1] == 'i' &&
			property[2] == 'g' &&
			property[3] == 'i' &&
			property[4] == 't' &&
			property[5] == '\0')
			return _DIGIT;
		break;
	case 'g':
		// graph
		if (property[1] == 'r' &&
			property[2] == 'a' &&
			property[3] == 'p' &&
			property[4] == 'h' &&
			property[5] == '\0')
			return _ALPHA | _DIGIT | _PUNCT;
		break;
	case 'l':
		// lower
		if (property[1] == 'o' &&
			property[2] == 'w' &&
			property[3] == 'e' &&
			property[4] == 'r' &&
			property[5] == '\0')
			return _LOWER;
		break;
	case 'p':
		// print, punct
		if (property[1] == 'r')
		{
			if (property[2] == 'i' &&
				property[3] == 'n' &&
				property[4] == 't' &&
				property[5] == '\0')
				return _ALPHA | _DIGIT | _PUNCT | _BLANK;
		}
		else if (property[1] == 'u')
		{
			if (property[2] == 'n' &&
				property[3] == 'c' &&
				property[4] == 't' &&
				property[5] == '\0')
				return _PUNCT;
		}
		break;
	case 's':
		// space
		if (property[1] == 'p' &&
			property[2] == 'a' &&
			property[3] == 'c' &&
			property[4] == 'e' &&
			property[5] == '\0')
			return _SPACE;
		break;
	case 'u':
		// upper
		if (property[1] == 'p' &&
			property[2] == 'p' &&
			property[3] == 'e' &&
			property[4] == 'r' &&
			property[5] == '\0')
			return _UPPER;
		break;
	case 'x':
		// xdigit
		if (property[1] == 'd' &&
			property[2] == 'i' &&
			property[3] == 'g' &&
			property[4] == 'i' &&
			property[5] == 't' &&
			property[6] == '\0')
			return _HEX;
		break;
	}
	return 0;
}
