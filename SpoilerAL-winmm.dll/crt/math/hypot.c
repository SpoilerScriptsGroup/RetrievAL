#ifndef _M_IX86
#include <math.h>
#include <float.h>
#include <stdint.h>

#pragma warning(disable:4273)

#define UI64(x) (*(uint64_t *)&(x))
#define DBL(x)  (*(double *)&(x))

double __cdecl _hypot(double x, double y)
{
	uint64_t a, b;

	/* Determine absolute values. */
	a = UI64(x) & INT64_MAX;
	b = UI64(y) & INT64_MAX;

	/* x is finite ? */
	if (a < 0x7FF0000000000000)
	{
		int e;

		/* y is not finite ? */
		if (b >= 0x7FF0000000000000)
			goto check_if_y_is_infinity;

		/* Find the bigger and the smaller one. */
		if (a >= b)
		{
			x = DBL(a);
			y = DBL(b);
		}
		else
		{
			x = DBL(b);
			y = DBL(a);
		}
		/* Now 0 <= y <= x. */

		/* Write x = x * 2^e, y = y * 2^e with 0 <= y <= x < 1. */
		x = frexp(x, &e);
		y = ldexp(y, -(e));

		/* Through the normalization, no unneeded overflow or underflow
		   will occur here. */
		return ldexp(sqrt(x * x + y * y), e);
	}

	/* x is infinity ? */
	if (a == 0x7FF0000000000000)
		goto return_positive_infinity;

check_if_y_is_infinity:
	/* y is infinity ? */
	if (b == 0x7FF0000000000000)
		goto return_positive_infinity;

	/* return the larger signaling NaN if x or y is NaN. */
#if 0
	return x + y;	// Not add due to compiler optimization.
#elif !defined(_M_IX86)
	return (a | 0x0008000000000000) >= (b | 0x0008000000000000) ? x : y;
#else
	{ { __asm fld x __asm fld y __asm fadd __asm fstp x } return x; }
#endif

return_positive_infinity:
	return HUGE_VAL;
}
#else
__declspec(naked) double __cdecl _hypot(double x, double y)
{
	extern const double fpconst_half;
	extern const double fpconst_one;
	extern const double fpconst_inf;
	#define _half fpconst_half
	#define _one  fpconst_one
	#define _inf  fpconst_inf

	__asm
	{
		#define x      (esp + 4)
		#define y      (esp + 12)
		#define LSW(x) ((x) + 0)
		#define MSW(x) ((x) + 4)

		push    esi
		push    edi
		mov     esi, dword ptr [LSW(x) + 8]     ; a = (x as uint64_t)
		mov     eax, dword ptr [MSW(x) + 8]     ;
		mov     edi, dword ptr [LSW(y) + 8]     ; b = (y as uint64_t)
		mov     ecx, dword ptr [MSW(y) + 8]     ;
		and     eax, 7FFFFFFFH                  ; a &= INT64_MAX
		and     ecx, 7FFFFFFFH                  ; b &= INT64_MAX
		cmp     eax, 7FF00000H                  ; x is not finite ?
		jae     L3                              ; Re-direct if x is not finite
		cmp     ecx, 7FF00000H                  ; y is not finite ?
		jae     L4                              ; Re-direct if y is not finite
		mov     edx, eax                        ; a < b ?
		cmp     esi, edi                        ;
		sbb     edx, ecx                        ;
		jb      L1                              ; Re-direct if a < b
		mov     dword ptr [esp - 16], esi       ; x = a as double
		mov     dword ptr [esp - 12], eax       ;
		mov     dword ptr [esp -  8], edi       ; y = b as double
		mov     dword ptr [esp -  4], ecx       ;
		jmp     L2                              ; End of case

		align   16
	L1:
		mov     dword ptr [esp - 16], edi       ; x = b as double
		mov     dword ptr [esp - 12], ecx       ;
		mov     dword ptr [esp -  8], esi       ; y = a as double
		mov     dword ptr [esp -  4], eax       ;
	L2:
		fld     qword ptr [esp - 16]            ; Load x
		fxtract                                 ; Get exponent and significand
		fmul    qword ptr [_half]               ; Significand * 0.5
		fmul    st(0), st(0)                    ; Compute x * x
		fxch                                    ; Swap st, st(1)
		fadd    qword ptr [_one]                ; Increment exponent
		fchs                                    ; Negate the exponent
		fld     qword ptr [esp - 8]             ; Load y
		fscale                                  ; Scale by power of 2
		fmul    st(0), st(0)                    ; Compute y * y
		faddp   st(2), st(0)                    ; Compute x * x + y * y
		fchs                                    ; Negate the exponent
		fxch                                    ; Exchange st, st(1)
		fsqrt                                   ; Take the square root
		fscale                                  ; Scale by power of 2
		fstp    st(1)                           ; Set new stack top and pop
#if MSC_COMPATIBLE
		fnstcw  word ptr [esp - 4]              ; Qword rounding
		mov     ax, word ptr [esp - 4]          ;
		or      ax, 0C00h                       ;
		mov     word ptr [esp - 8], ax          ;
		fldcw   word ptr [esp - 8]              ;
		fstp    qword ptr [esp - 12]            ;
		fld     qword ptr [esp - 12]            ;
		fldcw   word ptr [esp - 4]              ;
#endif
		pop     edi
		pop     esi
		ret

		align   16
	L3:
		xor     eax, 7FF00000H                  ; x is infinity ?
		or      eax, esi                        ;
		jz      L5                              ; Re-direct if x is infinity
	L4:
		xor     ecx, 7FF00000H                  ; y is infinity ?
		or      ecx, edi                        ;
		jz      L5                              ; Re-direct if y is infinity
		fld     qword ptr [x + 8]               ; Load x
		fld     qword ptr [y + 8]               ; Load y
		fadd                                    ; Compute x + y
		pop     edi
		pop     esi
		ret

		align   16
	L5:
		fld     qword ptr [_inf]                ; Set result to infinity
		pop     edi
		pop     esi
		ret

		#undef x
		#undef y
		#undef LSW
		#undef MSW
	}

	#undef _half
	#undef _one
	#undef _inf
}
#endif
