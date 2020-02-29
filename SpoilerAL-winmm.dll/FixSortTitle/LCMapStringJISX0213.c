#include <windows.h>

#ifndef _M_IX86
#ifdef __BORLANDC__
#pragma warn -8060
#pragma warn -8066
#endif

#define BSWAP16(value) (            \
    (((value) >> 8) & 0x000000FF) | \
    (((value) << 8) & 0x0000FF00))

#define JISX0213(c) (       \
    (c) == 'Ç§ﬁ' ? 0x82F2 : \
    (c) == 'Ç©'  ? 0x82F3 : \
    (c) == 'ÇØ'  ? 0x82F4 : \
    (c) == 'Ç©ﬂ' ? 0x82F5 : \
    (c) == 'Ç´ﬂ' ? 0x82F6 : \
    (c) == 'Ç≠ﬂ' ? 0x82F7 : \
    (c) == 'ÇØﬂ' ? 0x82F8 : \
    (c) == 'Ç±ﬂ' ? 0x82F9 : \
    (c) == 'ÉJﬂ' ? 0x8397 : \
    (c) == 'ÉLﬂ' ? 0x8398 : \
    (c) == 'ÉNﬂ' ? 0x8399 : \
    (c) == 'ÉPﬂ' ? 0x839A : \
    (c) == 'ÉRﬂ' ? 0x839B : \
    (c) == 'ÉZﬂ' ? 0x839C : \
    (c) == 'Écﬂ' ? 0x839D : \
    (c) == 'Égﬂ' ? 0x839E : \
    (c) == 'ÉN'  ? 0x83EC : \
    (c) == 'ÉV'  ? 0x83ED : \
    (c) == 'ÉX'  ? 0x83EE : \
    (c) == 'Ég'  ? 0x83EF : \
    (c) == 'Ék'  ? 0x83F0 : \
    (c) == 'Én'  ? 0x83F1 : \
    (c) == 'Éq'  ? 0x83F2 : \
    (c) == 'Ét'  ? 0x83F3 : \
    (c) == 'Éw'  ? 0x83F4 : \
    (c) == 'Éz'  ? 0x83F5 : \
    (c) == 'Év'  ? 0x83F6 : \
    (c) == 'ÉÄ'  ? 0x83F7 : \
    (c) == 'Éâ'  ? 0x83F8 : \
    (c) == 'Éä'  ? 0x83F9 : \
    (c) == 'Éã'  ? 0x83FA : \
    (c) == 'Éå'  ? 0x83FB : \
    (c) == 'Éç'  ? 0x83FC : \
    (c) == 'Éèﬁ' ? 0x8492 : \
    (c) == 'Éêﬁ' ? 0x8493 : \
    (c) == 'Éëﬁ' ? 0x8494 : \
    (c) == 'Éíﬁ' ? 0x8495 : (c))

int __stdcall LCMapStringJISX0213(
	IN           LCID   Locale,
	IN           DWORD  dwMapFlags,
	IN           LPCSTR lpSrcStr,
	IN           int    cchSrc,
	OUT OPTIONAL LPSTR  lpDestStr,
	IN           int    cchDest)
{
	LPSTR               lpBufferStr;
	unsigned int        cchBuffer;
	HANDLE              hHeap;
	unsigned char       *dest;
	const unsigned char *src;
	ptrdiff_t           offset;
	unsigned char       c1, c2;
	DWORD               dwErrCode;

	if (!(cchBuffer = cchSrc))
		return 0;
	if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) != LANG_JAPANESE ||
		!(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)))
		return LCMapStringA(Locale, dwMapFlags, lpSrcStr, cchSrc, lpDestStr, cchDest);
	if (cchBuffer == -1)
		cchBuffer = strlen(lpSrcStr) + 1;
	if (!(lpBufferStr = (LPSTR)HeapAlloc(hHeap = GetProcessHeap(), 0, cchBuffer * sizeof(char)))) {
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return 0;
	}
	dest = lpBufferStr;
	src = lpSrcStr + cchBuffer;
	offset = -(ptrdiff_t)cchBuffer;

	if (dwMapFlags & LCMAP_FULLWIDTH)
		if (dwMapFlags & LCMAP_HIRAGANA)
			goto FULLWIDTH_HIRAGANA;
		else if (dwMapFlags & LCMAP_KATAKANA)
			goto FULLWIDTH_KATAKANA;
		else
			goto FULLWIDTH;
	else if (dwMapFlags & LCMAP_HIRAGANA)
		goto HIRAGANA;
	else if (!(dwMapFlags & LCMAP_HALFWIDTH))
		goto KATAKANA;
	else if (dwMapFlags & LCMAP_KATAKANA)
		goto KATAKANA_HALFWIDTH;
	else
		goto HALFWIDTH;

FULLWIDTH_HIRAGANA:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			if (c1 >= 0xB3/*'≥'*/) {
				c2 = src[offset];
				if (c1 == 0xB3/*'≥'*/) {
					if (c2 == 0xDE/*'ﬁ'*/)
						goto FULLWIDTH_HIRAGANA_82F2_4;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/)
						goto FULLWIDTH_HIRAGANA_82F2_2;
				} else if (c1 >= 0xB6/*'∂'*/ && c1 <= 0xBA/*'∫'*/) {
					if (c2 == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_HIRAGANA_82F5_1;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_HIRAGANA_82F5_1:
						offset++;
						dest[0] = 0x82;
						c2 = c1 + 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xB6/*'∂'*/;
						goto FULLWIDTH_HIRAGANA_CONTINUE_1;
					}
				}
			}
			dest++;
			continue;
		}
		c2 = src[offset];
		offset++;
		if (c1 == 0x82) {
			if (offset && c2 >= 0xA4/*('Ç§' - 0x8200)*/) {
				c1 = src[offset];
				if (c2 == 0xA4/*('Ç§' - 0x8200)*/)
					goto FULLWIDTH_HIRAGANA_82F2_1;
				if (c2 >= 0xA9/*('Ç©' - 0x8200)*/ && c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
					if (c1 == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_HIRAGANA_82F5_2;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_HIRAGANA_82F5_2:
						cchBuffer--;
						offset++;
						c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xA9/*('Ç©' - 0x8200)*/;
					}
				}
			}
		} else if (c1 == 0x83) {
			if (!offset)
				goto FULLWIDTH_HIRAGANA_82F2_82F9;
			if (c2 >= 0x45/*('ÉE' - 0x8300)*/) {
				c1 = src[offset];
				if (c2 == 0x45/*('ÉE' - 0x8300)*/) {
				FULLWIDTH_HIRAGANA_82F2_1:
					if (c1 == 0xDE/*'ﬁ'*/)
						goto FULLWIDTH_HIRAGANA_82F2_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_HIRAGANA_82F2_2:
						offset++;
					FULLWIDTH_HIRAGANA_82F2_3:
						cchBuffer--;
					FULLWIDTH_HIRAGANA_82F2_4:
						offset++;
						*(unsigned short *)dest = 0xF282/*BSWAP16(JISX0213('Ç§ﬁ'))*/;
						goto FULLWIDTH_HIRAGANA_CONTINUE_2;
					}
				} else if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
					if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
						if (c1 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_HIRAGANA_82F5_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_HIRAGANA_82F5_3:
							cchBuffer--;
							offset++;
							dest[0] = 0x82;
							c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
						}
					} else {
					FULLWIDTH_HIRAGANA_82F2_82F9:
						if (c2 >= 0x94/*('Éî' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
							dest[0] = 0x82;
							c2 += 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
						}
					}
				}
			}
		}
	FULLWIDTH_HIRAGANA_CONTINUE_1:
		dest[1] = c2;
	FULLWIDTH_HIRAGANA_CONTINUE_2:
		dest += 2;
		if (offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

FULLWIDTH_KATAKANA:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			if (c1 >= 0xA6/*'¶'*/) {
				c2 = src[offset];
				if (c1 == 0xA6/*'¶'*/) {
					if (c2 == 0xDE/*'ﬁ'*/)
						goto FULLWIDTH_KATAKANA_8495;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8495:
						offset++;
						*(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('Éíﬁ'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c1 >= 0xB6/*'∂'*/) {
					if (c1 <= 0xBA/*'∫'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_KATAKANA_8397_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8397_1:
							offset++;
							dest[0] = 0x83;
							c2 = c1 - 0xB6/*'∂'*/ + 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
							goto FULLWIDTH_KATAKANA_CONTINUE_1;
						}
					} else if (c1 == 0xBE/*'æ'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_KATAKANA_839C_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839C_2;
					} else if (c1 == 0xC2/*'¬'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_KATAKANA_839D_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839D_2;
					} else if (c1 == 0xC4/*'ƒ'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_KATAKANA_839E_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839E_2;
					} else if (c1 == 0xDC/*'‹'*/) {
						if (c2 == 0xDE/*'ﬁ'*/)
							goto FULLWIDTH_KATAKANA_8492_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8492_1:
							offset++;
							*(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('Éèﬁ'))*/;
							goto FULLWIDTH_KATAKANA_CONTINUE_2;
						}
					}
				}
			}
			dest++;
			continue;
		}
		c2 = src[offset];
		offset++;
		if (c1 == 0x82) {
			if (!offset)
				goto FULLWIDTH_KATAKANA_8394_829B;
			if (c2 >= 0xA9/*('Ç©' - 0x8200)*/) {
				if (c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
					if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_KATAKANA_8397_2;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8397_2:
						cchBuffer--;
						offset++;
						dest[0] = 0x83;
						c2 -= 0xA9/*('Ç©' - 0x8200)*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
					}
				} else if (c2 == 0xB9/*('Çπ' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839C_1;
				else if (c2 == 0xC2/*('Ç¬' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839D_1;
				else if (c2 == 0xC6/*('Ç∆' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839E_1;
				else if (c2 >= 0xED/*('ÇÌ' - 0x8200)*/) {
					if (c2 <= 0xF0/*('Ç' - 0x8200)*/) {
						if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
							goto FULLWIDTH_KATAKANA_8492_2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8492_2:
							cchBuffer--;
							offset++;
							dest[0] = 0x84;
							c2 -= 0xED/*('ÇÌ' - 0x8200)*/ - 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/;
						}
					} else {
					FULLWIDTH_KATAKANA_8394_829B:
						if (c2 >= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
							dest[0] = 0x83;
							c2 -= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
						}
					}
				}
			}
		} else if (offset && c1 == 0x83) {
			if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
				if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
					if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_KATAKANA_8397_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8397_3:
						cchBuffer--;
						offset++;
						c2 += 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
					}
				} else if (c2 == 0x5A/*('ÉZ' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839C_1:
					if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_KATAKANA_839C_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839C_2:
						offset++;
					FULLWIDTH_KATAKANA_839C_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839C_4:
						offset++;
						*(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('ÉZﬂ'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 == 0x63/*('Éc' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839D_1:
					if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_KATAKANA_839D_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839D_2:
						offset++;
					FULLWIDTH_KATAKANA_839D_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839D_4:
						offset++;
						*(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('Écﬂ'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 == 0x67/*('Ég' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839E_1:
					if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
						goto FULLWIDTH_KATAKANA_839E_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839E_2:
						offset++;
					FULLWIDTH_KATAKANA_839E_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839E_4:
						offset++;
						*(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('Égﬂ'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 >= 0x8F/*('Éè' - 0x8300)*/ && c2 <= 0x92/*('Éí' - 0x8300)*/) {
					if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
						goto FULLWIDTH_KATAKANA_8492_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8492_3:
						cchBuffer--;
						offset++;
						dest[0] = 0x84;
						c2 += 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/ - 0x8F/*('Éè' - 0x8300)*/;
					}
				}
			}
		}
	FULLWIDTH_KATAKANA_CONTINUE_1:
		dest[1] = c2;
	FULLWIDTH_KATAKANA_CONTINUE_2:
		dest += 2;
		if (offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

FULLWIDTH:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			if (c1 >= 0xA6/*'¶'*/) {
				c2 = src[offset];
				if (c1 == 0xA6/*'¶'*/) {
					if (c2 == 0xDE/*'ﬁ'*/)
						goto FULLWIDTH_8495;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_8495:
						offset++;
						*(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('Éíﬁ'))*/;
						goto FULLWIDTH_CONTINUE_2;
					}
				} else if (c1 >= 0xB6/*'∂'*/) {
					if (c1 <= 0xBA/*'∫'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_8397_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8397_1:
							offset++;
							dest[0] = 0x83;
							c2 = c1 - 0xB6/*'∂'*/ + 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
							goto FULLWIDTH_CONTINUE_1;
						}
					} else if (c1 == 0xBE/*'æ'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839C_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_839C_2;
					} else if (c1 == 0xC2/*'¬'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839D_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_839D_2;
					} else if (c1 == 0xC4/*'ƒ'*/) {
						if (c2 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839E_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
							goto FULLWIDTH_839E_2;
					} else if (c1 == 0xDC/*'‹'*/) {
						if (c2 == 0xDE/*'ﬁ'*/)
							goto FULLWIDTH_8492_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8492_1:
							offset++;
							*(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('Éèﬁ'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					}
				}
			}
			dest++;
			continue;
		}
		c2 = src[offset];
		if (++offset) {
			if (c1 == 0x82) {
				if (c2 >= 0xA4/*('Ç§' - 0x8200)*/) {
					c1 = src[offset];
					if (c2 == 0xA4/*('Ç§' - 0x8200)*/) {
						if (c1 == 0xDE/*'ﬁ'*/)
							goto FULLWIDTH_82F2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_82F2:
							cchBuffer--;
							offset++;
							c2 = 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/;
							goto FULLWIDTH_CONTINUE_1;
						}
					} else if (c2 >= 0xA9/*('Ç©' - 0x8200)*/) {
						if (c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
							if (c1 == 0xDF/*'ﬂ'*/)
								goto FULLWIDTH_82F5;
							if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
								cchBuffer--;
								offset++;
							FULLWIDTH_82F5:
								cchBuffer--;
								offset++;
								c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xA9/*('Ç©' - 0x8200)*/;
							}
						} else if (c2 == 0xB9/*('Çπ' - 0x8200)*/)
							goto FULLWIDTH_839C_1;
						else if (c2 == 0xC2/*('Ç¬' - 0x8200)*/)
							goto FULLWIDTH_839D_1;
						else if (c2 == 0xC6/*('Ç∆' - 0x8200)*/)
							goto FULLWIDTH_839E_1;
						else if (c2 >= 0xED/*('ÇÌ' - 0x8200)*/ && c2 <= 0xF0/*('Ç' - 0x8200)*/) {
							if (c1 == 0xDE/*'ﬁ'*/)
								goto FULLWIDTH_8492_2;
							if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
								cchBuffer--;
								offset++;
							FULLWIDTH_8492_2:
								cchBuffer--;
								offset++;
								dest[0] = 0x84;
								c2 -= 0xED/*('ÇÌ' - 0x8200)*/ - 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/;
							}
						}
					}
				}
			} else if (c1 == 0x83) {
				if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
					if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
						if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_8397_2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8397_2:
							cchBuffer--;
							offset++;
							c2 += 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
						}
					} else if (c2 == 0x5A/*('ÉZ' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839C_1:
						if (c1 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839C_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839C_2:
							offset++;
						FULLWIDTH_839C_3:
							cchBuffer--;
						FULLWIDTH_839C_4:
							offset++;
							*(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('ÉZﬂ'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 == 0x63/*('Éc' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839D_1:
						if (c1 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839D_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839D_2:
							offset++;
						FULLWIDTH_839D_3:
							cchBuffer--;
						FULLWIDTH_839D_4:
							offset++;
							*(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('Écﬂ'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 == 0x67/*('Ég' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839E_1:
						if (c1 == 0xDF/*'ﬂ'*/)
							goto FULLWIDTH_839E_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839E_2:
							offset++;
						FULLWIDTH_839E_3:
							cchBuffer--;
						FULLWIDTH_839E_4:
							offset++;
							*(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('Égﬂ'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 >= 0x8F/*('Éè' - 0x8300)*/ && c2 <= 0x92/*('Éí' - 0x8300)*/) {
						if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
							goto FULLWIDTH_8492_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8492_3:
							cchBuffer--;
							offset++;
							dest[0] = 0x84;
							c2 += 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/ - 0x8F/*('Éè' - 0x8300)*/;
						}
					}
				}
			}
		}
	FULLWIDTH_CONTINUE_1:
		dest[1] = c2;
	FULLWIDTH_CONTINUE_2:
		dest += 2;
		if (offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

HIRAGANA:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x83) {
			if (c2 >= 0x94/*('Éî' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
				dest[0] = 0x82;
				c2 += 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
			}
		}
		dest[1] = c2;
		dest += 2;
		if (++offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

KATAKANA:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x82) {
			if (c2 >= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
				dest[0] = 0x83;
				c2 -= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
			}
		}
		dest[1] = c2;
		dest += 2;
		if (++offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

KATAKANA_HALFWIDTH:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x82) {
			if (c2 >= 0xF2) {
				if (c2 == 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/) {
					*(unsigned short *)dest = 0xDEB3/*BSWAP16('≥ﬁ')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 >= 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x83) {
			if (c2 >= 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/) {
				if (c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9C/*(JISX0213('ÉZﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFBE/*BSWAP16('æﬂ')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9D/*(JISX0213('Écﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC2/*BSWAP16('¬ﬂ')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9E/*(JISX0213('Égﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC4/*BSWAP16('ƒﬂ')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x84) {
			if (c2 == 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEDC/*BSWAP16('‹ﬁ')*/;
				goto KATAKANA_HALFWIDTH_CONTINUE;
			} else if (c2 == 0x95/*(JISX0213('Éíﬁ') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEA6/*BSWAP16('¶ﬁ')*/;
				goto KATAKANA_HALFWIDTH_CONTINUE;
			}
		}
		dest[1] = c2;
	KATAKANA_HALFWIDTH_CONTINUE:
		dest += 2;
		if (++offset)
			continue;
		break;
	}
	goto LCMAP_STRING;

HALFWIDTH:
	for (; ; ) {
		dest[0] = c1 = src[offset];
		if (!++offset)
			break;
		if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x83) {
			if (c2 >= 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/) {
				if (c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9C/*(JISX0213('ÉZﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFBE/*BSWAP16('æﬂ')*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9D/*(JISX0213('Écﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC2/*BSWAP16('¬ﬂ')*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9E/*(JISX0213('Égﬂ') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC4/*BSWAP16('ƒﬂ')*/;
					goto HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x84) {
			if (c2 == 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEDC/*BSWAP16('‹ﬁ')*/;
				goto HALFWIDTH_CONTINUE;
			} else if (c2 == 0x95/*(JISX0213('Éíﬁ') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEA6/*BSWAP16('¶ﬁ')*/;
				goto HALFWIDTH_CONTINUE;
			}
		}
		dest[1] = c2;
	HALFWIDTH_CONTINUE:
		dest += 2;
		if (++offset)
			continue;
		break;
	}

LCMAP_STRING:
	cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	dwErrCode = GetLastError();
	HeapFree(hHeap, 0, lpBufferStr);
	SetLastError(dwErrCode);
	return cchDest;
}
#else
#ifdef __BORLANDC__
#pragma warn -8070
#define align16
#else
#define align16 align 16
#endif

#pragma function(strlen)
__declspec(naked) int __stdcall LCMapStringJISX0213(
	IN           LCID   Locale,
	IN           DWORD  dwMapFlags,
	IN           LPCSTR lpSrcStr,
	IN           int    cchSrc,
	OUT OPTIONAL LPSTR  lpDestStr,
	IN           int    cchDest)
{
	__asm
	{
	        #define Locale      (esp +  4)
	        #define dwMapFlags  (esp +  8)
	        #define lpSrcStr    (esp + 12)
	        #define cchSrc      (esp + 16)
	        #define lpDestStr   (esp + 20)
	        #define cchDest     (esp + 24)

	        #define lpBufferStr (esp + 12)
	        #define hHeap       (esp + 16)
	        #define cchBuffer   ebx
	        #define offset      ebp
	        #define src         esi
	        #define dest        edi
	        #define c1          al
	        #define c2          cl

	        #define LCMAP_FLAGS (LCMAP_FULLWIDTH or LCMAP_HALFWIDTH or LCMAP_KATAKANA or LCMAP_HIRAGANA)

	        mov     eax, dword ptr [cchSrc]             //
	        mov     ecx, dword ptr [Locale]             //
	        test    eax, eax                            //     if (!(cchBuffer = cchSrc))
	        jz      L103                                //         return 0;
	        and     ecx, 0x03FF                         //
	        mov     edx, dword ptr [dwMapFlags]         //
	        cmp     ecx, LANG_JAPANESE                  //     if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) != LANG_JAPANESE ||
	        jne     L100                                //         !(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)))
	        test    edx, LCMAP_FLAGS                    //
	        jnz     L101                                //
	L100:   jmp     LCMapStringA                        //         return LCMapStringA(Locale, dwMapFlags, lpSrcStr, cchSrc, lpDestStr, cchDest);

	        align16                                     //
	L101:   cmp     eax, -1                             //     if (cchBuffer == -1)
	        jne     L102                                //
	        push    dword ptr [lpSrcStr]                //         cchBuffer = strlen(lpSrcStr) + 1;
	        call    strlen                              //
	        pop     ecx                                 //
	        inc     eax                                 //
	L102:   push    ebx                                 //     if (!(lpBufferStr = (LPSTR)HeapAlloc(hHeap = GetProcessHeap(), 0, cchBuffer * sizeof(char)))) {
	        mov     ebx, eax                            //
	        call    GetProcessHeap                      //
	        mov     dword ptr [hHeap + 4], eax          //
	        push    ebx                                 //
	        push    0                                   //
	        push    eax                                 //
	        call    HeapAlloc                           //
	        test    eax, eax                            //
	        jnz     L104                                //
	        push    ERROR_NOT_ENOUGH_MEMORY             //         SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	        call    SetLastError                        //
	        xor     eax, eax                            //         return 0;
	        pop     ebx                                 //
	L103:   ret     24                                  //     }

	        align16                                     //
	L104:   push    ebp                                 //
	        push    esi                                 //
	        push    edi                                 //
	        mov     edi, eax                            //     dest = lpBufferStr;
	        mov     edx, dword ptr [dwMapFlags + 16]    //
	        mov     esi, dword ptr [lpSrcStr + 16]      //     src = lpSrcStr + cchBuffer;
	        mov     dword ptr [lpBufferStr + 16], eax   //
	        lea     ebp, [ebx - 1]                      //     offset = -(ptrdiff_t)cchBuffer;
	        add     esi, ebx                            //
	        xor     ebp, -1                             //
	        xor     eax, eax                            //
	        xor     ecx, ecx                            //
	        test    edx, LCMAP_FULLWIDTH                //     if (dwMapFlags & LCMAP_FULLWIDTH)
	        jz      L105                                //
	        test    edx, LCMAP_HIRAGANA                 //         if (dwMapFlags & LCMAP_HIRAGANA)
	        jnz     L200                                //             goto FULLWIDTH_HIRAGANA;
	        test    edx, LCMAP_KATAKANA                 //         else if (dwMapFlags & LCMAP_KATAKANA)
	        jnz     L300                                //             goto FULLWIDTH_KATAKANA;
	                                                    //         else
	        jmp     L400                                //             goto FULLWIDTH;

	L105:   test    edx, LCMAP_HIRAGANA                 //     else if (dwMapFlags & LCMAP_HIRAGANA)
	        jnz     L500                                //         goto HIRAGANA;
	        test    edx, LCMAP_HALFWIDTH                //     else if (!(dwMapFlags & LCMAP_HALFWIDTH))
	        jz      L600                                //         goto KATAKANA;
	        test    edx, LCMAP_KATAKANA                 //     else if (dwMapFlags & LCMAP_KATAKANA)
	        jnz     L700                                //         goto KATAKANA_HALFWIDTH;
	                                                    //     else
	        jmp     L800                                //         goto HALFWIDTH;

	        align16                                     // FULLWIDTH_HIRAGANA:
	                                                    //     for (; ; ) {
	L200:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L204                                //
	        cmp     al, 0x9F                            //
	        jbe     L205                                //
	        cmp     al, 0xE0                            //
	        jb      L201                                //
	        cmp     al, 0xFC                            //
	        jbe     L205                                //
	        jmp     L204                                //

	L201:   cmp     al, 0xB3                            //             if (c1 >= 0xB3/*'≥'*/) {
	        jb      L204                                //
	        mov     cl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L202                                //                 if (c1 == 0xB3/*'≥'*/) {
	        cmp     cl, 0xDE                            //                     if (c2 == 0xDE/*'ﬁ'*/)
	        je      L211                                //                         goto FULLWIDTH_HIRAGANA_82F2_4;
	        cmp     cl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/)
	        jne     L204                                //
	        cmp     ebp, -1                             //
	        je      L204                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L204                                //
	        jmp     L209                                //                         goto FULLWIDTH_HIRAGANA_82F2_2;

	L202:   cmp     al, 0xB6                            //                 } else if (c1 >= 0xB6/*'∂'*/ && c1 <= 0xBA/*'∫'*/) {
	        jb      L204                                //
	        cmp     al, 0xBA                            //
	        ja      L204                                //
	        cmp     cl, 0xDF                            //                     if (c2 == 0xDF/*'ﬂ'*/)
	        je      L203                                //                         goto FULLWIDTH_HIRAGANA_82F5_1;
	        cmp     cl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L204                                //
	        cmp     ebp, -1                             //
	        je      L204                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L204                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F5_1:
	L203:   inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x82                //                         dest[0] = 0x82;
	        lea     ecx, [eax + 0xF5 - 0xB6]            //                         c2 = c1 + 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xB6/*'∂'*/;
	        jmp     L215                                //                         goto FULLWIDTH_HIRAGANA_CONTINUE_1;
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L204:   inc     edi                                 //             dest++;
	        jmp     L200                                //             continue;
	                                                    //         }
	        align16                                     //
	L205:   mov     cl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         offset++;
	        cmp     al, 0x82                            //         if (c1 == 0x82) {
	        jne     L207                                //
	        test    ebp, ebp                            //             if (offset && c2 >= 0xA4/*('Ç§' - 0x8200)*/) {
	        jz      L215                                //
	        cmp     cl, 0xA4                            //
	        jb      L215                                //
	        mov     al, byte ptr [esi + ebp]            //                 c1 = src[offset];
	                                                    //                 if (c2 == 0xA4/*('Ç§' - 0x8200)*/)
	        je      L208                                //                     goto FULLWIDTH_HIRAGANA_82F2_1;
	        cmp     cl, 0xA9                            //                 if (c2 >= 0xA9/*('Ç©' - 0x8200)*/ && c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
	        jb      L215                                //
	        cmp     cl, 0xB1                            //
	        ja      L215                                //
	        cmp     al, 0xDF                            //                     if (c1 == 0xDF/*'ﬂ'*/)
	        je      L206                                //                         goto FULLWIDTH_HIRAGANA_82F5_2;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L215                                //
	        cmp     ebp, -1                             //
	        je      L215                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L215                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F5_2:
	L206:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        add     cl, 0xF5 - 0xA9                     //                         c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xA9/*('Ç©' - 0x8200)*/;
	        jmp     L215                                //                     }
	                                                    //                 }
	                                                    //             }
	L207:   cmp     al, 0x83                            //         } else if (c1 == 0x83) {
	        jne     L215                                //
	        test    ebp, ebp                            //             if (!offset)
	        jz      L214                                //                 goto FULLWIDTH_HIRAGANA_82F2_82F9;
	        cmp     cl, 0x45                            //             if (c2 >= 0x45/*('ÉE' - 0x8300)*/) {
	        jb      L215                                //
	        mov     al, byte ptr [esi + ebp]            //                 c1 = src[offset];
	        jne     L212                                //                 if (c2 == 0x45/*('ÉE' - 0x8300)*/) {
	                                                    //                 FULLWIDTH_HIRAGANA_82F2_1:
	L208:   cmp     al, 0xDE                            //                     if (c1 == 0xDE/*'ﬁ'*/)
	        je      L210                                //                         goto FULLWIDTH_HIRAGANA_82F2_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L215                                //
	        cmp     ebp, -1                             //
	        je      L215                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L215                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_2:
	L209:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_3:
	L210:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_4:
	L211:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0xF282              //                         *(unsigned short *)dest = 0xF282/*BSWAP16(JISX0213('Ç§ﬁ'))*/;
	        jmp     L216                                //                         goto FULLWIDTH_HIRAGANA_CONTINUE_2;
	                                                    //                     }
	L212:   cmp     cl, 0x4A                            //                 } else if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
	        jb      L215                                //
	        cmp     cl, 0x52                            //                     if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
	        ja      L214                                //
	        cmp     al, 0xDF                            //                         if (c1 == 0xDF/*'ﬂ'*/)
	        je      L213                                //                             goto FULLWIDTH_HIRAGANA_82F5_3;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L215                                //
	        cmp     ebp, -1                             //
	        je      L215                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L215                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_HIRAGANA_82F5_3:
	L213:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x82                //                             dest[0] = 0x82;
	        add     cl, 0xF5 - 0x4A                     //                             c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
	        jmp     L215                                //                         }
	                                                    //                     } else {
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_82F9:
	L214:   cmp     cl, 0x94                            //                         if (c2 >= 0x94/*('Éî' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
	        jb      L215                                //
	        cmp     cl, 0x9B                            //
	        ja      L215                                //
	        mov     byte ptr [edi], 0x82                //                             dest[0] = 0x82;
	        add     cl, 0xF2 - 0x94                     //                             c2 += 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_HIRAGANA_CONTINUE_1:
	L215:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_HIRAGANA_CONTINUE_2:
	L216:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L200                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // FULLWIDTH_KATAKANA:
	                                                    //     for (; ; ) {
	L300:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L310                                //
	        cmp     al, 0x9F                            //
	        jbe     L311                                //
	        cmp     al, 0xE0                            //
	        jb      L301                                //
	        cmp     al, 0xFC                            //
	        jbe     L311                                //
	        jmp     L310                                //

	L301:   cmp     al, 0xA6                            //             if (c1 >= 0xA6/*'¶'*/) {
	        jb      L310                                //
	        mov     cl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L303                                //                 if (c1 == 0xA6/*'¶'*/) {
	        cmp     cl, 0xDE                            //                     if (c2 == 0xDE/*'ﬁ'*/)
	        je      L302                                //                         goto FULLWIDTH_KATAKANA_8495;
	        cmp     cl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L310                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8495:
	L302:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9584              //                         *(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('Éíﬁ'))*/;
	        jmp     L336                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L303:   cmp     al, 0xB6                            //                 } else if (c1 >= 0xB6/*'∂'*/) {
	        jb      L310                                //
	        cmp     al, 0xBA                            //                     if (c1 <= 0xBA/*'∫'*/) {
	        ja      L305                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L304                                //                             goto FULLWIDTH_KATAKANA_8397_1;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L310                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8397_1:
	L304:   inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        lea     ecx, [eax - 0xB6 + 0x97]            //                             c2 = c1 - 0xB6/*'∂'*/ + 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
	        jmp     L335                                //                             goto FULLWIDTH_KATAKANA_CONTINUE_1;
	                                                    //                         }
	L305:   cmp     al, 0xBE                            //                     } else if (c1 == 0xBE/*'æ'*/) {
	        jne     L306                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L322                                //                             goto FULLWIDTH_KATAKANA_839C_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L310                                //
	        jmp     L320                                //                             goto FULLWIDTH_KATAKANA_839C_2;

	L306:   cmp     al, 0xC2                            //                     } else if (c1 == 0xC2/*'¬'*/) {
	        jne     L307                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L327                                //                             goto FULLWIDTH_KATAKANA_839D_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L310                                //
	        jmp     L325                                //                             goto FULLWIDTH_KATAKANA_839D_2;

	L307:   cmp     al, 0xC4                            //                     } else if (c1 == 0xC4/*'ƒ'*/) {
	        jne     L308                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L332                                //                             goto FULLWIDTH_KATAKANA_839E_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L310                                //
	        jmp     L330                                //                             goto FULLWIDTH_KATAKANA_839E_2;

	L308:   cmp     al, 0xDC                            //                     } else if (c1 == 0xDC/*'‹'*/) {
	        jne     L310                                //
	        cmp     cl, 0xDE                            //                         if (c2 == 0xDE/*'ﬁ'*/)
	        je      L309                                //                             goto FULLWIDTH_KATAKANA_8492_1;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L310                                //
	        cmp     ebp, -1                             //
	        je      L310                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L310                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8492_1:
	L309:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9284              //                             *(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('Éèﬁ'))*/;
	        jmp     L336                                //                             goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L310:   inc     edi                                 //             dest++;
	        jmp     L300                                //             continue;
	                                                    //         }
	        align16                                     //
	L311:   mov     cl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         offset++;
	        cmp     al, 0x82                            //         if (c1 == 0x82) {
	        jne     L316                                //
	        test    ebp, ebp                            //             if (!offset)
	        jz      L315                                //                 goto FULLWIDTH_KATAKANA_8394_829B;
	        cmp     cl, 0xA9                            //             if (c2 >= 0xA9/*('Ç©' - 0x8200)*/) {
	        jb      L335                                //
	        cmp     cl, 0xB1                            //                 if (c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
	        ja      L313                                //
	        mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L312                                //                         goto FULLWIDTH_KATAKANA_8397_2;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8397_2:
	L312:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x83                //                         dest[0] = 0x83;
	        sub     cl, 0xA9 - 0x97                     //                         c2 -= 0xA9/*('Ç©' - 0x8200)*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
	        jmp     L335                                //                     }

	L313:   cmp     cl, 0xB9                            //                 } else if (c2 == 0xB9/*('Çπ' - 0x8200)*/)
	        je      L319                                //                     goto FULLWIDTH_KATAKANA_839C_1;
	        cmp     cl, 0xC2                            //                 else if (c2 == 0xC2/*('Ç¬' - 0x8200)*/)
	        je      L324                                //                     goto FULLWIDTH_KATAKANA_839D_1;
	        cmp     cl, 0xC6                            //                 else if (c2 == 0xC6/*('Ç∆' - 0x8200)*/)
	        je      L329                                //                     goto FULLWIDTH_KATAKANA_839E_1;
	        cmp     cl, 0xED                            //                 else if (c2 >= 0xED/*('ÇÌ' - 0x8200)*/) {
	        jb      L335                                //
	        cmp     cl, 0xF0                            //                     if (c2 <= 0xF0/*('Ç' - 0x8200)*/) {
	        ja      L315                                //
	        mov     al, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
	        cmp     al, 0xDE                            //
	        je      L314                                //                             goto FULLWIDTH_KATAKANA_8492_2;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L335                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8492_2:
	L314:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x84                //                             dest[0] = 0x84;
	        sub     cl, 0xED - 0x92                     //                             c2 -= 0xED/*('ÇÌ' - 0x8200)*/ - 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/;
	        jmp     L335                                //                         }
	                                                    //                     } else {
	                                                    //                     FULLWIDTH_KATAKANA_8394_829B:
	L315:   cmp     cl, 0xF2                            //                         if (c2 >= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
	        jb      L335                                //
	        cmp     cl, 0xF9                            //
	        ja      L335                                //
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        sub     cl, 0xF2 - 0x94                     //                             c2 -= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
	        jmp     L335                                //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L316:   test    ebp, ebp                            //         } else if (offset && c1 == 0x83) {
	        jz      L335                                //
	        cmp     al, 0x83                            //
	        jne     L335                                //
	        cmp     cl, 0x4A                            //             if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
	        jb      L335                                //
	        cmp     cl, 0x52                            //                 if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
	        ja      L318                                //
	        mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L317                                //                         goto FULLWIDTH_KATAKANA_8397_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8397_3:
	L317:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        add     cl, 0x97 - 0x4A                     //                         c2 += 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
	        jmp     L335                                //                     }

	L318:   cmp     cl, 0x5A                            //                 } else if (c2 == 0x5A/*('ÉZ' - 0x8300)*/) {
	        jne     L323                                //
	                                                    //                 FULLWIDTH_KATAKANA_839C_1:
	L319:   mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L321                                //                         goto FULLWIDTH_KATAKANA_839C_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839C_2:
	L320:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839C_3:
	L321:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839C_4:
	L322:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9C83              //                         *(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('ÉZﬂ'))*/;
	        jmp     L336                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L323:   cmp     cl, 0x63                            //                 } else if (c2 == 0x63/*('Éc' - 0x8300)*/) {
	        jne     L328                                //
	                                                    //                 FULLWIDTH_KATAKANA_839D_1:
	L324:   mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L326                                //                         goto FULLWIDTH_KATAKANA_839D_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839D_2:
	L325:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839D_3:
	L326:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839D_4:
	L327:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9D83              //                         *(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('Écﬂ'))*/;
	        jmp     L336                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L328:   cmp     cl, 0x63                            //                 } else if (c2 == 0x67/*('Ég' - 0x8300)*/) {
	        jne     L333                                //
	                                                    //                 FULLWIDTH_KATAKANA_839E_1:
	L329:   mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L331                                //                         goto FULLWIDTH_KATAKANA_839E_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839E_2:
	L330:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839E_3:
	L331:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839E_4:
	L332:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9E83              //                         *(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('Égﬂ'))*/;
	        jmp     L336                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L333:   cmp     cl, 0x8F                            //                 } else if (c2 >= 0x8F/*('Éè' - 0x8300)*/ && c2 <= 0x92/*('Éí' - 0x8300)*/) {
	        jb      L335                                //
	        cmp     cl, 0x92                            //
	        ja      L335                                //
	        mov     al, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
	        cmp     al, 0xDE                            //
	        je      L334                                //                         goto FULLWIDTH_KATAKANA_8492_3;
	        cmp     al, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L335                                //
	        cmp     ebp, -1                             //
	        je      L335                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L335                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8492_3:
	L334:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x84                //                         dest[0] = 0x84;
	        add     cl, 0x92 - 0x8F                     //                         c2 += 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/ - 0x8F/*('Éè' - 0x8300)*/;
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_KATAKANA_CONTINUE_1:
	L335:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_KATAKANA_CONTINUE_2:
	L336:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L300                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // FULLWIDTH:
	                                                    //     for (; ; ) {
	L400:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L410                                //
	        cmp     al, 0x9F                            //
	        jbe     L411                                //
	        cmp     al, 0xE0                            //
	        jb      L401                                //
	        cmp     al, 0xFC                            //
	        jbe     L411                                //
	        jmp     L410                                //

	L401:   cmp     al, 0xA6                            //             if (c1 >= 0xA6/*'¶'*/) {
	        jb      L410                                //
	        mov     cl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L403                                //                 if (c1 == 0xA6/*'¶'*/) {
	        cmp     cl, 0xDE                            //                     if (c2 == 0xDE/*'ﬁ'*/)
	        je      L402                                //                         goto FULLWIDTH_8495;
	        cmp     cl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L410                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_8495:
	L402:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9584              //                         *(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('Éíﬁ'))*/;
	        jmp     L437                                //                         goto FULLWIDTH_CONTINUE_2;
	                                                    //                     }
	L403:   cmp     al, 0xB6                            //                 } else if (c1 >= 0xB6/*'∂'*/) {
	        jb      L410                                //
	        cmp     al, 0xBA                            //                     if (c1 <= 0xBA/*'∫'*/) {
	        ja      L405                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L404                                //                             goto FULLWIDTH_8397_1;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L410                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8397_1:
	L404:   inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        lea     ecx, [eax - 0xB6 + 0x97]            //                             c2 = c1 - 0xB6/*'∂'*/ + 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_1;
	                                                    //                         }
	L405:   cmp     al, 0xBE                            //                     } else if (c1 == 0xBE/*'æ'*/) {
	        jne     L406                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L423                                //                             goto FULLWIDTH_839C_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L410                                //
	        jmp     L421                                //                             goto FULLWIDTH_839C_2;

	L406:   cmp     al, 0xC2                            //                     } else if (c1 == 0xC2/*'¬'*/) {
	        jne     L407                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L428                                //                             goto FULLWIDTH_839D_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L410                                //
	        jmp     L426                                //                             goto FULLWIDTH_839D_2;

	L407:   cmp     al, 0xC4                            //                     } else if (c1 == 0xC4/*'ƒ'*/) {
	        jne     L408                                //
	        cmp     cl, 0xDF                            //                         if (c2 == 0xDF/*'ﬂ'*/)
	        je      L433                                //                             goto FULLWIDTH_839E_4;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/)
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L410                                //
	        jmp     L431                                //                             goto FULLWIDTH_839E_2;

	L408:   cmp     al, 0xDC                            //                     } else if (c1 == 0xDC/*'‹'*/) {
	        jne     L410                                //
	        cmp     cl, 0xDE                            //                         if (c2 == 0xDE/*'ﬁ'*/)
	        je      L409                                //                             goto FULLWIDTH_8492_1;
	        cmp     cl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L410                                //
	        cmp     ebp, -1                             //
	        je      L410                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L410                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8492_1:
	L409:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9284              //                             *(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('Éèﬁ'))*/;
	        jmp     L437                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L410:   inc     edi                                 //             dest++;
	        jmp     L400                                //             continue;
	                                                    //         }
	        align16                                     //
	L411:   mov     cl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         if (++offset) {
	        jz      L436                                //
	        cmp     al, 0x82                            //             if (c1 == 0x82) {
	        jne     L417                                //
	        cmp     cl, 0xA4                            //                 if (c2 >= 0xA4/*('Ç§' - 0x8200)*/) {
	        jb      L436                                //
	        mov     al, byte ptr [esi + ebp]            //                     c1 = src[offset];
	        jne     L413                                //                     if (c2 == 0xA4/*('Ç§' - 0x8200)*/) {
	        cmp     al, 0xDE                            //                         if (c1 == 0xDE/*'ﬁ'*/)
	        je      L412                                //                             goto FULLWIDTH_82F2;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_82F2:
	L412:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     cl, 0xF2                            //                             c2 = 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_1;
	                                                    //                         }
	L413:   cmp     cl, 0xA9                            //                     } else if (c2 >= 0xA9/*('Ç©' - 0x8200)*/) {
	        jb      L436                                //
	        cmp     cl, 0xB1                            //                         if (c2 <= 0xB1/*('Ç±' - 0x8200)*/) {
	        ja      L415                                //
	        cmp     al, 0xDF                            //                             if (c1 == 0xDF/*'ﬂ'*/)
	        je      L414                                //                                 goto FULLWIDTH_82F5;
	        cmp     al, 0x81                            //                             if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L436                                //
	        dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	                                                    //                             FULLWIDTH_82F5:
	L414:   dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	        add     cl, 0xF5 - 0xA9                     //                                 c2 += 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ - 0xA9/*('Ç©' - 0x8200)*/;
	        jmp     L436                                //                             }

	L415:   cmp     cl, 0xB9                            //                         } else if (c2 == 0xB9/*('Çπ' - 0x8200)*/)
	        je      L420                                //                             goto FULLWIDTH_839C_1;
	        cmp     cl, 0xC2                            //                         else if (c2 == 0xC2/*('Ç¬' - 0x8200)*/)
	        je      L425                                //                             goto FULLWIDTH_839D_1;
	        cmp     cl, 0xC6                            //                         else if (c2 == 0xC6/*('Ç∆' - 0x8200)*/)
	        je      L430                                //                             goto FULLWIDTH_839E_1;
	        cmp     cl, 0xED                            //                         else if (c2 >= 0xED/*('ÇÌ' - 0x8200)*/ && c2 <= 0xF0/*('Ç' - 0x8200)*/) {
	        jb      L436                                //
	        cmp     cl, 0xF0                            //
	        ja      L436                                //
	        cmp     al, 0xDE                            //                             if (c1 == 0xDE/*'ﬁ'*/)
	        je      L416                                //                                 goto FULLWIDTH_8492_2;
	        cmp     al, 0x81                            //                             if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L436                                //
	        dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	                                                    //                             FULLWIDTH_8492_2:
	L416:   dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	        mov     byte ptr [edi], 0x84                //                                 dest[0] = 0x84;
	        sub     cl, 0xED - 0x92                     //                                 c2 -= 0xED/*('ÇÌ' - 0x8200)*/ - 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/;
	        jmp     L436                                //                             }
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	L417:   cmp     al, 0x83                            //             } else if (c1 == 0x83) {
	        jne     L436                                //
	        cmp     cl, 0x4A                            //                 if (c2 >= 0x4A/*('ÉJ' - 0x8300)*/) {
	        jb      L436                                //
	        cmp     cl, 0x52                            //                     if (c2 <= 0x52/*('ÉR' - 0x8300)*/) {
	        ja      L419                                //
	        mov     al, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDF/*'ﬂ'*/)
	        cmp     al, 0xDF                            //
	        je      L418                                //                             goto FULLWIDTH_8397_2;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8397_2:
	L418:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        add     cl, 0x97 - 0x4A                     //                             c2 += 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/ - 0x4A/*('ÉJ' - 0x8300)*/;
	        jmp     L436                                //                         }

	L419:   cmp     cl, 0x5A                            //                     } else if (c2 == 0x5A/*('ÉZ' - 0x8300)*/) {
	        jne     L424                                //
	        mov     al, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839C_1:
	L420:   cmp     al, 0xDF                            //                         if (c1 == 0xDF/*'ﬂ'*/)
	        je      L422                                //                             goto FULLWIDTH_839C_3;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839C_2:
	L421:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839C_3:
	L422:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839C_4:
	L423:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9C83              //                             *(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('ÉZﬂ'))*/;
	        jmp     L437                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L424:   cmp     cl, 0x63                            //                     } else if (c2 == 0x63/*('Éc' - 0x8300)*/) {
	        jne     L429                                //
	        mov     al, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839D_1:
	L425:   cmp     al, 0xDF                            //                         if (c1 == 0xDF/*'ﬂ'*/)
	        je      L427                                //                             goto FULLWIDTH_839D_3;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839D_2:
	L426:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839D_3:
	L427:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839D_4:
	L428:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9D83              //                             *(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('Écﬂ'))*/;
	        jmp     L437                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L429:   cmp     cl, 0x63                            //                     } else if (c2 == 0x67/*('Ég' - 0x8300)*/) {
	        jne     L434                                //
	        mov     al, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839E_1:
	L430:   cmp     al, 0xDF                            //                         if (c1 == 0xDF/*'ﬂ'*/)
	        je      L432                                //                             goto FULLWIDTH_839E_3;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('ÅK' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839E_2:
	L431:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839E_3:
	L432:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839E_4:
	L433:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9E83              //                             *(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('Égﬂ'))*/;
	        jmp     L437                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L434:   cmp     cl, 0x8F                            //                     } else if (c2 >= 0x8F/*('Éè' - 0x8300)*/ && c2 <= 0x92/*('Éí' - 0x8300)*/) {
	        jb      L436                                //
	        cmp     cl, 0x92                            //
	        ja      L436                                //
	        mov     al, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDE/*'ﬁ'*/)
	        cmp     al, 0xDE                            //
	        je      L435                                //                             goto FULLWIDTH_8492_3;
	        cmp     al, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('ÅJ' - 0x8100)*/) {
	        jne     L436                                //
	        cmp     ebp, -1                             //
	        je      L436                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L436                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8492_3:
	L435:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x84                //                             dest[0] = 0x84;
	        add     cl, 0x92 - 0x8F                     //                             c2 += 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/ - 0x8F/*('Éè' - 0x8300)*/;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_CONTINUE_1:
	L436:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_CONTINUE_2:
	L437:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L400                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // HIRAGANA:
	                                                    //     for (; ; ) {
	L500:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L501                                //
	        cmp     al, 0x9F                            //
	        jbe     L502                                //
	        cmp     al, 0xE0                            //
	        jb      L501                                //
	        cmp     al, 0xFC                            //
	        jbe     L502                                //
	L501:   inc     edi                                 //             dest++;
	        jmp     L500                                //             continue;
	                                                    //         }
	        align16                                     //
	L502:   mov     cl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        cmp     al, 0x83                            //         if (c1 == 0x83) {
	        jne     L503                                //
	        cmp     cl, 0x94                            //             if (c2 >= 0x94/*('Éî' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
	        jb      L503                                //
	        cmp     cl, 0x9B                            //
	        ja      L503                                //
	        mov     byte ptr [edi], 0x82                //                 dest[0] = 0x82;
	        add     cl, 0xF2 - 0x94                     //                 c2 += 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
	                                                    //             }
	                                                    //         }
	L503:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	        add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L500                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // KATAKANA:
	                                                    //     for (; ; ) {
	L600:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L601                                //
	        cmp     al, 0x9F                            //
	        jbe     L602                                //
	        cmp     al, 0xE0                            //
	        jb      L601                                //
	        cmp     al, 0xFC                            //
	        jbe     L602                                //
	L601:   inc     edi                                 //             dest++;
	        jmp     L600                                //             continue;
	                                                    //         }
	        align16                                     //
	L602:   mov     cl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        cmp     al, 0x82                            //         if (c1 == 0x82) {
	        jne     L603                                //
	        cmp     cl, 0xF2                            //             if (c2 >= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
	        jb      L603                                //
	        cmp     cl, 0xF9                            //
	        ja      L603                                //
	        mov     byte ptr [edi], 0x83                //                 dest[0] = 0x83;
	        sub     cl, 0xF2 - 0x94                     //                 c2 -= 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/ - 0x94/*('Éî' - 0x8300)*/;
	                                                    //             }
	                                                    //         }
	L603:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	        add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L600                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // KATAKANA_HALFWIDTH:
	                                                    //     for (; ; ) {
	L700:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L701                                //
	        cmp     al, 0x9F                            //
	        jbe     L702                                //
	        cmp     al, 0xE0                            //
	        jb      L701                                //
	        cmp     al, 0xFC                            //
	        jbe     L702                                //
	L701:   inc     edi                                 //             dest++;
	        jmp     L700                                //             continue;
	                                                    //         }
	        align16                                     //
	L702:   xor     ecx, ecx                            //         c2 = src[offset];
	        cmp     al, 0x82                            //         if (c1 == 0x82) {
	        mov     cl, byte ptr [esi + ebp]            //
	        jne     L704                                //
	        cmp     cl, 0xF2                            //             if (c2 >= 0xF2) {
	        jb      L710                                //
	        cmp     cl, 0xF2                            //                 if (c2 == 0xF2/*(JISX0213('Ç§ﬁ') - 0x8200)*/) {
	        jne     L703                                //
	        mov     word ptr [edi], 0xDEB3              //                     *(unsigned short *)dest = 0xDEB3/*BSWAP16('≥ﬁ')*/;
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L703:   cmp     cl, 0xF5                            //                 } else if (c2 >= 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('Ç±ﬂ') - 0x8200)*/) {
	        jb      L710                                //
	        cmp     cl, 0xF9                            //
	        ja      L710                                //
	        add     cx, 0xDFB6 - 0xF5                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0xF5/*(JISX0213('Ç©ﬂ') - 0x8200)*/;
	        mov     word ptr [edi], cx                  //
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L704:   cmp     al, 0x83                            //         } else if (c1 == 0x83) {
	        jne     L708                                //
	        cmp     cl, 0x97                            //             if (c2 >= 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/) {
	        jb      L710                                //
	        cmp     cl, 0x9B                            //                 if (c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
	        ja      L705                                //
	        add     cx, 0xDFB6 - 0x97                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
	        mov     word ptr [edi], cx                  //
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L705:   cmp     cl, 0x9C                            //                 } else if (c2 == 0x9C/*(JISX0213('ÉZﬂ') - 0x8300)*/) {
	        jne     L706                                //
	        mov     word ptr [edi], 0xDFBE              //                     *(unsigned short *)dest = 0xDFBE/*BSWAP16('æﬂ')*/;
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L706:   cmp     cl, 0x9D                            //                 } else if (c2 == 0x9D/*(JISX0213('Écﬂ') - 0x8300)*/) {
	        jne     L707                                //
	        mov     word ptr [edi], 0xDFC2              //                     *(unsigned short *)dest = 0xDFC2/*BSWAP16('¬ﬂ')*/;
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L707:   cmp     cl, 0x9E                            //                 } else if (c2 == 0x9E/*(JISX0213('Égﬂ') - 0x8300)*/) {
	        jne     L710                                //
	        mov     word ptr [edi], 0xDFC4              //                     *(unsigned short *)dest = 0xDFC4/*BSWAP16('ƒﬂ')*/;
	        jmp     L711                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L708:   cmp     al, 0x84                            //         } else if (c1 == 0x84) {
	        jne     L710                                //
	        cmp     cl, 0x92                            //             if (c2 == 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/) {
	        jne     L709                                //
	        mov     word ptr [edi], 0xDEDC              //                 *(unsigned short *)dest = 0xDEDC/*BSWAP16('‹ﬁ')*/;
	        jmp     L711                                //                 goto KATAKANA_HALFWIDTH_CONTINUE;

	L709:   cmp     cl, 0x95                            //             } else if (c2 == 0x95/*(JISX0213('Éíﬁ') - 0x8400)*/) {
	        jne     L710                                //
	        mov     word ptr [edi], 0xDEA6              //                 *(unsigned short *)dest = 0xDEA6/*BSWAP16('¶ﬁ')*/;
	        jmp     L711                                //                 goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //             }
	                                                    //         }
	L710:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	                                                    //     KATAKANA_HALFWIDTH_CONTINUE:
	L711:   add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L700                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align16                                     // HALFWIDTH:
	                                                    //     for (; ; ) {
	L800:   mov     al, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], al                  //
	        jz      L900                                //             break;
	        cmp     al, 0x81                            //         if (c1 < 0x81 || (c1 > 0x9F && (c1 < 0xE0 || c1 > 0xFC))) {
	        jb      L801                                //
	        cmp     al, 0x9F                            //
	        jbe     L802                                //
	        cmp     al, 0xE0                            //
	        jb      L801                                //
	        cmp     al, 0xFC                            //
	        jbe     L802                                //
	L801:   inc     edi                                 //             dest++;
	        jmp     L800                                //             continue;
	                                                    //         }
	        align16                                     //
	L802:   xor     ecx, ecx                            //         c2 = src[offset];
	        cmp     al, 0x83                            //         if (c1 == 0x83) {
	        mov     cl, byte ptr [esi + ebp]            //
	        jne     L806                                //
	        cmp     cl, 0x97                            //             if (c2 >= 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/) {
	        jb      L808                                //
	        cmp     cl, 0x9B                            //                 if (c2 <= 0x9B/*(JISX0213('ÉRﬂ') - 0x8300)*/) {
	        ja      L803                                //
	        add     cx, 0xDFB6 - 0x97                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('∂ﬂ')*/ - 0x97/*(JISX0213('ÉJﬂ') - 0x8300)*/;
	        mov     word ptr [edi], cx                  //
	        jmp     L809                                //                     goto HALFWIDTH_CONTINUE;

	L803:   cmp     cl, 0x9C                            //                 } else if (c2 == 0x9C/*(JISX0213('ÉZﬂ') - 0x8300)*/) {
	        jne     L804                                //
	        mov     word ptr [edi], 0xDFBE              //                     *(unsigned short *)dest = 0xDFBE/*BSWAP16('æﬂ')*/;
	        jmp     L809                                //                     goto HALFWIDTH_CONTINUE;

	L804:   cmp     cl, 0x9D                            //                 } else if (c2 == 0x9D/*(JISX0213('Écﬂ') - 0x8300)*/) {
	        jne     L805                                //
	        mov     word ptr [edi], 0xDFC2              //                     *(unsigned short *)dest = 0xDFC2/*BSWAP16('¬ﬂ')*/;
	        jmp     L809                                //                     goto HALFWIDTH_CONTINUE;

	L805:   cmp     cl, 0x9E                            //                 } else if (c2 == 0x9E/*(JISX0213('Égﬂ') - 0x8300)*/) {
	        jne     L808                                //
	        mov     word ptr [edi], 0xDFC4              //                     *(unsigned short *)dest = 0xDFC4/*BSWAP16('ƒﬂ')*/;
	        jmp     L809                                //                     goto HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L806:   cmp     al, 0x84                            //         } else if (c1 == 0x84) {
	        jne     L808                                //
	        cmp     cl, 0x92                            //             if (c2 == 0x92/*(JISX0213('Éèﬁ') - 0x8400)*/) {
	        jne     L807                                //
	        mov     word ptr [edi], 0xDEDC              //                 *(unsigned short *)dest = 0xDEDC/*BSWAP16('‹ﬁ')*/;
	        jmp     L809                                //                 goto HALFWIDTH_CONTINUE;

	L807:   cmp     cl, 0x95                            //             } else if (c2 == 0x95/*(JISX0213('Éíﬁ') - 0x8400)*/) {
	        jne     L808                                //
	        mov     word ptr [edi], 0xDEA6              //                 *(unsigned short *)dest = 0xDEA6/*BSWAP16('¶ﬁ')*/;
	        jmp     L809                                //                 goto HALFWIDTH_CONTINUE;
	                                                    //             }
	                                                    //         }
	L808:   mov     byte ptr [edi + 1], cl              //         dest[1] = c2;
	                                                    //     HALFWIDTH_CONTINUE:
	L809:   add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L800                                //             continue;
	                                                    //         break;
	                                                    //     }

	        align16                                     // LCMAP_STRING:
	L900:   mov     ecx, dword ptr [cchDest + 16]       //     cchDest = LCMapStringA(Locale, dwMapFlags, lpBufferStr, cchBuffer, lpDestStr, cchDest);
	        mov     edi, dword ptr [lpDestStr + 16]     //
	        mov     ebp, dword ptr [hHeap + 16]         //
	        mov     esi, dword ptr [lpBufferStr + 16]   //
	        mov     edx, dword ptr [dwMapFlags + 16]    //
	        mov     eax, dword ptr [Locale + 16]        //
	        push    ecx                                 //
	        push    edi                                 //
	        push    ebx                                 //
	        push    esi                                 //
	        push    edx                                 //
	        push    eax                                 //
	        call    LCMapStringA                        //
	        mov     ebx, eax                            //
	        pop     edi                                 //
	        call    GetLastError                        //     dwErrCode = GetLastError();
	        push    eax                                 //
	        push    esi                                 //     HeapFree(hHeap, 0, lpBufferStr);
	        push    0                                   //
	        push    ebp                                 //
	        call    HeapFree                            //
	        call    SetLastError                        //     SetLastError(dwErrCode);
	        mov     eax, ebx                            //     return cchDest;
	        pop     esi                                 //
	        pop     ebp                                 //
	        pop     ebx                                 //
	        ret     24                                  //

	        #undef Locale
	        #undef dwMapFlags
	        #undef lpSrcStr
	        #undef cchSrc
	        #undef lpDestStr
	        #undef cchDest
	        #undef lpBufferStr
	        #undef hHeap
	        #undef cchBuffer
	        #undef offset
	        #undef src
	        #undef dest
	        #undef c1
	        #undef c2
	        #undef LCMAP_FLAGS
	}
}
#endif
