#ifndef _M_IX86
#include <math.h>
#include <stdbool.h>

double __cdecl cbrt(double x)
{
	static const double one_third =  1.0 / 3.0;

	bool sign;

	if (x == 0.0)
		return x;
	if (sign = x < 0.0)
		x = -x;
	x = exp(log(x) * one_third);
	if (sign)
		x = -x;
	return x;
}
#else
__declspec(naked) double __cdecl cbrt(double x)
{
	extern double __cdecl _CIlog(/*st0 x*/);
	extern double __cdecl _CIexp(/*st0 x*/);

	static int one_third[] = { 0xAAAAAAAB, 0xAAAAAAAA, 0x00003FFD };

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		ftst                                ; Compare x with zero
		fstsw   ax                          ; Get the FPU status word
		test    ah, 40H                     ; x == 0 ?
		jnz     L1                          ; Re-direct if x == 0
		fstcw   word ptr [esp + 8]          ; Save control word
		mov     cx, word ptr [esp + 8]      ;
		and     ah, 01H                     ;
		or      cx, 0300H                   ;
		mov     word ptr [esp + 4], cx      ;
		fldcw   word ptr [esp + 4]          ; Set new precision control
		mov     byte ptr [esp + 4], ah      ; Save the negate falg
		fabs                                ; Take the absolute value
		call    _CIlog                      ; Compute the natural log(st)
		fld     tbyte ptr [one_third]       ; Load real number 1.0 / 3.0
		fmul                                ; Compute st / 3
		call    _CIexp                      ; Compute the exponent
		fldcw   word ptr [esp + 8]          ; Restore control word
		cmp     byte ptr [esp + 4], 0       ; x >= 0 ?
		je      L1                          ; Re-direct if x >= 0
		fchs                                ; Negate the result
	L1:
		ret
	}
}
#endif
