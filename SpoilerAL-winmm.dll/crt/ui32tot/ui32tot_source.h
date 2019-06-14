#include <windows.h>
#include <stdint.h>
#include "intrinsic.h"
#include "digitstbl.h"
#include "ui32tot.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef _UNICODE
typedef wchar_t tuchar_t;
typedef uint32_t tchar2_t;
#define digits100T ((tchar2_t *)digits100W)
#else
typedef unsigned char tuchar_t;
typedef uint16_t tchar2_t;
#define digits100T ((tchar2_t *)digits100A)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define T2(c) (tchar2_t)(tuchar_t)(c)
#else
#define T2(c) ((tchar2_t)(tuchar_t)(c) << (8 * sizeof(THCAR)))
#endif

#define RECIPROCAL_DIVISION(dividend, divisor, precision) \
	(((dividend) * (((1 << (precision)) + (divisor) - 1) / (divisor))) >> (precision))

#ifndef _M_IX86
size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
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
	{
		const uint64_t reciprocal_u8 = ((uint64_t)1 << (32 + 25)) / 10000000;
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
		*(tchar2_t *)&buffer[1] = digits100T[value >>  25     ]; value = (value & ((1 <<  25     ) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[3] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[5] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[7] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[9] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 10;
	}

LENGTH9:
	{
		const uint64_t reciprocal_u8 = ((uint64_t)1 << (32 + 25)) / 10000000;
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&buffer[0] = digits100T[value >>  25     ]; value = (value & ((1 <<  25     ) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[6] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[8] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 9;
	}

LENGTH8:
	{
		value = value * ((1 << 25) / 1000000 + 1)
			- (value >> 1)
			+ (value >> 4)
			- (value >> 7)
			- (value >> 12)
			- (value >> 17)
			- (value >> 18)
			+ (value >> 22);
		*(tchar2_t *)&buffer[0] = digits100T[value >>  25     ]; value = (value & ((1 <<  25     ) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[6] = digits100T[value >> (25 - 6)];
		              buffer[8] = TEXT('\0');
		return 8;
	}

LENGTH7:
	{
		value = value * ((1 << (25 - 2)) / 100000 + 1)
			- (value >> 3)
			+ (value >> 6)
			- (value >> 8)
			- (value >> 11)
			- (value >> 13)
			- (value >> 15)
			+ (value >> 17);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[6] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 7;
	}

LENGTH6:
	{
		value = value * ((1 << (25 - 2)) / 10000 + 1)
			- (value >> 3)
			- (value >> 6)
			+ (value >> 9);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 6)];
		              buffer[6] = TEXT('\0');
		return 6;
	}

LENGTH5:
	{
		value = value * ((1 << (25 - 4)) / 1000)
			+ (value >> 3)
			+ (value >> 5);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[4] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 5;
	}

LENGTH4:
	{
		value = value * ((1 << (25 - 4)) / 100 + 1);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 6)];
		              buffer[4] = TEXT('\0');
		return 4;
	}

LENGTH3:
	{
		value = value * ((1 << (25 - 6)) / 10 + 1);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[2] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 3;
	}

LENGTH2:
	{
		*(tchar2_t *)&buffer[0] = digits100T[value];
		              buffer[2] = TEXT('\0');
		return 2;
	}

LENGTH1:
	{
		*(tchar2_t *)&buffer[0] = T2(value + TEXT('0'));
		return 1;
	}
}
#else
__declspec(naked) size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define digits        digits100W
	#define tchar         word
	#define tchar2        dword
	#define sizeof_tchar2 4
	#define inc_tchar(r)  add r, 2
	#define t(r)          r##x
	#define t2(r)         e##r##x
#else
	#define digits        digits100A
	#define tchar         byte
	#define tchar2        word
	#define sizeof_tchar2 2
	#define inc_tchar(r)  inc r
	#define t(r)          r##l
	#define t2(r)         r##x
#endif

	__asm
	{
		push    ebx
		mov     ebx, ecx

		cmp     ecx, 1000000
		jb      L2
		cmp     ecx, 100000000
		jb      L1
		cmp     ecx, 1000000000
		jae     LENGTH10
		push    9
		jmp     LENGTH9

		align   16
	L1:
		cmp     ecx, 10000000
		jae     LENGTH8
		push    7                   __asm   lea     eax, [ecx + ecx * 4]
		shr     ebx,  6             __asm   lea     eax, [ecx + eax * 4]
		shr     ecx,  3             __asm   lea     eax, [ebx + eax * 4]
		shr     ebx,  8 -  6        __asm   sub     eax, ecx
		shr     ecx, 11 -  3        __asm   sub     eax, ebx
		shr     ebx, 13 -  8        __asm   sub     eax, ecx
		shr     ecx, 15 - 11        __asm   sub     eax, ebx
		shr     ebx, 17 - 13        __asm   sub     eax, ecx
		lea     ecx, [eax + ebx]    __asm   add     eax, ebx
		jmp     LENGTH7

		align   16
	L2:
		cmp     ecx, 10000
		jb      L4
		cmp     ecx, 100000
		jb      L3
		shl     ecx, 3              __asm   lea     eax, [ebx + ebx * 2]
		shl     eax, 8              __asm   lea     ecx, [ecx + ecx * 8]
		add     eax, ecx            __asm   sub     ecx, ebx
		shr     ecx, 9              __asm   sub     eax, ebx
		push    6                   __asm   sub     eax, ecx
		mov     ecx, eax
		jmp     LENGTH6

		align   16
	L3:
		shl     ecx, 8              __asm   lea     eax, [ebx + ebx * 2]
		push    5                   __asm   lea     eax, [ecx + eax * 2]
		shr     ecx, 3 + 8          __asm   lea     eax, [ebx + eax * 8]
		shr     ebx, 5              __asm   add     eax, ecx
		lea     ecx, [eax + ebx]    __asm   add     eax, ebx
		jmp     LENGTH5

		align   16
	L4:
		cmp     ecx, 100
		jb      L6
		cmp     ecx, 1000
		jb      L5
		imul    ecx, (1 << (25 - 4)) / 100 + 1
		push    4
		mov     eax, ecx
		jmp     LENGTH4

		align   16
	L5:
		imul    ecx, (1 << (25 - 6)) / 10 + 1
		push    3
		mov     eax, ecx
		jmp     LENGTH3

		align   16
	L6:
		cmp     ecx, 10
		jb      L7
		mov     eax, 2
		jmp     LENGTH2

		align   16
	L7:
		mov     eax, 1
		jmp     LENGTH1

		align   16
	LENGTH10:
		add     ecx, -3000000000
		jnc     L9
		sub     ecx, 1000000000
		jc      L8
		mov     tchar ptr [edx], '4'
		jmp     L12

		align   16
	L8:
		mov     tchar ptr [edx], '3'
		jmp     L11

		align   16
	L9:
		add     ecx, 1000000000
		jnc     L10
		mov     tchar ptr [edx], '2'
		jmp     L12

		align   16
	L10:
		mov     tchar ptr [edx], '1'
	L11:
		add     ecx, 1000000000
	L12:
		push    10
		inc_tchar(edx)
	LENGTH9:
		mov     eax, ecx
		lea     ecx, [ecx + ecx * 2]
		mov     ebx, edx
		mov     edx, 0x5AFE5357	// (0x200000000000000 / 10000000) & 0xFFFFFFFF
		mul     edx
		lea     ecx, [ecx + edx + 2]
		mov     edx, ebx
		mov     eax, ecx
		shr     ecx, 25
		and     eax, (1 << 25) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     eax, [eax + eax * 4]
		add     edx, sizeof_tchar2
		mov     ecx, eax
	LENGTH7:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     eax, [eax + eax * 4]
		add     edx, sizeof_tchar2
		mov     ecx, eax
	LENGTH5:
		shr     ecx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     eax, [eax + eax * 4]
		add     edx, sizeof_tchar2
		mov     ecx, eax
	LENGTH3:
		shr     ecx, 25 - 6
		and     eax, (1 << (25 - 6)) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     ecx, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		add     edx, sizeof_tchar2
		shr     ecx, 25 - 7
		pop     eax
	LENGTH1:
		add     ecx, '0'
		pop     ebx
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], t2(c)
#else
		mov     tchar ptr [edx], t(c)
		mov     tchar ptr [edx + sizeof_tchar], '\0'
#endif
		ret

		align   16
	LENGTH8:
		shl     ecx,  5             __asm   lea     eax, [ebx + ebx]
		shr     ebx,  1             __asm   add     eax, ecx
		shr     ecx,  4 +  5        __asm   sub     eax, ebx
		shr     ebx,  7 -  1        __asm   add     eax, ecx
		shr     ecx, 12 -  4        __asm   sub     eax, ebx
		shr     ebx, 17 -  7        __asm   sub     eax, ecx
		shr     ecx, 18 - 12        __asm   sub     eax, ebx
		shr     ebx, 22 - 17        __asm   sub     eax, ecx
		push    8                   __asm   add     eax, ebx
		mov     ecx, eax
		shr     ecx, 25
		and     eax, (1 << 25) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     eax, [eax + eax * 4]
		add     edx, sizeof_tchar2
		mov     ecx, eax
	LENGTH6:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     eax, [eax + eax * 4]
		add     edx, sizeof_tchar2
		mov     ecx, eax
	LENGTH4:
		shr     ecx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(b), tchar2 ptr [digits + ecx * sizeof_tchar2]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(b)
		lea     ecx, [eax + eax * 4]
		shr     ecx, 25 - 6
		add     edx, sizeof_tchar2
		pop     eax
	LENGTH2:
		mov     t2(c), tchar2 ptr [digits + ecx * sizeof_tchar2]
		pop     ebx
		mov     tchar2 ptr [edx], t2(c)
		mov     tchar ptr [edx + sizeof_tchar2], '\0'
		ret
	}

	#undef digits
	#undef tchar
	#undef tchar2
	#undef sizeof_tchar2
	#undef inc_tchar
	#undef t
	#undef t2
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to2t(uint32_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = (unsigned long)length + 1;
	*(buffer += length) = TEXT('\0');
	do
		*(--buffer) = (value & 0x01) + TEXT('0');
	while (value >>= 1);
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui32to2t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define sizeof_tchar 2
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define sizeof_tchar 1
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		bsr     eax, ecx
		lea     eax, [eax + 1]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		ret

		align   16
	L1:
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		mov     tchar ptr [edx], '\0'

		align   16
	L2:
		mov     eax, ecx
		dec_tchar(edx)
		and     eax, 1
		shr     ecx, 1
		lea     eax, [eax + '0']
		mov     tchar ptr [edx], t(a)
		jnz     L2

		pop     eax
		ret
	}

	#undef tchar
	#undef tchar2
	#undef sizeof_tchar
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to4t(uint32_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = ((unsigned long)length / 2) + 1;
	*(buffer += length) = TEXT('\0');
	do
		*(--buffer) = (value & 0x03) + TEXT('0');
	while (value >>= 2);
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui32to4t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define sizeof_tchar 2
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define sizeof_tchar 1
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		bsr     eax, ecx
		lea     eax, [eax + 2]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		ret

		align   16
	L1:
		shr     eax, 1
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		mov     tchar ptr [edx], '\0'

		align   16
	L2:
		mov     eax, ecx
		dec_tchar(edx)
		and     eax, 3
		shr     ecx, 2
		lea     eax, [eax + '0']
		mov     tchar ptr [edx], t(a)
		jnz     L2

		pop     eax
		ret
	}

	#undef tchar
	#undef tchar2
	#undef sizeof_tchar
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume((unsigned long)length < 32);
#if 0
	length = ((unsigned long)length / 3) + 1;
#else
	length = RECIPROCAL_DIVISION((unsigned long)length, 3, 5) + 1;
#endif
	*(buffer += length) = TEXT('\0');
	do
		*(--buffer) = (value & 0x07) + TEXT('0');
	while (value >>= 3);
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define sizeof_tchar 2
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define sizeof_tchar 1
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		bsr     eax, ecx
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		ret

		align   16
	L1:
		push    ebx
		lea     ebx, [eax + eax * 2]
		lea     eax, [ebx + eax * 8 + (1 << 5)]
		pop     ebx
		shr     eax, 5
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		mov     tchar ptr [edx], '\0'

		align   16
	L2:
		mov     eax, ecx
		dec_tchar(edx)
		and     eax, 7
		shr     ecx, 3
		lea     eax, [eax + '0']
		mov     tchar ptr [edx], t(a)
		jnz     L2

		pop     eax
		ret
	}

	#undef tchar
	#undef tchar2
	#undef sizeof_tchar
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	size_t              length;
	const unsigned char *digits;

	if (!_BitScanReverse((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = ((unsigned long)length / 4) + 1;
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
	do
		*(--buffer) = digits[value & 0x0F];
	while (value >>= 4);
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define sizeof_tchar 2
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define sizeof_tchar 1
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		#define upper (esp + 4)

		bsr     eax, ecx
		lea     eax, [eax + 4]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L3

		align   16
	L1:
		shr     eax, 2
		push    ebx
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		push    eax
		mov     eax, dword ptr [upper + 8]
		mov     tchar ptr [edx], '\0'
		test    eax, eax
		mov     ebx, offset digitsLarge
		mov     eax, offset digitsSmall
		cmovz   ebx, eax

		align   16
	L2:
		mov     eax, ecx
		dec_tchar(edx)
		and     eax, 15
		shr     ecx, 4
		mov     al, byte ptr [eax + ebx]
		mov     tchar ptr [edx], t(a)
		jnz     L2

		pop     eax
		pop     ebx
	L3:
		ret     4

		#undef upper
	}

	#undef tchar
	#undef tchar2
	#undef sizeof_tchar
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	size_t              length;
	const unsigned char *digits;

	if (!_BitScanReverse((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume((unsigned long)length < 32);
#if 0
	length = ((unsigned long)length / 5) + 1;
#else
	length = RECIPROCAL_DIVISION((unsigned long)length, 5, 6) + 1;
#endif
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
	do
		*(--buffer) = digits[value & 0x1F];
	while (value >>= 5);
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		#define upper (esp + 4)

		bsr     eax, ecx
		lea     eax, [eax + 5]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L3

		align   16
	L1:
		push    ebx
		lea     ebx, [eax + eax * 4]
		lea     eax, [ebx + eax * 8]
		mov     ebx, dword ptr [upper + 4]
		shr     eax, 6
		test    ebx, ebx
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		lea     edx, [edx + eax]
#endif
		mov     tchar ptr [edx], '\0'
		mov     ebx, offset digitsLarge
		mov     eax, offset digitsSmall
		cmovz   ebx, eax

		align   16
	L2:
		mov     eax, ecx
		dec_tchar(edx)
		and     eax, 31
		shr     ecx, 5
		mov     al, byte ptr [eax + ebx]
		mov     tchar ptr [edx], t(a)
		jnz     L2

		pop     eax
		pop     ebx
	L3:
		ret     4

		#undef upper
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall internal_ui32tot(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	size_t              length;
	const unsigned char *digits;
	TCHAR               *p1, *p2;

	__assume(radix >= 2 && radix <= 36);
	digits = upper ? digitsLarge : digitsSmall;
	p1 = buffer;
	do
	{
		unsigned int remainder;

		remainder = value % radix;
		value /= radix;
		*(p1++) = (TCHAR)digits[remainder];
	} while (value);
	length = p1 - (p2 = buffer);
	*(p1--) = TEXT('\0');
	while (p1 > p2)
	{
		TCHAR c1, c2;

		c1 = *p1;
		c2 = *p2;
		*(p1--) = c2;
		*(p2++) = c1;
	}
	return length;
}
#else
__declspec(naked) size_t __fastcall internal_ui32tot(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
#ifdef _UNICODE
	#define t(r)         r##x
	#define tchar        word
	#define sizeof_tchar 2
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
#else
	#define t(r)         r##l
	#define tchar        byte
	#define sizeof_tchar 1
	#define inc_tchar(r) inc r
	#define dec_tchar(r) dec r
#endif

	__asm
	{
		push    ebx
		push    esi
		mov     eax, ecx
		mov     ecx, edx
		mov     edx, dword ptr [esp + 8 + 4]
		mov     ebx, dword ptr [esp + 8 + 8]

		#define value  eax
		#define buffer (esp + 8 + 4)
		#define upper  edx
		#define radix  ebx
		#define digits esi
		#define p1     ecx
		#define p2     esi

		mov     dword ptr [buffer], p1
		dec_tchar(p1)
		test    upper, upper
		mov     digits, offset digitsLarge
		mov     edx, offset digitsSmall
		cmovz   digits, edx

		align   16
	L1:
		xor     edx, edx
		inc_tchar(p1)
		div     radix
		mov     dl, byte ptr [digits + edx]
		test    eax, eax
		mov     tchar ptr [p1], t(d)
		jnz     L1

		lea     eax, [p1 + sizeof_tchar]
		mov     p2, dword ptr [buffer]
		mov     tchar ptr [eax], '\0'
		sub     eax, p2
#ifdef _UNICODE
		shr     eax, 1
#endif
		jmp     L3

		align   16
	L2:
		mov     t(b), tchar ptr [p1]
		mov     t(d), tchar ptr [p2]
		mov     tchar ptr [p1], t(d)
		mov     tchar ptr [p2], t(b)
		dec_tchar(p1)
		inc_tchar(p2)
	L3:
		cmp     p1, p2
		ja      L2

		pop     esi
		pop     ebx
		ret     8

		#undef value
		#undef buffer
		#undef upper
		#undef radix
		#undef digits
		#undef p1
		#undef p2
	}

	#undef t
	#undef tchar
	#undef sizeof_tchar
	#undef inc_tchar
	#undef dec_tchar
}
#endif
