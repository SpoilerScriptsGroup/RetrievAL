#if defined(_ui64tot) && defined(_ui64tont) && defined(_ui32tont)
#include <windows.h>
#include <intrin.h>
#include "digitstbl.h"

#ifdef _MSC_VER
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#pragma intrinsic(__emulu)
#pragma intrinsic(_BitScanReverse)

typedef __int32          int32_t;
typedef __int64          int64_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#ifdef _UNICODE
typedef uint32_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100W)
#else
typedef uint16_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100A)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TO_TCHAR2(c) (tchar2_t)(c)
#define LOW(x) ((uint32_t *)&x)[0]
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << (sizeof(TCHAR) * 8))
#define LOW(x) ((uint32_t *)&x)[1]
#endif

#define _ui32to10t _ui32tont(10)
#define _ui32to16t _ui32tont(16)
#define _ui32to8t  _ui32tont(8)
#define _ui64to10t _ui64tont(10)
#define _ui64to16t _ui64tont(16)
#define _ui64to8t  _ui64tont(8)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer);

TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
	{
		/* digit */
		_ui64to10t(value, str);
	}
	else if (radix == 16)
	{
		/* letter or digit */
		_ui64to16t(value, str, TRUE);
	}
	else if (radix == 8)
	{
		/* digit */
		_ui64to8t(value, str);
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

size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	if (!(uint32_t)(value >> 32))
	{
		return _ui32to10t((uint32_t)value, buffer);
	}
	else
	{
		const uint64_t reciprocal_u8 = ((1ULL << (32 + 25)) / 10000000);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		TCHAR *p;

		p = buffer;
		if (value >= 10000000000000000)
			if (value >= 1000000000000000000)
				if (value >= 10000000000000000000)
					goto LENGTH20;
				else
					goto LENGTH19;
			else
				if (value >= 100000000000000000)
					goto LENGTH18;
				else
					goto LENGTH17;
		else
			if (value >= 10000000000000)
				if (value >= 1000000000000000)
					goto LENGTH16;
				else
					if (value >= 100000000000000)
						goto LENGTH15;
					else
						goto LENGTH14;
			else
				if (value >= 100000000000)
					if (value >= 1000000000000)
						goto LENGTH13;
					else
						goto LENGTH12;
				else
					if (value >= 10000000000)
						goto LENGTH11;
					else
						goto LENGTH10;

		#define PUTCHAR(power10)                                  \
		do                                                        \
		{                                                         \
		    if ((int64_t)(value -= 5 * power10) >= 0)             \
		    {                                                     \
		        if ((int64_t)(value -= 2 * power10) >= 0)         \
		        {                                                 \
		            if ((int64_t)(value -= 1 * power10) >= 0)     \
		            {                                             \
		                if ((int64_t)(value -= 1 * power10) >= 0) \
		                {                                         \
		                    *(p++) = TEXT('9');                   \
		                    break;                                \
		                }                                         \
		                *(p++) = TEXT('8');                       \
		            }                                             \
		            else                                          \
		            {                                             \
		                *(p++) = TEXT('7');                       \
		            }                                             \
		        }                                                 \
		        else                                              \
		        {                                                 \
		            if ((int64_t)(value += 1 * power10) >= 0)     \
		            {                                             \
		                *(p++) = TEXT('6');                       \
		                break;                                    \
		            }                                             \
		            *(p++) = TEXT('5');                           \
		        }                                                 \
		    }                                                     \
		    else                                                  \
		    {                                                     \
		        if ((int64_t)(value += 3 * power10) >= 0)         \
		        {                                                 \
		            if ((int64_t)(value -= 1 * power10) >= 0)     \
		            {                                             \
		                if ((int64_t)(value -= 1 * power10) >= 0) \
		                {                                         \
		                    *(p++) = TEXT('4');                   \
		                    break;                                \
		                }                                         \
		                *(p++) = TEXT('3');                       \
		            }                                             \
		            else                                          \
		            {                                             \
		                *(p++) = TEXT('2');                       \
		            }                                             \
		        }                                                 \
		        else                                              \
		        {                                                 \
		            if ((int64_t)(value += 1 * power10) >= 0)     \
		            {                                             \
		                *(p++) = TEXT('1');                       \
		                break;                                    \
		            }                                             \
		            *(p++) = TEXT('0');                           \
		        }                                                 \
		    }                                                     \
		    value += 1 * power10;                                 \
		} while (0)

	LENGTH20:
		value -= 10000000000000000000;
		*(p++) = TEXT('1');
	LENGTH19:
		PUTCHAR(1000000000000000000);
	LENGTH18:
		PUTCHAR(100000000000000000);
	LENGTH17:
		PUTCHAR(10000000000000000);
	LENGTH16:
		PUTCHAR(1000000000000000);
	LENGTH15:
		PUTCHAR(100000000000000);
	LENGTH14:
		PUTCHAR(10000000000000);
	LENGTH13:
		PUTCHAR(1000000000000);
	LENGTH12:
		PUTCHAR(100000000000);
	LENGTH11:
		PUTCHAR(10000000000);

		#undef PUTCHAR

		do
		{
			if ((uint32_t)(value >> 32))
			{
	LENGTH10:
				if ((int64_t)(value -= 7000000000) >= 0)
				{
					if ((int64_t)(value -= 1000000000) >= 0)
					{
						if ((int64_t)(value -= 1000000000) >= 0)
						{
							p[0] = TEXT('9');
							break;
						}
						p[0] = TEXT('8');
					}
					else
					{
						p[0] = TEXT('7');
					}
				}
				else
				{
					if ((int64_t)(value += 2000000000) >= 0)
					{
						if ((int64_t)(value -= 1000000000) >= 0)
						{
							p[0] = TEXT('6');
							break;
						}
						p[0] = TEXT('5');
					}
					else
					{
						p[0] = TEXT('4');
					}
				}
			}
			else
			{
				if (LOW(value) >= 1000000000)
				{
					if ((int32_t)(LOW(value) -= 3000000000) >= 0)
					{
						if ((int32_t)(LOW(value) -= 1000000000) >= 0)
						{
							p[0] = TEXT('4');
							break;
						}
						p[0] = TEXT('3');
					}
					else
					{
						if ((int32_t)(LOW(value) += 1000000000) >= 0)
						{
							p[0] = TEXT('2');
							break;
						}
						p[0] = TEXT('1');
					}
				}
				else
				{
					p[0] = TEXT('0');
					break;
				}
			}
			LOW(value) += 1000000000;
		} while (0);

		#define value LOW(value)

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&p[1] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[3] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[5] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[7] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&p[9] = TO_TCHAR2((value >> 25) + TEXT('0'));

		#undef value

		return p + 10 - buffer;
	}
}

size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	if (!(uint32_t)(value >> 32))
	{
		return _ui32to16t((uint32_t)value, buffer, upper);
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

size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	if (!(uint32_t)(value >> 32))
	{
		return _ui32to8t((uint32_t)value, buffer);
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
