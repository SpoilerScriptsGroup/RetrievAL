#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

extern HANDLE hHeap;

const char lpSSGVersion[4] = { '6', '.', '2', '\0' };

BOOL EnableParserFix = FALSE;

void __fastcall CheckSSGVersion(const vector_string *lines)
{
	EnableParserFix = FALSE;
	if (!vector_empty(lines))
	{
		const string *s;
		const char   *p;

		s = vector_begin(lines);
		p = string_begin(s);
		if (string_end(s) - p > 21 &&
			*(LPDWORD) p       == BSWAP32('SSG ') &&
			*(LPDWORD)(p +  4) == BSWAP32('for ') &&
			*(LPDWORD)(p +  8) == BSWAP32('Spoi') &&
			*(LPDWORD)(p + 12) == BSWAP32('lerA') &&
			*(LPDWORD)(p + 16) == BSWAP32('L ve') &&
			*(LPBYTE )(p + 20) ==         'r')
		{
			p += 21;
			while (*p == ' ' || *p == '\t')
				p++;
			if (strcmp(p, lpSSGVersion) >= 0)
				EnableParserFix = TRUE;
		}
	}
}
