#include <windows.h>
#include "atoitbl.h"

char * __fastcall Unescape(char *first, char *last)
{
	#define dest first

	while (dest < last)
	{
		char c;
		char *src;

		if ((c = *dest) != '\\')
		{
			if (++dest < last && IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				dest++;
			continue;
		}
		if ((src = dest + 1) < last)
		{
			size_t        count;
			unsigned char x;
			wchar_t       w;

			switch (c = *(src++))
			{
			case '0':
				*(dest++) = '\0';
				break;
			case 'a':
				*(dest++) = '\a';
				break;
			case 'b':
				*(dest++) = '\b';
				break;
			case 'f':
				*(dest++) = '\f';
				break;
			case 'n':
				*(dest++) = '\n';
				break;
			case 'r':
				*(dest++) = '\r';
				break;
			case 't':
				*(dest++) = '\t';
				break;
			case 'u':
			case 'U':
				if (src >= last)
					break;
				x = *src;
				if (!CTOI(&x, 'f', 16))
					goto DEFAULT;
				w = x;
				do	/* do { ... } while (0); */
				{
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if ((src++)[-4] == 'u' || src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					if (++src >= last)
						break;
					x = src[1];
					if (!CTOI(&x, 'f', 16))
						break;
					w = w * 0x10 + x;
					src++;
				} while (0);
				dest += WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, dest, -1, NULL, NULL);
				break;
			case 'v':
				*(dest++) = '\v';
				break;
			case 'x':
				if (src >= last)
					break;
				x = *src;
				if (!CTOI(&x, 'f', 16))
					goto DEFAULT;
				c = x;
				if (++src < last)
				{
					x = src[1];
					if (CTOI(&x, 'f', 16))
					{
						c = c * 0x10 + x;
						src++;
					}
				}
				*(dest++) = c;
				break;
			default:
			DEFAULT:
				*(dest++) = c;
				if (src < last && IsDBCSLeadByteEx(CP_THREAD_ACP, c))
					*(dest++) = *(src++);
				break;
			}
			count = last - src;
			last -= src - dest;
			memcpy(dest, src, count);
		}
		else
		{
			last--;
		}
		*last = '\0';
	}
	return last;

	#undef dest
}

