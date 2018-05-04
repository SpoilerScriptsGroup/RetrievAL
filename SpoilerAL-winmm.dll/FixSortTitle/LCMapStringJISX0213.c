#include <windows.h>

#ifdef __BORLANDC__
#pragma warn -8060
#endif

int __stdcall LCMapStringJISX0213(
	IN           LCID   Locale,
	IN           DWORD  dwMapFlags,
	IN           LPCSTR lpSrcStr,
	IN           int    cchSrc,
	OUT OPTIONAL LPSTR  lpDestStr,
	IN           int    cchDest)
{
	LPSTR        lpBufferStr;
	unsigned int cchBuffer;
	HANDLE       hHeap;

	cchBuffer = cchSrc != -1 ? cchSrc : strlen(lpSrcStr) + 1;
	if (!(lpBufferStr = (LPSTR)HeapAlloc(hHeap = GetProcessHeap(), 0, cchBuffer * sizeof(char)))) {
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return 0;
	}
	memcpy(lpBufferStr, lpSrcStr, cchBuffer * sizeof(char));
	if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) == LANG_JAPANESE && (dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_KATAKANA)))
		if (cchBuffer >= 2) {
			unsigned char *p;
			ptrdiff_t     offset;

			p = lpBufferStr + cchBuffer;
			offset = -(ptrdiff_t)cchBuffer + 1;
			do {
				unsigned char c;

				if ((c = p[offset - 1]) < 0x81 || (c > 0x9F && (c < 0xE0 || c > 0xFC))) {
					if (!(dwMapFlags & LCMAP_FULLWIDTH) || c < 0xB6)
						continue;
					if (c <= 0xBA) {
						if (p[offset] != 0xDF)
							continue;
						p[offset - 1] = 0x83;
						p[offset] = c - 0xB6 + 0x97;
					} else if (c == 0xBE || c == 0xC2 || c == 0xC4) {
						if (p[offset] != 0xDF)
							continue;
						p[offset - 1] = 0x83;
						p[offset] = (c - 0xBE + 2) / 4 + 0x9C;
					} else if (c == 0xA6 || c == 0xDC) {
						if (p[offset] != 0xDE)
							continue;
						p[offset - 1] = 0x84;
						p[offset] = (c - 0xA6) / 16 + 0x92;
					} else
						continue;
				} else {
					if ((dwMapFlags & LCMAP_KATAKANA) && c == 0x82 && (c = p[offset]) >= 0xF2 && c <= 0xF9) {
						p[offset - 1] = 0x83;
						p[offset] = c - (0xF2 - 0x94);
					}
				}
				if (!++offset)
					break;
			} while (++offset);
		}
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	HeapFree(hHeap, 0, lpBufferStr);
	return cchDest;
}

