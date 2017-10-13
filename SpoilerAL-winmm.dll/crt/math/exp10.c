#define SMALL 0

#ifndef _M_IX86
#include <math.h>
#include <errno.h>

double __cdecl exp10(double x)
{
	// log2(10)     3.321928094887362347870319429489390175864831393024580612054
	#define L2T_A   3.321899414062500000000000000000000000000000000000000000000	// 0x400A934000000000
	#define L2T_B   0.000028680824862347870319429489390175864831393024580612054	// 0x3EFE12F346E2BF92

	#define MAXL10  308.2547155599167438988686281978808594106264386171991463018	// DBL_MAX_EXP / log2(10)
	#define MINL10 -324.2093053301077472451967896182789898293405023346908989913	// -(DBL_MAX_EXP + DBL_MANT_DIG) / log2(10)

	if (!_isnan(x))
	{
		if (x < MAXL10)
		{
			if (x > MINL10)
			{
				double f1, f2, i1, i2, n;

				f1 = x * L2T_A;
				f2 = x * L2T_B;
				i1 = round(f1);
				i2 = round(f2);
				n = i1 + i2;
				f1 -= i1;
				f2 -= i2;
				f1 += f2;
				i1 = round(f1);
				n += i1;
				f1 -= i1;
				x = exp2(f1);
				x = ldexp(x, (int)n);
			}
			else
			{
				errno = ERANGE;
				x = 0;
			}
		}
		else
		{
			errno = ERANGE;
			x = HUGE_VAL;
		}
	}
	return x;

	#undef L2T_A
	#undef L2T_B
	#undef MAXL10
	#undef MINL10
}
#elif !SMALL
#include <errno.h>

errno_t * __cdecl _errno();

__declspec(naked) double __cdecl exp10(double x)
{
	// log2(10) ............... 3.321928094887362347870319429489390175864831393024580612054
	static const double l2t_a = 3.321899414062500000000000000000000000000000000000000000000;	// 0x400A934000000000
	static const double l2t_b = 0.000028680824862347870319429489390175864831393024580612054;	// 0x3EFE12F346E2BF92

	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		cmp     ah, 01000000B           ; Zero ?
		je      L1                      ; Re-direct if x == 0
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L2                      ; Re-direct if x is NaN or infinity
		fmul    qword ptr [l2t_a]       ; Multiply:                 f1 = x * l2t_a
		fld     qword ptr [esp + 4]     ; Load real from stack
		fmul    qword ptr [l2t_b]       ; Multiply:                 f2 = x * l2t_b
		fld     st(1)                   ; Duplicate f1
		frndint                         ; Round to integer:         i1 = round(f1)
		fld     st(1)                   ; Duplicate f2
		frndint                         ; Round to integer:         i2 = round(f2)
		fld     st(1)                   ; Duplicate i1
		fadd    st(0), st(1)            ; Add:                      n = i1 + i2
		fxch    st(4)                   ; Exchange st, st(4)
		fsubrp  st(2), st(0)            ; Subtract:                 f1 -= i1
		fsubp   st(2), st(0)            ; Subtract:                 f2 -= i2
		fadd    st(0), st(1)            ; Add:                      f1 += f2
		fst     st(1)                   ; Push f1
		frndint                         ; Round to integer:         i1 = round(f1)
		fadd    st(2), st(0)            ; Add:                      n += i1
		fsubp   st(1), st(0)            ; Subtract:                 f1 -= i1
		f2xm1                           ; Compute 2 to the (x - 1): x *= exp2(f1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2:      x = ldexp(x, n)
		fstp    st(1)                   ; Set new stack top and pop
		fstp    qword ptr [esp + 4]     ; Save x, 'fxam' is require the load memory
		fld     qword ptr [esp + 4]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate  C0, C2 and C3
		test    ah, 00000001B           ; NaN or infinity ?
		jnz     L3                      ; Re-direct if x is NaN or infinity (overflow)
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
#else
__declspec(naked) double __cdecl exp10(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fldl2t                          ; Load log base 2(10)
		fmulp   st(1), st(0)            ; Multiply x * log base 2(10)
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
