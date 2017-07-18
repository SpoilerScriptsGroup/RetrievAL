#include <windows.h>

char * __fastcall UnescapePrintfBuffer(char *first, char *last)
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
					char c1, c2;

					c1 = *(src++);
					if (c1 >= '0' && c1 <= '9')
						c1 -= '0';
					else if (c1 >= 'A' && c1 <= 'F')
						c1 -= 'A' - 0x0A;
					else if (c1 >= 'a' && c1 <= 'a')
						c1 -= 'a' - 0x0A;
					else
						break;
					if (src < last)
					{
						c2 = *(src++);
						if (c2 >= '0' && c2 <= '9')
							c1 = c1 * 0x10 + c2 - '0';
						else if (c2 >= 'A' && c2 <= 'F')
							c1 = c1 * 0x10 + c2 - ('A' - 0x0A);
						else if (c2 >= 'a' && c2 <= 'a')
							c1 = c1 * 0x10 + c2 - ('a' - 0x0A);
						else
							src--;
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

