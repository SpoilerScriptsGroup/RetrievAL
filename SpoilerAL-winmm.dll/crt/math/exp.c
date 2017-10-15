#define SMALL 0

#ifndef _M_IX86
#include <float.h>
#include <math.h>
#include <errno.h>

// this C source code is not 80 bit floating point.
// precision is less than inline assembler.
double __cdecl exp(double x)
{
	// log2(e)          1.442695040888963407359924681001892137426645954152985934135
	#define L2E_A       1.442687988281250000000000000000000000000000000000000000000	// 0x3FF7154000000000
	#define L2E_B       0.000007052607713407359924681001892137426645954152985934135	// 0x3EDD94AE0BF85DDF

	#define M_LN_MAX_D  709.7827128933839968432456923731728057093121375849013802195	// log(2) * DBL_MAX_EXP
	#define M_LN_MIN_D -708.3964185322641062244112281302564525731611373161808697113	// log(2) * (DBL_MIN_EXP - 1)

	if (x)
	{
		if (!_isnan(x))
		{
			if (fabs(x) < M_LN_MAX_D)
			{
				if (fabs(x) > M_LN_MIN_D)
				{
					double f1, f2, i1, i2, n;

					f1 = x * L2E_A;
					f2 = x * L2E_B;
					i1 = round(f1);
					i2 = round(f2);
					n = i1 + i2;
					f1 -= i1;
					f2 -= i2;
					f1 += f2;
					i1 = round(f1);
					n += i1;
					f1 -= i1;
					f1 = exp2(f1);
					x = ldexp(f1, (int)n);
					if (!x || x > DBL_MAX)
						errno = ERANGE;
				}
				else
				{
					errno = ERANGE;
					x = x >= 0 ? 0.0 : -0.0;
				}
			}
			else
			{
				errno = ERANGE;
				x = x >= 0 ? HUGE_VAL : -HUGE_VAL;
			}
		}
		else
		{
			errno = EDOM;
		}
	}
	else
	{
		x = 1;
	}
	return x;

	#undef L2E_A
	#undef L2E_B
	#undef M_LN_MAX_D
	#undef M_LN_MIN_D
}
#elif !SMALL
#include <errno.h>
#include <float.h>

errno_t * __cdecl _errno();

extern const double _half;

#define CW_EM_MASK       0x003F
#define CW_EM_INVALID    0x0001
#define CW_EM_DENORMAL   0x0002
#define CW_EM_ZERODIVIDE 0x0004
#define CW_EM_OVERFLOW   0x0008
#define CW_EM_UNDERFLOW  0x0010
#define CW_EM_INEXACT    0x0020
#define CW_EM_DEFAULT    0x003F
#define CW_RESERVED      0x0040
#define CW_RC_MASK       0x0C00
#define CW_RC_NEAR       0x0000
#define CW_RC_DOWN       0x0400
#define CW_RC_UP         0x0800
#define CW_RC_CHOP       0x0C00
#define CW_RC_DEFAULT    CW_RC_NEAR
#define CW_PC_MASK       0x0300
#define CW_PC_24         0x0100
#define CW_PC_53         0x0200
#define CW_PC_64         0x0300
#define CW_PC_DEFAULT    CW_PC_64
#define CW_IC_MASK       0x1000
#define CW_IC_PROJECTIVE 0x0000
#define CW_IC_AFFINE     0x1000
#define CW_IC_DEFAULT    CW_IC_PROJECTIVE

__declspec(naked) double __cdecl exp(double x)
{
	// log2(e) ................ 1.442695040888963407359924681001892137426645954152985934135
	static const double l2e_a = 1.442687988281250000000000000000000000000000000000000000000;	// 0x3FF7154000000000
	static const double l2e_b = 0.000007052607713407359924681001892137426645954152985934135;	// 0x3EDD94AE0BF85DDF

	#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
	#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

	__asm
	{
		emms
		sub     esp, 12                 ; Allocate temporary space
		fnstcw  word ptr [esp + 8]      ; Save control word
		mov     cx, word ptr [esp + 8]  ; Modify control word
		and     cx, CW_MASK             ;
		or      cx, CW_NEW              ;
		mov     word ptr [esp], cx      ;
		fldcw   word ptr [esp]          ; Set new control word
		fld     qword ptr [esp + 16]    ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L3                      ; Re-direct if x is NaN or infinity
	L1:
		fmul    qword ptr [l2e_a]       ; Multiply:                     f1 = (long double)x * l2e_a
		fld     qword ptr [esp + 16]    ; Load x
		fmul    qword ptr [l2e_b]       ; Multiply:                     f2 = (long double)x * l2e_b
		fld     st(1)                   ; Duplicate f1
		frndint                         ; Round to integer:             i1 = nearbyintl(f1)
		fld     st(1)                   ; Duplicate f2
		frndint                         ; Round to integer:             i2 = nearbyintl(f2)
		fld     st(1)                   ; Duplicate i1
		fld     st(1)                   ; Duplicate i2
		fadd                            ; Add:                          n = i1 + i2
		fxch    st(4)                   ; Exchange st0, st4
		fsubrp  st(2), st(0)            ; Subtract:                     f1 -= i1
		fsubp   st(2), st(0)            ; Subtract:                     f2 -= i2
		fadd    st(0), st(1)            ; Add:                          f1 += f2
		fst     st(1)                   ; Push f1
		frndint                         ; Round to integer:             i1 = nearbyintl(f1)
		fadd    st(2), st(0)            ; Add:                          n += i1
		fsub                            ; Subtract:                     f1 -= i1
		f2xm1                           ; Compute 2 to the (x - 1):     f1 = exp2l(f1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2:          x = ldexpl(f1, n)
		fstp    st(1)                   ; Set new stack top and pop
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp]         ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp]         ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L2                      ; Re-direct if x is zero
		cmp     ah, 00000101B           ; Not infinity ?
		jne     L4                      ; Re-direct if x is not infinity
		mov     ax, cx                  ; Control word has not CW_RC_CHOP ?
		and     ax, CW_RC_MASK          ;
		cmp     ax, CW_RC_CHOP          ;
		je      L2                      ; Re-direct if control word has not CW_RC_CHOP
		or      cx, CW_RC_CHOP          ; Modify control word
		mov     word ptr [esp], cx      ;
		fldcw   word ptr [esp]          ; Set new control word
		fstp    st(0)                   ; Set new stack top and pop
		fld     qword ptr [esp + 16]    ; Load x
		jmp     L1                      ; End of case
	L2:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ERANGE ; Set range error (ERANGE)
		jmp     L4                      ; End of case
	L3:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], EDOM   ; Set domain error (EDOM)
	L4:
		fclex                           ; Clear exceptions
		fldcw   word ptr [esp + 8]      ; Restore control word
		add     esp, 12                 ; Deallocate temporary space
		ret
	}

	#undef CW_MASK
	#undef CW_NEW
}
#else
__declspec(naked) double __cdecl exp(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldl2e                          ; Load log base 2(e)
		fmul                            ; Multiply x * log base 2(e)
		fst     st(1)                   ; Push result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st, st(1)
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		ret
	}
}
#endif
