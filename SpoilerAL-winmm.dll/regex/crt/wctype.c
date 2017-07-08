#include <wctype.h>
#include <string.h>

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
	typedef struct
	{
		char const* string;
		wctype_t    value;
	} TABLE;

	const TABLE table[] =
	{
		{ "alnum" , _ALPHA | _DIGIT                   },
		{ "alpha" , _ALPHA                            },
		{ "cntrl" , _CONTROL                          },
		{ "digit" , _DIGIT                            },
		{ "graph" , _ALPHA | _DIGIT | _PUNCT          },
		{ "lower" , _LOWER                            },
		{ "print" , _ALPHA | _DIGIT | _PUNCT | _BLANK },
		{ "punct" , _PUNCT                            },
		{ "blank" , _BLANK                            },
		{ "space" , _SPACE                            },
		{ "upper" , _UPPER                            },
		{ "xdigit", _HEX                              },
	};

	const TABLE *p, *end;

	p = table;
	end = (const TABLE *)((char *)table + sizeof(table));
	do
	{
		if (strcmp(p->string, property) == 0)
			return p->value;
	} while (++p != end);
	return 0;
}
