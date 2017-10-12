#ifndef _M_IX86
#include <float.h>
#include <math.h>
#include <errno.h>

double __cdecl ldexp10(double x, int exp)
{
	// log2(10)   3.321928094887362347870319429489390175864831393024580612054
	#define L2T_A 3.321899414062500000000000000000000000000000000000000000000	// 0x400A934000000000
	#define L2T_B 0.000028680824862347870319429489390175864831393024580612054	// 0x3EFE12F346E2BF92

	if (!_isnan(x))
	{
		if (x && exp)
		{
			double f1, f2, i1, i2;
			int    n;

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
			n += (int)i1;
			f1 -= i1;
			x *= exp2(f1);
			x = ldexp(x, n);
			if (!x || fabs(x) > DBL_MAX)
				errno = ERANGE;
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

errno_t * __cdecl _errno();

__declspec(naked) double __cdecl ldexp10(double x, int exp)
{
	// log2(10) ............... 3.321928094887362347870319429489390175864831393024580612054
	static const double l2t_a = 3.321899414062500000000000000000000000000000000000000000000;	// 0x400A934000000000
	static const double l2t_b = 0.000028680824862347870319429489390175864831393024580612054;	// 0x3EFE12F346E2BF92

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L1                      ; Re-direct if x == 0
		cmp     ah, 00000001B           ; NaN ?
		je      L2                      ; Re-direct if x is NaN
		cmp     dword ptr [esp + 12], 0 ; Compare exp with zero
		je      L1                      ; Re-direct if exp == 0
		fxtract                         ; Get exponent and significand: x = frexp(x, &n)
		fld1                            ; Load constant 1
		fld1                            ; Load constant 1
		fadd                            ; Constant 2
		fdiv                            ; Significand / 2
		fxch                            ; Swap st, st(1)
		fld1                            ; Load constant 1
		fadd                            ; Increment exponent
		fild    dword ptr [esp + 12]    ; Load exp as integer
		fmul    qword ptr [l2t_a]       ; Multiply:                     f1 = exp * l2t_a
		fild    dword ptr [esp + 12]    ; Load exp as integer
		fmul    qword ptr [l2t_b]       ; Multiply:                     f2 = exp * l2t_b
		fld     st(1)                   ; Duplicate f1
		frndint                         ; Round to integer:             i1 = round(f1)
		fld     st(1)                   ; Duplicate f2
		frndint                         ; Round to integer:             i2 = round(f2)
		fld     st(1)                   ; Duplicate i1
		fadd    st(0), st(1)            ; Add:                          n += i1 + i2
		faddp   st(5), st(0)            ; Add
		fsubp   st(2), st(0)            ; Subtract:                     f2 -= i2
		fsubp   st(2), st(0)            ; Subtract:                     f1 -= i1
		fadd    st(0), st(1)            ; Add:                          f1 += f2
		fst     st(1)                   ; Push f1
		frndint                         ; Round to integer:             i1 = round(f1)
		fadd    st(2), st(0)            ; Add:                          n += i1
		fsub                            ; Subtract:                     f1 -= i1
		f2xm1                           ; Compute 2 to the (x - 1):     x *= exp2(f1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fmul    st(0), st(2)            ; Multiply
		fstp    st(2)                   ; Set new stack top and pop
		fscale                          ; Scale by power of 2:          x = ldexp(x, n)
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp + 4]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 00000101B           ; Infinity ?
		je      L3                      ; Re-direct if x is infinity (overflow)
		cmp     ah, 01000000B           ; Zero ?
		je      L3                      ; Re-direct if x is zero (underflow)
	L1:
		ret
	L2:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], EDOM   ; Set domain error (EDOM)
		ret
	L3:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ERANGE ; Set range error (ERANGE)
		ret
	}
}
#endif
