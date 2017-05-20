#if defined(_ultot) && defined(_ultotn)
#include <windows.h>
#include <intrin.h>
#include "digitslut.h"
#include "digitshex.h"
#pragma intrinsic(_BitScanReverse)

typedef unsigned __int32 uint32_t;

#ifdef _UNICODE
typedef unsigned __int32 tchar2_t;
#define digitsLutT digitsLutW
#else
typedef unsigned __int16 tchar2_t;
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
		/* digit */
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
	uint32_t lo, hi;

	if (value >= 1000000)
		if (value >= 100000000)
			if (value >= 1000000000)
				goto LENGTH10;
			else
				goto LENGTH9;
		else
			if (value >= 10000000)
				goto LENGTH8;
			else
				goto LENGTH7;
	else
		if (value >= 10000)
			if (value >= 100000)
				goto LENGTH6;
			else
				goto LENGTH5;
		else
			if (value >= 100)
				if (value >= 1000)
					goto LENGTH4;
				else
					goto LENGTH3;
			else
				if (value >= 10)
					goto LENGTH2;
				else
					goto LENGTH1;

LENGTH10:
	hi = value / 100000;
	lo = value % 100000;
	hi = hi * ((1 << 25) / 1000 + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[hi >> 25]; hi = (hi & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[hi >> 25]; hi = (hi & 0x01FFFFFF) * 10;
	              buffer[4] = (TCHAR)(hi >> 25) + TEXT('0');
	lo = lo * ((1 << 25) / 1000 + 1) - (lo >> 2);
	*(tchar2_t *)&buffer[5] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[7] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 10;
	*(tchar2_t *)&buffer[9] = (tchar2_t)(lo >> 25) + TEXT('0');
	return 10;

LENGTH9:
	hi = value / 100000;
	lo = value % 100000;
	hi = hi * ((1 << 25) / 100 + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[hi >> 25]; hi = (hi & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[hi >> 25];
	lo = lo * ((1 << 25) / 1000 + 1) - (lo >> 2);
	*(tchar2_t *)&buffer[4] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[6] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 10;
	*(tchar2_t *)&buffer[8] = (tchar2_t)(lo >> 25) + TEXT('0');
	return 9;

LENGTH8:
	hi = value / 10000;
	lo = value % 10000;
	hi = hi * ((1 << 25) / 100 + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[hi >> 25]; hi = (hi & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[hi >> 25];
	lo = lo * ((1 << 25) / 100 + 1);
	*(tchar2_t *)&buffer[4] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[6] = ((tchar2_t *)digitsLutT)[lo >> 25];
	              buffer[8] = TEXT('\0');
	return 8;

LENGTH7:
	hi = value / 100000;
	lo = value % 100000;
	hi = hi * ((1 << 25) + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[hi >> 25];
	lo = lo * ((1 << 25) / 1000 + 1) - (lo >> 2);
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[4] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 10;
	*(tchar2_t *)&buffer[6] = (tchar2_t)(lo >> 25) + TEXT('0');
	return 7;

LENGTH6:
	hi = value / 10000;
	lo = value % 10000;
	hi = hi * ((1 << 25) + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[hi >> 25];
	lo = lo * ((1 << 25) / 100 + 1);
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[lo >> 25]; lo = (lo & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[4] = ((tchar2_t *)digitsLutT)[lo >> 25];
	              buffer[6] = TEXT('\0');
	return 6;

LENGTH5:
	value = value * ((1 << 25) / 1000 + 1) - (value / 4);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[value >> 25]; value = (value & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[value >> 25]; value = (value & 0x01FFFFFF) * 10;
	*(tchar2_t *)&buffer[4] = (tchar2_t)(value >> 25) + TEXT('0');
	return 5;

LENGTH4:
	value = value * ((1 << 25) / 100 + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[value >> 25]; value = (value & 0x01FFFFFF) * 100;
	*(tchar2_t *)&buffer[2] = ((tchar2_t *)digitsLutT)[value >> 25];
	              buffer[4] = TEXT('\0');
	return 4;

LENGTH3:
	value = value * ((1 << 25) / 10 + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[value >> 25]; value = (value & 0x01FFFFFF) * 10;
	*(tchar2_t *)&buffer[2] = (tchar2_t)(value >> 25) + TEXT('0');
	return 3;

LENGTH2:
	value = value * ((1 << 25) + 1);
	*(tchar2_t *)&buffer[0] = ((tchar2_t *)digitsLutT)[value >> 25];
	              buffer[2] = TEXT('\0');
	return 2;

LENGTH1:
	*(tchar2_t *)&buffer[0] = (tchar2_t)value + TEXT('0');
	return 1;
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
