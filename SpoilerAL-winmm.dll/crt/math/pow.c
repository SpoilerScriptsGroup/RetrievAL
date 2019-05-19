#if !defined(_M_IX86)
#include <math.h>
#include <float.h>
#include <errno.h>

#define USE_LONGDOUBLE 1

#if USE_LONGDOUBLE
#include "longdouble.h"
#endif

double __cdecl pow(double x, double y)
{
	if (y)
	{
		if (x)
		{
			double dummy;

			if (x >= 0 || !modf(y, &dummy))
			{
#if USE_LONGDOUBLE
				#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
				#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)
#endif

				unsigned char sign;
#if USE_LONGDOUBLE
				uint16_t      cw;
				longdouble    s, e, a;
#endif

				sign = 0;
				if (x < 0)
				{
					x = -x;
					if (modf(y * 0.5, &dummy))
					{
						sign = 1;
					}
				}
#if !USE_LONGDOUBLE
				x = log(x);
				if (x < 0)
				{
					x = -x;
					y = -y;
				}
				if (x <= 1 || y <= DBL_MAX / x)
				{
					x = exp(x * y);
				}
				else
				{
					errno = ERANGE;
					x = HUGE_VAL;
				}
#else
#if defined(__cplusplus)
				cw = longdouble::fstcw();
				longdouble::fldcw((cw & CW_MASK) | CW_NEW);
				s = x;
				s = s.fxtract(&e);
				s = s.fyl2x(1);
				a = e * y;
				e = s * y;
				e = e + a;
				e = e.frndint();
				a = e - a;
				a = a / y;
				s = s - a;
				s = s * y;
				s = s.f2xm1();
				s = s + 1;
				s = s.fscale(e);
				x = (double)s;
				longdouble::fldcw(cw);
#else
				cw = _fstcw();
				_fldcw((cw & CW_MASK) | CW_NEW);
				s = _fld_r8(x);
				s = _fxtract(s, &e);
				s = _fyl2x(s, _fld1());
				a = _fmul(e, _fld_r8(y));
				e = _fmul(s, _fld_r8(y));
				e = _fadd(e, a);
				e = _frndint(e);
				a = _fsub(e, a);
				a = _fdiv(a, _fld_r8(y));
				s = _fsub(s, a);
				s = _fmul(s, _fld_r8(y));
				s = _f2xm1(s);
				s = _fadd(s, _fld1());
				s = _fscale(s, e);
				x = _fst_r8(s);
				_fldcw(cw);
#endif
				if (fabs(x) > DBL_MAX)
				{
					errno = ERANGE;
				}
#endif
				if (sign)
				{
					x = -x;
				}

#if USE_LONGDOUBLE
				#undef CW_MASK
				#undef CW_NEW
#endif
			}
			else
			{
				x = 0;
				errno = EDOM;
			}
		}
		else if (y < 0)
		{
			errno = EDOM;
		}
	}
	else
	{
		x = 1;
	}
	return x;
}
#else
#include <errno.h>

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

__declspec(naked) double __cdecl pow(double x, double y)
{
	double __cdecl _CIpow(/*st1 x, st0 y*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fld     qword ptr [esp + 12]        ; Load real from stack
		jmp     _CIpow
	}
}

__declspec(naked) double __cdecl _CIpow(/*st1 x, st0 y*/)
{
	extern const double _half;
	extern const double _one;

	#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
	#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   mov     byte ptr [esp + 2], dl  /* Save negation flag */ \
		__asm   fstp    qword ptr [esp + 4]     /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */ \
		__asm   fld     qword ptr [esp + 4]     /* Load x */ \
		__asm   mov     dl, byte ptr [esp + 2]  /* Load negation flag */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		ftst                                ; Compare y with zero
		fstsw   ax                          ; Get the FPU status word
		sahf                                ; Store AH into Flags
		jne     L1                          ; Re - direct if y != 0
		fld1                                ; Load real number 1
		jmp     L9                          ; End of case
	L1:
		sub     esp, 12                     ; Allocate temporary space
		xor     edx, edx                    ; Set negation flag to zero
		fnstcw  word ptr [esp]              ; Save control word
		mov     word ptr [esp + 4], ax      ; Save flags of compare y with zero
		mov     cx, word ptr [esp]          ;
		fxch                                ; Swap st, st(1)
		ftst                                ; Compare x with zero
		fstsw   ax                          ; Get the FPU status word
		sahf                                ; Store AH into Flags
		jb      L2                          ; Re-direct if x < 0
		ja      L5                          ; Re-direct if x > 0
		fstsw   word ptr [esp + 4]          ; Get the FPU status word
		sahf                                ; Store AH into Flags
		jb      L3                          ; Re-direct if y < 0
		jmp     L8                          ; End of case
	L2:
		fld     st(1)                       ; Duplicate y as st1
		frndint                             ; Round to integer
		fcomip  st(0), st(2)                ; y = int(y) ?
		je      L4                          ; Proceed if y = int(y)
		fldz                                ; Set result to zero
	L3:
		set_errno(EDOM)                     ; Set domain error (EDOM)
		jmp     L8                          ; End of case
	L4:
		or      cx, CW_RC_CHOP              ; Modify control word
		mov     word ptr [esp + 4], cx      ;
		fldcw   word ptr [esp + 4]          ; Set new control word
		fchs                                ; Set x = -x
		fld     st(1)                       ; Duplicate y as st1
		fmul    qword ptr [_half]           ; Compute y * 0.5
		fld     st(0)                       ; Duplicate result
		frndint                             ; Round to integer
		fsub                                ; Subtract
		ftst                                ; Compare result with zero
		fstsw   ax                          ; Get the FPU status word
		fstp    st(0)                       ; Set new top of stack
		sahf                                ; Store AH into Flags
		setnz   dl                          ; Set bit if y is odd
	L5:
		and     cx, CW_MASK                 ; Modify control word
		or      cx, CW_NEW                  ;
		mov     word ptr [esp + 4], cx      ;
		fldcw   word ptr [esp + 4]          ; Set new control word
		fld     st(0)                       ; Duplicate x
		fxtract                             ; Get exponent and significand  s = significand, e = exponent
		fld1                                ; Load real number 1
		fxch                                ; Swap st, st(1)
		fyl2x                               ; Compute the natural log(x)    s = fyl2x(s, 1)
		fxch                                ; Swap st, st(1)
		fmul    st(0), st(3)                ; Multiply                      a = e * y
		fld     st(1)                       ; Duplicate s
		fmul    st(0), st(4)                ; Multiply                      e = s * y
		fadd    st(0), st(1)                ; Add                           e += a
		frndint                             ; Round to integer              e = frndint(e)
		fxch                                ; Swap st, st(1)
		fsubr   st(0), st(1)                ; Subtract                      a = e - a
		fdiv    st(0), st(4)                ; Divide                        a /= y
		fsubp   st(2), st(0)                ; Subtract                      s -= a
		fxch                                ; Swap st, st(1)
		fmul    st(0), st(3)                ; Multiply                      s *= y
		f2xm1                               ; Compute 2 to the (x - 1)      s = f2xm1(s)
		fadd    qword ptr [_one]            ; Add                           s += 1
		fscale                              ; Scale by power of 2           x = fscale(s, e)
		fstp    st(1)                       ; Set new stack top and pop
		fst     qword ptr [esp + 4]         ; Save x, cast to qword
		fld     qword ptr [esp + 4]         ; Load x
		fldcw   word ptr [esp]              ; Restore control word
		fxam                                ; Examine st
		fstsw   ax                          ; Get the FPU status word
		and     ah, 01000101B               ; Isolate C0, C2 and C3
		cmp     ah, 00000101B               ; Infinity ?
		je      L6                          ; Re-direct if x is infinity
		fstp    st(0)                       ; Set new top of stack
		jmp     L7                          ; End of case
	L6:
		fstp    st(1)                       ; Set new stack top and pop
		set_errno(ERANGE)                   ; Set range error (ERANGE)
	L7:
		test    dl, dl                      ; Negation required ?
		jz      L8                          ; No, re-direct
		fchs                                ; Negate the result
	L8:
		add     esp, 12                     ; Deallocate temporary space
	L9:
		fstp    st(1)                       ; Set new stack top and pop
		fstp    st(1)                       ; Set new stack top and pop
		ret
	}

	#undef CW_MASK
	#undef CW_NEW
	#undef set_errno
}
#endif
