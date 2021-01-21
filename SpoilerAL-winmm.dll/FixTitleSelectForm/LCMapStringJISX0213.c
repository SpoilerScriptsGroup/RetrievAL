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
    (c) == '���' ? 0x82F2 : \
    (c) == '��'  ? 0x82F3 : \
    (c) == '��'  ? 0x82F4 : \
    (c) == '���' ? 0x82F5 : \
    (c) == '���' ? 0x82F6 : \
    (c) == '���' ? 0x82F7 : \
    (c) == '���' ? 0x82F8 : \
    (c) == '���' ? 0x82F9 : \
    (c) == '�J�' ? 0x8397 : \
    (c) == '�L�' ? 0x8398 : \
    (c) == '�N�' ? 0x8399 : \
    (c) == '�P�' ? 0x839A : \
    (c) == '�R�' ? 0x839B : \
    (c) == '�Z�' ? 0x839C : \
    (c) == '�c�' ? 0x839D : \
    (c) == '�g�' ? 0x839E : \
    (c) == '�N'  ? 0x83EC : \
    (c) == '�V'  ? 0x83ED : \
    (c) == '�X'  ? 0x83EE : \
    (c) == '�g'  ? 0x83EF : \
    (c) == '�k'  ? 0x83F0 : \
    (c) == '�n'  ? 0x83F1 : \
    (c) == '�q'  ? 0x83F2 : \
    (c) == '�t'  ? 0x83F3 : \
    (c) == '�w'  ? 0x83F4 : \
    (c) == '�z'  ? 0x83F5 : \
    (c) == '�v'  ? 0x83F6 : \
    (c) == '��'  ? 0x83F7 : \
    (c) == '��'  ? 0x83F8 : \
    (c) == '��'  ? 0x83F9 : \
    (c) == '��'  ? 0x83FA : \
    (c) == '��'  ? 0x83FB : \
    (c) == '��'  ? 0x83FC : \
    (c) == '���' ? 0x8492 : \
    (c) == '���' ? 0x8493 : \
    (c) == '���' ? 0x8494 : \
    (c) == '���' ? 0x8495 : (c))

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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			if (c1 >= 0xB3/*'�'*/) {
				c2 = src[offset];
				if (c1 == 0xB3/*'�'*/) {
					if (c2 == 0xDE/*'�'*/)
						goto FULLWIDTH_HIRAGANA_82F2_4;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/)
						goto FULLWIDTH_HIRAGANA_82F2_2;
				} else if (c1 >= 0xB6/*'�'*/ && c1 <= 0xBA/*'�'*/) {
					if (c2 == 0xDF/*'�'*/)
						goto FULLWIDTH_HIRAGANA_82F5_1;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_HIRAGANA_82F5_1:
						offset++;
						dest[0] = 0x82;
						c2 = c1 + 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xB6/*'�'*/;
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
			if (offset && c2 >= 0xA4/*('��' - 0x8200)*/) {
				c1 = src[offset];
				if (c2 == 0xA4/*('��' - 0x8200)*/)
					goto FULLWIDTH_HIRAGANA_82F2_1;
				if (c2 >= 0xA9/*('��' - 0x8200)*/ && c2 <= 0xB1/*('��' - 0x8200)*/) {
					if (c1 == 0xDF/*'�'*/)
						goto FULLWIDTH_HIRAGANA_82F5_2;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_HIRAGANA_82F5_2:
						cchBuffer--;
						offset++;
						c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xA9/*('��' - 0x8200)*/;
					}
				}
			}
		} else if (c1 == 0x83) {
			if (!offset)
				goto FULLWIDTH_HIRAGANA_82F2_82F9;
			if (c2 >= 0x45/*('�E' - 0x8300)*/) {
				c1 = src[offset];
				if (c2 == 0x45/*('�E' - 0x8300)*/) {
				FULLWIDTH_HIRAGANA_82F2_1:
					if (c1 == 0xDE/*'�'*/)
						goto FULLWIDTH_HIRAGANA_82F2_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_HIRAGANA_82F2_2:
						offset++;
					FULLWIDTH_HIRAGANA_82F2_3:
						cchBuffer--;
					FULLWIDTH_HIRAGANA_82F2_4:
						offset++;
						*(unsigned short *)dest = 0xF282/*BSWAP16(JISX0213('���'))*/;
						goto FULLWIDTH_HIRAGANA_CONTINUE_2;
					}
				} else if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
					if (c2 <= 0x52/*('�R' - 0x8300)*/) {
						if (c1 == 0xDF/*'�'*/)
							goto FULLWIDTH_HIRAGANA_82F5_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_HIRAGANA_82F5_3:
							cchBuffer--;
							offset++;
							dest[0] = 0x82;
							c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0x4A/*('�J' - 0x8300)*/;
						}
					} else {
					FULLWIDTH_HIRAGANA_82F2_82F9:
						if (c2 >= 0x94/*('��' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
							dest[0] = 0x82;
							c2 += 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			if (c1 >= 0xA6/*'�'*/) {
				c2 = src[offset];
				if (c1 == 0xA6/*'�'*/) {
					if (c2 == 0xDE/*'�'*/)
						goto FULLWIDTH_KATAKANA_8495;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8495:
						offset++;
						*(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('���'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c1 >= 0xB6/*'�'*/) {
					if (c1 <= 0xBA/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_KATAKANA_8397_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8397_1:
							offset++;
							dest[0] = 0x83;
							c2 = c1 - 0xB6/*'�'*/ + 0x97/*(JISX0213('�J�') - 0x8300)*/;
							goto FULLWIDTH_KATAKANA_CONTINUE_1;
						}
					} else if (c1 == 0xBE/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_KATAKANA_839C_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839C_2;
					} else if (c1 == 0xC2/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_KATAKANA_839D_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839D_2;
					} else if (c1 == 0xC4/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_KATAKANA_839E_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_KATAKANA_839E_2;
					} else if (c1 == 0xDC/*'�'*/) {
						if (c2 == 0xDE/*'�'*/)
							goto FULLWIDTH_KATAKANA_8492_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8492_1:
							offset++;
							*(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('���'))*/;
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
			if (c2 >= 0xA9/*('��' - 0x8200)*/) {
				if (c2 <= 0xB1/*('��' - 0x8200)*/) {
					if ((c1 = src[offset]) == 0xDF/*'�'*/)
						goto FULLWIDTH_KATAKANA_8397_2;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8397_2:
						cchBuffer--;
						offset++;
						dest[0] = 0x83;
						c2 -= 0xA9/*('��' - 0x8200)*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
					}
				} else if (c2 == 0xB9/*('��' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839C_1;
				else if (c2 == 0xC2/*('��' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839D_1;
				else if (c2 == 0xC6/*('��' - 0x8200)*/)
					goto FULLWIDTH_KATAKANA_839E_1;
				else if (c2 >= 0xED/*('��' - 0x8200)*/) {
					if (c2 <= 0xF0/*('��' - 0x8200)*/) {
						if ((c1 = src[offset]) == 0xDE/*'�'*/)
							goto FULLWIDTH_KATAKANA_8492_2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_KATAKANA_8492_2:
							cchBuffer--;
							offset++;
							dest[0] = 0x84;
							c2 -= 0xED/*('��' - 0x8200)*/ - 0x92/*(JISX0213('���') - 0x8400)*/;
						}
					} else {
					FULLWIDTH_KATAKANA_8394_829B:
						if (c2 >= 0xF2/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
							dest[0] = 0x83;
							c2 -= 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
						}
					}
				}
			}
		} else if (offset && c1 == 0x83) {
			if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
				if (c2 <= 0x52/*('�R' - 0x8300)*/) {
					if ((c1 = src[offset]) == 0xDF/*'�'*/)
						goto FULLWIDTH_KATAKANA_8397_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8397_3:
						cchBuffer--;
						offset++;
						c2 += 0x97/*(JISX0213('�J�') - 0x8300)*/ - 0x4A/*('�J' - 0x8300)*/;
					}
				} else if (c2 == 0x5A/*('�Z' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839C_1:
					if ((c1 = src[offset]) == 0xDF/*'�'*/)
						goto FULLWIDTH_KATAKANA_839C_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839C_2:
						offset++;
					FULLWIDTH_KATAKANA_839C_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839C_4:
						offset++;
						*(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('�Z�'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 == 0x63/*('�c' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839D_1:
					if ((c1 = src[offset]) == 0xDF/*'�'*/)
						goto FULLWIDTH_KATAKANA_839D_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839D_2:
						offset++;
					FULLWIDTH_KATAKANA_839D_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839D_4:
						offset++;
						*(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('�c�'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 == 0x67/*('�g' - 0x8300)*/) {
				FULLWIDTH_KATAKANA_839E_1:
					if ((c1 = src[offset]) == 0xDF/*'�'*/)
						goto FULLWIDTH_KATAKANA_839E_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
						cchBuffer--;
					FULLWIDTH_KATAKANA_839E_2:
						offset++;
					FULLWIDTH_KATAKANA_839E_3:
						cchBuffer--;
					FULLWIDTH_KATAKANA_839E_4:
						offset++;
						*(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('�g�'))*/;
						goto FULLWIDTH_KATAKANA_CONTINUE_2;
					}
				} else if (c2 >= 0x8F/*('��' - 0x8300)*/ && c2 <= 0x92/*('��' - 0x8300)*/) {
					if ((c1 = src[offset]) == 0xDE/*'�'*/)
						goto FULLWIDTH_KATAKANA_8492_3;
					if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_KATAKANA_8492_3:
						cchBuffer--;
						offset++;
						dest[0] = 0x84;
						c2 += 0x92/*(JISX0213('���') - 0x8400)*/ - 0x8F/*('��' - 0x8300)*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			if (c1 >= 0xA6/*'�'*/) {
				c2 = src[offset];
				if (c1 == 0xA6/*'�'*/) {
					if (c2 == 0xDE/*'�'*/)
						goto FULLWIDTH_8495;
					if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
						cchBuffer--;
						offset++;
					FULLWIDTH_8495:
						offset++;
						*(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('���'))*/;
						goto FULLWIDTH_CONTINUE_2;
					}
				} else if (c1 >= 0xB6/*'�'*/) {
					if (c1 <= 0xBA/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_8397_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8397_1:
							offset++;
							dest[0] = 0x83;
							c2 = c1 - 0xB6/*'�'*/ + 0x97/*(JISX0213('�J�') - 0x8300)*/;
							goto FULLWIDTH_CONTINUE_1;
						}
					} else if (c1 == 0xBE/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_839C_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_839C_2;
					} else if (c1 == 0xC2/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_839D_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_839D_2;
					} else if (c1 == 0xC4/*'�'*/) {
						if (c2 == 0xDF/*'�'*/)
							goto FULLWIDTH_839E_4;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
							goto FULLWIDTH_839E_2;
					} else if (c1 == 0xDC/*'�'*/) {
						if (c2 == 0xDE/*'�'*/)
							goto FULLWIDTH_8492_1;
						if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8492_1:
							offset++;
							*(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('���'))*/;
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
				if (c2 >= 0xA4/*('��' - 0x8200)*/) {
					c1 = src[offset];
					if (c2 == 0xA4/*('��' - 0x8200)*/) {
						if (c1 == 0xDE/*'�'*/)
							goto FULLWIDTH_82F2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_82F2:
							cchBuffer--;
							offset++;
							c2 = 0xF2/*(JISX0213('���') - 0x8200)*/;
							goto FULLWIDTH_CONTINUE_1;
						}
					} else if (c2 >= 0xA9/*('��' - 0x8200)*/) {
						if (c2 <= 0xB1/*('��' - 0x8200)*/) {
							if (c1 == 0xDF/*'�'*/)
								goto FULLWIDTH_82F5;
							if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
								cchBuffer--;
								offset++;
							FULLWIDTH_82F5:
								cchBuffer--;
								offset++;
								c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xA9/*('��' - 0x8200)*/;
							}
						} else if (c2 == 0xB9/*('��' - 0x8200)*/)
							goto FULLWIDTH_839C_1;
						else if (c2 == 0xC2/*('��' - 0x8200)*/)
							goto FULLWIDTH_839D_1;
						else if (c2 == 0xC6/*('��' - 0x8200)*/)
							goto FULLWIDTH_839E_1;
						else if (c2 >= 0xED/*('��' - 0x8200)*/ && c2 <= 0xF0/*('��' - 0x8200)*/) {
							if (c1 == 0xDE/*'�'*/)
								goto FULLWIDTH_8492_2;
							if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
								cchBuffer--;
								offset++;
							FULLWIDTH_8492_2:
								cchBuffer--;
								offset++;
								dest[0] = 0x84;
								c2 -= 0xED/*('��' - 0x8200)*/ - 0x92/*(JISX0213('���') - 0x8400)*/;
							}
						}
					}
				}
			} else if (c1 == 0x83) {
				if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
					if (c2 <= 0x52/*('�R' - 0x8300)*/) {
						if ((c1 = src[offset]) == 0xDF/*'�'*/)
							goto FULLWIDTH_8397_2;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8397_2:
							cchBuffer--;
							offset++;
							c2 += 0x97/*(JISX0213('�J�') - 0x8300)*/ - 0x4A/*('�J' - 0x8300)*/;
						}
					} else if (c2 == 0x5A/*('�Z' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839C_1:
						if (c1 == 0xDF/*'�'*/)
							goto FULLWIDTH_839C_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839C_2:
							offset++;
						FULLWIDTH_839C_3:
							cchBuffer--;
						FULLWIDTH_839C_4:
							offset++;
							*(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('�Z�'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 == 0x63/*('�c' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839D_1:
						if (c1 == 0xDF/*'�'*/)
							goto FULLWIDTH_839D_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839D_2:
							offset++;
						FULLWIDTH_839D_3:
							cchBuffer--;
						FULLWIDTH_839D_4:
							offset++;
							*(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('�c�'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 == 0x67/*('�g' - 0x8300)*/) {
						c1 = src[offset];
					FULLWIDTH_839E_1:
						if (c1 == 0xDF/*'�'*/)
							goto FULLWIDTH_839E_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
							cchBuffer--;
						FULLWIDTH_839E_2:
							offset++;
						FULLWIDTH_839E_3:
							cchBuffer--;
						FULLWIDTH_839E_4:
							offset++;
							*(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('�g�'))*/;
							goto FULLWIDTH_CONTINUE_2;
						}
					} else if (c2 >= 0x8F/*('��' - 0x8300)*/ && c2 <= 0x92/*('��' - 0x8300)*/) {
						if ((c1 = src[offset]) == 0xDE/*'�'*/)
							goto FULLWIDTH_8492_3;
						if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
							cchBuffer--;
							offset++;
						FULLWIDTH_8492_3:
							cchBuffer--;
							offset++;
							dest[0] = 0x84;
							c2 += 0x92/*(JISX0213('���') - 0x8400)*/ - 0x8F/*('��' - 0x8300)*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x83) {
			if (c2 >= 0x94/*('��' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
				dest[0] = 0x82;
				c2 += 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x82) {
			if (c2 >= 0xF2/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
				dest[0] = 0x83;
				c2 -= 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x82) {
			if (c2 >= 0xF2) {
				if (c2 == 0xF2/*(JISX0213('���') - 0x8200)*/) {
					*(unsigned short *)dest = 0xDEB3/*BSWAP16('��')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 >= 0xF5/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0xF5/*(JISX0213('���') - 0x8200)*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x83) {
			if (c2 >= 0x97/*(JISX0213('�J�') - 0x8300)*/) {
				if (c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9C/*(JISX0213('�Z�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFBE/*BSWAP16('��')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9D/*(JISX0213('�c�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC2/*BSWAP16('��')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9E/*(JISX0213('�g�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC4/*BSWAP16('��')*/;
					goto KATAKANA_HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x84) {
			if (c2 == 0x92/*(JISX0213('���') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEDC/*BSWAP16('��')*/;
				goto KATAKANA_HALFWIDTH_CONTINUE;
			} else if (c2 == 0x95/*(JISX0213('���') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEA6/*BSWAP16('��')*/;
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
		if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
			dest++;
			continue;
		}
		c2 = src[offset];
		if (c1 == 0x83) {
			if (c2 >= 0x97/*(JISX0213('�J�') - 0x8300)*/) {
				if (c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
					*(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9C/*(JISX0213('�Z�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFBE/*BSWAP16('��')*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9D/*(JISX0213('�c�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC2/*BSWAP16('��')*/;
					goto HALFWIDTH_CONTINUE;
				} else if (c2 == 0x9E/*(JISX0213('�g�') - 0x8300)*/) {
					*(unsigned short *)dest = 0xDFC4/*BSWAP16('��')*/;
					goto HALFWIDTH_CONTINUE;
				}
			}
		} else if (c1 == 0x84) {
			if (c2 == 0x92/*(JISX0213('���') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEDC/*BSWAP16('��')*/;
				goto HALFWIDTH_CONTINUE;
			} else if (c2 == 0x95/*(JISX0213('���') - 0x8400)*/) {
				*(unsigned short *)dest = 0xDEA6/*BSWAP16('��')*/;
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
#ifdef _MSC_VER
	#define align(n) align n
#else
	#define align(n)
#endif

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
	        #define c1          cl
	        #define c2          dl

	        #define LCMAP_FLAGS (LCMAP_FULLWIDTH or LCMAP_HALFWIDTH or LCMAP_KATAKANA or LCMAP_HIRAGANA)

	        mov     eax, dword ptr [cchSrc]             //
	        mov     ecx, dword ptr [Locale]             //
	        test    eax, eax                            //     if (!(cchBuffer = cchSrc))
	        jz      L103                                //         return 0;
	        and     ecx, 0x03FF                         //
	        mov     edx, dword ptr [dwMapFlags]         //
	        cmp     ecx, LANG_JAPANESE                  //     if (PRIMARYLANGID(LANGIDFROMLCID(Locale)) != LANG_JAPANESE ||
	        jne     L100                                //         !(dwMapFlags & (LCMAP_FULLWIDTH | LCMAP_HALFWIDTH | LCMAP_KATAKANA | LCMAP_HIRAGANA)))
	        and     edx, LCMAP_FLAGS                    //
	        jnz     L101                                //
	L100:   jmp     LCMapStringA                        //         return LCMapStringA(Locale, dwMapFlags, lpSrcStr, cchSrc, lpDestStr, cchDest);

	        align(16)                                   //
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
	        push    ERROR_OUTOFMEMORY                   //         SetLastError(ERROR_OUTOFMEMORY);
	        call    SetLastError                        //
	        xor     eax, eax                            //         return 0;
	        pop     ebx                                 //
	L103:   ret     24                                  //     }

	        align(16)                                   //
	L104:   push    ebp                                 //
	        push    esi                                 //
	        push    edi                                 //
	        mov     edi, eax                            //     dest = lpBufferStr;
	        mov     eax, dword ptr [dwMapFlags + 16]    //
	        mov     esi, dword ptr [lpSrcStr + 16]      //     src = lpSrcStr + cchBuffer;
	        mov     dword ptr [lpBufferStr + 16], edi   //
	        lea     ebp, [ebx - 1]                      //     offset = -(ptrdiff_t)cchBuffer;
	        add     esi, ebx                            //
	        xor     ebp, -1                             //
	        xor     ecx, ecx                            //
	        xor     edx, edx                            //
	        test    eax, LCMAP_FULLWIDTH                //     if (dwMapFlags & LCMAP_FULLWIDTH)
	        jz      L105                                //
	        test    eax, LCMAP_HIRAGANA                 //         if (dwMapFlags & LCMAP_HIRAGANA)
	        jnz     L200                                //             goto FULLWIDTH_HIRAGANA;
	        test    eax, LCMAP_KATAKANA                 //         else if (dwMapFlags & LCMAP_KATAKANA)
	        jnz     L300                                //             goto FULLWIDTH_KATAKANA;
	                                                    //         else
	        jmp     L400                                //             goto FULLWIDTH;

	L105:   test    eax, LCMAP_HIRAGANA                 //     else if (dwMapFlags & LCMAP_HIRAGANA)
	        jnz     L500                                //         goto HIRAGANA;
	        test    eax, LCMAP_HALFWIDTH                //     else if (!(dwMapFlags & LCMAP_HALFWIDTH))
	        jz      L600                                //         goto KATAKANA;
	        test    eax, LCMAP_KATAKANA                 //     else if (dwMapFlags & LCMAP_KATAKANA)
	        jnz     L700                                //         goto KATAKANA_HALFWIDTH;
	                                                    //     else
	        jmp     L800                                //         goto HALFWIDTH;

	        align(16)                                   // FULLWIDTH_HIRAGANA:
	                                                    //     for (; ; ) {
	L200:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L204                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L204                                //
	        cmp     cl, 0xB3                            //             if (c1 >= 0xB3/*'�'*/) {
	        jb      L203                                //
	        mov     dl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L201                                //                 if (c1 == 0xB3/*'�'*/) {
	        cmp     dl, 0xDE                            //                     if (c2 == 0xDE/*'�'*/)
	        je      L210                                //                         goto FULLWIDTH_HIRAGANA_82F2_4;
	        cmp     dl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/)
	        jne     L203                                //
	        cmp     ebp, -1                             //
	        je      L203                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L203                                //
	        jmp     L208                                //                         goto FULLWIDTH_HIRAGANA_82F2_2;

	L201:   cmp     cl, 0xB6                            //                 } else if (c1 >= 0xB6/*'�'*/ && c1 <= 0xBA/*'�'*/) {
	        jb      L203                                //
	        cmp     cl, 0xBA                            //
	        ja      L203                                //
	        cmp     dl, 0xDF                            //                     if (c2 == 0xDF/*'�'*/)
	        je      L202                                //                         goto FULLWIDTH_HIRAGANA_82F5_1;
	        cmp     dl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L203                                //
	        cmp     ebp, -1                             //
	        je      L203                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L203                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F5_1:
	L202:   inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x82                //                         dest[0] = 0x82;
	        lea     edx, [ecx + 0xF5 - 0xB6]            //                         c2 = c1 + 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xB6/*'�'*/;
	        jmp     L214                                //                         goto FULLWIDTH_HIRAGANA_CONTINUE_1;
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L203:   inc     edi                                 //             dest++;
	        jmp     L200                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L204:   mov     dl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         offset++;
	        cmp     cl, 0x82                            //         if (c1 == 0x82) {
	        jne     L206                                //
	        test    ebp, ebp                            //             if (offset && c2 >= 0xA4/*('��' - 0x8200)*/) {
	        jz      L214                                //
	        cmp     dl, 0xA4                            //
	        jb      L214                                //
	        mov     cl, byte ptr [esi + ebp]            //                 c1 = src[offset];
	                                                    //                 if (c2 == 0xA4/*('��' - 0x8200)*/)
	        je      L207                                //                     goto FULLWIDTH_HIRAGANA_82F2_1;
	        cmp     dl, 0xA9                            //                 if (c2 >= 0xA9/*('��' - 0x8200)*/ && c2 <= 0xB1/*('��' - 0x8200)*/) {
	        jb      L214                                //
	        cmp     dl, 0xB1                            //
	        ja      L214                                //
	        cmp     cl, 0xDF                            //                     if (c1 == 0xDF/*'�'*/)
	        je      L205                                //                         goto FULLWIDTH_HIRAGANA_82F5_2;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L214                                //
	        cmp     ebp, -1                             //
	        je      L214                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L214                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F5_2:
	L205:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        add     dl, 0xF5 - 0xA9                     //                         c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xA9/*('��' - 0x8200)*/;
	        jmp     L214                                //                     }
	                                                    //                 }
	                                                    //             }
	L206:   cmp     cl, 0x83                            //         } else if (c1 == 0x83) {
	        jne     L214                                //
	        test    ebp, ebp                            //             if (!offset)
	        jz      L213                                //                 goto FULLWIDTH_HIRAGANA_82F2_82F9;
	        cmp     dl, 0x45                            //             if (c2 >= 0x45/*('�E' - 0x8300)*/) {
	        jb      L214                                //
	        mov     cl, byte ptr [esi + ebp]            //                 c1 = src[offset];
	        jne     L211                                //                 if (c2 == 0x45/*('�E' - 0x8300)*/) {
	                                                    //                 FULLWIDTH_HIRAGANA_82F2_1:
	L207:   cmp     cl, 0xDE                            //                     if (c1 == 0xDE/*'�'*/)
	        je      L209                                //                         goto FULLWIDTH_HIRAGANA_82F2_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L214                                //
	        cmp     ebp, -1                             //
	        je      L214                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L214                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_2:
	L208:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_3:
	L209:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_4:
	L210:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0xF282              //                         *(unsigned short *)dest = 0xF282/*BSWAP16(JISX0213('���'))*/;
	        jmp     L215                                //                         goto FULLWIDTH_HIRAGANA_CONTINUE_2;
	                                                    //                     }
	L211:   cmp     dl, 0x4A                            //                 } else if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
	        jb      L214                                //
	        cmp     dl, 0x52                            //                     if (c2 <= 0x52/*('�R' - 0x8300)*/) {
	        ja      L213                                //
	        cmp     cl, 0xDF                            //                         if (c1 == 0xDF/*'�'*/)
	        je      L212                                //                             goto FULLWIDTH_HIRAGANA_82F5_3;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L214                                //
	        cmp     ebp, -1                             //
	        je      L214                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L214                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_HIRAGANA_82F5_3:
	L212:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x82                //                             dest[0] = 0x82;
	        add     dl, 0xF5 - 0x4A                     //                             c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0x4A/*('�J' - 0x8300)*/;
	        jmp     L214                                //                         }
	                                                    //                     } else {
	                                                    //                     FULLWIDTH_HIRAGANA_82F2_82F9:
	L213:   cmp     dl, 0x94                            //                         if (c2 >= 0x94/*('��' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
	        jb      L214                                //
	        cmp     dl, 0x9B                            //
	        ja      L214                                //
	        mov     byte ptr [edi], 0x82                //                             dest[0] = 0x82;
	        add     dl, 0xF2 - 0x94                     //                             c2 += 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_HIRAGANA_CONTINUE_1:
	L214:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_HIRAGANA_CONTINUE_2:
	L215:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L200                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // FULLWIDTH_KATAKANA:
	                                                    //     for (; ; ) {
	L300:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L310                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L310                                //
	        cmp     cl, 0xA6                            //             if (c1 >= 0xA6/*'�'*/) {
	        jb      L309                                //
	        mov     dl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L302                                //                 if (c1 == 0xA6/*'�'*/) {
	        cmp     dl, 0xDE                            //                     if (c2 == 0xDE/*'�'*/)
	        je      L301                                //                         goto FULLWIDTH_KATAKANA_8495;
	        cmp     dl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L309                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8495:
	L301:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9584              //                         *(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('���'))*/;
	        jmp     L335                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L302:   cmp     cl, 0xB6                            //                 } else if (c1 >= 0xB6/*'�'*/) {
	        jb      L309                                //
	        cmp     cl, 0xBA                            //                     if (c1 <= 0xBA/*'�'*/) {
	        ja      L304                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L303                                //                             goto FULLWIDTH_KATAKANA_8397_1;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L309                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8397_1:
	L303:   inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        lea     edx, [ecx - 0xB6 + 0x97]            //                             c2 = c1 - 0xB6/*'�'*/ + 0x97/*(JISX0213('�J�') - 0x8300)*/;
	        jmp     L334                                //                             goto FULLWIDTH_KATAKANA_CONTINUE_1;
	                                                    //                         }
	L304:   cmp     cl, 0xBE                            //                     } else if (c1 == 0xBE/*'�'*/) {
	        jne     L305                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L321                                //                             goto FULLWIDTH_KATAKANA_839C_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L309                                //
	        jmp     L319                                //                             goto FULLWIDTH_KATAKANA_839C_2;

	L305:   cmp     cl, 0xC2                            //                     } else if (c1 == 0xC2/*'�'*/) {
	        jne     L306                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L326                                //                             goto FULLWIDTH_KATAKANA_839D_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L309                                //
	        jmp     L324                                //                             goto FULLWIDTH_KATAKANA_839D_2;

	L306:   cmp     cl, 0xC4                            //                     } else if (c1 == 0xC4/*'�'*/) {
	        jne     L307                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L331                                //                             goto FULLWIDTH_KATAKANA_839E_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L309                                //
	        jmp     L329                                //                             goto FULLWIDTH_KATAKANA_839E_2;

	L307:   cmp     cl, 0xDC                            //                     } else if (c1 == 0xDC/*'�'*/) {
	        jne     L309                                //
	        cmp     dl, 0xDE                            //                         if (c2 == 0xDE/*'�'*/)
	        je      L308                                //                             goto FULLWIDTH_KATAKANA_8492_1;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L309                                //
	        cmp     ebp, -1                             //
	        je      L309                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L309                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8492_1:
	L308:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9284              //                             *(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('���'))*/;
	        jmp     L335                                //                             goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L309:   inc     edi                                 //             dest++;
	        jmp     L300                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L310:   mov     dl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         offset++;
	        cmp     cl, 0x82                            //         if (c1 == 0x82) {
	        jne     L315                                //
	        test    ebp, ebp                            //             if (!offset)
	        jz      L314                                //                 goto FULLWIDTH_KATAKANA_8394_829B;
	        cmp     dl, 0xA9                            //             if (c2 >= 0xA9/*('��' - 0x8200)*/) {
	        jb      L334                                //
	        cmp     dl, 0xB1                            //                 if (c2 <= 0xB1/*('��' - 0x8200)*/) {
	        ja      L312                                //
	        mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L311                                //                         goto FULLWIDTH_KATAKANA_8397_2;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8397_2:
	L311:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x83                //                         dest[0] = 0x83;
	        sub     dl, 0xA9 - 0x97                     //                         c2 -= 0xA9/*('��' - 0x8200)*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
	        jmp     L334                                //                     }

	L312:   cmp     dl, 0xB9                            //                 } else if (c2 == 0xB9/*('��' - 0x8200)*/)
	        je      L318                                //                     goto FULLWIDTH_KATAKANA_839C_1;
	        cmp     dl, 0xC2                            //                 else if (c2 == 0xC2/*('��' - 0x8200)*/)
	        je      L323                                //                     goto FULLWIDTH_KATAKANA_839D_1;
	        cmp     dl, 0xC6                            //                 else if (c2 == 0xC6/*('��' - 0x8200)*/)
	        je      L328                                //                     goto FULLWIDTH_KATAKANA_839E_1;
	        cmp     dl, 0xED                            //                 else if (c2 >= 0xED/*('��' - 0x8200)*/) {
	        jb      L334                                //
	        cmp     dl, 0xF0                            //                     if (c2 <= 0xF0/*('��' - 0x8200)*/) {
	        ja      L314                                //
	        mov     cl, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDE/*'�'*/)
	        cmp     cl, 0xDE                            //
	        je      L313                                //                             goto FULLWIDTH_KATAKANA_8492_2;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L334                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_KATAKANA_8492_2:
	L313:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x84                //                             dest[0] = 0x84;
	        sub     dl, 0xED - 0x92                     //                             c2 -= 0xED/*('��' - 0x8200)*/ - 0x92/*(JISX0213('���') - 0x8400)*/;
	        jmp     L334                                //                         }
	                                                    //                     } else {
	                                                    //                     FULLWIDTH_KATAKANA_8394_829B:
	L314:   cmp     dl, 0xF2                            //                         if (c2 >= 0xF2/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
	        jb      L334                                //
	        cmp     dl, 0xF9                            //
	        ja      L334                                //
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        sub     dl, 0xF2 - 0x94                     //                             c2 -= 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
	        jmp     L334                                //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L315:   test    ebp, ebp                            //         } else if (offset && c1 == 0x83) {
	        jz      L334                                //
	        cmp     cl, 0x83                            //
	        jne     L334                                //
	        cmp     dl, 0x4A                            //             if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
	        jb      L334                                //
	        cmp     dl, 0x52                            //                 if (c2 <= 0x52/*('�R' - 0x8300)*/) {
	        ja      L317                                //
	        mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L316                                //                         goto FULLWIDTH_KATAKANA_8397_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8397_3:
	L316:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        add     dl, 0x97 - 0x4A                     //                         c2 += 0x97/*(JISX0213('�J�') - 0x8300)*/ - 0x4A/*('�J' - 0x8300)*/;
	        jmp     L334                                //                     }

	L317:   cmp     dl, 0x5A                            //                 } else if (c2 == 0x5A/*('�Z' - 0x8300)*/) {
	        jne     L322                                //
	                                                    //                 FULLWIDTH_KATAKANA_839C_1:
	L318:   mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L320                                //                         goto FULLWIDTH_KATAKANA_839C_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839C_2:
	L319:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839C_3:
	L320:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839C_4:
	L321:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9C83              //                         *(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('�Z�'))*/;
	        jmp     L335                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L322:   cmp     dl, 0x63                            //                 } else if (c2 == 0x63/*('�c' - 0x8300)*/) {
	        jne     L327                                //
	                                                    //                 FULLWIDTH_KATAKANA_839D_1:
	L323:   mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L325                                //                         goto FULLWIDTH_KATAKANA_839D_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839D_2:
	L324:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839D_3:
	L325:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839D_4:
	L326:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9D83              //                         *(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('�c�'))*/;
	        jmp     L335                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L327:   cmp     dl, 0x63                            //                 } else if (c2 == 0x67/*('�g' - 0x8300)*/) {
	        jne     L332                                //
	                                                    //                 FULLWIDTH_KATAKANA_839E_1:
	L328:   mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L330                                //                         goto FULLWIDTH_KATAKANA_839E_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839E_2:
	L329:   inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_839E_3:
	L330:   dec     ebx                                 //                         cchBuffer--;
	                                                    //                     FULLWIDTH_KATAKANA_839E_4:
	L331:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9E83              //                         *(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('�g�'))*/;
	        jmp     L335                                //                         goto FULLWIDTH_KATAKANA_CONTINUE_2;
	                                                    //                     }
	L332:   cmp     dl, 0x8F                            //                 } else if (c2 >= 0x8F/*('��' - 0x8300)*/ && c2 <= 0x92/*('��' - 0x8300)*/) {
	        jb      L334                                //
	        cmp     dl, 0x92                            //
	        ja      L334                                //
	        mov     cl, byte ptr [esi + ebp]            //                     if ((c1 = src[offset]) == 0xDE/*'�'*/)
	        cmp     cl, 0xDE                            //
	        je      L333                                //                         goto FULLWIDTH_KATAKANA_8492_3;
	        cmp     cl, 0x81                            //                     if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L334                                //
	        cmp     ebp, -1                             //
	        je      L334                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L334                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_KATAKANA_8492_3:
	L333:   dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	        mov     byte ptr [edi], 0x84                //                         dest[0] = 0x84;
	        add     dl, 0x92 - 0x8F                     //                         c2 += 0x92/*(JISX0213('���') - 0x8400)*/ - 0x8F/*('��' - 0x8300)*/;
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_KATAKANA_CONTINUE_1:
	L334:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_KATAKANA_CONTINUE_2:
	L335:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L300                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // FULLWIDTH:
	                                                    //     for (; ; ) {
	L400:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L410                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L410                                //
	        cmp     cl, 0xA6                            //             if (c1 >= 0xA6/*'�'*/) {
	        jb      L409                                //
	        mov     dl, byte ptr [esi + ebp]            //                 c2 = src[offset];
	        jne     L402                                //                 if (c1 == 0xA6/*'�'*/) {
	        cmp     dl, 0xDE                            //                     if (c2 == 0xDE/*'�'*/)
	        je      L401                                //                         goto FULLWIDTH_8495;
	        cmp     dl, 0x81                            //                     if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L409                                //
	        dec     ebx                                 //                         cchBuffer--;
	        inc     ebp                                 //                         offset++;
	                                                    //                     FULLWIDTH_8495:
	L401:   inc     ebp                                 //                         offset++;
	        mov     word ptr [edi], 0x9584              //                         *(unsigned short *)dest = 0x9584/*BSWAP16(JISX0213('���'))*/;
	        jmp     L436                                //                         goto FULLWIDTH_CONTINUE_2;
	                                                    //                     }
	L402:   cmp     cl, 0xB6                            //                 } else if (c1 >= 0xB6/*'�'*/) {
	        jb      L409                                //
	        cmp     cl, 0xBA                            //                     if (c1 <= 0xBA/*'�'*/) {
	        ja      L404                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L403                                //                             goto FULLWIDTH_8397_1;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L409                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8397_1:
	L403:   inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x83                //                             dest[0] = 0x83;
	        lea     edx, [ecx - 0xB6 + 0x97]            //                             c2 = c1 - 0xB6/*'�'*/ + 0x97/*(JISX0213('�J�') - 0x8300)*/;
	        jmp     L435                                //                             goto FULLWIDTH_CONTINUE_1;
	                                                    //                         }
	L404:   cmp     cl, 0xBE                            //                     } else if (c1 == 0xBE/*'�'*/) {
	        jne     L405                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L422                                //                             goto FULLWIDTH_839C_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L409                                //
	        jmp     L420                                //                             goto FULLWIDTH_839C_2;

	L405:   cmp     cl, 0xC2                            //                     } else if (c1 == 0xC2/*'�'*/) {
	        jne     L406                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L427                                //                             goto FULLWIDTH_839D_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L409                                //
	        jmp     L425                                //                             goto FULLWIDTH_839D_2;

	L406:   cmp     cl, 0xC4                            //                     } else if (c1 == 0xC4/*'�'*/) {
	        jne     L407                                //
	        cmp     dl, 0xDF                            //                         if (c2 == 0xDF/*'�'*/)
	        je      L432                                //                             goto FULLWIDTH_839E_4;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/)
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L409                                //
	        jmp     L430                                //                             goto FULLWIDTH_839E_2;

	L407:   cmp     cl, 0xDC                            //                     } else if (c1 == 0xDC/*'�'*/) {
	        jne     L409                                //
	        cmp     dl, 0xDE                            //                         if (c2 == 0xDE/*'�'*/)
	        je      L408                                //                             goto FULLWIDTH_8492_1;
	        cmp     dl, 0x81                            //                         if (c2 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L409                                //
	        cmp     ebp, -1                             //
	        je      L409                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L409                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8492_1:
	L408:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9284              //                             *(unsigned short *)dest = 0x9284/*BSWAP16(JISX0213('���'))*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	L409:   inc     edi                                 //             dest++;
	        jmp     L400                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L410:   mov     dl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        inc     ebp                                 //         if (++offset) {
	        jz      L435                                //
	        cmp     cl, 0x82                            //             if (c1 == 0x82) {
	        jne     L416                                //
	        cmp     dl, 0xA4                            //                 if (c2 >= 0xA4/*('��' - 0x8200)*/) {
	        jb      L435                                //
	        mov     cl, byte ptr [esi + ebp]            //                     c1 = src[offset];
	        jne     L412                                //                     if (c2 == 0xA4/*('��' - 0x8200)*/) {
	        cmp     cl, 0xDE                            //                         if (c1 == 0xDE/*'�'*/)
	        je      L411                                //                             goto FULLWIDTH_82F2;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_82F2:
	L411:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     dl, 0xF2                            //                             c2 = 0xF2/*(JISX0213('���') - 0x8200)*/;
	        jmp     L435                                //                             goto FULLWIDTH_CONTINUE_1;
	                                                    //                         }
	L412:   cmp     dl, 0xA9                            //                     } else if (c2 >= 0xA9/*('��' - 0x8200)*/) {
	        jb      L435                                //
	        cmp     dl, 0xB1                            //                         if (c2 <= 0xB1/*('��' - 0x8200)*/) {
	        ja      L414                                //
	        cmp     cl, 0xDF                            //                             if (c1 == 0xDF/*'�'*/)
	        je      L413                                //                                 goto FULLWIDTH_82F5;
	        cmp     cl, 0x81                            //                             if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L435                                //
	        dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	                                                    //                             FULLWIDTH_82F5:
	L413:   dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	        add     dl, 0xF5 - 0xA9                     //                                 c2 += 0xF5/*(JISX0213('���') - 0x8200)*/ - 0xA9/*('��' - 0x8200)*/;
	        jmp     L435                                //                             }

	L414:   cmp     dl, 0xB9                            //                         } else if (c2 == 0xB9/*('��' - 0x8200)*/)
	        je      L419                                //                             goto FULLWIDTH_839C_1;
	        cmp     dl, 0xC2                            //                         else if (c2 == 0xC2/*('��' - 0x8200)*/)
	        je      L424                                //                             goto FULLWIDTH_839D_1;
	        cmp     dl, 0xC6                            //                         else if (c2 == 0xC6/*('��' - 0x8200)*/)
	        je      L429                                //                             goto FULLWIDTH_839E_1;
	        cmp     dl, 0xED                            //                         else if (c2 >= 0xED/*('��' - 0x8200)*/ && c2 <= 0xF0/*('��' - 0x8200)*/) {
	        jb      L435                                //
	        cmp     dl, 0xF0                            //
	        ja      L435                                //
	        cmp     cl, 0xDE                            //                             if (c1 == 0xDE/*'�'*/)
	        je      L415                                //                                 goto FULLWIDTH_8492_2;
	        cmp     cl, 0x81                            //                             if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L435                                //
	        dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	                                                    //                             FULLWIDTH_8492_2:
	L415:   dec     ebx                                 //                                 cchBuffer--;
	        inc     ebp                                 //                                 offset++;
	        mov     byte ptr [edi], 0x84                //                                 dest[0] = 0x84;
	        sub     dl, 0xED - 0x92                     //                                 c2 -= 0xED/*('��' - 0x8200)*/ - 0x92/*(JISX0213('���') - 0x8400)*/;
	        jmp     L435                                //                             }
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	L416:   cmp     cl, 0x83                            //             } else if (c1 == 0x83) {
	        jne     L435                                //
	        cmp     dl, 0x4A                            //                 if (c2 >= 0x4A/*('�J' - 0x8300)*/) {
	        jb      L435                                //
	        cmp     dl, 0x52                            //                     if (c2 <= 0x52/*('�R' - 0x8300)*/) {
	        ja      L418                                //
	        mov     cl, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDF/*'�'*/)
	        cmp     cl, 0xDF                            //
	        je      L417                                //                             goto FULLWIDTH_8397_2;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8397_2:
	L417:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        add     dl, 0x97 - 0x4A                     //                             c2 += 0x97/*(JISX0213('�J�') - 0x8300)*/ - 0x4A/*('�J' - 0x8300)*/;
	        jmp     L435                                //                         }

	L418:   cmp     dl, 0x5A                            //                     } else if (c2 == 0x5A/*('�Z' - 0x8300)*/) {
	        jne     L423                                //
	        mov     cl, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839C_1:
	L419:   cmp     cl, 0xDF                            //                         if (c1 == 0xDF/*'�'*/)
	        je      L421                                //                             goto FULLWIDTH_839C_3;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839C_2:
	L420:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839C_3:
	L421:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839C_4:
	L422:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9C83              //                             *(unsigned short *)dest = 0x9C83/*BSWAP16(JISX0213('�Z�'))*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L423:   cmp     dl, 0x63                            //                     } else if (c2 == 0x63/*('�c' - 0x8300)*/) {
	        jne     L428                                //
	        mov     cl, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839D_1:
	L424:   cmp     cl, 0xDF                            //                         if (c1 == 0xDF/*'�'*/)
	        je      L426                                //                             goto FULLWIDTH_839D_3;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839D_2:
	L425:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839D_3:
	L426:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839D_4:
	L427:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9D83              //                             *(unsigned short *)dest = 0x9D83/*BSWAP16(JISX0213('�c�'))*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L428:   cmp     dl, 0x63                            //                     } else if (c2 == 0x67/*('�g' - 0x8300)*/) {
	        jne     L433                                //
	        mov     cl, byte ptr [esi + ebp]            //                         c1 = src[offset];
	                                                    //                     FULLWIDTH_839E_1:
	L429:   cmp     cl, 0xDF                            //                         if (c1 == 0xDF/*'�'*/)
	        je      L431                                //                             goto FULLWIDTH_839E_3;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4B/*('�K' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4B      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839E_2:
	L430:   inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_839E_3:
	L431:   dec     ebx                                 //                             cchBuffer--;
	                                                    //                         FULLWIDTH_839E_4:
	L432:   inc     ebp                                 //                             offset++;
	        mov     word ptr [edi], 0x9E83              //                             *(unsigned short *)dest = 0x9E83/*BSWAP16(JISX0213('�g�'))*/;
	        jmp     L436                                //                             goto FULLWIDTH_CONTINUE_2;
	                                                    //                         }
	L433:   cmp     dl, 0x8F                            //                     } else if (c2 >= 0x8F/*('��' - 0x8300)*/ && c2 <= 0x92/*('��' - 0x8300)*/) {
	        jb      L435                                //
	        cmp     dl, 0x92                            //
	        ja      L435                                //
	        mov     cl, byte ptr [esi + ebp]            //                         if ((c1 = src[offset]) == 0xDE/*'�'*/)
	        cmp     cl, 0xDE                            //
	        je      L434                                //                             goto FULLWIDTH_8492_3;
	        cmp     cl, 0x81                            //                         if (c1 == 0x81 && offset != -1 && src[offset + 1] == 0x4A/*('�J' - 0x8100)*/) {
	        jne     L435                                //
	        cmp     ebp, -1                             //
	        je      L435                                //
	        cmp     byte ptr [esi + ebp + 1], 0x4A      //
	        jne     L435                                //
	        dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	                                                    //                         FULLWIDTH_8492_3:
	L434:   dec     ebx                                 //                             cchBuffer--;
	        inc     ebp                                 //                             offset++;
	        mov     byte ptr [edi], 0x84                //                             dest[0] = 0x84;
	        add     dl, 0x92 - 0x8F                     //                             c2 += 0x92/*(JISX0213('���') - 0x8400)*/ - 0x8F/*('��' - 0x8300)*/;
	                                                    //                         }
	                                                    //                     }
	                                                    //                 }
	                                                    //             }
	                                                    //         }
	                                                    //     FULLWIDTH_CONTINUE_1:
	L435:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	                                                    //     FULLWIDTH_CONTINUE_2:
	L436:   add     edi, 2                              //         dest += 2;
	        test    ebp, ebp                            //         if (offset)
	        jnz     L400                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // HIRAGANA:
	                                                    //     for (; ; ) {
	L500:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L501                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L501                                //
	        inc     edi                                 //             dest++;
	        jmp     L500                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L501:   mov     dl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        cmp     cl, 0x83                            //         if (c1 == 0x83) {
	        jne     L502                                //
	        cmp     dl, 0x94                            //             if (c2 >= 0x94/*('��' - 0x8300)*/ && c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
	        jb      L502                                //
	        cmp     dl, 0x9B                            //
	        ja      L502                                //
	        mov     byte ptr [edi], 0x82                //                 dest[0] = 0x82;
	        add     dl, 0xF2 - 0x94                     //                 c2 += 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
	                                                    //             }
	                                                    //         }
	L502:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	        add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L500                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // KATAKANA:
	                                                    //     for (; ; ) {
	L600:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L601                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L601                                //
	        inc     edi                                 //             dest++;
	        jmp     L600                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L601:   mov     dl, byte ptr [esi + ebp]            //         c2 = src[offset];
	        cmp     cl, 0x82                            //         if (c1 == 0x82) {
	        jne     L602                                //
	        cmp     dl, 0xF2                            //             if (c2 >= 0xF2/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
	        jb      L602                                //
	        cmp     dl, 0xF9                            //
	        ja      L602                                //
	        mov     byte ptr [edi], 0x83                //                 dest[0] = 0x83;
	        sub     dl, 0xF2 - 0x94                     //                 c2 -= 0xF2/*(JISX0213('���') - 0x8200)*/ - 0x94/*('��' - 0x8300)*/;
	                                                    //             }
	                                                    //         }
	L602:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	        add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L600                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // KATAKANA_HALFWIDTH:
	                                                    //     for (; ; ) {
	L700:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L701                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L701                                //
	        inc     edi                                 //             dest++;
	        jmp     L700                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L701:   xor     edx, edx                            //         c2 = src[offset];
	        cmp     cl, 0x82                            //         if (c1 == 0x82) {
	        mov     dl, byte ptr [esi + ebp]            //
	        jne     L703                                //
	        cmp     dl, 0xF2                            //             if (c2 >= 0xF2) {
	        jb      L709                                //
	        cmp     dl, 0xF2                            //                 if (c2 == 0xF2/*(JISX0213('���') - 0x8200)*/) {
	        jne     L702                                //
	        mov     word ptr [edi], 0xDEB3              //                     *(unsigned short *)dest = 0xDEB3/*BSWAP16('��')*/;
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L702:   cmp     dl, 0xF5                            //                 } else if (c2 >= 0xF5/*(JISX0213('���') - 0x8200)*/ && c2 <= 0xF9/*(JISX0213('���') - 0x8200)*/) {
	        jb      L709                                //
	        cmp     dl, 0xF9                            //
	        ja      L709                                //
	        add     dx, 0xDFB6 - 0xF5                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0xF5/*(JISX0213('���') - 0x8200)*/;
	        mov     word ptr [edi], dx                  //
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L703:   cmp     cl, 0x83                            //         } else if (c1 == 0x83) {
	        jne     L707                                //
	        cmp     dl, 0x97                            //             if (c2 >= 0x97/*(JISX0213('�J�') - 0x8300)*/) {
	        jb      L709                                //
	        cmp     dl, 0x9B                            //                 if (c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
	        ja      L704                                //
	        add     dx, 0xDFB6 - 0x97                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
	        mov     word ptr [edi], dx                  //
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L704:   cmp     dl, 0x9C                            //                 } else if (c2 == 0x9C/*(JISX0213('�Z�') - 0x8300)*/) {
	        jne     L705                                //
	        mov     word ptr [edi], 0xDFBE              //                     *(unsigned short *)dest = 0xDFBE/*BSWAP16('��')*/;
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L705:   cmp     dl, 0x9D                            //                 } else if (c2 == 0x9D/*(JISX0213('�c�') - 0x8300)*/) {
	        jne     L706                                //
	        mov     word ptr [edi], 0xDFC2              //                     *(unsigned short *)dest = 0xDFC2/*BSWAP16('��')*/;
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;

	L706:   cmp     dl, 0x9E                            //                 } else if (c2 == 0x9E/*(JISX0213('�g�') - 0x8300)*/) {
	        jne     L709                                //
	        mov     word ptr [edi], 0xDFC4              //                     *(unsigned short *)dest = 0xDFC4/*BSWAP16('��')*/;
	        jmp     L710                                //                     goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L707:   cmp     cl, 0x84                            //         } else if (c1 == 0x84) {
	        jne     L709                                //
	        cmp     dl, 0x92                            //             if (c2 == 0x92/*(JISX0213('���') - 0x8400)*/) {
	        jne     L708                                //
	        mov     word ptr [edi], 0xDEDC              //                 *(unsigned short *)dest = 0xDEDC/*BSWAP16('��')*/;
	        jmp     L710                                //                 goto KATAKANA_HALFWIDTH_CONTINUE;

	L708:   cmp     dl, 0x95                            //             } else if (c2 == 0x95/*(JISX0213('���') - 0x8400)*/) {
	        jne     L709                                //
	        mov     word ptr [edi], 0xDEA6              //                 *(unsigned short *)dest = 0xDEA6/*BSWAP16('��')*/;
	        jmp     L710                                //                 goto KATAKANA_HALFWIDTH_CONTINUE;
	                                                    //             }
	                                                    //         }
	L709:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	                                                    //     KATAKANA_HALFWIDTH_CONTINUE:
	L710:   add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L700                                //             continue;
	        jmp     L900                                //         break;
	                                                    //     }
	                                                    //     goto LCMAP_STRING;

	        align(16)                                   // HALFWIDTH:
	                                                    //     for (; ; ) {
	L800:   mov     dl, byte ptr [esi + ebp]            //         dest[0] = c1 = src[offset];
	        inc     ebp                                 //         if (!++offset)
	        mov     byte ptr [edi], dl                  //
	        jz      L900                                //             break;
	        mov     cl, dl                              //         if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1) {
	        sub     dl, 0x81                            //
	        cmp     dl, 0x9F - 0x81 + 1                 //
	        jb      L801                                //
	        sub     dl, 0xE0 - 0x81                     //
	        cmp     dl, 0xFC - 0xE0 + 1                 //
	        jb      L801                                //
	        inc     edi                                 //             dest++;
	        jmp     L800                                //             continue;
	                                                    //         }
	        align(16)                                   //
	L801:   xor     edx, edx                            //         c2 = src[offset];
	        cmp     cl, 0x83                            //         if (c1 == 0x83) {
	        mov     dl, byte ptr [esi + ebp]            //
	        jne     L805                                //
	        cmp     dl, 0x97                            //             if (c2 >= 0x97/*(JISX0213('�J�') - 0x8300)*/) {
	        jb      L807                                //
	        cmp     dl, 0x9B                            //                 if (c2 <= 0x9B/*(JISX0213('�R�') - 0x8300)*/) {
	        ja      L802                                //
	        add     dx, 0xDFB6 - 0x97                   //                     *(unsigned short *)dest = c2 + 0xDFB6/*BSWAP16('��')*/ - 0x97/*(JISX0213('�J�') - 0x8300)*/;
	        mov     word ptr [edi], dx                  //
	        jmp     L808                                //                     goto HALFWIDTH_CONTINUE;

	L802:   cmp     dl, 0x9C                            //                 } else if (c2 == 0x9C/*(JISX0213('�Z�') - 0x8300)*/) {
	        jne     L803                                //
	        mov     word ptr [edi], 0xDFBE              //                     *(unsigned short *)dest = 0xDFBE/*BSWAP16('��')*/;
	        jmp     L808                                //                     goto HALFWIDTH_CONTINUE;

	L803:   cmp     dl, 0x9D                            //                 } else if (c2 == 0x9D/*(JISX0213('�c�') - 0x8300)*/) {
	        jne     L804                                //
	        mov     word ptr [edi], 0xDFC2              //                     *(unsigned short *)dest = 0xDFC2/*BSWAP16('��')*/;
	        jmp     L808                                //                     goto HALFWIDTH_CONTINUE;

	L804:   cmp     dl, 0x9E                            //                 } else if (c2 == 0x9E/*(JISX0213('�g�') - 0x8300)*/) {
	        jne     L807                                //
	        mov     word ptr [edi], 0xDFC4              //                     *(unsigned short *)dest = 0xDFC4/*BSWAP16('��')*/;
	        jmp     L808                                //                     goto HALFWIDTH_CONTINUE;
	                                                    //                 }
	                                                    //             }
	L805:   cmp     cl, 0x84                            //         } else if (c1 == 0x84) {
	        jne     L807                                //
	        cmp     dl, 0x92                            //             if (c2 == 0x92/*(JISX0213('���') - 0x8400)*/) {
	        jne     L806                                //
	        mov     word ptr [edi], 0xDEDC              //                 *(unsigned short *)dest = 0xDEDC/*BSWAP16('��')*/;
	        jmp     L808                                //                 goto HALFWIDTH_CONTINUE;

	L806:   cmp     dl, 0x95                            //             } else if (c2 == 0x95/*(JISX0213('���') - 0x8400)*/) {
	        jne     L807                                //
	        mov     word ptr [edi], 0xDEA6              //                 *(unsigned short *)dest = 0xDEA6/*BSWAP16('��')*/;
	        jmp     L808                                //                 goto HALFWIDTH_CONTINUE;
	                                                    //             }
	                                                    //         }
	L807:   mov     byte ptr [edi + 1], dl              //         dest[1] = c2;
	                                                    //     HALFWIDTH_CONTINUE:
	L808:   add     edi, 2                              //         dest += 2;
	        inc     ebp                                 //         if (++offset)
	        jnz     L800                                //             continue;
	                                                    //         break;
	                                                    //     }

	        align(16)                                   // LCMAP_STRING:
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

	#undef align
}
#endif
