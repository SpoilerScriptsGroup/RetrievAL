#if defined(_ultot) && defined(_ui32tont)
#include <windows.h>

#define _ui32to10t _ui32tont(10)
#define _ui32to16t _ui32tont(16)
#define _ui32to8t  _ui32tont(8)

size_t __fastcall _ui32to10t(unsigned __int32 value, TCHAR *buffer);
size_t __fastcall _ui32to16t(unsigned __int32 value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to8t(unsigned __int32 value, TCHAR *buffer);

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
	{
		/* digit */
		_ui32to10t(value, str);
	}
	else if (radix == 16)
	{
		/* letter or digit */
		_ui32to16t(value, str, TRUE);
	}
	else if (radix == 8)
	{
		/* digit */
		_ui32to8t(value, str);
	}
	else
	{
		TCHAR *p1, *p2;

		p2 = p1 = str;
		if (radix == 2)
		{
			/* digit */
			do
				/* convert to ascii and store */
				*(p1++) = ((TCHAR)value & 1) + TEXT('0');
			while (value >>= 1);
		}
		else if (radix == 4)
		{
			/* digit */
			do
				/* convert to ascii and store */
				*(p1++) = ((TCHAR)value & 3) + TEXT('0');
			while (value >>= 2);
		}
		else if (radix > 1)
		{
			if (radix <= 10)
			{
				do
				{
					*(p1++) = (TCHAR)(value % (unsigned int)radix) + TEXT('0');
					value /= (unsigned int)radix;
				} while (value);
			}
			else if (radix <= 10 + TEXT('Z') - TEXT('A') + 1)
			{
				do
				{
					unsigned long rem;

					rem = value % (unsigned int)radix;
					value /= (unsigned int)radix;
					*(p1++) = (TCHAR)rem + (rem < 10 ? TEXT('0') : TEXT('A') - 10);
				} while (value);
			}
		}

		/* We now have the digit of the number in the buffer, but in reverse */
		/* order.  Thus we reverse them now.                                 */

		*(p1--) = TEXT('\0');   /* terminate string; p points to last digit */
		while (p1 > p2)         /* repeat until halfway */
		{
			TCHAR c1, c2;

			c1 = *p1;           /* swap *p1 and *p2 */
			c2 = *p2;
			*(p1--) = c2;
			*(p2++) = c1;       /* advance to next two digits */
		}
	}
	return str;
}

#include "ui32tont\ui32tont.h"

#endif
