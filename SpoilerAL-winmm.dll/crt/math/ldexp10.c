#ifndef _M_IX86
#include <float.h>
#include <math.h>
#include <errno.h>

double __cdecl ldexp10(double x, int exp)
{
	// log2(10)   3.321928094887362347870319429489390175864831393024580612054
	#define L2T_A 3.321899414062500000000000000000000000000000000000000000000	// 0x400A934000000000
	#define L2T_B 0.000028680824862347870319429489390175864831393024580612054	// 0x3EFE12F346E2BF92

	#define DBL_RND_FIX 0.99999999999999988897769753748434595763683319091796875
	#define FLT_RND_FIX 0.99999994039535533563167746251565404236316680908203125

	if (!_isnan(x))
	{
		if (x && exp)
		{
			double f1, f2, i1, i2;
			int    n, e;

			x = frexp(x, &n);
			f1 = exp * L2T_A;
			f2 = exp * L2T_B;
			i1 = round(f1);
			i2 = round(f2);
			n += (int)(i1 + i2);
			f1 -= i1;
			f2 -= i2;
			f1 += f2;
			i1 = round(f1);
			if (i1 != f1)
			{
				n += (int)i1;
				f1 -= i1;
			}
			x *= exp2(f1);
			x = frexp(x, &e);
			n += e;
			if (n != DBL_MAX_EXP || fabs(x) < DBL_RND_FIX)
			{
				if (n != FLT_MAX_EXP || fabs(x) < FLT_RND_FIX)
				{
					x = ldexp(x, n);
					if (!x || x > DBL_MAX)
						errno = ERANGE;
				}
				else
				{
					x = x >= 0.0 ? FLT_MAX : -FLT_MAX;
				}
			}
			else
			{
				x = x >= 0.0 ? DBL_MAX : -DBL_MAX;
			}
		}
	}
	else
	{
		errno = EDOM;
	}
	return x;

	#undef L2T_A
	#undef L2T_B
}
#else
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

__declspec(naked) double __cdecl ldexp10(double x, int exp)
{
	// log2(10) ............... 3.321928094887362347870319429489390175864831393024580612054
	static const double l2t_a = 3.321899414062500000000000000000000000000000000000000000000;	// 0x400A934000000000
	static const double l2t_b = 0.000028680824862347870319429489390175864831393024580612054;	// 0x3EFE12F346E2BF92

	static const double dbl_max_exp = DBL_MAX_EXP;
	static const double dbl_max     = DBL_MAX;
	static const double dbl_rnd_fix = 0.99999999999999988897769753748434595763683319091796875;
	static const double flt_max_exp = FLT_MAX_EXP;
	static const double flt_max     = FLT_MAX;
	static const double flt_rnd_fix = 0.99999994039535511358707253748434595763683319091796875;

	#define CONTROL_MASK ~CW_RC_MASK
	#define CONTROL_WORD (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

	__asm
	{
		emms
		push    eax                     ; Allocate temporary space
		fnstcw  word ptr [esp]          ; Save control word
		mov     ax, word ptr [esp]      ; Modify control word
		and     ax, CONTROL_MASK        ;
		or      ax, CONTROL_WORD        ;
		mov     word ptr [esp + 2], ax  ;
		fldcw   word ptr [esp + 2]      ; Set new control word
		fld     qword ptr [esp + 8]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L10                     ; Re-direct if x == 0
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L6                      ; Re-direct if x is NaN or infinity
		cmp     dword ptr [esp + 16], 0 ; Compare exp with zero
		je      L10                     ; Re-direct if exp == 0
		fxtract                         ; Get exponent and significand
		fld     qword ptr [_half]       ; Load 0.5
		fmul                            ; Significand * 0.5
		fxch                            ; Swap st, st(1)
		fld1                            ; Load constant 1
		fadd                            ; Increment exponent
		fild    dword ptr [esp + 16]    ; Load exp as integer
		fmul    qword ptr [l2t_a]       ; Multiply:                     f1 = exp * l2t_a
		fild    dword ptr [esp + 16]    ; Load exp as integer
		fmul    qword ptr [l2t_b]       ; Multiply:                     f2 = exp * l2t_b
		fld     st(1)                   ; Duplicate f1
		fclex                           ; Clear exceptions
		frndint                         ; Round to integer:             i1 = round(f1)
		fld     st(1)                   ; Duplicate f2
		fclex                           ; Clear exceptions
		frndint                         ; Round to integer:             i2 = round(f2)
		fld     st(1)                   ; Duplicate i1
		fadd    st(0), st(1)            ; Add:                          n += i1 + i2
		faddp   st(5), st(0)            ; Add
		fsubp   st(2), st(0)            ; Subtract:                     f2 -= i2
		fsubp   st(2), st(0)            ; Subtract:                     f1 -= i1
		fadd    st(0), st(1)            ; Add:                          f1 += f2
		fst     st(1)                   ; Push f1
		fclex                           ; Clear exceptions
		frndint                         ; Round to integer:             i1 = round(f1)
		fcom                            ; i1 == f1 ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		je      L1                      ; Re-direct if i1 == f1
		fadd    st(2), st(0)            ; Add:                          n += i1
		fsub                            ; Subtract:                     f1 -= i1
		jmp     L2                      ; End of case
	L1:
		fstp    st(0)                   ; Set new stack top and pop
	L2:
		f2xm1                           ; Compute 2 to the (x - 1):     x *= f2xm1(f1) + 1;
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fmul    st(0), st(2)            ; Multiply
		fxtract                         ; Get exponent and significand
		fld     qword ptr [_half]       ; Load 0.5
		fmul                            ; Significand * 0.5
		fxch                            ; Swap st, st(1)
		fld1                            ; Load constant 1
		fadd                            ; Increment exponent
		faddp   st(2), st(0)            ; Add
		fxch                            ; Swap st, st(1)
		fld     qword ptr [dbl_max_exp] ; Load DBL_MAX_EXP
		fcomp                           ; DBL_MAX_EXP == n ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		je      L3                      ; Re-direct if DBL_MAX_EXP == n
		fld     qword ptr [flt_max_exp] ; Load FLT_MAX_EXP
		fcomp                           ; FLT_MAX_EXP != n ?
		fstsw   ax                      ; Get the FPU status word
		fxch                            ; Swap st, st(1)
		sahf                            ; Set flags based on test
		jne     L4                      ; Re-direct if FLT_MAX_EXP != n
		fld     qword ptr [flt_rnd_fix] ; Load 0.999...
		fld     st(1)                   ; Duplicate x
		fabs                            ; Take the absolute value
		fcompp                          ; fabs(x) < 0.999... ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		jb      L4                      ; Re-direct if fabs(x) < 0.999...
		jmp     L7                      ; End of case
	L3:
		fxch                            ; Swap st, st(1)
		fld     qword ptr [dbl_rnd_fix] ; Load 0.999...
		fld     st(1)                   ; Duplicate x
		fabs                            ; Take the absolute value
		fcompp                          ; fabs(x) >= 0.999... ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		jae     L8                      ; Re-direct if fabs(x) >= 0.999...
	L4:
		fscale                          ; Scale by power of 2:          x = ldexp(x, n);
		fstp    st(1)                   ; Set new stack top and pop
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp + 8]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 8]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L5                      ; Re-direct if x is NaN or infinity
		cmp     ah, 01000000B           ; Zero ?
		jne     L10                     ; Re-direct if x is not zero (not underflow)
	L5:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ERANGE ; Set range error (ERANGE)
		jmp     L10                     ; End of case
	L6:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], EDOM   ; Set domain error (EDOM)
		jmp     L10                     ; End of case
	L7:
		fstp    st(1)                   ; Set new stack top and pop
		ftst                            ; Compare x with zero
		fstsw   ax                      ; Put test result in ax
		fstp    st(0)                   ; Set new stack top and pop
		fld     qword ptr [flt_max]     ; Load FLT_MAX
		jmp     L9                      ; End of case
	L8:
		fstp    st(1)                   ; Set new stack top and pop
		ftst                            ; Compare x with zero
		fstsw   ax                      ; Put test result in ax
		fstp    st(0)                   ; Set new stack top and pop
		fld     qword ptr [dbl_max]     ; Load DBL_MAX
	L9:
		sahf                            ; Set flags based on test
		jae     L10                     ; Re-direct if x >= 0
		fchs                            ; Negate the DBL_MAX
	L10:
		fclex                           ; Clear exceptions
		fldcw   word ptr [esp]          ; Restore control word
		pop     eax                     ; Deallocate temporary space
		ret
	}

	#undef CONTROL_MASK
	#undef CONTROL_WORD
}
#endif
