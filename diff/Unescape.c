#include <windows.h>

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
					unsigned char c1;

					c1 = *(src++);
					if ((char)(c1 -= '0') < 0)
						break;
					if (c1 > 9)
						if ((char)(c1 -= 'A' - '0') < 0 || c1 > 0x0F - 0x0A &&
							(char)(c1 -= 'a' - 'A') < 0 || c1 > 0x0F - 0x0A)
							break;
						else
							c1 += 0x0A;
					do	// do { ... } while (0);
					{
						unsigned char c2;

						if (src >= last)
							break;
						c2 = *(src + 1);
						if ((char)(c2 -= '0') < 0)
							break;
						if (c2 > 9)
							if ((char)(c2 -= 'A' - '0') < 0 || c2 > 0x0F - 0x0A &&
								(char)(c2 -= 'a' - 'A') < 0 || c2 > 0x0F - 0x0A)
								break;
							else
								c2 += 0x0A;
						c1 = c1 * 0x10 + c2;
						src++;
					} while (0);
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

