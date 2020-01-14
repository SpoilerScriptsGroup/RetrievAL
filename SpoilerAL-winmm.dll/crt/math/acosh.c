__declspec(naked) double __cdecl acosh(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fld1                                ; Load real number 1
		fld     st(1)                       ; Duplicate st1
		fmul    st(0), st(0)                ; Compute st * st
		fsubr                               ; Decrement
		fsqrt                               ; Take the square root
		fadd                                ; 2 to the st
		jmp     _CIlog                      ; Compute the natural log(st)
	}
}
