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
	if (property[0] && property[1] && property[2] && property[3])
	{
		typedef struct
		{
			char     string[8];
			wctype_t value;
		} ELEMENT;

		const ELEMENT table[] =
		{
			{ { 'a', 'l', 'n', 'u', 'm'      /* alnum  */ }, _ALPHA | _DIGIT                   },
			{ { 'a', 'l', 'p', 'h', 'a'      /* alpha  */ }, _ALPHA                            },
			{ { 'b', 'l', 'a', 'n', 'k'      /* blank  */ }, _BLANK                            },
			{ { 'c', 'n', 't', 'r', 'l'      /* cntrl  */ }, _CONTROL                          },
			{ { 'd', 'i', 'g', 'i', 't'      /* digit  */ }, _DIGIT                            },
			{ { 'g', 'r', 'a', 'p', 'h'      /* graph  */ }, _ALPHA | _DIGIT | _PUNCT          },
			{ { 'l', 'o', 'w', 'e', 'r'      /* lower  */ }, _LOWER                            },
			{ { 'p', 'r', 'i', 'n', 't'      /* print  */ }, _ALPHA | _DIGIT | _PUNCT | _BLANK },
			{ { 'p', 'u', 'n', 'c', 't'      /* punct  */ }, _PUNCT                            },
			{ { 's', 'p', 'a', 'c', 'e'      /* space  */ }, _SPACE                            },
			{ { 'u', 'p', 'p', 'e', 'r'      /* upper  */ }, _UPPER                            },
			{ { 'x', 'd', 'i', 'g', 'i', 't' /* xdigit */ }, _HEX                              },
		};

		size_t offset;

		offset = sizeof(table);
		do
		{
			offset -= sizeof(ELEMENT);
			if (*(unsigned long *)property != *(unsigned long *)((char *)table + offset))
				continue;
			if (((char *)table + offset)[4] != property[4])
				break;
			if (((char *)table + offset)[5] != property[5])
				break;
			if (((char *)table + offset)[5] && property[6])
				break;
			return ((ELEMENT *)((char *)table + offset))->value;
		} while (offset);
	}
	return 0;
}
