#if defined(_ui64tot) && defined(_ui64totn)
#include <windows.h>
#include "intrinsic.h"
#include "digitslut.h"
#include "digithex.h"

typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

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
	TCHAR *p = buffer;
	if (value < 100000000)
	{
		uint32_t v = (uint32_t)value;
		if (v < 10000)
		{
			const uint32_t d1 = (v / 100) << 1;
			const uint32_t d2 = (v % 100) << 1;

			if (v >= 1000)
				*p++ = gDigitsLut[d1];
			if (v >= 100)
				*p++ = gDigitsLut[d1 + 1];
			if (v >= 10)
				*p++ = gDigitsLut[d2];
			*p++ = gDigitsLut[d2 + 1];
		}
		else
		{
			// value = bbbbcccc
			const uint32_t b = v / 10000;
			const uint32_t c = v % 10000;

			const uint32_t d1 = (b / 100) << 1;
			const uint32_t d2 = (b % 100) << 1;

			const uint32_t d3 = (c / 100) << 1;
			const uint32_t d4 = (c % 100) << 1;

			if (value >= 10000000)
				*p++ = gDigitsLut[d1];
			if (value >= 1000000)
				*p++ = gDigitsLut[d1 + 1];
			if (value >= 100000)
				*p++ = gDigitsLut[d2];
			*p++ = gDigitsLut[d2 + 1];

			*p++ = gDigitsLut[d3];
			*p++ = gDigitsLut[d3 + 1];
			*p++ = gDigitsLut[d4];
			*p++ = gDigitsLut[d4 + 1];
		}
	}
	else if (value < 10000000000000000)
	{
		const uint32_t v0 = (uint32_t)(value / 100000000);
		const uint32_t v1 = (uint32_t)(value % 100000000);

		const uint32_t b0 = v0 / 10000;
		const uint32_t c0 = v0 % 10000;

		const uint32_t d1 = (b0 / 100) << 1;
		const uint32_t d2 = (b0 % 100) << 1;

		const uint32_t d3 = (c0 / 100) << 1;
		const uint32_t d4 = (c0 % 100) << 1;

		const uint32_t b1 = v1 / 10000;
		const uint32_t c1 = v1 % 10000;

		const uint32_t d5 = (b1 / 100) << 1;
		const uint32_t d6 = (b1 % 100) << 1;

		const uint32_t d7 = (c1 / 100) << 1;
		const uint32_t d8 = (c1 % 100) << 1;

		if (value >= 1000000000000000)
			*p++ = gDigitsLut[d1];
		if (value >= 100000000000000)
			*p++ = gDigitsLut[d1 + 1];
		if (value >= 10000000000000)
			*p++ = gDigitsLut[d2];
		if (value >= 1000000000000)
			*p++ = gDigitsLut[d2 + 1];
		if (value >= 100000000000)
			*p++ = gDigitsLut[d3];
		if (value >= 10000000000)
			*p++ = gDigitsLut[d3 + 1];
		if (value >= 1000000000)
			*p++ = gDigitsLut[d4];
		if (value >= 100000000)
			*p++ = gDigitsLut[d4 + 1];

		*p++ = gDigitsLut[d5];
		*p++ = gDigitsLut[d5 + 1];
		*p++ = gDigitsLut[d6];
		*p++ = gDigitsLut[d6 + 1];
		*p++ = gDigitsLut[d7];
		*p++ = gDigitsLut[d7 + 1];
		*p++ = gDigitsLut[d8];
		*p++ = gDigitsLut[d8 + 1];
	}
	else
	{
		const uint32_t a = (uint32_t)(value / 10000000000000000); // 1 to 1844
		value %= 10000000000000000;

		if (a < 10)
			*p++ = TEXT('0') + (TCHAR)a;
		else if (a < 100)
		{
			const uint32_t i = a << 1;
			*p++ = gDigitsLut[i];
			*p++ = gDigitsLut[i + 1];
		}
		else if (a < 1000)
		{
			*p++ = TEXT('0') + (TCHAR)(a / 100);

			const uint32_t i = (a % 100) << 1;
			*p++ = gDigitsLut[i];
			*p++ = gDigitsLut[i + 1];
		}
		else
		{
			const uint32_t i = (a / 100) << 1;
			const uint32_t j = (a % 100) << 1;
			*p++ = gDigitsLut[i];
			*p++ = gDigitsLut[i + 1];
			*p++ = gDigitsLut[j];
			*p++ = gDigitsLut[j + 1];
		}

		const uint32_t v0 = (uint32_t)(value / 100000000);
		const uint32_t v1 = (uint32_t)(value % 100000000);

		const uint32_t b0 = v0 / 10000;
		const uint32_t c0 = v0 % 10000;

		const uint32_t d1 = (b0 / 100) << 1;
		const uint32_t d2 = (b0 % 100) << 1;

		const uint32_t d3 = (c0 / 100) << 1;
		const uint32_t d4 = (c0 % 100) << 1;

		const uint32_t b1 = v1 / 10000;
		const uint32_t c1 = v1 % 10000;

		const uint32_t d5 = (b1 / 100) << 1;
		const uint32_t d6 = (b1 % 100) << 1;

		const uint32_t d7 = (c1 / 100) << 1;
		const uint32_t d8 = (c1 % 100) << 1;

		*p++ = gDigitsLut[d1];
		*p++ = gDigitsLut[d1 + 1];
		*p++ = gDigitsLut[d2];
		*p++ = gDigitsLut[d2 + 1];
		*p++ = gDigitsLut[d3];
		*p++ = gDigitsLut[d3 + 1];
		*p++ = gDigitsLut[d4];
		*p++ = gDigitsLut[d4 + 1];
		*p++ = gDigitsLut[d5];
		*p++ = gDigitsLut[d5 + 1];
		*p++ = gDigitsLut[d6];
		*p++ = gDigitsLut[d6 + 1];
		*p++ = gDigitsLut[d7];
		*p++ = gDigitsLut[d7 + 1];
		*p++ = gDigitsLut[d8];
		*p++ = gDigitsLut[d8 + 1];
	}
	*p = TEXT('\0');
	return p - buffer;
}

size_t __fastcall _ui64tot16(uint64_t value, TCHAR *buffer, BOOL upper)
{
	const char    *digits;
	TCHAR         *p, *end;
	unsigned long bits;

	digits = upper ? digitsLarge : digitsSmall;
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
