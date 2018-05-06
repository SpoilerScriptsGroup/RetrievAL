#include <windows.h>
#include <limits.h>

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
    (c) == 'ワﾞ' ? 0x8492 : \
    (c) == 'ヰﾞ' ? 0x8493 : \
    (c) == 'ヱﾞ' ? 0x8494 : \
    (c) == 'ヲﾞ' ? 0x8495 : 0)

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
		(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)))
	{
		unsigned char  *dest, *src;
		size_t         count;
		unsigned short prev;

		src = dest = lpBufferStr;
		count = cchBuffer;
		prev = 0;
		while (count--) {
			unsigned short c;
			unsigned char  high;

			if ((c = *(src++)) >= 0x81 && (c <= 0x9F || (c >= 0xE0 && c <= 0xFC)) && count) {
				count--;
				c = (c << 8) | *(src++);
			}
			if (dwMapFlags & LCMAP_FULLWIDTH) {
				unsigned short out;

				out = 0;
				switch (c) {
				case 0x00DE/*'ﾞ' */:
				case 0x814A/*'゛'*/:
					if (dwMapFlags & LCMAP_HIRAGANA) {
						switch (prev) {
						case 0x00B3/*'ｳ' */:
						case 0x82A4/*'う'*/:
						case 0x8345/*'ウ'*/: goto FULLWIDTH_82F2;
						}
					} else if (dwMapFlags & LCMAP_KATAKANA) {
						switch (prev) {
						case 0x82A4/*'う'*/: out = 0x8394/*'ヴ'*/; break;
						case 0x00DC/*'ﾜ' */:
						case 0x82ED/*'わ'*/:
						case 0x838F/*'ワ'*/: goto FULLWIDTH_8492;
						case 0x82EE/*'ゐ'*/:
						case 0x8390/*'ヰ'*/: goto FULLWIDTH_8493;
						case 0x82EF/*'ゑ'*/:
						case 0x8391/*'ヱ'*/: goto FULLWIDTH_8494;
						case 0x00A6/*'ｦ' */:
						case 0x82F0/*'を'*/:
						case 0x8392/*'ヲ'*/: goto FULLWIDTH_8495;
						}
					} else {
						switch (prev) {
						case 0x82A4/*'う'*/: FULLWIDTH_82F2: out = 0x82F2/*JISX0213('うﾞ')*/; break;
						case 0x00DC/*'ﾜ' */:
						case 0x82ED/*'わ'*/:
						case 0x838F/*'ワ'*/: FULLWIDTH_8492: out = 0x8492/*JISX0213('ワﾞ')*/; break;
						case 0x82EE/*'ゐ'*/:
						case 0x8390/*'ヰ'*/: FULLWIDTH_8493: out = 0x8493/*JISX0213('ヰﾞ')*/; break;
						case 0x82EF/*'ゑ'*/:
						case 0x8391/*'ヱ'*/: FULLWIDTH_8494: out = 0x8494/*JISX0213('ヱﾞ')*/; break;
						case 0x00A6/*'ｦ' */:
						case 0x82F0/*'を'*/:
						case 0x8392/*'ヲ'*/: FULLWIDTH_8495: out = 0x8495/*JISX0213('ヲﾞ')*/; break;
						}
					}
					break;
				case 0x00DF/*'ﾟ' */:
				case 0x814B/*'゜'*/:
					if (dwMapFlags & LCMAP_HIRAGANA) {
						switch (prev) {
						case 0x00B6/*'ｶ' */:
						case 0x82A9/*'か'*/:
						case 0x834A/*'カ'*/: goto FULLWIDTH_82F5;
						case 0x00B7/*'ｷ' */:
						case 0x82AB/*'き'*/:
						case 0x834C/*'キ'*/: goto FULLWIDTH_82F6;
						case 0x00B8/*'ｸ' */:
						case 0x82AD/*'く'*/:
						case 0x834E/*'ク'*/: goto FULLWIDTH_82F7;
						case 0x00B9/*'ｹ' */:
						case 0x82AF/*'け'*/:
						case 0x8350/*'ケ'*/: goto FULLWIDTH_82F8;
						case 0x00BA/*'ｺ' */:
						case 0x82B1/*'こ'*/:
						case 0x8352/*'コ'*/: goto FULLWIDTH_82F9;
						}
					} else if (dwMapFlags & LCMAP_KATAKANA) {
						switch (prev) {
						case 0x00B6/*'ｶ' */:
						case 0x82A9/*'か'*/:
						case 0x834A/*'カ'*/: goto FULLWIDTH_8397;
						case 0x00B7/*'ｷ' */:
						case 0x82AB/*'き'*/:
						case 0x834C/*'キ'*/: goto FULLWIDTH_8398;
						case 0x00B8/*'ｸ' */:
						case 0x82AD/*'く'*/:
						case 0x834E/*'ク'*/: goto FULLWIDTH_8399;
						case 0x00B9/*'ｹ' */:
						case 0x82AF/*'け'*/:
						case 0x8350/*'ケ'*/: goto FULLWIDTH_839A;
						case 0x00BA/*'ｺ' */:
						case 0x82B1/*'こ'*/:
						case 0x8352/*'コ'*/: goto FULLWIDTH_839B;
						case 0x00BE/*'ｾ' */:
						case 0x82B9/*'せ'*/:
						case 0x835A/*'セ'*/: goto FULLWIDTH_839C;
						case 0x00C2/*'ﾂ' */:
						case 0x82C2/*'つ'*/:
						case 0x8363/*'ツ'*/: goto FULLWIDTH_839D;
						case 0x00C4/*'ﾄ' */:
						case 0x82C6/*'と'*/:
						case 0x8367/*'ト'*/: goto FULLWIDTH_839E;
						}
					} else {
						switch (prev) {
						case 0x82A9/*'か'*/: FULLWIDTH_82F5: out = 0x82F5/*JISX0213('かﾟ')*/; break;
						case 0x82AB/*'き'*/: FULLWIDTH_82F6: out = 0x82F6/*JISX0213('きﾟ')*/; break;
						case 0x82AD/*'く'*/: FULLWIDTH_82F7: out = 0x82F7/*JISX0213('くﾟ')*/; break;
						case 0x82AF/*'け'*/: FULLWIDTH_82F8: out = 0x82F8/*JISX0213('けﾟ')*/; break;
						case 0x82B1/*'こ'*/: FULLWIDTH_82F9: out = 0x82F9/*JISX0213('こﾟ')*/; break;
						case 0x00B6/*'ｶ' */:
						case 0x834A/*'カ'*/: FULLWIDTH_8397: out = 0x8397/*JISX0213('カﾟ')*/; break;
						case 0x00B7/*'ｷ' */:
						case 0x834C/*'キ'*/: FULLWIDTH_8398: out = 0x8398/*JISX0213('キﾟ')*/; break;
						case 0x00B8/*'ｸ' */:
						case 0x834E/*'ク'*/: FULLWIDTH_8399: out = 0x8399/*JISX0213('クﾟ')*/; break;
						case 0x00B9/*'ｹ' */:
						case 0x8350/*'ケ'*/: FULLWIDTH_839A: out = 0x839A/*JISX0213('ケﾟ')*/; break;
						case 0x00BA/*'ｺ' */:
						case 0x8352/*'コ'*/: FULLWIDTH_839B: out = 0x839B/*JISX0213('コﾟ')*/; break;
						case 0x00BE/*'ｾ' */:
						case 0x835A/*'セ'*/: FULLWIDTH_839C: out = 0x839C/*JISX0213('セﾟ')*/; break;
						case 0x00C2/*'ﾂ' */:
						case 0x8363/*'ツ'*/: FULLWIDTH_839D: out = 0x839D/*JISX0213('ツﾟ')*/; break;
						case 0x00C4/*'ﾄ' */:
						case 0x8367/*'ト'*/: FULLWIDTH_839E: out = 0x839E/*JISX0213('トﾟ')*/; break;
						}
					}
					break;
				}
				if (out) {
					size_t is_wide;

					dest -= (is_wide = prev > UCHAR_MAX) + 1;
					cchBuffer -= is_wide + (c > UCHAR_MAX);
					c = out;
				}
			} else if (dwMapFlags & LCMAP_HIRAGANA) {
				switch (c) {
				case 0x8394/*'ヴ'           */: c = 0x82F2/*JISX0213('うﾞ')*/; break;
				case 0x8395/*'ヵ'           */: c = 0x82F3/*JISX0213('か' )*/; break;
				case 0x8396/*'ヶ'           */: c = 0x82F4/*JISX0213('け' )*/; break;
				case 0x8397/*JISX0213('カﾟ')*/: c = 0x82F5/*JISX0213('かﾟ')*/; break;
				case 0x8398/*JISX0213('キﾟ')*/: c = 0x82F6/*JISX0213('きﾟ')*/; break;
				case 0x8399/*JISX0213('クﾟ')*/: c = 0x82F7/*JISX0213('くﾟ')*/; break;
				case 0x839A/*JISX0213('ケﾟ')*/: c = 0x82F8/*JISX0213('けﾟ')*/; break;
				case 0x839B/*JISX0213('コﾟ')*/: c = 0x82F9/*JISX0213('こﾟ')*/; break;
				}
			} else if (dwMapFlags & LCMAP_KATAKANA) {
				if (!(dwMapFlags & LCMAP_HALFWIDTH)) {
					switch (c) {
					case 0x82F2/*JISX0213('うﾞ')*/: c = 0x8394/*'ヴ'           */; break;
					case 0x82F3/*JISX0213('か' )*/: c = 0x8395/*'ヵ'           */; break;
					case 0x82F4/*JISX0213('け' )*/: c = 0x8396/*'ヶ'           */; break;
					case 0x82F5/*JISX0213('かﾟ')*/: c = 0x8397/*JISX0213('カﾟ')*/; break;
					case 0x82F6/*JISX0213('きﾟ')*/: c = 0x8398/*JISX0213('キﾟ')*/; break;
					case 0x82F7/*JISX0213('くﾟ')*/: c = 0x8399/*JISX0213('クﾟ')*/; break;
					case 0x82F8/*JISX0213('けﾟ')*/: c = 0x839A/*JISX0213('ケﾟ')*/; break;
					case 0x82F9/*JISX0213('こﾟ')*/: c = 0x839B/*JISX0213('コﾟ')*/; break;
					}
				} else {
					switch (c) {
					case 0x82F2/*JISX0213('うﾞ')*/: c = 0xB3DE/*'ｳﾞ'*/; break;
					case 0x82F5/*JISX0213('かﾟ')*/:
					case 0x8397/*JISX0213('カﾟ')*/: goto HALFWIDTH_B6DF;
					case 0x82F6/*JISX0213('きﾟ')*/:
					case 0x8398/*JISX0213('キﾟ')*/: goto HALFWIDTH_B7DF;
					case 0x82F7/*JISX0213('くﾟ')*/:
					case 0x8399/*JISX0213('クﾟ')*/: goto HALFWIDTH_B8DF;
					case 0x82F8/*JISX0213('けﾟ')*/:
					case 0x839A/*JISX0213('ケﾟ')*/: goto HALFWIDTH_B9DF;
					case 0x82F9/*JISX0213('こﾟ')*/:
					case 0x839B/*JISX0213('コﾟ')*/: goto HALFWIDTH_BADF;
					case 0x839C/*JISX0213('セﾟ')*/: goto HALFWIDTH_BEDF;
					case 0x839D/*JISX0213('ツﾟ')*/: goto HALFWIDTH_C2DF;
					case 0x839E/*JISX0213('トﾟ')*/: goto HALFWIDTH_C4DF;
					case 0x8492/*JISX0213('ワﾞ')*/: goto HALFWIDTH_DCDE;
					case 0x8495/*JISX0213('ヲﾞ')*/: goto HALFWIDTH_A6DE;
					}
				}
			} else if (dwMapFlags & LCMAP_HALFWIDTH) {
				switch (c) {
				case 0x8397/*JISX0213('カﾟ')*/: HALFWIDTH_B6DF: c = 0xB6DF/*'ｶﾟ'*/; break;
				case 0x8398/*JISX0213('キﾟ')*/: HALFWIDTH_B7DF: c = 0xB7DF/*'ｷﾟ'*/; break;
				case 0x8399/*JISX0213('クﾟ')*/: HALFWIDTH_B8DF: c = 0xB8DF/*'ｸﾟ'*/; break;
				case 0x839A/*JISX0213('ケﾟ')*/: HALFWIDTH_B9DF: c = 0xB9DF/*'ｹﾟ'*/; break;
				case 0x839B/*JISX0213('コﾟ')*/: HALFWIDTH_BADF: c = 0xBADF/*'ｺﾟ'*/; break;
				case 0x839C/*JISX0213('セﾟ')*/: HALFWIDTH_BEDF: c = 0xBEDF/*'ｾﾟ'*/; break;
				case 0x839D/*JISX0213('ツﾟ')*/: HALFWIDTH_C2DF: c = 0xC2DF/*'ﾂﾟ'*/; break;
				case 0x839E/*JISX0213('トﾟ')*/: HALFWIDTH_C4DF: c = 0xC4DF/*'ﾄﾟ'*/; break;
				case 0x8492/*JISX0213('ワﾞ')*/: HALFWIDTH_DCDE: c = 0xDCDE/*'ﾜﾞ'*/; break;
				case 0x8495/*JISX0213('ヲﾞ')*/: HALFWIDTH_A6DE: c = 0xA6DE/*'ｦﾞ'*/; break;
				}
			}
			if (high = (unsigned char)((prev = c) >> 8))
				*(dest++) = high;
			*(dest++) = (unsigned char)c;
		}
	}
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	HeapFree(hHeap, 0, lpBufferStr);
	return cchDest;
}

