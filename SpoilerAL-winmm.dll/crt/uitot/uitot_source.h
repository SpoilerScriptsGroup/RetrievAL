#include <windows.h>
#include <stdint.h>
#include "intrinsic.h"
#include "digitstbl.h"
#include "uitot.h"

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
#define LO(x) ((uint32_t *)&x)[0]
#define HI(x) ((uint32_t *)&x)[1]
#else
#define T2(c) ((tchar2_t)(tuchar_t)(c) << (8 * sizeof(THCAR)))
#define LO(x) ((uint32_t *)&x)[1]
#define HI(x) ((uint32_t *)&x)[0]
#endif

#define RECIPROCAL_DIVISION(dividend, divisor, precision) \
	(((dividend) * (((1U << (precision)) + (divisor) - 1) / (divisor))) >> (precision))

#ifndef _M_IX86
size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
	if (value >= 100000000)
		if (value < 1000000000)
			goto LENGTH9;
		else
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
					goto LENGTH8;

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
		value = value * (((1 << (25 - 4)) + 1000 - 1) / 1000 - 1)
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
			+ 22;
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
			+ 10;
		*(tchar2_t *)&buffer[0] = digits100T[value >>  25     ]; value = (value & ((1 <<  25     ) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[2] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[6] = digits100T[value >> (25 - 6)];
		buffer[8] = TEXT('\0');
		return 8;
	}

LENGTH9:
	{
		const uint64_t reciprocal_u8 = ((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000;
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 1;
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
		*(tchar2_t *)&buffer[0] = digits100T[ull   >>  32     ]; ull = __emulu((uint32_t)ull, 100);
		*(tchar2_t *)&buffer[2] = digits100T[ull   >>  32     ]; value = ((uint32_t)ull >> (32 - 25)   ) * (100 >> 2);
		*(tchar2_t *)&buffer[4] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[6] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&buffer[8] = digits100T[value >> (25 - 6)];
		buffer[10] = TEXT('\0');
		return 10;
	}
}
#else
__declspec(naked) size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define digits digits100W
	#define tchar  word
	#define tchar2 dword
	#define t(r)   r##x
	#define t2(r)  e##r##x
#else
	#define digits digits100A
	#define tchar  byte
	#define tchar2 word
	#define t(r)   r##l
	#define t2(r)  r##x
#endif

	__asm
	{
		cmp     ecx, 100000000
		jb      L1
		cmp     ecx, 1000000000
		jae     LENGTH10
	//LENGTH9:
		push    esi
		push    9
		mov     eax, 0x5AFE5358	// (((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000) & UINT32_MAX
		mov     esi, edx
		mul     ecx
		lea     ecx, [ecx + ecx * 2]
		add     edx, 1
		add     ecx, edx
		mov     edx, esi
		mov     eax, ecx
		jmp     POST9

		align   16
	LENGTH10:
		push    esi
		push    10
		mov     eax, 0x55E63B89	// ((UINT64_C(1) << (32 + 25)) + 100000000 - 1) / 100000000
		mov     esi, edx
		mul     ecx
		shrd    eax, edx, 25
		shr     edx, 25
		xor     ecx, ecx
		mov     t2(c), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		mov     edx, 100
		mul     edx
		shr     eax, 32 - 25
		mov     tchar2 ptr [esi], t2(c)
		mov     ecx, edx
		lea     edx, [esi + (size TCHAR * 2)]
		jmp     POST10

		align   16
	L1:
		cmp     ecx, 10000
		jae     L2
		cmp     ecx, 100
		jae     L3
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
		cmp     ecx, 1000000
		jae     L4
		cmp     ecx, 100000
		jb      LENGTH5
		jmp     LENGTH6

		align   16
	L3:
		cmp     ecx, 1000
		jb      LENGTH3
		jmp     LENGTH4

		align   16
	L4:
		cmp     ecx, 10000000
		jae     LENGTH8
	//LENGTH7:
		push    esi                 __asm   lea     esi, [ecx + 1856]
		push    7                   __asm   lea     eax, [ecx + ecx * 4]
		shr     esi,  6             __asm   lea     eax, [ecx + eax * 4]
		shr     ecx,  3             __asm   lea     eax, [esi + eax * 4]
		shr     esi,  8 -  6        __asm   sub     eax, ecx
		shr     ecx, 11 -  3        __asm   sub     eax, esi
		shr     esi, 13 -  8        __asm   sub     eax, ecx
		shr     ecx, 15 - 11        __asm   sub     eax, esi
		sub     eax, ecx
		mov     ecx, eax
		jmp     POST7

		align   16
	LENGTH5:
		push    esi                 __asm   mov     esi, ecx
		shl     ecx, 8              __asm   lea     eax, [esi + esi * 2]
		push    5                   __asm   lea     eax, [ecx + eax * 2]
		shr     ecx, 3 + 8          __asm   lea     eax, [esi + eax * 8]
		shr     esi, 5              __asm   add     eax, ecx
		lea     ecx, [eax + esi]    __asm   add     eax, esi
		jmp     POST5

		align   16
	LENGTH3:
		imul    ecx, ((1 << (25 - 6)) + 10 - 1) / 10
		push    esi
		push    3
		mov     eax, ecx
		jmp     POST3

		align   16
	POST9:
		shr     ecx, 25
		and     eax, (1 << 25) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	POST7:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	POST5:
		shr     ecx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	POST3:
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
	LENGTH8:
		push    esi                 __asm   lea     esi, [ecx + 7]
		shl     ecx,  5             __asm   lea     eax, [esi + esi]
		shr     esi,  1             __asm   add     eax, ecx
		shr     ecx,  4 +  5        __asm   sub     eax, esi
		shr     esi,  7 -  1        __asm   add     eax, ecx
		shr     ecx, 12 -  4        __asm   sub     eax, esi
		shr     esi, 17 -  7        __asm   sub     eax, ecx
		shr     ecx, 18 - 12        __asm   sub     eax, esi
		push    8                   __asm   sub     eax, ecx
		mov     ecx, eax
		jmp     POST8

		align   16
	LENGTH6:
		push    esi                 __asm   mov     esi, ecx
		shl     ecx, 3              __asm   lea     eax, [esi + esi * 2]
		shl     eax, 8              __asm   lea     ecx, [ecx + ecx * 8]
		add     eax, ecx            __asm   sub     ecx, esi
		shr     ecx, 9              __asm   sub     eax, esi
		push    6                   __asm   sub     eax, ecx
		mov     ecx, eax
		jmp     POST6

		align   16
	LENGTH4:
		imul    ecx, ((1 << (25 - 4)) + 100 - 1) / 100
		push    esi
		push    4
		mov     eax, ecx
		jmp     POST4

		align   16
	POST8:
		shr     ecx, 25
		and     eax, (1 << 25) - 1
	POST10:
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	POST6:
		shr     ecx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(c), tchar2 ptr [digits + ecx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [edx], t2(c)
		lea     eax, [eax + eax * 4]
		add     edx, size TCHAR * 2
		mov     ecx, eax
	POST4:
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
	#undef t
	#undef t2
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to10t((uint32_t)value, buffer);
	}
	else
	{
		const uint64_t reciprocal_u8 = ((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000 - 1;
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

		#define PUTCHAR(power)                                      \
		do                                                          \
		{                                                           \
		    if (!_sub_u64(value, 5 * power, &value))                \
		    {                                                       \
		        if (!_sub_u64(value, 2 * power, &value))            \
		        {                                                   \
		            if (!_sub_u64(value, 1 * power, &value))        \
		            {                                               \
		                if (!_sub_u64(value, 1 * power, &value))    \
		                {                                           \
		                    *(p++) = TEXT('9');                     \
		                    break;                                  \
		                }                                           \
		                *(p++) = TEXT('8');                         \
		            }                                               \
		            else                                            \
		            {                                               \
		                *(p++) = TEXT('7');                         \
		            }                                               \
		        }                                                   \
		        else                                                \
		        {                                                   \
		            if (_add_u64(value, 1 * power, &value))         \
		            {                                               \
		                *(p++) = TEXT('6');                         \
		                break;                                      \
		            }                                               \
		            *(p++) = TEXT('5');                             \
		        }                                                   \
		    }                                                       \
		    else                                                    \
		    {                                                       \
		        if (_add_u64(value, 3 * power, &value))             \
		        {                                                   \
		            if (!_sub_u64(value, 1 * power, &value))        \
		            {                                               \
		                if (!_sub_u64(value, 1 * power, &value))    \
		                {                                           \
		                    *(p++) = TEXT('4');                     \
		                    break;                                  \
		                }                                           \
		                *(p++) = TEXT('3');                         \
		            }                                               \
		            else                                            \
		            {                                               \
		                *(p++) = TEXT('2');                         \
		            }                                               \
		        }                                                   \
		        else                                                \
		        {                                                   \
		            if (_add_u64(value, 1 * power, &value))         \
		            {                                               \
		                *(p++) = TEXT('1');                         \
		                break;                                      \
		            }                                               \
		            *(p++) = TEXT('0');                             \
		        }                                                   \
		    }                                                       \
		    value += 1 * power;                                     \
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
			if (value >> 32)
			{
	LENGTH10:
				if (!_sub_u64(value, 7000000000, &value))
				{
					if (!_sub_u32(LO(value), 1000000000, &LO(value)))
					{
						if (!_sub_u32(LO(value), 1000000000, &LO(value)))
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
					if (_add_u32(LO(value), 2000000000, &LO(value)))
					{
						if (!_sub_u32(LO(value), 1000000000, &LO(value)))
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
				if (LO(value) >= 2000000000)
				{
					if (!_sub_u32(LO(value), 3000000000, &LO(value)))
					{
						if (!_sub_u32(LO(value), 1000000000, &LO(value)))
						{
							p[0] = TEXT('4');
							break;
						}
						p[0] = TEXT('3');
					}
					else
					{
						p[0] = TEXT('2');
					}
				}
				else
				{
					if (!_sub_u32(LO(value), 1000000000, &LO(value)))
					{
						p[0] = TEXT('1');
						break;
					}
					p[0] = TEXT('0');
				}
			}
			LO(value) += 1000000000;
		} while (0);

		#define value LO(value)

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&p[1] = digits100T[value >>  25     ]; value = (value & ((1 <<  25     ) - 1)) * (100 >> 2);
		*(tchar2_t *)&p[3] = digits100T[value >> (25 - 2)]; value = (value & ((1 << (25 - 2)) - 1)) * (100 >> 2);
		*(tchar2_t *)&p[5] = digits100T[value >> (25 - 4)]; value = (value & ((1 << (25 - 4)) - 1)) * (100 >> 2);
		*(tchar2_t *)&p[7] = digits100T[value >> (25 - 6)]; value = (value & ((1 << (25 - 6)) - 1)) * ( 10 >> 1);
		*(tchar2_t *)&p[9] = T2(       (value >> (25 - 7))) + TEXT('0');

		#undef value

		return p + 10 - buffer;
	}
}
#else
__declspec(naked) size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
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
		mov     eax, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    edx, edx
		jnz     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to10t

	L1:
		cmp     edx, 0x002386F2	// 10000000000000000 >> 32
		ja      L2
		jb      L9
		cmp     eax, 0x6FC10000	// 10000000000000000 & UINT32_MAX
		jae     L8
		jmp     L11

	L2:
		cmp     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		ja      L3
		jb      L6
		cmp     eax, 0xA7640000	// 1000000000000000000 & UINT32_MAX
		jae     L5
		jmp     L7

	L3:
		cmp     edx, 0x8AC72304	// 10000000000000000000 >> 32
		ja      L4
		jb      L5
		cmp     eax, 0x89E80000	// 10000000000000000000 & UINT32_MAX
		jb      L5
	L4:
		push    20
		jmp     LENGTH20

	L5:
		push    19
		jmp     LENGTH19

	L6:
		cmp     edx, 0x01634578	// 100000000000000000 >> 32
		ja      L7
		jb      L8
		cmp     eax, 0x5D8A0000	// 100000000000000000 & UINT32_MAX
		jb      L8
	L7:
		push    18
		jmp     LENGTH18

	L8:
		push    17
		jmp     LENGTH17

	L9:
		cmp     edx, 0x00000918	// 10000000000000 >> 32
		ja      L10
		jb      L15
		cmp     eax, 0x4E72A000	// 10000000000000 & UINT32_MAX
		jae     L14
		jmp     L17

	L10:
		cmp     edx, 0x00038D7E	// 1000000000000000 >> 32
		ja      L11
		jb      L12
		cmp     eax, 0xA4C68000	// 1000000000000000 & UINT32_MAX
		jb      L13
	L11:
		push    16
		jmp     LENGTH16

	L12:
		cmp     edx, 0x00005AF3	// 100000000000000 >> 32
		ja      L13
		jb      L14
		cmp     eax, 0x107A4000	// 100000000000000 & UINT32_MAX
		jb      L14
	L13:
		push    15
		jmp     LENGTH15

	L14:
		push    14
		jmp     LENGTH14

	L15:
		cmp     edx, 0x00000017	// 100000000000 >> 32
		ja      L16
		jb      L19
		cmp     eax, 0x4876E800	// 100000000000 & UINT32_MAX
		jae     L18
		jmp     L20

	L16:
		cmp     edx, 0x000000E8	// 1000000000000 >> 32
		ja      L17
		jb      L18
		cmp     eax, 0xD4A51000	// 1000000000000 & UINT32_MAX
		jb      L18
	L17:
		push    13
		jmp     LENGTH13

	L18:
		push    12
		jmp     LENGTH12

	L19:
		cmp     edx, 0x00000002	// 10000000000 >> 32
		ja      L20
		jb      L21
		cmp     eax, 0x540BE400	// 10000000000 & UINT32_MAX
		jb      L21
	L20:
		push    11
		jmp     LENGTH11

	L21:
		push    10
		jmp     LENGTH10

	LENGTH20:
		mov     tchar ptr [ecx], '1'
		sub     eax, 0x89E80000	// 10000000000000000000 & UINT32_MAX
		sbb     edx, 0x8AC72304	// 10000000000000000000 >> 32
		inc_tchar(ecx)

	#define PUTCHAR(                         \
	    LBL,                                 \
	    X1LO, X1HI,                          \
	    X2LO, X2HI,                          \
	    X3LO, X3HI,                          \
	    X5LO, X5HI)                          \
	__asm   LBL:                             \
	__asm       sub     eax, X5LO            \
	__asm       sbb     edx, X5HI            \
	__asm       jc      LBL##_5              \
	__asm       sub     eax, X2LO            \
	__asm       sbb     edx, X2HI            \
	__asm       jc      LBL##_3              \
	__asm       sub     eax, X1LO            \
	__asm       sbb     edx, X1HI            \
	__asm       jc      LBL##_1              \
	__asm       sub     eax, X1LO            \
	__asm       sbb     edx, X1HI            \
	__asm       jc      LBL##_2              \
	__asm       mov     tchar ptr [ecx], '9' \
	__asm       jmp     LBL##_11             \
	__asm                                    \
	__asm   LBL##_1:                         \
	__asm       mov     tchar ptr [ecx], '7' \
	__asm       jmp     LBL##_10             \
	__asm                                    \
	__asm   LBL##_2:                         \
	__asm       mov     tchar ptr [ecx], '8' \
	__asm       jmp     LBL##_10             \
	__asm                                    \
	__asm   LBL##_3:                         \
	__asm       add     eax, X1LO            \
	__asm       adc     edx, X1HI            \
	__asm       jnc     LBL##_4              \
	__asm       mov     tchar ptr [ecx], '6' \
	__asm       jmp     LBL##_11             \
	__asm                                    \
	__asm   LBL##_4:                         \
	__asm       mov     tchar ptr [ecx], '5' \
	__asm       jmp     LBL##_10             \
	__asm                                    \
	__asm   LBL##_5:                         \
	__asm       add     eax, X3LO            \
	__asm       adc     edx, X3HI            \
	__asm       jnc     LBL##_8              \
	__asm       sub     eax, X1LO            \
	__asm       sbb     edx, X1HI            \
	__asm       jc      LBL##_6              \
	__asm       sub     eax, X1LO            \
	__asm       sbb     edx, X1HI            \
	__asm       jc      LBL##_7              \
	__asm       mov     tchar ptr [ecx], '4' \
	__asm       jmp     LBL##_11             \
	__asm                                    \
	__asm   LBL##_6:                         \
	__asm       mov     tchar ptr [ecx], '2' \
	__asm       jmp     LBL##_10             \
	__asm                                    \
	__asm   LBL##_7:                         \
	__asm       mov     tchar ptr [ecx], '3' \
	__asm       jmp     LBL##_10             \
	__asm                                    \
	__asm   LBL##_8:                         \
	__asm       add     eax, X1LO            \
	__asm       adc     edx, X1HI            \
	__asm       jnc     LBL##_9              \
	__asm       mov     tchar ptr [ecx], '1' \
	__asm       jmp     LBL##_11             \
	__asm                                    \
	__asm   LBL##_9:                         \
	__asm       mov     tchar ptr [ecx], '0' \
	__asm   LBL##_10:                        \
	__asm       add     eax, X1LO            \
	__asm       adc     edx, X1HI            \
	__asm   LBL##_11:                        \
	__asm       inc_tchar(ecx)

	PUTCHAR(LENGTH19,
		0xA7640000, 0x0DE0B6B3,     // 1000000000000000000
		0x4EC80000, 0x1BC16D67,     // 2000000000000000000
		0xF62C0000, 0x29A2241A,     // 3000000000000000000
		0x44F40000, 0x45639182)     // 5000000000000000000

	PUTCHAR(LENGTH18,
		0x5D8A0000, 0x01634578,     // 100000000000000000
		0xBB140000, 0x02C68AF0,     // 200000000000000000
		0x189E0000, 0x0429D069,     // 300000000000000000
		0xD3B20000, 0x06F05B59)     // 500000000000000000

	PUTCHAR(LENGTH17,
		0x6FC10000, 0x002386F2,     // 10000000000000000
		0xDF820000, 0x00470DE4,     // 20000000000000000
		0x4F430000, 0x006A94D7,     // 30000000000000000
		0x2EC50000, 0x00B1A2BC)     // 50000000000000000

	PUTCHAR(LENGTH16,
		0xA4C68000, 0x00038D7E,     // 1000000000000000
		0x498D0000, 0x00071AFD,     // 2000000000000000
		0xEE538000, 0x000AA87B,     // 3000000000000000
		0x37E08000, 0x0011C379)     // 5000000000000000

	PUTCHAR(LENGTH15,
		0x107A4000, 0x00005AF3,     // 100000000000000
		0x20F48000, 0x0000B5E6,     // 200000000000000
		0x316EC000, 0x000110D9,     // 300000000000000
		0x52634000, 0x0001C6BF)     // 500000000000000

	PUTCHAR(LENGTH14,
		0x4E72A000, 0x00000918,     // 10000000000000
		0x9CE54000, 0x00001230,     // 20000000000000
		0xEB57E000, 0x00001B48,     // 30000000000000
		0x883D2000, 0x00002D79)     // 50000000000000

	PUTCHAR(LENGTH13,
		0xD4A51000, 0x000000E8,     // 1000000000000
		0xA94A2000, 0x000001D1,     // 2000000000000
		0x7DEF3000, 0x000002BA,     // 3000000000000
		0x27395000, 0x0000048C)     // 5000000000000

	PUTCHAR(LENGTH12,
		0x4876E800, 0x00000017,     // 100000000000
		0x90EDD000, 0x0000002E,     // 200000000000
		0xD964B800, 0x00000045,     // 300000000000
		0x6A528800, 0x00000074)     // 500000000000

	PUTCHAR(LENGTH11,
		0x540BE400, 0x00000002,     // 10000000000
		0xA817C800, 0x00000004,     // 20000000000
		0xFC23AC00, 0x00000006,     // 30000000000
		0xA43B7400, 0x0000000B)     // 50000000000

	#undef PUTCHAR

		test    edx, edx
		jz      L27
	LENGTH10:
		sub     eax, 0xA13B8600	// 7000000000 & UINT32_MAX
		sbb     edx, 0x00000001	// 7000000000 >> 32
		jc      L24
		sub     eax, 1000000000
		jc      L22
		sub     eax, 1000000000
		jc      L23
		mov     tchar ptr [ecx], '9'
		jmp     L33

	L22:
		mov     tchar ptr [ecx], '7'
		jmp     L32

	L23:
		mov     tchar ptr [ecx], '8'
		jmp     L32

	L24:
		add     eax, 2000000000
		jnc     L25
		sub     eax, 1000000000
		jc      L26
		mov     tchar ptr [ecx], '6'
		jmp     L33

	L25:
		mov     tchar ptr [ecx], '4'
		jmp     L32

	L26:
		mov     tchar ptr [ecx], '5'
		jmp     L32

	L27:
		sub     eax, 2000000000
		jc      L30
		sub     eax, 1000000000
		jc      L28
		sub     eax, 1000000000
		jc      L29
		mov     tchar ptr [ecx], '4'
		jmp     L33

	L28:
		mov     tchar ptr [ecx], '2'
		jmp     L32

	L29:
		mov     tchar ptr [ecx], '3'
		jmp     L32

	L30:
		add     eax, 1000000000
		jnc     L31
		mov     tchar ptr [ecx], '1'
		jmp     L33

	L31:
		mov     tchar ptr [ecx], '0'
	L32:
		add     eax, 1000000000
	L33:
		inc_tchar(ecx)

		push    ebx
		mov     edx, 0x5AFE5357	// (((UINT64_C(1) << (32 + 25)) + 10000000 - 1) / 10000000 - 1) & UINT32_MAX
		lea     ebx, [eax + eax * 2]
		mul     edx
		lea     edx, [edx + ebx + 2]
		mov     eax, edx
		shr     edx, 25
		and     eax, (1 << 25) - 1
		mov     t2(b), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [ecx], t2(b)
		lea     eax, [eax + eax * 4]
		add     ecx, size TCHAR * 2
		mov     edx, eax
		shr     edx, 25 - 2
		and     eax, (1 << (25 - 2)) - 1
		mov     t2(b), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [ecx], t2(b)
		lea     eax, [eax + eax * 4]
		add     ecx, size TCHAR * 2
		mov     edx, eax
		shr     edx, 25 - 4
		and     eax, (1 << (25 - 4)) - 1
		mov     t2(b), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		lea     eax, [eax + eax * 4]
		mov     tchar2 ptr [ecx], t2(b)
		lea     eax, [eax + eax * 4]
		add     ecx, size TCHAR * 2
		mov     edx, eax
		shr     edx, 25 - 6
		and     eax, (1 << (25 - 6)) - 1
		mov     t2(b), tchar2 ptr [digits + edx * (size TCHAR * 2)]
		lea     edx, [eax + eax * 4]
		mov     tchar2 ptr [ecx], t2(b)
		add     ecx, size TCHAR * 2
		shr     edx, 25 - 7
		pop     ebx
		add     edx, '0'
		pop     eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], t2(d)
#else
		mov     tchar ptr [ecx], t(d)
		mov     tchar ptr [ecx + (size TCHAR * 2)], '\0'
#endif
		ret     8
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
	length = *(unsigned long *)&length + 1;
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
size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = *(unsigned long *)&length + 1;
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x01) + TEXT('0');
	while (value >>= 1);
#else
	if (HI(value))
		do
			*(--buffer) = (value & 0x01) + TEXT('0');
		while ((value >>= 1) >> 32);
	do
		*(--buffer) = (LO(value) & 0x01) + TEXT('0');
	while (LO(value) >>= 1);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
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
		#define lo     (esp + 4)
		#define hi     (esp + 8)
		#define buffer ecx

		push    esi
		mov     esi, dword ptr [hi + 4]
		mov     edx, dword ptr [lo + 4]

		bsr     eax, esi
		lea     eax, [eax + 32 + 1]
		jnz     L1
		bsr     eax, edx
		lea     eax, [eax + 1]
		jz      L6
	L1:
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		add     ecx, eax
#endif
		xor     eax, eax
		mov     tchar ptr [ecx], '\0'
		test    esi, esi
		jnz     L3
		jmp     L5

		align   16
	L2:
		rcr     edx, 1
		adc     eax, '0'
		dec_tchar(ecx)
		mov     tchar ptr [ecx], t(a)
		xor     eax, eax
	L3:
		shr     esi, 1
		jnz     L2

		rcr     edx, 1

		align   16
	L4:
		adc     eax, '0'
		dec_tchar(ecx)
		mov     tchar ptr [ecx], t(a)
		xor     eax, eax
	L5:
		shr     edx, 1
		jnz     L4

		mov     tchar ptr [ecx - size TCHAR], '1'
		pop     eax
		pop     esi
		ret     8

		align   16
	L6:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     esi
		ret     8

		#undef lo
		#undef hi
		#undef buffer
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
	length = (*(unsigned long *)&length / 2) + 1;
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
size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = (*(unsigned long *)&length / 2) + 1;
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x03) + TEXT('0');
	while (value >>= 2);
#else
	if (HI(value))
		do
			*(--buffer) = (value & 0x03) + TEXT('0');
		while ((value >>= 2) >> 32);
	do
		*(--buffer) = (LO(value) & 0x03) + TEXT('0');
	while (LO(value) >>= 2);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
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
		#define lo     (esp + 4)
		#define hi     (esp + 8)
		#define buffer ecx

		push    esi
		mov     esi, dword ptr [hi + 4]
		mov     edx, dword ptr [lo + 4]

		bsr     eax, esi
		lea     eax, [eax + 32 + 2]
		jnz     L1
		bsr     eax, edx
		lea     eax, [eax + 2]
		jz      L4
	L1:
		shr     eax, 1
		test    esi, esi
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L3

		align   16
	L2:
		mov     eax, edx
		dec_tchar(ecx)
		shr     esi, 1
		rcr     edx, 1
		and     eax, 3
		shr     esi, 1
		lea     eax, [eax + '0']
		rcr     edx, 1
		mov     tchar ptr [ecx], t(a)
		test    esi, esi
		jnz     L2

		align   16
	L3:
		mov     eax, edx
		dec_tchar(ecx)
		shr     edx, 2
		and     eax, 3
		add     eax, '0'
		test    edx, edx
		mov     tchar ptr [ecx], t(a)
		jnz     L3

		pop     eax
		pop     esi
		ret     8

		align   16
	L4:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     esi
		ret     8

		#undef lo
		#undef hi
		#undef buffer
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
	__assume(*(unsigned long *)&length < 32);
#if 0
	length = (*(unsigned long *)&length / 3) + 1;
#else
	length = RECIPROCAL_DIVISION(*(unsigned long *)&length, 3, 5) + 1;
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
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume(*(unsigned long *)&length < 64);
#if 0
	length = (*(unsigned long *)&length / 3) + 1;
#else
	length = RECIPROCAL_DIVISION(*(unsigned long *)&length, 3, 7) + 1;
#endif
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x07) + TEXT('0');
	while (value >>= 3);
#else
	if (HI(value))
		do
			*(--buffer) = (value & 0x07) + TEXT('0');
		while ((value >>= 3) >> 32);
	do
		*(--buffer) = (LO(value) & 0x07) + TEXT('0');
	while (LO(value) >>= 3);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
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
		#define lo     (esp + 4)
		#define hi     (esp + 8)
		#define buffer ecx

		push    esi
		push    edi
		mov     edi, dword ptr [hi + 8]
		mov     esi, dword ptr [lo + 8]

		bsr     eax, edi
		lea     eax, [eax + 32 + 3]
		jnz     L1
		bsr     eax, esi
		lea     eax, [eax + 3]
		jz      L4
	L1:
		lea     edx, [eax + eax * 4]
		lea     eax, [eax + eax * 2]
		lea     eax, [eax + edx * 8]
		shr     eax, 7
		test    edi, edi
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L3

		align   16
	L2:
		mov     eax, esi
		mov     edx, edi
		shr     esi, 3
		and     eax, 7
		shl     edx, 32 - 3
		add     eax, '0'
		dec_tchar(ecx)
		or      esi, edx
		shr     edi, 3
		mov     tchar ptr [ecx], t(a)
		test    edi, edi
		jnz     L2

		align   16
	L3:
		mov     eax, esi
		dec_tchar(ecx)
		shr     esi, 3
		and     eax, 7
		add     eax, '0'
		test    esi, esi
		mov     tchar ptr [ecx], t(a)
		jnz     L3

		pop     eax
		pop     edi
		pop     esi
		ret     8

		align   16
	L4:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     edi
		pop     esi
		ret     8

		#undef lo
		#undef hi
		#undef buffer
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
	length = (*(unsigned long *)&length / 4) + 1;
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
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	size_t      length;
	const TCHAR *digits;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = (*(unsigned long *)&length / 4) + 1;
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = digits[(size_t)value & 0x0F];
	while (value >>= 4);
#else
	if (HI(value))
		do
			*(--buffer) = digits[(size_t)value & 0x0F];
		while ((value >>= 4) >> 32);
	do
		*(--buffer) = digits[LO(value) & 0x0F];
	while (LO(value) >>= 4);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
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
		#define lo     (esp + 4)
		#define hi     (esp + 8)
		#define buffer ecx
		#define upper  edx

		push    ebx
		push    esi
		mov     esi, dword ptr [hi + 8]
		mov     ebx, dword ptr [lo + 8]

		bsr     eax, esi
		lea     eax, [eax + 32 + 4]
		jnz     L1
		bsr     eax, ebx
		lea     eax, [eax + 4]
		jz      L4
	L1:
		shr     eax, 2
		push    edi
		push    eax
		cmp     edx, 1
		sbb     edx, edx
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		add     ecx, eax
#endif
		and     edx, ptrdiff_digitsSmall
		mov     tchar ptr [ecx], '\0'
		add     edx, offset digitsLarge
		test    esi, esi
		jz      L3

		align   16
	L2:
		mov     eax, ebx
		mov     edi, esi
		shr     ebx, 4
		and     eax, 15
		shl     edi, 32 - 4
		mov     t(a), tchar ptr [edx + eax * size TCHAR]
		dec_tchar(ecx)
		or      ebx, edi
		shr     esi, 4
		mov     tchar ptr [ecx], t(a)
		test    esi, esi
		jnz     L2

		align   16
	L3:
		mov     eax, ebx
		dec_tchar(ecx)
		shr     ebx, 4
		and     eax, 15
		mov     t(a), tchar ptr [edx + eax * size TCHAR]
		test    ebx, ebx
		mov     tchar ptr [ecx], t(a)
		jnz     L3

		pop     eax
		pop     edi
		pop     esi
		pop     ebx
		ret     8

		align   16
	L4:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     esi
		pop     ebx
		ret     8

		#undef lo
		#undef hi
		#undef buffer
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
	__assume(*(unsigned long *)&length < 32);
#if 0
	length = (*(unsigned long *)&length / 5) + 1;
#else
	length = RECIPROCAL_DIVISION(*(unsigned long *)&length, 5, 6) + 1;
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
size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	size_t      length;
	const TCHAR *digits;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume(*(unsigned long *)&length < 64);
#if 0
	length = (*(unsigned long *)&length / 5) + 1;
#else
	length = RECIPROCAL_DIVISION(*(unsigned long *)&length, 5, 6) + 1;
#endif
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = digits[(size_t)value & 0x1F];
	while (value >>= 5);
#else
	if (HI(value))
		do
			*(--buffer) = digits[(size_t)value & 0x1F];
		while ((value >>= 5) >> 32);
	do
		*(--buffer) = digits[LO(value) & 0x1F];
	while (LO(value) >>= 5);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
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
		#define lo     (esp + 4)
		#define hi     (esp + 8)
		#define buffer ecx
		#define upper  edx

		push    ebx
		push    esi
		mov     esi, dword ptr [hi + 8]
		mov     ebx, dword ptr [lo + 8]

		bsr     eax, esi
		lea     eax, [eax + 32]
		jnz     L1
		bsr     eax, ebx
		jz      L4
	L1:
		push    edi
		lea     edi, [eax + eax * 4]
		lea     eax, [edi + eax * 8 + (1 << 6)]
		shr     eax, 6
		cmp     edx, 1
		sbb     edx, edx
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		add     ecx, eax
#endif
		and     edx, ptrdiff_digitsSmall
		push    eax
		add     edx, offset digitsLarge
		mov     tchar ptr [ecx], '\0'
		test    esi, esi
		jz      L3

		align   16
	L2:
		mov     eax, ebx
		mov     edi, esi
		shr     ebx, 5
		and     eax, 31
		shl     edi, 32 - 5
		mov     t(a), tchar ptr [edx + eax * size TCHAR]
		dec_tchar(ecx)
		or      ebx, edi
		shr     esi, 5
		mov     tchar ptr [ecx], t(a)
		test    esi, esi
		jnz     L2

		align   16
	L3:
		mov     eax, ebx
		dec_tchar(ecx)
		shr     ebx, 5
		and     eax, 31
		mov     t(a), tchar ptr [edx + eax * size TCHAR]
		test    ebx, ebx
		mov     tchar ptr [ecx], t(a)
		jnz     L3

		pop     eax
		pop     edi
		pop     esi
		pop     ebx
		ret     8

		align   16
	L4:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * size TCHAR)
#endif
		mov     eax, 1
		pop     esi
		pop     ebx
		ret     8

		#undef lo
		#undef hi
		#undef buffer
		#undef upper
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#define PRECISION32(divisor) (  \
    (divisor) ==  2 ? 0 :       \
    (divisor) ==  3 ? 1 :       \
    (divisor) ==  4 ? 0 :       \
    (divisor) ==  5 ? 2 :       \
    (divisor) ==  6 ? 2 :       \
    (divisor) ==  7 ? 3 :       \
    (divisor) ==  8 ? 0 :       \
    (divisor) ==  9 ? 1 :       \
    (divisor) == 10 ? 3 :       \
    (divisor) == 11 ? 3 :       \
    (divisor) == 12 ? 3 :       \
    (divisor) == 13 ? 2 :       \
    (divisor) == 14 ? 4 :       \
    (divisor) == 15 ? 3 :       \
    (divisor) == 16 ? 0 :       \
    (divisor) == 17 ? 4 :       \
    (divisor) == 18 ? 2 :       \
    (divisor) == 19 ? 5 :       \
    (divisor) == 20 ? 4 :       \
    (divisor) == 21 ? 5 :       \
    (divisor) == 22 ? 4 :       \
    (divisor) == 23 ? 4 :       \
    (divisor) == 24 ? 4 :       \
    (divisor) == 25 ? 3 :       \
    (divisor) == 26 ? 3 :       \
    (divisor) == 27 ? 5 :       \
    (divisor) == 28 ? 5 :       \
    (divisor) == 29 ? 4 :       \
    (divisor) == 30 ? 4 :       \
    (divisor) == 31 ? 5 :       \
    (divisor) == 32 ? 0 :       \
    (divisor) == 33 ? 3 :       \
    (divisor) == 34 ? 5 :       \
    (divisor) == 35 ? 6 :       \
    (divisor) == 36 ? 3 :       \
    0)

#define PRECISION64(divisor) (  \
    (divisor) ==  2 ? 0 :       \
    (divisor) ==  3 ? 1 :       \
    (divisor) ==  4 ? 0 :       \
    (divisor) ==  5 ? 2 :       \
    (divisor) ==  6 ? 2 :       \
    (divisor) ==  7 ? 3 :       \
    (divisor) ==  8 ? 0 :       \
    (divisor) ==  9 ? 3 :       \
    (divisor) == 10 ? 3 :       \
    (divisor) == 11 ? 1 :       \
    (divisor) == 12 ? 3 :       \
    (divisor) == 13 ? 2 :       \
    (divisor) == 14 ? 4 :       \
    (divisor) == 15 ? 3 :       \
    (divisor) == 16 ? 0 :       \
    (divisor) == 17 ? 4 :       \
    (divisor) == 18 ? 4 :       \
    (divisor) == 19 ? 4 :       \
    (divisor) == 20 ? 4 :       \
    (divisor) == 21 ? 5 :       \
    (divisor) == 22 ? 2 :       \
    (divisor) == 23 ? 5 :       \
    (divisor) == 24 ? 4 :       \
    (divisor) == 25 ? 5 :       \
    (divisor) == 26 ? 3 :       \
    (divisor) == 27 ? 4 :       \
    (divisor) == 28 ? 5 :       \
    (divisor) == 29 ? 5 :       \
    (divisor) == 30 ? 4 :       \
    (divisor) == 31 ? 5 :       \
    (divisor) == 32 ? 0 :       \
    (divisor) == 33 ? 1 :       \
    (divisor) == 34 ? 5 :       \
    (divisor) == 35 ? 5 :       \
    (divisor) == 36 ? 5 :       \
    0)

#define RECIPROCAL32(divisor) \
    (((UINT64_C(1) << (32 + PRECISION32(divisor))) + (divisor) - 1) / (divisor))

#define RECIPROCAL64_FORMULA(operand, divisor) \
    ((UINT64_C(1) << (32 + PRECISION64(divisor))) operand (divisor))

#define RECIPROCAL64_HIGH(divisor) \
    RECIPROCAL64_FORMULA(/, divisor)

#define RECIPROCAL64(divisor) \
    ((RECIPROCAL64_HIGH(divisor) << 32) + (((RECIPROCAL64_FORMULA(%, divisor) << 32) + (divisor) - 1) / (divisor)))

#define RADIX_MIN 3
#define RADIX_MAX 36

#if RADIX_MIN <= 2
#define MAX_LENGTH32 32
#define MAX_LENGTH64 64
#else
#define MAX_LENGTH32 21
#define MAX_LENGTH64 41
#endif

static const uint32_t _reciprocal32[] = {
#if RADIX_MIN <= 2
	(uint32_t)RECIPROCAL32( 2),
#endif
	(uint32_t)RECIPROCAL32( 3), (uint32_t)RECIPROCAL32( 4),
	(uint32_t)RECIPROCAL32( 5), (uint32_t)RECIPROCAL32( 6),
	(uint32_t)RECIPROCAL32( 7), (uint32_t)RECIPROCAL32( 8),
	(uint32_t)RECIPROCAL32( 9), (uint32_t)RECIPROCAL32(10),
	(uint32_t)RECIPROCAL32(11), (uint32_t)RECIPROCAL32(12),
	(uint32_t)RECIPROCAL32(13), (uint32_t)RECIPROCAL32(14),
	(uint32_t)RECIPROCAL32(15), (uint32_t)RECIPROCAL32(16),
	(uint32_t)RECIPROCAL32(17), (uint32_t)RECIPROCAL32(18),
	(uint32_t)RECIPROCAL32(19), (uint32_t)RECIPROCAL32(20),
	(uint32_t)RECIPROCAL32(21), (uint32_t)RECIPROCAL32(22),
	(uint32_t)RECIPROCAL32(23), (uint32_t)RECIPROCAL32(24),
	(uint32_t)RECIPROCAL32(25), (uint32_t)RECIPROCAL32(26),
	(uint32_t)RECIPROCAL32(27), (uint32_t)RECIPROCAL32(28),
	(uint32_t)RECIPROCAL32(29), (uint32_t)RECIPROCAL32(30),
	(uint32_t)RECIPROCAL32(31), (uint32_t)RECIPROCAL32(32),
	(uint32_t)RECIPROCAL32(33), (uint32_t)RECIPROCAL32(34),
	(uint32_t)RECIPROCAL32(35), (uint32_t)RECIPROCAL32(36),
};

static const uint32_t _mask32[] = {
#if RADIX_MIN <= 2
	RECIPROCAL32( 2) >> 32 ? -1 : 0,
#endif
	RECIPROCAL32( 3) >> 32 ? -1 : 0, RECIPROCAL32( 4) >> 32 ? -1 : 0,
	RECIPROCAL32( 5) >> 32 ? -1 : 0, RECIPROCAL32( 6) >> 32 ? -1 : 0,
	RECIPROCAL32( 7) >> 32 ? -1 : 0, RECIPROCAL32( 8) >> 32 ? -1 : 0,
	RECIPROCAL32( 9) >> 32 ? -1 : 0, RECIPROCAL32(10) >> 32 ? -1 : 0,
	RECIPROCAL32(11) >> 32 ? -1 : 0, RECIPROCAL32(12) >> 32 ? -1 : 0,
	RECIPROCAL32(13) >> 32 ? -1 : 0, RECIPROCAL32(14) >> 32 ? -1 : 0,
	RECIPROCAL32(15) >> 32 ? -1 : 0, RECIPROCAL32(16) >> 32 ? -1 : 0,
	RECIPROCAL32(17) >> 32 ? -1 : 0, RECIPROCAL32(18) >> 32 ? -1 : 0,
	RECIPROCAL32(19) >> 32 ? -1 : 0, RECIPROCAL32(20) >> 32 ? -1 : 0,
	RECIPROCAL32(21) >> 32 ? -1 : 0, RECIPROCAL32(22) >> 32 ? -1 : 0,
	RECIPROCAL32(23) >> 32 ? -1 : 0, RECIPROCAL32(24) >> 32 ? -1 : 0,
	RECIPROCAL32(25) >> 32 ? -1 : 0, RECIPROCAL32(26) >> 32 ? -1 : 0,
	RECIPROCAL32(27) >> 32 ? -1 : 0, RECIPROCAL32(28) >> 32 ? -1 : 0,
	RECIPROCAL32(29) >> 32 ? -1 : 0, RECIPROCAL32(30) >> 32 ? -1 : 0,
	RECIPROCAL32(31) >> 32 ? -1 : 0, RECIPROCAL32(32) >> 32 ? -1 : 0,
	RECIPROCAL32(33) >> 32 ? -1 : 0, RECIPROCAL32(34) >> 32 ? -1 : 0,
	RECIPROCAL32(35) >> 32 ? -1 : 0, RECIPROCAL32(36) >> 32 ? -1 : 0,
};

static const uint8_t _precision32[] = {
#if RADIX_MIN <= 2
	PRECISION32( 2),
#endif
	PRECISION32( 3), PRECISION32( 4), PRECISION32( 5), PRECISION32( 6),
	PRECISION32( 7), PRECISION32( 8), PRECISION32( 9), PRECISION32(10),
	PRECISION32(11), PRECISION32(12), PRECISION32(13), PRECISION32(14),
	PRECISION32(15), PRECISION32(16), PRECISION32(17), PRECISION32(18),
	PRECISION32(19), PRECISION32(20), PRECISION32(21), PRECISION32(22),
	PRECISION32(23), PRECISION32(24), PRECISION32(25), PRECISION32(26),
	PRECISION32(27), PRECISION32(28), PRECISION32(29), PRECISION32(30),
	PRECISION32(31), PRECISION32(32), PRECISION32(33), PRECISION32(34),
	PRECISION32(35), PRECISION32(36),
};

static const uint64_t _reciprocal64[] = {
#if RADIX_MIN <= 2
	RECIPROCAL64( 2),
#endif
	RECIPROCAL64( 3), RECIPROCAL64( 4), RECIPROCAL64( 5), RECIPROCAL64( 6),
	RECIPROCAL64( 7), RECIPROCAL64( 8), RECIPROCAL64( 9), RECIPROCAL64(10),
	RECIPROCAL64(11), RECIPROCAL64(12), RECIPROCAL64(13), RECIPROCAL64(14),
	RECIPROCAL64(15), RECIPROCAL64(16), RECIPROCAL64(17), RECIPROCAL64(18),
	RECIPROCAL64(19), RECIPROCAL64(20), RECIPROCAL64(21), RECIPROCAL64(22),
	RECIPROCAL64(23), RECIPROCAL64(24), RECIPROCAL64(25), RECIPROCAL64(26),
	RECIPROCAL64(27), RECIPROCAL64(28), RECIPROCAL64(29), RECIPROCAL64(30),
	RECIPROCAL64(31), RECIPROCAL64(32), RECIPROCAL64(33), RECIPROCAL64(34),
	RECIPROCAL64(35), RECIPROCAL64(36),
};

static const int32_t _mask64[] = {
#if RADIX_MIN <= 2
	RECIPROCAL64_HIGH( 2) >> 32 ? -1 : 0,
#endif
	RECIPROCAL64_HIGH( 3) >> 32 ? -1 : 0, RECIPROCAL64_HIGH( 4) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH( 5) >> 32 ? -1 : 0, RECIPROCAL64_HIGH( 6) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH( 7) >> 32 ? -1 : 0, RECIPROCAL64_HIGH( 8) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH( 9) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(10) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(11) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(12) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(13) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(14) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(15) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(16) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(17) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(18) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(19) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(20) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(21) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(22) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(23) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(24) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(25) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(26) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(27) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(28) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(29) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(30) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(31) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(32) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(33) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(34) >> 32 ? -1 : 0,
	RECIPROCAL64_HIGH(35) >> 32 ? -1 : 0, RECIPROCAL64_HIGH(36) >> 32 ? -1 : 0,
};

static const uint8_t _precision64[] = {
#if RADIX_MIN <= 2
	PRECISION64( 2),
#endif
	PRECISION64( 3), PRECISION64( 4), PRECISION64( 5), PRECISION64( 6),
	PRECISION64( 7), PRECISION64( 8), PRECISION64( 9), PRECISION64(10),
	PRECISION64(11), PRECISION64(12), PRECISION64(13), PRECISION64(14),
	PRECISION64(15), PRECISION64(16), PRECISION64(17), PRECISION64(18),
	PRECISION64(19), PRECISION64(20), PRECISION64(21), PRECISION64(22),
	PRECISION64(23), PRECISION64(24), PRECISION64(25), PRECISION64(26),
	PRECISION64(27), PRECISION64(28), PRECISION64(29), PRECISION64(30),
	PRECISION64(31), PRECISION64(32), PRECISION64(33), PRECISION64(34),
	PRECISION64(35), PRECISION64(36),
};

#undef PRECISION32
#undef PRECISION64
#undef RECIPROCAL32
#undef RECIPROCAL64_FORMULA
#undef RECIPROCAL64_HIGH
#undef RECIPROCAL64

#ifndef _M_IX86
size_t __fastcall internal_ui32tot(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	#define sizeof_buffer ((MAX_LENGTH32 * sizeof(TCHAR) + sizeof(void *) - 1) & -(int)sizeof(void *))
	#define reciprocal32  (_reciprocal32 - RADIX_MIN)
	#define mask32        (_mask32       - RADIX_MIN)
	#define precision32   (_precision32  - RADIX_MIN)

	size_t      length;
	const TCHAR *digits;
	TCHAR       stack[sizeof_buffer / sizeof(TCHAR)], *p;

	__assume(radix >= RADIX_MIN && radix <= RADIX_MAX);
	digits = upper ? digitsLarge : digitsSmall;
	p = stack + _countof(stack);
	do
	{
		uint32_t remainder;

		remainder = value;
		value =
			(
				(__emulu(value, reciprocal32[radix]) >> 32) +
				(value & mask32[radix])
			) >> precision32[radix];
		remainder -= value * radix;
		*(--p) = digits[remainder];
	} while (value);
	memcpy(buffer, p, (length = stack + _countof(stack) - p) * sizeof(TCHAR));
	buffer[length] = TEXT('\0');
	return length;

	#undef sizeof_buffer
	#undef reciprocal32
	#undef mask32
	#undef precision32
}
#else
__declspec(naked) size_t __fastcall internal_ui32tot(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	#define sizeof_buffer ((MAX_LENGTH32 * (size TCHAR) + 3) & -4)
	#define reciprocal32  (_reciprocal32 - RADIX_MIN * 4)
	#define mask32        (_mask32       - RADIX_MIN * 4)
	#define precision32   (_precision32  - RADIX_MIN    )

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
		push    ebx
		push    ebp
		push    esi
		push    edi

		#define value  ecx
		#define buffer edx
		#define upper  (esp + 16 + 4)
		#define radix  ebp
		#define digits edi
		#define p1     esi
		#define p2     edi

		mov     digits, dword ptr [upper]
		mov     radix, dword ptr [esp + 16 + 8]
		mov     p1, esp
		cmp     digits, 1
		sbb     digits, digits
		mov     eax, value
		and     digits, ptrdiff_digitsSmall
		sub     esp, sizeof_buffer
		add     digits, offset digitsLarge
		push    buffer

		align   16
	L1:
		mov     ecx, dword ptr [mask32 + radix * 4]
		mov     ebx, eax
		mov     edx, dword ptr [reciprocal32 + radix * 4]
		and     ecx, eax
		mul     edx
		xor     eax, eax
		add     edx, ecx
		adc     eax, eax
		mov     cl, byte ptr [precision32 + radix]
		shrd    edx, eax, cl
		mov     eax, edx
		imul    edx, radix
		sub     ebx, edx
		dec_tchar(p1)
		mov     t(d), tchar ptr [digits + ebx * size TCHAR]
		test    eax, eax
		mov     tchar ptr [p1], t(d)
		jnz     L1

		mov     ecx, -4
		lea     eax, [esp + sizeof_buffer + 4]
		pop     p2
		sub     eax, p1
		add     ecx, eax
		jnc     L3

		align   16
	L2:
		mov     edx, dword ptr [p1]
		add     p1, 4
		mov     dword ptr [p2], edx
		add     p2, 4
		sub     ecx, 4
		jae     L2
	L3:
		cmp     ecx, -2
		jb      L4
		mov     dx, word ptr [p1]
		add     p1, 2
		mov     word ptr [p2], dx
		add     p2, 2
	L4:
#ifdef _UNICODE
		shr     eax, 1
		xor     ecx, ecx
#else
		and     ecx, 1
		jz      L5
		mov     dl, byte ptr [p1]
		dec     ecx
		mov     byte ptr [p2], dl
		inc     p2
	L5:
#endif
		mov     tchar ptr [p2], t(c)
		add     esp, sizeof_buffer
		pop     edi
		pop     esi
		pop     ebp
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

	#undef sizeof_buffer
	#undef reciprocal32
	#undef mask32
	#undef precision32
	#undef t
	#undef tchar
	#undef inc_tchar
	#undef dec_tchar
}
#endif

#ifndef _M_IX86
size_t __fastcall internal_ui64tot(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	#define sizeof_buffer ((MAX_LENGTH64 * sizeof(TCHAR) + sizeof(void *) - 1) & -(int)sizeof(void *))
	#define reciprocal32  (_reciprocal32 - RADIX_MIN)
	#define mask32        (_mask32       - RADIX_MIN)
	#define precision32   (_precision32  - RADIX_MIN)
	#define reciprocal64  (_reciprocal64 - RADIX_MIN)
	#define mask64        (_mask64       - RADIX_MIN)
	#define precision64   (_precision64  - RADIX_MIN)

	size_t      length;
	const TCHAR *digits;
	TCHAR       stack[sizeof_buffer / sizeof(TCHAR)], *p;

	__assume(radix >= RADIX_MIN && radix <= RADIX_MAX);
	digits = upper ? digitsLarge : digitsSmall;
	p = stack + _countof(stack);
#if INTPTR_MAX != INT32_MAX
	do
	{
		uint32_t remainder;
		uint8_t carry;

		remainder = (uint32_t)value;
		carry = _add_u64(
			__umulh(value, reciprocal64[radix]),
			value & mask64[radix],
			&value);
		value = __shiftright128(value, carry, precision64[radix]);
		remainder -= (uint32_t)value * radix;
		*(--p) = digits[remainder];
	} while (value);
#else
	while (value >> 32)
	{
		uint32_t remainder, carry;

		remainder = (uint32_t)value;
		carry = _add_u64(
			__umulh(value, reciprocal64[radix]),
			value & (((uint64_t)mask64[radix] << 32) | mask64[radix]),
			&value);
		value =
			(uint32_t)__ull_rshift(value, precision64[radix]) |
			(__ull_rshift(((uint64_t)carry << 32) | (value >> 32), precision64[radix]) << 32);
		remainder -= (uint32_t)value * radix;
		*(--p) = digits[remainder];
	}
	do
	{
		uint32_t remainder;

		remainder = LO(value);
		LO(value) =
			(
				(__emulu(LO(value), reciprocal32[radix]) >> 32) +
				(LO(value) & mask32[radix])
			) >> precision32[radix];
		remainder -= LO(value) * radix;
		*(--p) = digits[remainder];
	} while (LO(value));
#endif
	memcpy(buffer, p, (length = stack + _countof(stack) - p) * sizeof(TCHAR));
	buffer[length] = TEXT('\0');
	return length;

	#undef sizeof_buffer
	#undef reciprocal32
	#undef mask32
	#undef precision32
	#undef reciprocal64
	#undef mask64
	#undef precision64
}
#else
__declspec(naked) size_t __fastcall internal_ui64tot(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	#define sizeof_buffer ((MAX_LENGTH64 * (size TCHAR) + 3) & -4)
	#define reciprocal32  (_reciprocal32 - RADIX_MIN * 4)
	#define mask32        (_mask32       - RADIX_MIN * 4)
	#define precision32   (_precision32  - RADIX_MIN    )
	#define reciprocal64  (_reciprocal64 - RADIX_MIN * 8)
	#define mask64        (_mask64       - RADIX_MIN * 4)
	#define precision64   (_precision64  - RADIX_MIN    )

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
		push    ebx
		push    ebp
		push    esi
		push    edi

		#define buffer ecx
		#define upper  edx
		#define lo     eax
		#define hi     edx
		#define radix  ebp
		#define digits edi
		#define p1     esi
		#define p2     edi

		mov     p1, esp
		cmp     upper, 1
		sbb     digits, digits
		mov     lo, dword ptr [esp + 16 + 4]
		mov     hi, dword ptr [esp + 16 + 8]
		mov     radix, dword ptr [esp + 16 + 12]
		sub     esp, sizeof_buffer
		and     digits, ptrdiff_digitsSmall
		push    buffer
		add     digits, offset digitsLarge
		test    hi, hi
		jz      L2
		push    digits

		align   16
	L1:
		push    lo
		push    hi
		mov     ecx, dword ptr [reciprocal64 + radix * 8]
		xor     ebx, ebx
		mul     ecx
		mov     eax, dword ptr [esp]
		mov     edi, edx
		mul     ecx
		add     edi, eax
		mov     eax, dword ptr [esp + 4]
		adc     ebx, edx
		mov     edx, dword ptr [reciprocal64 + radix * 8 + 4]
		mul     edx
		xor     ecx, ecx
		add     edi, eax
		adc     ebx, edx
		mov     eax, dword ptr [esp]
		adc     ecx, ecx
		mov     edi, dword ptr [mask64 + radix * 4]
		mul     dword ptr [reciprocal64 + radix * 8 + 4]
		add     eax, ebx
		mov     ebx, dword ptr [esp + 4]
		adc     edx, ecx
		pop     ecx
		and     ebx, edi
		and     ecx, edi
		xor     edi, edi
		add     eax, ebx
		adc     edx, ecx
		mov     cl, byte ptr [precision64 + radix]
		adc     edi, edi
		pop     ebx
		shrd    eax, edx, cl
		shrd    edx, edi, cl
		mov     ecx, eax
		mov     digits, dword ptr [esp]
		imul    ecx, radix
		sub     ebx, ecx
		dec_tchar(p1)
		mov     t(c), tchar ptr [digits + ebx * size TCHAR]
		test    hi, hi
		mov     tchar ptr [p1], t(c)
		jnz     L1
		pop     digits

		align   16
	L2:
		mov     ecx, dword ptr [mask32 + radix * 4]
		mov     ebx, eax
		mov     edx, dword ptr [reciprocal32 + radix * 4]
		and     ecx, eax
		mul     edx
		xor     eax, eax
		add     edx, ecx
		adc     eax, eax
		mov     cl, byte ptr [precision32 + radix]
		shrd    edx, eax, cl
		mov     eax, edx
		imul    edx, radix
		sub     ebx, edx
		dec_tchar(p1)
		mov     t(d), tchar ptr [digits + ebx * size TCHAR]
		test    eax, eax
		mov     tchar ptr [p1], t(d)
		jnz     L2

		mov     ecx, -4
		lea     eax, [esp + sizeof_buffer + 4]
		pop     p2
		sub     eax, p1
		add     ecx, eax
		jnc     L4

		align   16
	L3:
		mov     edx, dword ptr [p1]
		add     p1, 4
		mov     dword ptr [p2], edx
		add     p2, 4
		sub     ecx, 4
		jae     L3
	L4:
		cmp     ecx, -2
		jb      L5
		mov     dx, word ptr [p1]
		add     p1, 2
		mov     word ptr [p2], dx
		add     p2, 2
	L5:
#ifdef _UNICODE
		shr     eax, 1
		xor     ecx, ecx
#else
		and     ecx, 1
		jz      L6
		mov     dl, byte ptr [p1]
		dec     ecx
		mov     byte ptr [p2], dl
		inc     p2
	L6:
#endif
		mov     tchar ptr [p2], t(c)
		add     esp, sizeof_buffer
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     12

		#undef buffer
		#undef upper
		#undef lo
		#undef hi
		#undef radix
		#undef digits
		#undef p1
		#undef p2
	}

	#undef sizeof_buffer
	#undef reciprocal32
	#undef mask32
	#undef precision32
	#undef reciprocal64
	#undef mask64
	#undef precision64
	#undef t
	#undef tchar
	#undef inc_tchar
	#undef dec_tchar
}
#endif

#undef RADIX_MIN
#undef RADIX_MAX
#undef MAX_LENGTH32
#undef MAX_LENGTH64
