#if defined(_ui64tot) && defined(_ui64totn)
#include <windows.h>
#include "intrinsic.h"
#include "digitslut.h"
#include "digitshex.h"

typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#ifdef _UNICODE
typedef unsigned __int32 tchar2;
#define digitsLutT digitsLutW
#else
typedef unsigned __int16 tchar2;
#define digitsLutT digitsLutA
#endif

#define _ui64tot10 _ui64totn(10)
#define _ui64tot16 _ui64totn(16)
#define _ui64tot8  _ui64totn(8)

size_t __fastcall _ui64tot10(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64tot16(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui64tot8(uint64_t value, TCHAR *buffer);

TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
	{
		/* digit */
		_ui64tot10(value, str);
	}
	else if (radix == 16)
	{
		/* letter or digit */
		_ui64tot16(value, str, TRUE);
	}
	else if (radix == 8)
	{
		/* letter or digit */
		_ui64tot8(value, str);
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
					unsigned __int64 rem;

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

size_t __fastcall _ui64tot10(uint64_t value, TCHAR *buffer)
{
	TCHAR    *p;
	uint32_t a, b, c, d1, d2, d3, d4, d5, d6, d7, d8;

	p = buffer;
	if (value < 100000000)
	{
		uint32_t v;

		v = (uint32_t)value;
		if (v < 10000)
		{
			d1 = (v / 100) << 1;
			d2 = (v % 100) << 1;
			if (v >= 100)
			{
				if (v >= 1000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
				else
					*p++ = digitsLutT[d1 + 1];
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
			}
			else
			{
				if (v >= 10)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
				else
					*p++ = digitsLutT[d2 + 1];
			}
		}
		else
		{
			// value = bbbbcccc
			b = v / 10000;
			c = v % 10000;
			d1 = (b / 100) << 1;
			d2 = (b % 100) << 1;
			if (value >= 1000000)
			{
				if (value >= 10000000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
				else
					*p++ = digitsLutT[d1 + 1];
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
			}
			else
			{
				if (value >= 100000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
				else
					*p++ = digitsLutT[d2 + 1];
			}
			d3 = (c / 100) << 1;
			d4 = (c % 100) << 1;
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d3];
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];
		}
	}
	else
	{
		uint32_t v0, v1, b0, b1, c0, c1;

		if (value < 10000000000000000)
		{
			v0 = (uint32_t)(value / 100000000);
			v1 = (uint32_t)(value % 100000000);
			b0 = v0 / 10000;
			c0 = v0 % 10000;
			d1 = (b0 / 100) << 1;
			d2 = (b0 % 100) << 1;
			d3 = (c0 / 100) << 1;
			d4 = (c0 % 100) << 1;
			if (value >= 1000000000000)
			{
				if (value >= 100000000000000)
				{
					if (value >= 1000000000000000)
						*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
					else
						*p++ = digitsLutT[d1 + 1];
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
				}
				else
				{
					if (value >= 10000000000000)
						*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
					else
						*p++ = digitsLutT[d2 + 1];
				}
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d3];
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];
			}
			else
			{
				if (value >= 10000000000)
				{
					if (value >= 100000000000)
						*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d3];
					else
						*p++ = digitsLutT[d3 + 1];
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];
				}
				else
				{
					if (value >= 1000000000)
						*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];
					else if (value >= 100000000)
						*p++ = digitsLutT[d4 + 1];
				}
			}
		}
		else
		{
			a = (uint32_t)(value / 10000000000000000); // 1 to 1844
			value %= 10000000000000000;
			if (a >= 100)
			{
				uint32_t i, j;

				i = a / 100;
				j = a % 100;
				if (a >= 1000)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[i << 1];
				else
					*p++ = (TCHAR)i + TEXT('0');
				*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[j << 1];
			}
			else
			{
				if (a >= 10)
					*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[a << 1];
				else
					*p++ = (TCHAR)a + TEXT('0');
			}
			v0 = (uint32_t)(value / 100000000);
			v1 = (uint32_t)(value % 100000000);
			b0 = v0 / 10000;
			c0 = v0 % 10000;
			d1 = (b0 / 100) << 1;
			d2 = (b0 % 100) << 1;
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d1];
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d2];
			d3 = (c0 / 100) << 1;
			d4 = (c0 % 100) << 1;
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d3];
			*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d4];

		}
		b1 = v1 / 10000;
		c1 = v1 % 10000;
		d5 = (b1 / 100) << 1;
		d6 = (b1 % 100) << 1;
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d5];
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d6];
		d7 = (c1 / 100) << 1;
		d8 = (c1 % 100) << 1;
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d7];
		*((*(tchar2 **)&p)++) = *(tchar2 *)&digitsLutT[d8];
	}
	*p = TEXT('\0');
	return p - buffer;
}

size_t __fastcall _ui64tot16(uint64_t value, TCHAR *buffer, BOOL upper)
{
	const char    *digits;
	TCHAR         *p, *end;
	unsigned long bits;

	digits = upper ? digitsHexLarge : digitsHexSmall;
	p = buffer;
	if (_BitScanReverse64(&bits, value))
		p += bits / 4;
	*(end = p + 1) = TEXT('\0');
	do
	{
		*(p--) = digits[(size_t)value & 0x0F];
	} while (value >>= 4);
	return end - buffer;
}

size_t __fastcall _ui64tot8(uint64_t value, TCHAR *buffer)
{
	TCHAR         *p, *end;
	unsigned long bits;

	p = buffer;
	if (_BitScanReverse64(&bits, value))
		p += bits / 3;
	*(end = p + 1) = TEXT('\0');
	do
	{
		*(p--) = ((TCHAR)value & 0x07) + TEXT('0');
	} while (value >>= 3);
	return end - buffer;
}
#endif
