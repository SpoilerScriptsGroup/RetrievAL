#include <windows.h>
#include <limits.h>
#include "intrinsic.h"

#ifdef __BORLANDC__
#pragma warn -8060
#endif

#define JISX0213(c) (       \
    (c) == 'うﾞ' ? 0x82F2 : \
    (c) == 'か'  ? 0x82F3 : \
    (c) == 'け'  ? 0x82F4 : \
    (c) == 'かﾟ' ? 0x82F5 : \
    (c) == 'きﾟ' ? 0x82F6 : \
    (c) == 'くﾟ' ? 0x82F7 : \
    (c) == 'けﾟ' ? 0x82F8 : \
    (c) == 'こﾟ' ? 0x82F9 : \
    (c) == 'カﾟ' ? 0x8397 : \
    (c) == 'キﾟ' ? 0x8398 : \
    (c) == 'クﾟ' ? 0x8399 : \
    (c) == 'ケﾟ' ? 0x839A : \
    (c) == 'コﾟ' ? 0x839B : \
    (c) == 'セﾟ' ? 0x839C : \
    (c) == 'ツﾟ' ? 0x839D : \
    (c) == 'トﾟ' ? 0x839E : \
    (c) == 'ク'  ? 0x83EC : \
    (c) == 'シ'  ? 0x83ED : \
    (c) == 'ス'  ? 0x83EE : \
    (c) == 'ト'  ? 0x83EF : \
    (c) == 'ヌ'  ? 0x83F0 : \
    (c) == 'ハ'  ? 0x83F1 : \
    (c) == 'ヒ'  ? 0x83F2 : \
    (c) == 'フ'  ? 0x83F3 : \
    (c) == 'ヘ'  ? 0x83F4 : \
    (c) == 'ホ'  ? 0x83F5 : \
    (c) == 'プ'  ? 0x83F6 : \
    (c) == 'ム'  ? 0x83F7 : \
    (c) == 'ラ'  ? 0x83F8 : \
    (c) == 'リ'  ? 0x83F9 : \
    (c) == 'ル'  ? 0x83FA : \
    (c) == 'レ'  ? 0x83FB : \
    (c) == 'ロ'  ? 0x83FC : \
    (c) == 'ワﾞ' ? 0x848F : \
    (c) == 'ヰﾞ' ? 0x8490 : \
    (c) == 'ヱﾞ' ? 0x8491 : \
    (c) == 'ヲﾞ' ? 0x8492 : 0)

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
	if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) == LANG_JAPANESE &&
		(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)) &&
		cchBuffer > 1)
	{
		unsigned char *p;
		ptrdiff_t     offset;
		unsigned int  prev, c;
		size_t        erase;

		p = lpBufferStr + (cchBuffer - 1);
		offset = -(ptrdiff_t)(cchBuffer - 1);
		prev = 0;
		do {
			if ((c = p[offset]) >= 0x81 && (c <= 0x9F || (c >= 0xE0 && c <= 0xFC)))
				c = (c << 8) | p[offset + 1];
			if (dwMapFlags & LCMAP_FULLWIDTH) {
				erase = 0;
				switch (c) {
				case 0x00DE/*'ﾞ'*/:
				case 0x814A/*'゛'*/:
					switch (prev) {
					case 0x00B3/*'ｳ'*/:
						if (!(dwMapFlags & LCMAP_HIRAGANA))
							break;
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = BSWAP16(0x82F2/*JISX0213('うﾞ')*/);
						break;
					case 0x00DC/*'ﾜ'*/:
					case 0x00A6/*'ｦ'*/:
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							(0x00DC/*'ﾜ'*/ - prev) / 16 + 0x8492/*JISX0213('ワﾞ')*/);
						break;
					case 0x82A4/*'う'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = !(dwMapFlags & LCMAP_KATAKANA) ?
							BSWAP16(0x82F2/*JISX0213('うﾞ')*/) :
							BSWAP16(0x8394/*'ヴ'*/);
						break;
					case 0x82ED/*'わ'*/:
					case 0x82EE/*'ゐ'*/:
					case 0x82EF/*'ゑ'*/:
					case 0x82F0/*'を'*/:
						if (!(dwMapFlags & LCMAP_KATAKANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							prev - 0x82ED/*'わ'*/ + 0x8492/*JISX0213('ワﾞ')*/);
						break;
					case 0x8345/*'ウ'*/:
						if (!(dwMapFlags & LCMAP_HIRAGANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = BSWAP16(0x82F2/*JISX0213('うﾞ')*/);
						break;
					case 0x838F/*'ワ'*/:
					case 0x8390/*'ヰ'*/:
					case 0x8391/*'ヱ'*/:
					case 0x8392/*'ヲ'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							prev - 0x838F/*'ワ'*/ + 0x8492/*JISX0213('ワﾞ')*/);
						break;
					}
					break;
				case 0x00DF/*'ﾟ'*/:
				case 0x814B/*'゜'*/:
					switch (prev) {
					case 0x00B6/*'ｶ'*/:
					case 0x00B7/*'ｷ'*/:
					case 0x00B8/*'ｸ'*/:
					case 0x00B9/*'ｹ'*/:
					case 0x00BA/*'ｺ'*/:
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							prev + ((dwMapFlags & LCMAP_HIRAGANA) ?
								0x82F5/*JISX0213('かﾟ')*/ - 0x00B6/*'ｶ'*/ :
								0x8397/*JISX0213('カﾟ')*/ - 0x00B6/*'ｶ'*/));
						break;
					case 0x00BE/*'ｾ'*/:
					case 0x00C2/*'ﾂ'*/:
					case 0x00C4/*'ﾄ'*/:
						if (dwMapFlags & LCMAP_HIRAGANA)
							break;
						erase = c > UCHAR_MAX;
						*(unsigned short *)&p[offset - 1] = _byteswap_ushort(
							(prev - 0x00BE/*'ｾ'*/ + 2) / 4 + 0x839C/*JISX0213('セﾟ')*/);
						break;
					case 0x82A9/*'か'*/:
					case 0x82AB/*'き'*/:
					case 0x82AD/*'く'*/:
					case 0x82AF/*'け'*/:
					case 0x82B1/*'こ'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x82A9/*'か'*/) / 2 + (!(dwMapFlags & LCMAP_KATAKANA) ?
								0x82F5/*JISX0213('かﾟ')*/ :
								0x8397/*JISX0213('カﾟ')*/));
						break;
					case 0x82B9/*'せ'*/:
					case 0x82C2/*'つ'*/:
					case 0x82C6/*'と'*/:
						if (!(dwMapFlags & LCMAP_KATAKANA))
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x82B9/*'せ'*/ + 3) / 8 + 0x839C/*JISX0213('セﾟ')*/);
						break;
					case 0x834A/*'カ'*/:
					case 0x834C/*'キ'*/:
					case 0x834E/*'ク'*/:
					case 0x8350/*'ケ'*/:
					case 0x8352/*'コ'*/:
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x834A/*'カ'*/) / 2 + ((dwMapFlags & LCMAP_HIRAGANA) ?
								0x82F5/*JISX0213('かﾟ')*/ :
								0x8397/*JISX0213('カﾟ')*/));
						break;
					case 0x835A/*'セ'*/:
					case 0x8363/*'ツ'*/:
					case 0x8367/*'ト'*/:
						if (dwMapFlags & LCMAP_HIRAGANA)
							break;
						erase = c > UCHAR_MAX ? 2 : 1;
						*(unsigned short *)&p[offset - 2] = _byteswap_ushort(
							(prev - 0x835A/*'セ'*/ + 3) / 8 + 0x839C/*JISX0213('セﾟ')*/);
						break;
					}
					break;
				}
				if (erase) {
					cchBuffer -= erase;
					offset += erase;
					p -= erase;
					memcpy(p + offset, p + offset + erase, -offset + 1);
					if (!offset)
						break;
				}
			} else if (dwMapFlags & LCMAP_KATAKANA) {
				switch (c) {
				case 0x82F2/*JISX0213('うﾞ')*/:
					*(unsigned short *)&p[offset] = !(dwMapFlags & LCMAP_HALFWIDTH) ?
						BSWAP16(0x8394/*'ヴ'*/) :
						BSWAP16(0xB3DE/*'ｳﾞ'*/);
					break;
				case 0x82F3/*JISX0213('か')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0x8395/*'ヵ'*/);
					break;
				case 0x82F4/*JISX0213('け')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0x8396/*'ヶ'*/);
					break;
				case 0x82F5/*JISX0213('かﾟ')*/:
				case 0x82F6/*JISX0213('きﾟ')*/:
				case 0x82F7/*JISX0213('くﾟ')*/:
				case 0x82F8/*JISX0213('けﾟ')*/:
				case 0x82F9/*JISX0213('こﾟ')*/:
					*(unsigned short *)&p[offset] = !(dwMapFlags & LCMAP_HALFWIDTH) ?
						_byteswap_ushort(c - 0x82F5/*JISX0213('かﾟ')*/ + 0x8397/*JISX0213('カﾟ')*/) :
						c - 0x82F5/*JISX0213('かﾟ')*/ + BSWAP16(0xB6DF/*'ｶﾟ'*/);
					break;
				}
			} else if (dwMapFlags & LCMAP_HIRAGANA) {
				switch (c) {
				case 0x8394/*'ヴ'*/:
				case 0x8395/*'ヵ'*/:
				case 0x8396/*'ヶ'*/:
				case 0x8397/*JISX0213('カﾟ')*/:
				case 0x8398/*JISX0213('キﾟ')*/:
				case 0x8399/*JISX0213('クﾟ')*/:
				case 0x839A/*JISX0213('ケﾟ')*/:
				case 0x839B/*JISX0213('コﾟ')*/:
					*(unsigned short *)&p[offset] = _byteswap_ushort(
						c - 0x8394/*'ヴ'*/ + 0x82F2/*JISX0213('うﾞ')*/);
					break;
				}
			} else if (dwMapFlags & LCMAP_HALFWIDTH) {
				switch (c) {
				case 0x8394/*'ヴ'*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xB3DE/*'ｳﾞ'*/);
					break;
				case 0x8397/*JISX0213('カﾟ')*/:
				case 0x8398/*JISX0213('キﾟ')*/:
				case 0x8399/*JISX0213('クﾟ')*/:
				case 0x839A/*JISX0213('ケﾟ')*/:
				case 0x839B/*JISX0213('コﾟ')*/:
					*(unsigned short *)&p[offset] = c - 0x8397/*JISX0213('カﾟ')*/ + BSWAP16(0xB6DF/*'ｶﾟ'*/);
					break;
				case 0x839C/*JISX0213('セﾟ')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xBEDF/*'ｾﾟ'*/);
					break;
				case 0x839D/*JISX0213('ツﾟ')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xC2DF/*'ﾂﾟ'*/);
					break;
				case 0x839E/*JISX0213('トﾟ')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xC4DF/*'ﾄﾟ'*/);
					break;
				case 0x8492/*JISX0213('ワﾞ')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xDCDE/*'ﾜﾞ'*/);
					break;
				case 0x8495/*JISX0213('ヲﾞ')*/:
					*(unsigned short *)&p[offset] = BSWAP16(0xA6DE/*'ｦﾞ'*/);
					break;
				}
			}
			if (c > UCHAR_MAX && !++offset)
				break;
			prev = c;
		} while (++offset);
	}
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	HeapFree(hHeap, 0, lpBufferStr);
	return cchDest;
}

