#if defined(UTF8NICMP) || defined(UTF8NBICMP)
#include <stddef.h>

#ifndef UTF8NICMP
#define UTF8NICMP UTF8NBICMP
#endif
int __cdecl UTF8NICMP(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	if (count)
		do
		{
			unsigned char c1, c2;

			c1 = *(string1++);
			c2 = *(string2++);
			if ((char)c1 >= 0)
			{
				if (!(c1 -= c2))
					if (c2)
						continue;
					else
						break;
				if (c1 == 'A' - 'a')
				{
					if ((char)c2 >= 'a' && c2 <= 'z')
						continue;
				}
				else if (c1 == 'a' - 'A')
				{
					if ((char)c2 >= 'A' && c2 <= 'Z')
						continue;
				}
				c1 += c2;
			}
			else if (c1 == c2)
			{
#ifdef UTF8NBICMP
				if (!--count)
					break;
#endif
				if (c1 <= 0xDF)
					goto LENGTH2;
				if (c1 <= 0xEF)
					goto LENGTH3;
				c1 = *(string1++);
				c2 = *(string2++);
				if (c1 == c2)
				{
					if (!c1)
						break;
#ifdef UTF8NBICMP
					if (!--count)
						break;
#endif
			LENGTH3:
					c1 = *(string1++);
					c2 = *(string2++);
					if (c1 == c2)
					{
						if (!c1)
							break;
#ifdef UTF8NBICMP
						if (!--count)
							break;
#endif
			LENGTH2:
						c1 = *(string1++);
						c2 = *(string2++);
						if (c1 == c2)
							if (c1)
								continue;
							else
								break;
					}
				}
			}
			return (int)c1 - (int)c2;
		} while (--count);
	return 0;
}
#endif
