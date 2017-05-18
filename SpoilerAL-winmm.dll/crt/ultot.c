#if defined(_ultot) && defined(_ultotn)
#include <windows.h>
#include <intrin.h>
#include "digitslut.h"
#include "digitshex.h"
#pragma intrinsic(_BitScanReverse)

typedef unsigned __int32 uint32_t;

#ifdef _UNICODE
typedef unsigned __int32 tchar2;
#define digitsLutT digitsLutW
#else
typedef unsigned __int16 tchar2;
#define digitsLutT digitsLutA
#endif

#define _ultot10 _ultotn(10)
#define _ultot16 _ultotn(16)
#define _ultot8  _ultotn(8)

size_t __fastcall _ultot10(uint32_t value, TCHAR *buffer);
size_t __fastcall _ultot16(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ultot8(uint32_t value, TCHAR *buffer);

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
	{
		/* digit */
		_ultot10(value, str);
	}
	else if (radix == 16)
	{
		/* letter or digit */
		_ultot16(value, str, TRUE);
	}
	else if (radix == 8)
	{
		/* letter or digit */
		_ultot8(value, str);
	}
	else
	{
		TCHAR *p1, *p2;

		p2 = p1 = str;
		if (radix == 4)
		{
			/* digit */
			do
			{
				/* convert to ascii and store */
				*(p1++) = ((TCHAR)value & 3) + TEXT('0');
			} while (value >>= 2);
		}
		else if (radix == 2)
		{
			/* digit */
			do
			{
				/* convert to ascii and store */
				*(p1++) = ((TCHAR)value & 1) + TEXT('0');
			} while (value >>= 1);
		}
		else if (radix > 1)
		{
			if (radix < 10)
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

size_t __fastcall _ultot10(uint32_t value, TCHAR *buffer)
{
	TCHAR    *p;
	uint32_t a, b, c, d1, d2, d3, d4;

	p = buffer;
	if (value < 10000)
	{
		d1 = (value / 100) << 1;
		d2 = (value % 100) << 1;
		if (value >= 100)
		{
			if (value >= 1000)
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
			else
				*(p++) = digitsLutT[d1 + 1];
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
		}
		else
		{
			if (value >= 10)
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
			else
				*(p++) = digitsLutT[d2 + 1];
		}
	}
	else
	{
		if (value < 100000000)
		{
			// value = bbbbcccc
			b = value / 10000;
			c = value % 10000;
			d1 = (b / 100) << 1;
			d2 = (b % 100) << 1;
			if (value >= 1000000)
			{
				if (value >= 10000000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
				else
					*(p++) = digitsLutT[d1 + 1];
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
			}
			else
			{
				if (value >= 100000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
				else
					*(p++) = digitsLutT[d2 + 1];
			}
		}
		else
		{
			// value = aabbbbcccc in decimal
			a = value / 100000000; // 1 to 42
			value %= 100000000;
			if (a >= 10)
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[a << 1];
			else
				*(p++) = (TCHAR)a + TEXT('0');
			b = value / 10000; // 0 to 9999
			c = value % 10000; // 0 to 9999
			d1 = (b / 100) << 1;
			d2 = (b % 100) << 1;
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
		}
		d3 = (c / 100) << 1;
		d4 = (c % 100) << 1;
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d3];
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];
	}
	*p = TEXT('\0');
	return p - buffer;
}

size_t __fastcall _ultot16(uint32_t value, TCHAR *buffer, BOOL upper)
{
	const char    *digits;
	TCHAR         *p, *end;
	unsigned long bits;

	digits = upper ? digitsHexLarge : digitsHexSmall;
	p = buffer;
	if (_BitScanReverse(&bits, value))
		p += bits / 4;
	*(end = p + 1) = TEXT('\0');
	do
	{
		*(p--) = digits[(size_t)value & 0x0F];
	} while (value >>= 4);
	return end - buffer;
}

size_t __fastcall _ultot8(uint32_t value, TCHAR *buffer)
{
	TCHAR         *p, *end;
	unsigned long bits;

	p = buffer;
	if (_BitScanReverse(&bits, value))
		p += bits / 3;
	*(end = p + 1) = TEXT('\0');
	do
	{
		*(p--) = ((TCHAR)value & 0x07) + TEXT('0');
	} while (value >>= 3);
	return end - buffer;
}
#endif
