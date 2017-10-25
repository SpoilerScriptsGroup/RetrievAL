#pragma function(pow)

#if !defined(_M_IX86)
#include <math.h>
#include <float.h>
#include <errno.h>

#pragma function(log)
#pragma function(exp)

double __cdecl pow(double x, double y)
{
	double dummy;

	if (x)
	{
		if (x >= 0 || !modf(y, &dummy))
		{
			unsigned char sign;

			sign = 0;
			if (x < 0)
			{
				if (modf(y * 0.5, &dummy))
				{
					sign = 1;
				}
				x = -x;
			}
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
			if (sign)
			{
				x = -x;
			}
		}
	}
	else if (!y || (x < 0 && modf(y, &dummy)))
	{
		errno = EDOM;
		x = 0;
	}
	return x;
}
#else
#include <errno.h>

errno_t * __cdecl _errno();

extern const double _half;
extern const double _quarter;

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
	#define CW_MASK ~(/*CW_PC_MASK | */CW_RC_MASK)
	#define CW_NEW  (CW_PC_64 | CW_RC_NEAR | CW_EM_UNDERFLOW | CW_EM_OVERFLOW)

	__asm
	{
		emms
		sub     esp, 16                 ; Allocate temporary space
		mov     byte ptr [esp], 0       ; Set negation flag to zero
		fnstcw  word ptr [esp + 4]      ; Save control word
		mov     cx, word ptr [esp + 4]  ;
		fld     qword ptr [esp + 28]    ; Load real from stack
		fld     qword ptr [esp + 20]    ; Load real from stack
		ftst                            ; Compare x with zero
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Store AH into Flags
		jb      L1                      ; Re-direct if x < 0
		ja      L3                      ; Re-direct if x > 0
		fxch                            ; Swap st, st(1)
		ftst                            ; Compare y with zero
		fstsw   ax                      ; Get the FPU status word
		fstp    st(0)                   ; Set new stack top and pop
		sahf                            ; Store AH into Flags
		ja      L10                     ; Re-direct if y > 0
		jmp     L9                      ; End of case
	L1:
		fld     st(1)                   ; Duplicate y as st1
		frndint                         ; Round to integer
		fcomip  st(0), st(2)            ; y = int(y) ?
		je      L2                      ; Proceed if y = int(y)
		fstp    st(0)                   ; Set new stack top and pop
		fstp    st(0)                   ; Set new stack top and pop
		fldz                            ; Set result to zero
		jmp     L9                      ; End of case
	L2:
		mov     ax, cx                  ; Modify control word
		or      ax, CW_RC_CHOP          ;
		mov     word ptr [esp + 8], ax  ;
		fldcw   word ptr [esp + 8]      ; Set new control word
		fabs                            ; x = |x|
		fld     st(1)                   ; Duplicate y as st1
		fmul    qword ptr [_half]       ; Compute y * 0.5
		fld     st(0)                   ; Duplicate result
		frndint                         ; Round to integer
		fsub                            ; Subtract
		ftst                            ; Compare result with zero
		fstsw   ax                      ; Get the FPU status word
		fstp    st(0)                   ; Set new stack top and pop
		sahf                            ; Store AH into Flags
		jz      L3                      ; Re-direct if result == 0
		mov     byte ptr [esp], 1       ; Set bit if y is odd
	L3:
		and     cx, CW_MASK             ; Modify control word
		or      cx, CW_NEW              ;
		mov     word ptr [esp + 8], cx  ;
		fldcw   word ptr [esp + 8]      ; Set new control word
	L4:
		fxch                            ; Swap st, st(1)
		ftst                            ; Compare y with zero
		fxch                            ; Swap st, st(1)
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Store AH into Flags
		jnz     L5                      ; Re-direct if y != 0
		fld1                            ; Load real number 1
		jmp     L6                      ; End of case
	L5:
		fld     st(0)                   ; Duplicate x
		fxtract                         ; Get exponent and significand
		fld1                            ; Load real number 1
		fxch                            ; Swap st, st(1)
		fyl2x                           ; Compute the natural log(x)
		fxch                            ; Swap st, st(1)
		fadd    st(0), st(1)            ; Compute the n + exponent
		fmul    st(0), st(3)            ; Compute n * y
		frndint                         ; Round to integer
		fxch                            ; Swap st, st(1)
		fld     st(0)                   ; Duplicate x
		fmul    st(0), st(4)            ; Compute x * y
		fld     st(0)                   ; Duplicate x
		frndint                         ; Round to integer
		fld     st(0)                   ; Duplicate x
		fsubp   st(2), st(0)            ; Subtract
		fdiv    st(0), st(5)            ; Compute x / y
		fsubp   st(2), st(0)            ; Subtract
		fxch                            ; Swap st, st(1)
		fmul    st(0), st(4)            ; Compute x * y
		fld     st(0)                   ; Duplicate x
		fld     st(0)                   ; Duplicate x
		fadd                            ; 2 to the x
		fadd                            ; 2 to the x
		fadd                            ; 2 to the x
		fmul    qword ptr [_quarter]    ; Compute x * 0.25
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    qword ptr [esp + 8]     ; Save x, 'fxam' is require the load memory
		fstp    st(0)                   ; Set new stack top and pop
		fld     qword ptr [esp + 8]     ; Load x
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate C0, C2 and C3
		mov     dx, cx                  ;
		cmp     ah, 00000101B           ; Infinity ?
		je      L7                      ; Re-direct if x is infinity
	L6:
		fstp    st(1)                   ; Set new stack top and pop
		fstp    st(1)                   ; Set new stack top and pop
		fldcw   word ptr [esp + 4]      ; Restore control word
		test    byte ptr [esp], 1       ; Negation required ?
		jz      L10                     ; No, re-direct
		fchs                            ; Negate the result
		jmp     L10                     ; End of case
	L7:
		and     dx, CW_RC_MASK          ;
		or      cx, CW_RC_CHOP          ;
		cmp     dx, CW_RC_CHOP          ; Control word has not CW_RC_CHOP ?
		je      L8                      ; Re-direct if control word has not CW_RC_CHOP
		mov     word ptr [esp + 8], cx  ; Modify control word
		fldcw   word ptr [esp + 8]      ; Set new control word
		fstp    st(0)                   ; Set new stack top and pop
		jmp     L5                      ; End of case
	L8:
		fstp    st(1)                   ; Set new stack top and pop
		fstp    st(1)                   ; Set new stack top and pop
		fldcw   word ptr [esp + 4]      ; Restore control word
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ERANGE ; Set range error (ERANGE)
		jmp     L10                     ; End of case
	L9:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], EDOM   ; Set domain error (EDOM)
	L10:
		add     esp, 16                 ; Deallocate temporary space
		ret
	}

	#undef CW_MASK
	#undef CW_NEW
}
#endif
