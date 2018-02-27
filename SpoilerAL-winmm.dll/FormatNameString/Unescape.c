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
			size_t count;

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
			case 'v':
				*(dest++) = '\v';
				break;
			case 'x':
				if (src < last)
				{
					unsigned char c1, c2;

					if ((c1 = ATOITBL(*(src++))) > 0x0F)
						break;
					if (src < last && (c2 = ATOITBL(*(src + 1))) <= 0x0F)
					{
						c1 = c1 * 0x10 + c2;
						src++;
					}
					*(dest++) = c1;
				}
				break;
			default:
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

