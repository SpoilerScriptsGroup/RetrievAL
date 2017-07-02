#include <wctype.h>

wctype_t __cdecl wctype(const char *property)
{
	switch (property[0])
	{
	case 'a':
		if (property[1] != 'l')
			break;
		switch (property[2])
		{
		case 'n':
			// alnum
			if (property[3] == 'u' && property[4] == 'm' && property[5] == '\0')
				return _UPPER | _LOWER | _DIGIT;
			break;
		case 'p':
			// alpha
			if (property[3] == 'h' && property[4] == 'a' && property[5] == '\0')
				return _UPPER | _LOWER;
			break;
		}
		break;
	case 'b':
		// blank
		if (property[1] == 'l' && property[2] == 'a' &&property[3] == 'n' && property[4] == 'k' && property[5] == '\0')
			return _BLANK;
		break;
	case 'c':
		// cntrl
		if (property[1] == 'n' && property[2] == 't' &&property[3] == 'r' && property[4] == 'l' && property[5] == '\0')
			return _CONTROL;
		break;
	case 'd':
		// digit
		if (property[1] == 'i' && property[2] == 'g' &&property[3] == 'i' && property[4] == 't' && property[5] == '\0')
			return _DIGIT;
		break;
	case 'g':
		// graph
		if (property[1] == 'r' && property[2] == 'a' &&property[3] == 'p' && property[4] == 'h' && property[5] == '\0')
			return _UPPER | _LOWER | _DIGIT | _PUNCT;
		break;
	case 'l':
		// lower
		if (property[1] == 'o' && property[2] == 'w' &&property[3] == 'e' && property[4] == 'r' && property[5] == '\0')
			return _LOWER;
		break;
	case 'p':
		switch (property[1])
		{
		case 'r':
			// print
			if (property[2] == 'i' && property[3] == 'n' && property[4] == 't' && property[5] == '\0')
				return _UPPER | _LOWER | _DIGIT | _PUNCT | _BLANK;
			break;
		case 'u':
			// punct
			if (property[2] == 'n' && property[3] == 'c' && property[4] == 't' && property[5] == '\0')
				return _PUNCT;
			break;
		}
		break;
	case 's':
		// space
		if (property[1] == 'p' && property[2] == 'a' &&property[3] == 'c' && property[4] == 'e' && property[5] == '\0')
			return _SPACE;
		break;
	case 'u':
		// upper
		if (property[1] == 'p' && property[2] == 'p' &&property[3] == 'e' && property[4] == 'r' && property[5] == '\0')
			return _UPPER;
		break;
	case 'x':
		// xdigit
		if (property[1] == 'd' && property[2] == 'i' &&property[3] == 'g' && property[4] == 'i' && property[5] == 't' && property[6] == '\0')
			return _HEX;
		break;
	}
	return 0;
}
