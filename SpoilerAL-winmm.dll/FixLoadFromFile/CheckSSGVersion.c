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

#pragma intrinsic(memcmp)

void __fastcall CheckSSGVersion(const char *p, size_t size)
{
	#define VERSION_LENGTH      3	// strlen("6.2"), strlen("6.3")
	#define LAST_VERSION_LENGTH 3	// strlen(lpSSGVersion), (sizeof(lpSSGVersion) - 1)

	EnableParserFix = FALSE;
	FixTheProcedure = FALSE;
	ExtensionTSSDir = FALSE;
	if (size >= 21 + VERSION_LENGTH &&
		*(LPDWORD) p       == BSWAP32('SSG ') &&
		*(LPDWORD)(p +  4) == BSWAP32('for ') &&
		*(LPDWORD)(p +  8) == BSWAP32('Spoi') &&
		*(LPDWORD)(p + 12) == BSWAP32('lerA') &&
		*(LPDWORD)(p + 16) == BSWAP32('L ve') &&
		*(LPBYTE )(p + 20) ==         'r')
	{
		const char *last;

		last = p + size - VERSION_LENGTH;
		p += 21;
		while (*p == ' ' || *p == '\t')
			if (p++ >= last)
				return;
		if (memcmp(p, "6.2", VERSION_LENGTH) >= 0)
		{
			if (memcmp(p, "6.3", VERSION_LENGTH) >= 0)
			{
#if LAST_VERSION_LENGTH > VERSION_LENGTH
				if (last - p >= LAST_VERSION_LENGTH - VERSION_LENGTH &&
					memcmp(p, lpSSGVersion, LAST_VERSION_LENGTH) >= 0)
#else
				if (memcmp(p, lpSSGVersion, LAST_VERSION_LENGTH) >= 0)
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

	#undef VERSION_LENGTH
	#undef LAST_VERSION_LENGTH
}
