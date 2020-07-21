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
	(((dividend) * (((1U << (precision)) + (divisor) - 1) / (divisor))) >> (precision))

#ifndef _M_IX86
size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
	if (value >= 1000000000)
		goto LENGTH10;
	else
		if (value < 10000)
			if (value < 100)
				if (value < 10)
					goto LENGTH1;
				else
					goto LENGTH2;
			else
				if (value < 1000)
					goto LENGTH3;
				else
					goto LENGTH4;
		else
			if (value < 1000000)
				if (value < 100000)
					goto LENGTH5;
				else
					goto LENGTH6;
			else
				if (value < 10000000)
					goto LENGTH7;
				else
					if (value < 100000000)
						goto LENGTH8;
					else
						goto LENGTH9;

LENGTH1:
	{
		*(tchar2_t *)&buffer[0] = T2(value + TEXT('0'));
		return 1;
	}

LENGTH2:
	{
		*(tchar2_t *)&buffer[0] = digits100T[value];
		buffer[2] = TEXT('\0');
		return 2;
	}

LENGTH3:
	{
		value = value * (((1 << (25 - 6)) + 10 - 1) / 10);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[2] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 3;
	}

LENGTH4:
	{
		value = value * (((1 << (25 - 4)) + 100 - 1) / 100);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 6)];
		buffer[4] = TEXT('\0');
		return 4;
	}

LENGTH5:
	{
		value = value * ((((1 << (25 - 4)) + 1000 - 1) / 1000) - 1)
			+ (value >> 3)
			+ (value >> 5);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&buffer[4] = T2(       (value >> (25 - 7)) + TEXT('0'));
		return 5;
	}

LENGTH6:
	{
		value = value * (((1 << (25 - 2)) + 10000 - 1) / 10000)
			- (value >> 3)
			- (value >> 6)
			+ (value >> 9);
		*(tchar2_t *)&buffer[0] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 6)];
		buffer[6] = TEXT('\0');
		return 6;
	}

LENGTH7:
	{
		value = value * (((1 << (25 - 2)) + 100000 - 1) / 100000)
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

LENGTH8:
	{
		value = value * (((1 << 25) + 1000000 - 1) / 1000000)
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

LENGTH9:
	{
		const uint64_t reciprocal_u8 = ((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000 - 1;
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

LENGTH10:
	{
		uint64_t ull;

		ull = __emulu(value, ((UINT64_C(1) << (32 + 25)) + 100000000 - 1) / 100000000) >> 25;
		*(tchar2_t *)&buffer[0] = digits100T[ ull                                >> 32];
		*(tchar2_t *)&buffer[2] = digits100T[(ull = __emulu((uint32_t)ull, 100)) >> 32];
		*(tchar2_t *)&buffer[4] = digits100T[(ull = __emulu((uint32_t)ull, 100)) >> 32];
		*(tchar2_t *)&buffer[6] = digits100T[(ull = __emulu((uint32_t)ull, 100)) >> 32];
		*(tchar2_t *)&buffer[8] = digits100T[       __emulu((uint32_t)ull, 100)  >> 32];
		buffer[10] = TEXT('\0');
		return 10;
	}
}
#else
__declspec(naked) size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define digits       digits100W
	#define tchar        word
	#define tchar2       dword
	#define inc_tchar(r) add r, 2
	#define t(r)         r##x
	#define t2(r)        e##r##x
#else
	#define digits       digits100A
	#define tchar        byte
	#define tchar2       word
	#define inc_tchar(r) inc r
	#define t(r)         r##l
	#define t2(r)        r##x
#endif

	__asm
	{
		cmp     ecx, 1000000000
		jb      L1
	//LENGTH10:
		push    esi
		push    edi
		mov     eax, 0x55E63B89	// ((UINT64_C(1) << (32 + 25)) + 100000000 - 1) / 100000000
		mov     esi, edx
		mul     ecx
		shrd    eax, edx, 25
		shr     edx, 25
		xor     ecx, ecx
		mov     edi, 100
		mov     t2(c), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		mul     edi
		mov     tchar2 ptr [esi], t2(c)
		mov     t2(c), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		mul     edi
		mov     tchar2 ptr [esi + (size TCHAR * 2)], t2(c)
		mov     t2(c), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		mul     edi
		mov     tchar2 ptr [esi + (size TCHAR * 4)], t2(c)
		mov     ecx, edx
		mul     edi
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		mov     t2(d), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		mov     tchar2 ptr [esi + (size TCHAR * 6)], t2(c)
		mov     tchar2 ptr [esi + (size TCHAR * 8)], t2(d)
		mov     tchar ptr [esi + (size TCHAR * 10)], '\0'
		mov     eax, 10
		pop     edi
		pop     esi
		ret

		align   16
	L1:
		cmp     ecx, 10000
		jae     L3
		cmp     ecx, 100
		jae     L2
		cmp     ecx, 10
		jae     LENGTH2
	//LENGTH1:
		add     ecx, '0'
#if __BYTE_ORDER == __LITTLE_ENDIAN
		xor     eax, eax
		mov     tchar2 ptr [edx], t2(c)
		inc     eax
#else
		mov     eax, 1
		mov     tchar ptr [edx], t(c)
		mov     tchar ptr [edx + size TCHAR], '\0'
#endif
		ret

		align   16
	LENGTH2:
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		mov     eax, 2
		mov     tchar2 ptr [edx], t2(c)
		mov     tchar ptr [edx + (size TCHAR * 2)], '\0'
		ret

		align   16
	L2:
		cmp     ecx, 1000
		jb      PRE3
		jmp     PRE4

		align   16
	L3:
		cmp     ecx, 1000000
		jae     L4
		cmp     ecx, 100000
		jb      PRE5
		jmp     PRE6

		align   16
	L4:
		cmp     ecx, 10000000
		jb      PRE7
		cmp     ecx, 100000000
		jb      PRE8
	//PRE9:
		push    esi
		push    9
		mov     esi, edx
		mov     edx, 0x5AFE5357	// (((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000 - 1) & 0xFFFFFFFF
		mov     eax, ecx
		lea     ecx, [ecx + ecx * 2]
		mul     edx
		lea     ecx, [ecx + edx + 2]
		mov     edx, esi
		mov     eax, ecx
		jmp     LENGTH9

		align   16
	PRE7:
		push    esi                 __asm   push    7
		mov     esi, ecx            __asm   lea     eax, [ecx + ecx * 4]
		shr     esi,  6             __asm   lea     eax, [ecx + eax * 4]
		shr     ecx,  3             __asm   lea     eax, [esi + eax * 4]
		shr     esi,  8 -  6        __asm   sub     eax, ecx
		shr     ecx, 11 -  3        __asm   sub     eax, esi
		shr     esi, 13 -  8        __asm   sub     eax, ecx
		shr     ecx, 15 - 11        __asm   sub     eax, esi
		shr     esi, 17 - 13        __asm   sub     eax, ecx
		lea     ecx, [eax + esi]    __asm   add     eax, esi
		jmp     LENGTH7

		align   16
	PRE5:
		push    esi                 __asm   mov     esi, ecx
		shl     ecx, 8              __asm   lea     eax, [esi + esi * 2]
		push    5                   __asm   lea     eax, [ecx + eax * 2]
		shr     ecx, 3 + 8          __asm   lea     eax, [esi + eax * 8]
		shr     esi, 5              __asm   add     eax, ecx
		lea     ecx, [eax + esi]    __asm   add     eax, esi
		jmp     LENGTH5

		align   16
	PRE3:
		imul    ecx, ((1 << (25 - 6)) + 10 - 1) / 10
		push    esi
		push    3
		mov     eax, ecx
		jmp     LENGTH3

		align   16
	LENGTH9:
		shr     ecx, 25
		and     eax, (1 << 25) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	LENGTH7:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	LENGTH5:
		shr     ecx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	LENGTH3:
		shr     ecx, 25 - 6
		and     eax, (1 << (25 - 6)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		add     edx, size TCHAR * 2
		mov     tchar2 ptr [edx - (size TCHAR * 2)], t2(c)
		lea     ecx, [eax + eax * 4]
		shr     ecx, 25 - 7
		pop     eax
		add     ecx, '0'
		pop     esi
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], t2(c)
#else
		mov     tchar ptr [edx], t(c)
		mov     tchar ptr [edx + size TCHAR], '\0'
#endif
		ret

		align   16
	PRE8:
		push    esi                 __asm   mov     esi, ecx
		shl     ecx,  5             __asm   lea     eax, [esi + esi]
		shr     esi,  1             __asm   add     eax, ecx
		shr     ecx,  4 +  5        __asm   sub     eax, esi
		shr     esi,  7 -  1        __asm   add     eax, ecx
		shr     ecx, 12 -  4        __asm   sub     eax, esi
		shr     esi, 17 -  7        __asm   sub     eax, ecx
		shr     ecx, 18 - 12        __asm   sub     eax, esi
		shr     esi, 22 - 17        __asm   sub     eax, ecx
		push    8                   __asm   add     eax, esi
		mov     ecx, eax
		jmp     LENGTH8

		align   16
	PRE6:
		push    esi                 __asm   mov     esi, ecx
		shl     ecx, 3              __asm   lea     eax, [esi + esi * 2]
		shl     eax, 8              __asm   lea     ecx, [ecx + ecx * 8]
		add     eax, ecx            __asm   sub     ecx, esi
		shr     ecx, 9              __asm   sub     eax, esi
		push    6                   __asm   sub     eax, ecx
		mov     ecx, eax
		jmp     LENGTH6

		align   16
	PRE4:
		imul    ecx, ((1 << (25 - 4)) + 100 - 1) / 100
		push    esi
		push    4
		mov     eax, ecx
		jmp     LENGTH4

		align   16
	LENGTH8:
		shr     ecx, 25
		and     eax, (1 << 25) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	LENGTH6:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	LENGTH4:
		shr     ecx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     ecx, [eax + eax * 4]
		shr     ecx, 25 - 6
		pop     eax
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		pop     esi
		mov     tchar2 ptr [edx + (size TCHAR * 2)], t2(c)
		mov     tchar ptr [edx + (size TCHAR * 4)], '\0'
		ret
	}

	#undef digits
	#undef tchar
	#undef tchar2
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
		bsr     eax, ecx
		lea     eax, [eax + 1]
		jz      L3
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		xor     eax, eax
		mov     tchar ptr [edx], '\0'
		jmp     L2

		align   16
	L1:
		adc     eax, '0'
		dec_tchar(edx)
		mov     tchar ptr [edx], t(a)
		xor     eax, eax
	L2:
		shr     ecx, 1
		jnz     L1

		mov     tchar ptr [edx - size TCHAR], '1'
		pop     eax
		ret

		align   16
	L3:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		ret
	}

	#undef tchar
	#undef tchar2
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
		bsr     eax, ecx
		lea     eax, [eax + 2]
		jz      L2
		shr     eax, 1
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		mov     tchar ptr [edx], '\0'

		align   16
	L1:
		mov     eax, ecx
		dec_tchar(edx)
		shr     ecx, 2
		and     eax, 3
		add     eax, '0'
		test    ecx, ecx
		mov     tchar ptr [edx], t(a)
		jnz     L1

		pop     eax
		ret

		align   16
	L2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		ret
	}

	#undef tchar
	#undef tchar2
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
		bsr     eax, ecx
		jz      L2
		push    esi
		lea     esi, [eax + eax * 2]
		lea     eax, [esi + eax * 8 + (1 << 5)]
		pop     esi
		shr     eax, 5
		push    eax
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		mov     tchar ptr [edx], '\0'

		align   16
	L1:
		mov     eax, ecx
		dec_tchar(edx)
		shr     ecx, 3
		and     eax, 7
		add     eax, '0'
		test    ecx, ecx
		mov     tchar ptr [edx], t(a)
		jnz     L1

		pop     eax
		ret

		align   16
	L2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		ret
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	size_t      length;
	const TCHAR *digits;

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
		push    esi
		jz      L2
		shr     eax, 2
		mov     esi, dword ptr [upper + 4]
		inc     eax
		cmp     esi, 1
		sbb     esi, esi
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		and     esi, ptrdiff_digitsSmall
		push    eax
		add     esi, offset digitsLarge
		mov     tchar ptr [edx], '\0'

		align   16
	L1:
		mov     eax, ecx
		dec_tchar(edx)
		shr     ecx, 4
		and     eax, 15
		mov     t(a), tchar ptr [esi + eax * size TCHAR]
		test    ecx, ecx
		mov     tchar ptr [edx], t(a)
		jnz     L1

		pop     eax
		pop     esi
		ret     4

		align   16
	L2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     esi
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
size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper)
{
	size_t      length;
	const TCHAR *digits;

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
		jz      L2
		push    esi
		lea     esi, [eax + eax * 4]
		lea     eax, [esi + eax * 8]
		mov     esi, dword ptr [upper + 4]
		shr     eax, 6
		cmp     esi, 1
		sbb     esi, esi
#ifdef _UNICODE
		lea     edx, [edx + eax * 2]
#else
		add     edx, eax
#endif
		and     esi, ptrdiff_digitsSmall
		push    eax
		add     esi, offset digitsLarge
		mov     tchar ptr [edx], '\0'

		align   16
	L1:
		mov     eax, ecx
		dec_tchar(edx)
		shr     ecx, 5
		and     eax, 31
		mov     t(a), tchar ptr [esi + eax * size TCHAR]
		test    ecx, ecx
		mov     tchar ptr [edx], t(a)
		jnz     L1

		pop     eax
		pop     esi
		ret     4

		align   16
	L2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [edx], '0'
#else
		mov     tchar2 ptr [edx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
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
	size_t      length;
	const TCHAR *digits;
	TCHAR       *p1, *p2;

	__assume(radix >= 2 && radix <= 36);
	digits = upper ? digitsLarge : digitsSmall;
	p1 = buffer;
	do
	{
		unsigned int remainder;

		remainder = value % radix;
		value /= radix;
		*(p1++) = digits[remainder];
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
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
#else
	#define t(r)         r##l
	#define tchar        byte
	#define inc_tchar(r) inc r
	#define dec_tchar(r) dec r
#endif

	__asm
	{
		push    esi
		push    edi

		#define value  eax
		#define buffer (esp + 8 + 4)
		#define upper  (esp + 8 + 4)
		#define radix  ecx
		#define digits edi
		#define p1     esi
		#define p2     edi

		mov     value, ecx
		mov     digits, dword ptr [upper]
		mov     radix, dword ptr [esp + 8 + 8]
		cmp     digits, 1
		sbb     digits, digits
		mov     p1, edx
		and     digits, ptrdiff_digitsSmall
		mov     dword ptr [buffer], edx
		add     digits, offset digitsLarge
		dec_tchar(p1)

		align   16
	L1:
		xor     edx, edx
		inc_tchar(p1)
		div     radix
		mov     t(d), tchar ptr [digits + edx * size TCHAR]
		test    eax, eax
		mov     tchar ptr [p1], t(d)
		jnz     L1

		lea     eax, [p1 + size TCHAR]
		mov     p2, dword ptr [buffer]
		mov     tchar ptr [eax], '\0'
		sub     eax, p2
#ifdef _UNICODE
		shr     eax, 1
#endif
		jmp     L3

		align   16
	L2:
		mov     t(c), tchar ptr [p1]
		mov     t(d), tchar ptr [p2]
		mov     tchar ptr [p1], t(d)
		mov     tchar ptr [p2], t(c)
		dec_tchar(p1)
		inc_tchar(p2)
	L3:
		cmp     p1, p2
		ja      L2

		pop     edi
		pop     esi
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
	#undef inc_tchar
	#undef dec_tchar
}
#endif
