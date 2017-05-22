#if defined(_ui64tot) && defined(_ui64totn) && defined(_ultotn)
#include <windows.h>
#include <intrin.h>
#include "digitslut.h"
#include "digitshex.h"

#ifdef _MSC_VER
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#pragma intrinsic(__emulu)
#pragma intrinsic(_BitScanReverse)

typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#ifdef _UNICODE
typedef unsigned __int32 tchar2_t;
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TO_TCHAR2(c) (tchar2_t)(c)
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << 16)
#endif
#define digitsLutT ((tchar2_t *)digitsLutW)
#else
typedef unsigned __int16 tchar2_t;
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TO_TCHAR2(c) (tchar2_t)(c)
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << 8)
#endif
#define digitsLutT ((tchar2_t *)digitsLutA)
#endif

#define _ultot10   _ultotn(10)
#define _ultot16   _ultotn(16)
#define _ultot8    _ultotn(8)
#define _ui64tot10 _ui64totn(10)
#define _ui64tot16 _ui64totn(16)
#define _ui64tot8  _ui64totn(8)

size_t __fastcall _ultot10(uint32_t value, TCHAR *buffer);
size_t __fastcall _ultot16(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ultot8(uint32_t value, TCHAR *buffer);
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
		/* digit */
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
	if (!(uint32_t)(value >> 32))
	{
		return _ultot10((uint32_t)value, buffer);
	}
	else if (value < 1000000000000000000)
	{
		const uint64_t reciprocal_u8 = ((1ULL << 57) / 10000000);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		size_t   n;
		uint32_t a, b;

		a = value % 1000000000;
		value /= 1000000000;
		b = value % 1000000000;
		n = _ultot10(b, buffer);
		a = (uint32_t)(__emulu(a, reciprocal_lo) >> 32)
			+ a * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[n +  0] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  2] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  4] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  6] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[n +  8] = TO_TCHAR2((a >> 25) + TEXT('0'));
		return n + 9;
	}
	else
	{
		const uint64_t reciprocal_u8 = ((1ULL << 57) / 10000000);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		size_t   n;
		uint32_t a, b;

		a = value % 1000000000;
		value /= 1000000000;
		b = value % 1000000000;
		value /= 1000000000;
		n = _ultot10((uint32_t)value, buffer);
		b = (uint32_t)(__emulu(b, reciprocal_lo) >> 32)
			+ b * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[n +  0] = digitsLutT[b >> 25]; b = (b & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  2] = digitsLutT[b >> 25]; b = (b & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  4] = digitsLutT[b >> 25]; b = (b & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n +  6] = digitsLutT[b >> 25]; b = (b & 0x01FFFFFF) * 10;
		              buffer[n +  8] = (TCHAR)((b >> 25) + TEXT('0'));
		a = (uint32_t)(__emulu(a, reciprocal_lo) >> 32)
			+ a * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[n +  9] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n + 11] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n + 13] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[n + 15] = digitsLutT[a >> 25]; a = (a & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[n + 17] = TO_TCHAR2((a >> 25) + TEXT('0'));
		return n + 18;
	}
}

size_t __fastcall _ui64tot16(uint64_t value, TCHAR *buffer, BOOL upper)
{
	if (!(uint32_t)(value >> 32))
	{
		return _ultot16((uint32_t)value, buffer, upper);
	}
	else
	{
		const char    *digits;
		TCHAR         *p, *end;
		unsigned long bits;

		digits = upper ? digitsHexLarge : digitsHexSmall;
		p = buffer;
		_BitScanReverse(&bits, (uint32_t)(value >> 32));
		p += (bits + 32) / 4;
		*(end = p + 1) = TEXT('\0');
		do
		{
			*(p--) = digits[(size_t)value & 0x0F];
		} while (value >>= 4);
		return end - buffer;
	}
}

size_t __fastcall _ui64tot8(uint64_t value, TCHAR *buffer)
{
	if (!(uint32_t)(value >> 32))
	{
		return _ultot8((uint32_t)value, buffer);
	}
	else
	{
		TCHAR         *p, *end;
		unsigned long bits;

		p = buffer;
		_BitScanReverse(&bits, (uint32_t)(value >> 32));
		p += (bits + 32) / 3;
		*(end = p + 1) = TEXT('\0');
		do
		{
			*(p--) = ((TCHAR)value & 0x07) + TEXT('0');
		} while (value >>= 3);
		return end - buffer;
	}
}
#endif
