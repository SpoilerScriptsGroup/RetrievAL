#include <windows.h>
#include "atoitbl.h"

char * __fastcall Unescape(char *first, char *last)
{
	#define p ((LPBYTE)first)

	if (p < last)
	{
		for (; ; )
		{
			BYTE c, *src;

			if ((c = *p) != '\\')
				if (++p < last && !IsDBCSLeadByteEx(CP_THREAD_ACP, c) || ++p < last)
					continue;
				else
					break;
			if ((src = p + 1) < last)
			{
				BYTE   x;
				size_t size;

				switch (c = *(src++))
				{
				case '0':
					*(p++) = '\0';
					break;
				case 'a':
					*(p++) = '\a';
					break;
				case 'b':
					*(p++) = '\b';
					break;
				case 'f':
					*(p++) = '\f';
					break;
				case 'n':
					*(p++) = '\n';
					break;
				case 'r':
					*(p++) = '\r';
					break;
				case 't':
					*(p++) = '\t';
					break;
				case 'v':
					*(p++) = '\v';
					break;
				case 'u':
				case 'U':
					if (src < last)
					{
						x = *src;
						if (CTOI(&x, 'f', 16))
						{
							wchar_t w;

							w = x;
							do	/* do { ... } while (0); */
							{
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if ((src++)[-4] == 'u' || src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								if (++src >= last)
									break;
								x = *src;
								if (!CTOI(&x, 'f', 16))
									break;
								w = w * 0x10 + x;
								src++;
							} while (0);
							p += WideCharToMultiByte(CP_THREAD_ACP, 0, &w, 1, p, 2, NULL, NULL);
							break;
						}
					}
					*(p++) = c;
					break;
				case 'x':
					if (src < last)
					{
						x = *src;
						if (CTOI(&x, 'f', 16))
						{
							c = x;
							if (++src < last)
							{
								x = *src;
								if (CTOI(&x, 'f', 16))
								{
									c = c * 0x10 + x;
									src++;
								}
							}
						}
					}
					*(p++) = c;
					break;
				default:
					*(p++) = c;
					if (src < last && IsDBCSLeadByteEx(CP_THREAD_ACP, c))
						*(p++) = *(src++);
					break;
				}
				if (size = (last -= src - p) - p)
				{
					memcpy(p, src, size);
					*last = '\0';
					continue;
				}
			}
			else
			{
				last--;
			}
			*last = '\0';
			break;
		}
	}
	return last;

	#undef p
}

