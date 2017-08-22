#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

extern HANDLE hHeap;

const char lpSSGVersion[4] = { '6', '.', '2', '\0' };

BOOL EnableParserFix = FALSE;

void __fastcall CheckSSGVersion(const string *FirstLine)
{
	const char *p, *end;

	EnableParserFix = FALSE;
	p = string_begin(FirstLine);
	end = string_end(FirstLine);
	if (end - p > 21 &&
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
