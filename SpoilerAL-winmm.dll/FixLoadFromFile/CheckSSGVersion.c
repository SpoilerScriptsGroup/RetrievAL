#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "version.h"

extern HANDLE hHeap;

#define OLD_VERSION_LENGTH 3
#define SSG_VERSION_LENGTH 3

const char lpSSGVersion[SSG_VERSION_LENGTH + 1] = { VERSION_MAJOR + '0', '.', VERSION_MINOR + '0', '\0' };

BOOL EnableParserFix = FALSE;
BOOL FixTheProcedure = FALSE;
BOOL ExtensionTSSDir = FALSE;

#pragma intrinsic(memcmp)

void __fastcall CheckSSGVersion(const char *p, size_t size)
{
	EnableParserFix = FALSE;
	FixTheProcedure = FALSE;
	ExtensionTSSDir = FALSE;
	if (size >= 21 + OLD_VERSION_LENGTH &&
		*(LPDWORD) p       == BSWAP32('SSG ') &&
		*(LPDWORD)(p +  4) == BSWAP32('for ') &&
		*(LPDWORD)(p +  8) == BSWAP32('Spoi') &&
		*(LPDWORD)(p + 12) == BSWAP32('lerA') &&
		*(LPDWORD)(p + 16) == BSWAP32('L ve') &&
		*(LPBYTE )(p + 20) ==         'r')
	{
		const char *last;
		char       c;

		last = p + size - OLD_VERSION_LENGTH;
		p += 21 - 1;
		while ((c = *(++p)) == ' ' || c == '\t')
			if (p >= last)
				return;
		if (memcmp(p, "6.2", OLD_VERSION_LENGTH) >= 0)
		{
			if (memcmp(p, "6.3", OLD_VERSION_LENGTH) >= 0)
			{
#if SSG_VERSION_LENGTH > OLD_VERSION_LENGTH
				if (last - p >= SSG_VERSION_LENGTH - OLD_VERSION_LENGTH &&
					memcmp(p, lpSSGVersion, SSG_VERSION_LENGTH) >= 0)
#else
				if (memcmp(p, lpSSGVersion, SSG_VERSION_LENGTH) >= 0)
#endif
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
