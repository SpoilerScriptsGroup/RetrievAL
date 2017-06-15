#if defined(_ui32tont)
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
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << (sizeof(TCHAR) * 8))
#endif

#define _ui32to10t _ui32tont(10)
#define _ui32to16t _ui32tont(16)
#define _ui32to8t  _ui32tont(8)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
	if (value >= 1000000u)
		if (value >= 100000000u)
			if (value >= 1000000000u)
				goto LENGTH10;
			else
				goto LENGTH9;
		else
			if (value >= 10000000u)
				goto LENGTH8;
			else
				goto LENGTH7;
	else
		if (value >= 10000u)
			if (value >= 100000u)
				goto LENGTH6;
			else
				goto LENGTH5;
		else
			if (value >= 100u)
				if (value >= 1000u)
					goto LENGTH4;
				else
					goto LENGTH3;
			else
				if (value >= 10u)
					goto LENGTH2;
				else
					goto LENGTH1;

LENGTH10:
	{
		const uint64_t reciprocal_u8 = ((1ULL << (32 + 25)) / 10000000u);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		do	// do { ... } while (0);
		{
			if ((int32_t)(value -= 3000000000) >= 0)
			{
				if ((int32_t)(value -= 1000000000) >= 0)
				{
					buffer[0] = TEXT('4');
					break;
				}
				buffer[0] = TEXT('3');
			}
			else
			{
				if ((int32_t)(value += 1000000000) >= 0)
				{
					buffer[0] = TEXT('2');
					break;
				}
				buffer[0] = TEXT('1');
			}
			value += 1000000000;
		} while (0);
		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[1] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[3] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[5] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[7] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[9] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 10;
	}

LENGTH9:
	{
		const uint64_t reciprocal_u8 = (((uint64_t)1 << (32 + 25)) / 10000000);
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[6] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[8] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 9;
	}

LENGTH8:
	{
		value = value * ((1 << 25) / 1000000)
			+ (value >> 1)
			+ (value >> 4)
			- (value >> 7)
			- (value >> 12)
			- (value >> 17)
			- (value >> 18)
			+ (value >> 21)
			- (value >> 23);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[6] = digitsDec100T[value >> 25];
		              buffer[8] = TEXT('\0');
		return 8;
	}

LENGTH7:
	{
		value = value * ((1 << 25) / 100000)
			+ (value >> 1)
			+ (value >> 5)
			+ (value >> 6)
			- (value >> 9)
			- (value >> 11)
			- (value >> 13)
			+ (value >> 15);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[6] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 7;
	}

LENGTH6:
	{
		value = value * ((1 << 25) / 10000)
			+ (value >> 1)
			- (value >> 4)
			+ (value >> 7);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[4] = digitsDec100T[value >> 25];
		              buffer[6] = TEXT('\0');
		return 6;
	}

LENGTH5:
	{
		value = value * ((1 << 25) / 1000 + 1) - (value >> 2);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[4] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 5;
	}

LENGTH4:
	{
		value = value * ((1 << 25) / 100 + 1);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&buffer[2] = digitsDec100T[value >> 25];
		              buffer[4] = TEXT('\0');
		return 4;
	}

LENGTH3:
	{
		value = value * ((1 << 25) / 10 + 1);
		*(tchar2_t *)&buffer[0] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&buffer[2] = TO_TCHAR2((value >> 25) + TEXT('0'));
		return 3;
	}

LENGTH2:
	{
		*(tchar2_t *)&buffer[0] = digitsDec100T[value];
		              buffer[2] = TEXT('\0');
		return 2;
	}

LENGTH1:
	{
		*(tchar2_t *)&buffer[0] = TO_TCHAR2(value + TEXT('0'));
		return 1;
	}
}

size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	TCHAR      *p;
	size_t     length;
	const char *digits;

	p = buffer;
	if (value >= 0x10000u)
		if (value >= 0x1000000u)
			if (value >= 0x10000000u)
				p += 8;
			else
				p += 7;
		else
			if (value >= 0x100000u)
				p += 6;
			else
				p += 5;
	else
		if (value >= 0x100u)
			if (value >= 0x1000u)
				p += 4;
			else
				p += 3;
		else
			if (value >= 0x10u)
				p += 2;
			else
				p += 1;
	*p = TEXT('\0');
	length = p - buffer;
	digits = upper ? digitsHexLarge : digitsHexSmall;
	do
	{
		*(--p) = digits[(size_t)value & 0x0F];
	} while (value >>= 4);
	return length;
}

size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer)
{
	TCHAR  *p;
	size_t length;

	p = buffer;
	if (value >= 010000000u)
		if (value >= 01000000000u)
			if (value >= 010000000000u)
				p += 11;
			else
				p += 10;
		else
			if (value >= 0100000000u)
				p += 9;
			else
				p += 8;
	else
		if (value >= 010000u)
			if (value >= 01000000u)
				p += 7;
			else
				if (value >= 0100000u)
					p += 6;
				else
					p += 5;
		else
			if (value >= 0100u)
				if (value >= 01000u)
					p += 4;
				else
					p += 3;
			else
				if (value >= 010u)
					p += 2;
				else
					p += 1;
	*p = TEXT('\0');
	length = p - buffer;
	do
	{
		*(--p) = ((TCHAR)value & 0x07) + TEXT('0');
	} while (value >>= 3);
	return length;
}
#endif
