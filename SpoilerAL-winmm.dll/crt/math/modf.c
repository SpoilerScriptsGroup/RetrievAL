#ifndef _M_IX86
#include <stdint.h>
#ifdef _WIN64
#include <intrin.h>
#pragma intrinsic(_BitScanReverse64)

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || '\4\3\2\1' == 0x01020304 || defined(_MSC_VER)
#define __LITTLE_ENDIAN__   1
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || '\4\3\2\1' == 0x04030201
#define __BIG_ENDIAN__      1
#else
#error Current byte order is not supported.
#endif
#endif

double __cdecl modf(double x, double *intptr)
{
	#define DBL_MANT_BIT  52
	#define DBL_ONE       0x3FF0000000000000
	#define DBL_EXP_BIAS  (DBL_ONE >> DBL_MANT_BIT)
	#define DBL_SIGN_MASK 0x8000000000000000
	#define DBL_EXP_MASK  0x7FF0000000000000
	#define DBL_MANT_MASK 0x000FFFFFFFFFFFFF

	uint64_t fracpart, intpart;
	uint64_t exp;
	uint32_t index;

	fracpart = *(uint64_t *)&x;
	exp = fracpart & DBL_EXP_MASK;
	if ((int64_t)(exp -= DBL_ONE) < 0)
	{
		// |x| < 1
		intpart = fracpart & DBL_SIGN_MASK;
	}
	else if (exp >= ((uint64_t)DBL_MANT_BIT << DBL_MANT_BIT))
	{
		// |x| >= (1 << 52)
		intpart = fracpart;
		fracpart &= DBL_SIGN_MASK;
	}
	else
	{
		// 1 <= |x| < (1 << 52)
		intpart = fracpart & ((int64_t)~DBL_MANT_MASK >> (exp >> DBL_MANT_BIT));
		fracpart -= intpart;
		if (_BitScanReverse64(&index, fracpart))
		{
			fracpart <<= DBL_MANT_BIT - index;
			fracpart &= DBL_MANT_MASK;
			fracpart |= (index + DBL_EXP_BIAS - DBL_MANT_BIT) << DBL_MANT_BIT;
		}
		fracpart |= intpart & DBL_SIGN_MASK;
	}
	*(uint64_t *)intptr = intpart;
	return *(double *)&fracpart;

	#undef DBL_MANT_BIT
	#undef DBL_ONE
	#undef DBL_EXP_BIAS
	#undef DBL_SIGN_MASK
	#undef DBL_EXP_MASK
	#undef DBL_MANT_MASK
}
#else	// _WIN64
#if defined(_MSC_VER) && defined(_M_IX86) && _MSC_VER >= 1310	// _BitScanReverse64
#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
static __inline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask)
{
	unsigned char Result;

	if (Result = _BitScanReverse(Index, (unsigned long)(Mask >> 32)))
		*Index += 32;
	else
		Result = _BitScanReverse(Index, (unsigned long)Mask);
	return Result;
}
#elif defined(_MSC_VER) && defined(_M_IX86)	// _BitScanReverse64
static __inline unsigned char _BitScanReverse64(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     ecx, dword ptr [Index]
		bsr     eax, dword ptr [Mask + 4]
		lea     eax, [eax + 32]
		jnz     L1
		bsr     eax, dword ptr [Mask]
	L1:
		mov     dword ptr [ecx], eax
		setnz   al
	}
}
#else	// _BitScanReverse64
static __inline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask)
{
	if (!Mask)
		return 0;
	for (*Index = 63; (int64_t)Mask >= 0; Mask <<= 1)
		--(*Index);
	return 1;
}
#endif	// _BitScanReverse64

double __cdecl modf(double x, double *intptr)
{
#if defined(__LITTLE_ENDIAN__)
	#define LSW(x) ((uint32_t *)&(x))[0]
	#define MSW(x) ((uint32_t *)&(x))[1]
#else
	#define LSW(x) ((uint32_t *)&(x))[1]
	#define MSW(x) ((uint32_t *)&(x))[0]
#endif
	#define ULL(x) (*(uint64_t *)&(x))
	#define DBL_MANT_BIT  52
	#define DBL_ONE       0x3FF0000000000000
	#define DBL_EXP_BIAS  (DBL_ONE >> DBL_MANT_BIT)
	#define MSW_SIGN_MASK 0x80000000
	#define MSW_EXP_MASK  0x7FF00000
	#define MSW_MIN_EXP   0x00100000
	#define MSW_MANT_MASK 0x000FFFFF
	#define MSW_MANT_BIT  (DBL_MANT_BIT - 32)
	#define MSW_ONE       (DBL_ONE >> 32)

	uint32_t lsw, msw;
	uint32_t exp;
	uint32_t index;

	lsw = LSW(x);
	msw = MSW(x);
	exp = msw & MSW_EXP_MASK;
	if ((int32_t)(exp -= MSW_ONE) < 0)
	{
		// |x| < 1
		LSW(*intptr) = 0;
		MSW(*intptr) = msw & MSW_SIGN_MASK;
	}
	else if (exp >= (DBL_MANT_BIT * MSW_MIN_EXP))
	{
		// |x| >= (1 << 52)
		goto INTEGER;
	}
	else
	{
		// 1 <= |x| < (1 << 52)
		if (exp <= (MSW_MANT_BIT * MSW_MIN_EXP))
		{
			if (exp != (MSW_MANT_BIT * MSW_MIN_EXP) && !lsw)
			{
				goto INTEGER;
			}
			msw &= (int32_t)~MSW_MANT_MASK >> (exp >> MSW_MANT_BIT);
			goto INTEGER_AND_FRACTION;
		}
		lsw &= UINT32_MAX << (DBL_MANT_BIT - (exp >> MSW_MANT_BIT));
		if (lsw == LSW(x))
		{
		INTEGER:
			LSW(*intptr) = lsw;
			MSW(*intptr) = msw;
			LSW(x) = 0;
			MSW(x) = msw & MSW_SIGN_MASK;
		}
		else
		{
		INTEGER_AND_FRACTION:
			LSW(*intptr) = lsw;
			MSW(*intptr) = msw;
			ULL(x) -= ULL(*intptr);
			if (_BitScanReverse64(&index, ULL(x)))
			{
				ULL(x) <<= DBL_MANT_BIT - index;
				MSW(x) &= MSW_MANT_MASK;
				MSW(x) |= (index + DBL_EXP_BIAS - DBL_MANT_BIT) << MSW_MANT_BIT;
			}
			MSW(x) |= msw & MSW_SIGN_MASK;
		}
	}
	return x;

	#undef LSW
	#undef MSW
	#undef ULL
	#undef DBL_ONE
	#undef DBL_EXP_BIAS
	#undef DBL_MANT_BIT
	#undef MSW_SIGN_MASK
	#undef MSW_EXP_MASK
	#undef MSW_MIN_EXP
	#undef MSW_MANT_MASK
	#undef MSW_MANT_BIT
	#undef MSW_ONE
}
#endif	// _WIN64
#else	// _M_IX86
__declspec(naked) double __cdecl modf(double x, double *intptr)
{
#if PREFER_ALU
#if defined(__LITTLE_ENDIAN__)
	#define OFFSET_LSW 0
	#define OFFSET_MSW 4
#else
	#define OFFSET_LSW 4
	#define OFFSET_MSW 0
#endif
	#define DBL_MANT_BIT  52
	#define DBL_EXP_BIAS  3FFh
	#define MSW_SIGN_MASK 80000000h
	#define MSW_EXP_MASK  7FF00000h
	#define MSW_MIN_EXP   00100000h
	#define MSW_MANT_MASK 000FFFFFh
	#define MSW_MANT_BIT  (DBL_MANT_BIT - 32)
	#define MSW_ONE       3FF00000h

	__asm
	{
		#define x      (esp + 4)
		#define lsw    (x + OFFSET_LSW)
		#define msw    (x + OFFSET_MSW)
		#define intptr (esp + 12)

		mov     eax, dword ptr [lsw]
		mov     ecx, dword ptr [msw]
		mov     edx, ecx
		and     ecx, MSW_EXP_MASK
		sub     ecx, MSW_ONE
		jae     L1
		mov     eax, dword ptr [intptr]
		and     edx, MSW_SIGN_MASK
		mov     dword ptr [eax + OFFSET_LSW], 0
		mov     dword ptr [eax + OFFSET_MSW], edx
		jmp     L8

		align   16
	L1:
		cmp     ecx, DBL_MANT_BIT * MSW_MIN_EXP
		jae     L4
		cmp     ecx, MSW_MANT_BIT * MSW_MIN_EXP
		ja      L3
		je      L2
		test    eax, eax
		jz      L4
	L2:
		shr     ecx, MSW_MANT_BIT
		mov     eax, 0FFF00000h
		sar     eax, cl
		xor     ecx, ecx
		and     edx, eax
		jmp     L5

		align   16
	L3:
		shr     ecx, MSW_MANT_BIT
		or      eax, -1
		neg     ecx
		add     ecx, DBL_MANT_BIT
		shl     eax, cl
		mov     ecx, dword ptr [lsw]
		and     ecx, eax
		mov     eax, dword ptr [lsw]
		cmp     ecx, eax
		jne     L5
	L4:
		mov     ecx, dword ptr [intptr]
		mov     dword ptr [ecx + OFFSET_LSW], eax
		mov     dword ptr [ecx + OFFSET_MSW], edx
		and     edx, MSW_SIGN_MASK
		mov     dword ptr [lsw], 0
		mov     dword ptr [msw], edx
		jmp     L8

		align   16
	L5:
		mov     eax, dword ptr [intptr]
		push    esi
		mov     dword ptr [eax + OFFSET_LSW], ecx
		mov     dword ptr [eax + OFFSET_MSW], edx
		mov     esi, edx
		mov     eax, dword ptr [lsw + 4]
		mov     edx, dword ptr [msw + 4]
		sub     eax, ecx
		sbb     edx, esi
		and     esi, MSW_SIGN_MASK
		bsr     ecx, edx
		lea     ecx, [ecx + 32]
		jnz     L6
		bsr     ecx, eax
		jz      L7
	L6:
		push    edi
		mov     edi, ecx
		add     edi, DBL_EXP_BIAS - DBL_MANT_BIT
		xor     ecx, -1
		shl     edi, MSW_MANT_BIT
		add     ecx, DBL_MANT_BIT + 1
		shld    edx, eax, cl
		shl     eax, cl
		and     edx, MSW_MANT_MASK
		or      edx, edi
		pop     edi
	L7:
		or      edx, esi
		pop     esi
		mov     dword ptr [lsw], eax
		mov     dword ptr [msw], edx
	L8:
		fld     qword ptr [x]
		ret

		#undef x
		#undef lsw
		#undef msw
		#undef intptr
	}

	#undef OFFSET_LSW
	#undef OFFSET_MSW
	#undef DBL_MANT_BIT
	#undef DBL_EXP_BIAS
	#undef MSW_SIGN_MASK
	#undef MSW_EXP_MASK
	#undef MSW_MIN_EXP
	#undef MSW_MANT_MASK
	#undef MSW_MANT_BIT
	#undef MSW_ONE
#else
	extern const unsigned int fpconst_x0F63;
	#define _x0F63 fpconst_x0F63

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		mov     ecx, dword ptr [esp + 12]       ; Put integer address in ecx
		fld     st(0)                           ; Duplicate st
		fstcw   word ptr [esp - 4]              ; Save control word
		fclex                                   ; Clear exceptions
		fldcw   word ptr [_x0F63]               ; Set new rounding control
		frndint                                 ; Round to integer
		fclex                                   ; Clear exceptions
		fldcw   word ptr [esp - 4]              ; Restore control word
		fst     qword ptr [ecx]                 ; Store integer part
		fsub                                    ; Subtract to get fraction
		ret
	}

	#undef _x0F63
#endif
}
#endif	// _M_IX86
