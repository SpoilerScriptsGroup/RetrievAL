#include <wctype.h>
#include <string.h>

wctype_t __cdecl wctype(const char *property)
{
	const struct
	{
		char const* string;
		wctype_t    value;
	}
	table[] =
	{
		{ "alnum" , _ALPHA | _DIGIT                   },
		{ "alpha" , _ALPHA                            },
		{ "cntrl" , _CONTROL                          },
		{ "digit" , _DIGIT                            },
		{ "graph" , _PUNCT | _ALPHA | _DIGIT          },
		{ "lower" , _LOWER                            },
		{ "print" , _BLANK | _PUNCT | _ALPHA | _DIGIT },
		{ "punct" , _PUNCT                            },
		{ "blank" , _BLANK                            },
		{ "space" , _SPACE                            },
		{ "upper" , _UPPER                            },
		{ "xdigit", _HEX                              },
		{ NULL    ,  0                                }
	};
	for (size_t index = 0; table[index].string; index++)
		if (strcmp(table[index].string, property) == 0)
			return table[index].value;
	return 0;
}
