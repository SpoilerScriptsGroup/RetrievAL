#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "version.h"

extern HANDLE hHeap;

const char lpSSGVersion[4] = { VERSION_MAJOR + '0', '.', VERSION_MINOR + '0', '\0' };

BOOL EnableParserFix = FALSE;
BOOL FixTheProcedure = FALSE;
BOOL ExtensionTSSDir = FALSE;

void __fastcall CheckSSGVersion(const char *begin, const char *end)
{
	EnableParserFix = FALSE;
	FixTheProcedure = FALSE;
	ExtensionTSSDir = FALSE;
	if (end - begin > 21 &&
		*(LPDWORD) begin       == BSWAP32('SSG ') &&
		*(LPDWORD)(begin +  4) == BSWAP32('for ') &&
		*(LPDWORD)(begin +  8) == BSWAP32('Spoi') &&
		*(LPDWORD)(begin + 12) == BSWAP32('lerA') &&
		*(LPDWORD)(begin + 16) == BSWAP32('L ve') &&
		*(LPBYTE )(begin + 20) ==         'r')
	{
		begin += 21;
		while (*begin == ' ' || *begin == '\t')
			begin++;
		if (strcmp(begin, "6.2") >= 0)
		{
			if (strcmp(begin, "6.3") >= 0)
			{
				if (strcmp(begin, lpSSGVersion) >= 0)
				{
					// SSG for SpoilerAL ver 6.4
					ExtensionTSSDir = TRUE;
				}
				// SSG for SpoilerAL ver 6.3
				FixTheProcedure = TRUE;
			}
			// SSG for SpoilerAL ver 6.2
			EnableParserFix = TRUE;
		}
	}
}
