#if defined(_ui64tont) && defined(_ui32tont)
#include <windows.h>
#include <intrin.h>
#include "digitstbl.h"

#ifdef _MSC_VER
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#pragma intrinsic(__emulu)

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
#define LO(x) ((uint32_t *)&x)[0]
#define HI(x) ((uint32_t *)&x)[1]
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << (sizeof(TCHAR) * 8))
#define LO(x) ((uint32_t *)&x)[1]
#define HI(x) ((uint32_t *)&x)[0]
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

size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	if (!HI(value))
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
			if (HI(value))
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
				if (LO(value) >= 1000000000)
				{
					if ((int32_t)(LO(value) -= 3000000000) >= 0)
					{
						if ((int32_t)(LO(value) -= 1000000000) >= 0)
						{
							p[0] = TEXT('4');
							break;
						}
						p[0] = TEXT('3');
					}
					else
					{
						if ((int32_t)(LO(value) += 1000000000) >= 0)
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
			LO(value) += 1000000000;
		} while (0);

		#define value LO(value)

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
	if (!HI(value))
	{
		return _ui32to16t((uint32_t)value, buffer, upper);
	}
	else
	{
		TCHAR      *p;
		size_t     length;
		const char *digits;

		p = buffer;
		if (HI(value) >= 0x10000u)
			if (HI(value) >= 0x1000000u)
				if (HI(value) >= 0x10000000u)
					p += 16;
				else
					p += 15;
			else
				if (HI(value) >= 0x100000u)
					p += 14;
				else
					p += 13;
		else
			if (HI(value) >= 0x100u)
				if (HI(value) >= 0x1000u)
					p += 12;
				else
					p += 11;
			else
				if (HI(value) >= 0x10u)
					p += 10;
				else
					p += 9;
		*p = TEXT('\0');
		length = p - buffer;
		digits = upper ? digitsHexLarge : digitsHexSmall;
		do
		{
			*(--p) = digits[(size_t)value & 0x0F];
		} while (value >>= 4);
		return length;
	}
}

size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	if (!HI(value))
	{
		return _ui32to8t((uint32_t)value, buffer);
	}
	else
	{
		TCHAR  *p;
		size_t length;

		p = buffer;
		if (HI(value) >= (uint32_t)(01000000000000000000u >> 32))
			if (HI(value) >= (uint32_t)(0100000000000000000000u >> 32))
				if (HI(value) >= (uint32_t)(01000000000000000000000u >> 32))
					p += 22;
				else
					p += 21;
			else
				if (HI(value) >= (uint32_t)(010000000000000000000u >> 32))
					p += 20;
				else
					p += 19;
		else
			if (HI(value) >= (uint32_t)(0100000000000000u >> 32))
				if (HI(value) >= (uint32_t)(010000000000000000u >> 32))
					if (HI(value) >= (uint32_t)(0100000000000000000u >> 32))
						p += 18;
					else
						p += 17;
				else
					if (HI(value) >= (uint32_t)(01000000000000000u >> 32))
						p += 16;
					else
						p += 15;
			else
				if (HI(value) >= (uint32_t)(01000000000000u >> 32))
					if (HI(value) >= (uint32_t)(010000000000000u >> 32))
						p += 14;
					else
						p += 13;
				else
					if (HI(value) >= (uint32_t)(0100000000000u >> 32))
						p += 12;
					else
						p += 11;
		*p = TEXT('\0');
		length = p - buffer;
		do
		{
			*(--p) = ((TCHAR)value & 0x07) + TEXT('0');
		} while (value >>= 3);
		return length;
	}
}
#endif
