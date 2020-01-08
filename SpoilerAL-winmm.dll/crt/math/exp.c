#define SMALL 0

#ifndef _M_IX86
#include <longdouble.h>
#include <float.h>
#include <math.h>
#include <errno.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

EXTERN_C double __cdecl exp(double x)
{
	// log2(e)    1.442695040888963407359924681001892137426645954152985934135
	#define L2E_A 1.442687988281250000000000000000000000000000000000000000000	// 0x3FF7154000000000
	#define L2E_B 0.000007052607713407359924681001892137426645954152985934135	// 0x3EDD94AE0BF85DDF

	if (_finite(x))
	{
		if (x)
		{
			#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
			#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

			uint16_t   cw;
			longdouble a, b, i, n;

#ifdef __cplusplus
			cw = longdouble::fstcw();
			longdouble::fldcw((cw & CW_MASK) | CW_NEW);
			a = (longdouble)x * L2E_A;
			n = a.frndint();
			a -= n;
			b = (longdouble)x * L2E_B;
			i = b.frndint();
			n += i;
			b -= i;
			a += b;
			i = a.frndint();
			n += i;
			a -= i;
			a = a.f2xm1();
			++a;
			x = (double)a.fscale(n);
			longdouble::fldcw(cw);
#else
			cw = _fstcw();
			_fldcw((cw & CW_MASK) | CW_NEW);
			a = _fmul(_fld_r8(x), _fld_r8(L2E_A));
			n = _frndint(a);
			a = _fsub(a, n);
			b = _fmul(_fld_r8(x), _fld_r8(L2E_B));
			i = _frndint(b);
			n = _fadd(n, i);
			b = _fsub(b, i);
			a = _fadd(a, b);
			i = _frndint(a);
			n = _fadd(n, i);
			a = _fsub(a, i);
			a = _f2xm1(a);
			a = _finc(a);
			a = _fscale(a, n);
			x = _fst_r8(a);
			_fldcw(cw);
#endif
			if (fabs(x) > DBL_MAX)
			{
				errno = ERANGE;
			}

			#undef CW_MASK
			#undef CW_NEW
		}
		else
		{
			x = 1;
		}
	}
	else if (x == -INFINITY)
	{
		x = 0;
	}
	return x;

	#undef L2E_A
	#undef L2E_B
}
#elif !SMALL
#include <errno.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

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

EXTERN_C const double fpconst_one;
#define _one fpconst_one

EXTERN_C __declspec(naked) double __cdecl exp(double x)
{
	double __cdecl _CIexp(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIexp
	}
}

EXTERN_C __declspec(naked) double __cdecl _CIexp(/*st0 x*/)
{
	// log2(e) ................ 1.442695040888963407359924681001892137426645954152985934135
	static const double l2e_a = 1.442687988281250000000000000000000000000000000000000000000;	// 0x3FF7154000000000
	static const double l2e_b = 0.000007052607713407359924681001892137426645954152985934135;	// 0x3EDD94AE0BF85DDF

	#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
	#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp]         /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp]         /* Load x */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		test    ah, 00000001B               ; NaN or infinity ?
		jnz     L3                          ; Re-direct if x is NaN or infinity
		sub     esp, 12                     ; Allocate temporary space
		fnstcw  word ptr [esp + 8]          ; Save control word
		mov     cx, word ptr [esp + 8]      ; Modify control word
		and     cx, CW_MASK                 ;
		or      cx, CW_NEW                  ;
		mov     word ptr [esp], cx          ; Set new control word
		fldcw   word ptr [esp]              ;
		fld     st(0)                       ; Duplicate x
		fmul    qword ptr [l2e_a]           ; Multiply:                     a = (long double)x * l2e_a
		fld     st(0)                       ; Duplicate a
		frndint                             ; Round to integer:             n = nearbyintl(a)
		fsub    st(1), st(0)                ; Subtract:                     a -= n
		fxch    st(2)                       ; Swap st, st(2)
		fmul    qword ptr [l2e_b]           ; Multiply:                     b = (long double)x * l2e_b
		fld     st(0)                       ; Duplicate b
		frndint                             ; Round to integer:             i = nearbyintl(b)
		fadd    st(3), st(0)                ; Add:                          n += i
		fsub                                ; Subtract:                     b -= i
		fadd                                ; Add:                          a += b
		fld     st(0)                       ; Duplicate a
		frndint                             ; Round to integer:             i = nearbyintl(a)
		fadd    st(2), st(0)                ; Add:                          n += i
		fsub                                ; Subtract:                     a -= i
		f2xm1                               ; Compute 2 to the (x - 1):     a = exp2l(a)
		fadd    qword ptr [_one]            ; 2 to the x
		fscale                              ; Scale by power of 2:          a = ldexpl(a, n)
		fstp    st(1)                       ; Set new stack top and pop
		fst     qword ptr [esp]             ; Save x, cast to qword
		fld     qword ptr [esp]             ; Load x
		fldcw   word ptr [esp + 8]          ; Restore control word
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Infinity ?
		je      L1                          ; Re-direct if x is infinity
		fstp    st(0)                       ; Set new top of stack
		jmp     L2                          ; End of case

		align   16
	L1:
		fstp    st(1)                       ; Set new stack top and pop
		set_errno(ERANGE)                   ; Set range error (ERANGE)
	L2:
		add     esp, 12                     ; Deallocate temporary space
		ret

		align   16
	L3:
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Not infinity ?
		jne     L4                          ; Re-direct if x is not infinity
		ftst                                ; Compare x with zero
		fstsw   ax                          ; Get the FPU status word
		sahf                                ; Store AH into Flags
		ja      L4                          ; Re-direct if x > 0
		fstp    st(0)                       ; Set new top of stack
		fldz                                ; Set result to zero
	L4:
		ret
	}

	#undef CW_MASK
	#undef CW_NEW
	#undef set_errno
}
#else
#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

EXTERN_C const double fpconst_one;
#define _one fpconst_one

EXTERN_C __declspec(naked) double __cdecl exp(double x)
{
	double __cdecl _CIexp(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIexp
	}
}

EXTERN_C __declspec(naked) double __cdecl _CIexp(/*st0 x*/)
{
	__asm
	{
		fldl2e                              ; Load log base 2(e)
		fmul                                ; Multiply x * log base 2(e)
		fld     st(0)                       ; Duplicate result
		frndint                             ; Round to integer
		fsub    st(1), st(0)                ; Subtract
		fxch                                ; Exchange st, st(1)
		f2xm1                               ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]            ; 2 to the x
		fscale                              ; Scale by power of 2
		fstp    st(1)                       ; Set new stack top and pop
		ret
	}
}
#endif
