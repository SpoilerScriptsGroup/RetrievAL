#pragma function(log10)

#ifndef _M_IX86
#include <math.h>

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651128918916605082
#endif

double __cdecl log10(double x)
{
	return log(x) * M_LOG10E;
}
#else
#include <errno.h>

__declspec(naked) double __cdecl log10(double x)
{
	double __cdecl _CIlog10(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		jmp     _CIlog10
	}
}

__declspec(naked) double __cdecl _CIlog10(/*st0 x*/)
{
	extern const double _minus_inf;
	extern const double _nan_ind;

#ifdef _DEBUG
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fxam                            ; Examine st
		fstsw   ax                      ; Get the FPU status word
		and     ah, 01000101B           ; Isolate C0, C2 and C3
		cmp     ah, 00000001B           ; NaN ?
		je      L1                      ; Re-direct if x is NaN
		ftst                            ; Compare x with zero
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Store AH into Flags
		jbe     L2                      ; Re-direct if x <= 0
		fldlg2                          ; Load log base 10 of 2
		fxch                            ; Exchange st, st(1)
		fyl2x                           ; Compute the log base 10(x)
	L1:
		ret
	L2:
		fstp    st(0)                   ; Set new top of stack
		je      L3                      ; Re-direct if x == 0
		set_errno(EDOM)                 ; Set domain error (EDOM)
		fld     qword ptr [_nan_ind]    ; Load NaN(indeterminate)
		ret
	L3:
		set_errno(ERANGE)               ; Set range error (ERANGE)
		fld     qword ptr [_minus_inf]  ; Load -infinity
		ret
	}

	#undef set_errno
}
#endif
