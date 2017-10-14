#pragma function(pow)

#if 1//!defined(_M_IX86)
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
				if (modf(y / 2, &dummy))
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
#else	// low precision
#include <errno.h>

errno_t * __cdecl _errno();

__declspec(naked) double __cdecl pow(double x, double y)
{
	__asm
	{
		emms
		sub     esp, 12                 ; Allocate temporary space
		push    ebx                     ; Save register ebx
		mov     dword ptr [esp + 4], 0  ; Set negation flag to zero
		fld     qword ptr [esp + 28]    ; Load real from stack
		fld     qword ptr [esp + 20]    ; Load real from stack
		ftst                            ; Compare x with zero
		fstsw   ax                      ; Get the FPU status word
		mov     al, ah                  ; Move condition flags to AL
		lahf                            ; Load Flags into AH
		and     al, 01000101B           ; Isolate  C0, C2 and C3
		and     ah, not 01000101B       ; Turn off CF, PF and ZF
		or      ah, al                  ; Set new  CF, PF and ZF
		sahf                            ; Store AH into Flags
		jb      L1                      ; Re-direct if x < 0
		ja      L3                      ; Re-direct if x > 0
		fxch                            ; Swap st, st(1)
		ftst                            ; Compare y with zero
		fxch                            ; Restore x as top of stack
		fstsw   ax                      ; Get the FPU status word
		mov     al, ah                  ; Move condition flags to AL
		lahf                            ; Load Flags into AH
		and     al, 01000101B           ; Isolate  C0, C2 and C3
		and     ah, not 01000101B       ; Turn off CF, PF and ZF
		or      ah, al                  ; Set new  CF, PF and ZF
		sahf                            ; Store AH into Flags
		ja      L3                      ; Re-direct if y > 0
		fstp    st(1)                   ; Set new stack top and pop
		mov     ebx, EDOM               ; Set domain error (EDOM)
		jmp     L5                      ; End of case
	L1:
		fxch                            ; Put y on top of stack
		fld     st(0)                   ; Duplicate y as st1
		frndint                         ; Round to integer
		fxch                            ; Put y on top of stack
		fcomp                           ; y = int(y) ?
		fstsw   ax                      ; Get the FPU status word
		mov     al, ah                  ; Move condition flags to AL
		lahf                            ; Load Flags into AH
		and     al, 01000101B           ; Isolate  C0, C2 and C3
		and     ah, not 01000101B       ; Turn off CF, PF and ZF
		or      ah, al                  ; Set new  CF, PF and ZF
		sahf                            ; Store AH into Flags
		je      L2                      ; Proceed if y = int(y)
		fstp    st(1)                   ; Set new stack top and pop
		fldz                            ; Set result to zero
		fstp    st(1)                   ; Set new stack top and pop
		mov     ebx, EDOM               ; Set domain error (EDOM)
		jmp     L5                      ; End of case
	L2:
		fist    dword ptr [esp + 4]     ; Store y as integer
		and     dword ptr [esp + 4], 1  ; Set bit if y is odd
		fxch                            ; Put x on top of stack
		fabs                            ; x = |x|
	L3:
		fldln2                          ; Load log base e of 2
		fxch    st(1)                   ; Exchange st0, st1
		fyl2x                           ; Compute the natural log(x)
		fmul                            ; Compute y * ln(x)
		fldl2e                          ; Load log base 2(e)
		fmul                            ; Multiply x * log base 2(e)
		fst     st(1)                   ; Push result
		frndint                         ; Round to integer
		fsub    st(1), st(0)            ; Subtract
		fxch                            ; Exchange st0, st1
		f2xm1                           ; Compute 2 to the (x - 1)
		fld1                            ; Load real number 1
		fadd                            ; 2 to the x
		fscale                          ; Scale by power of 2
		fstp    st(1)                   ; Set new stack top and pop
		test    dword ptr [esp + 4], 1  ; Negation required ?
		jz      L4                      ; No, re-direct
		fchs                            ; Negate the result
	L4:
		fstp    qword ptr [esp + 8]     ; Save (double)pow(x, y)
		fld     qword ptr [esp + 8]     ; Load (double)pow(x, y)
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		cmp     ah, 5                   ; Infinity ?
		jne     L6                      ; No, end of case
		mov     ebx, ERANGE             ; Set range error (ERANGE)
		                                ; Get errno pointer offset
	L5:
		call    _errno                  ; Get C errno variable pointer
		mov     dword ptr [eax], ebx    ; Set errno
	L6:
		pop     ebx                     ; Restore register ebx
		add     esp, 12                 ; Deallocate temporary space
		ret
	}
}
#endif
