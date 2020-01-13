__declspec(naked) double __cdecl atanh(double x)
{
	double __cdecl _CIlog(/*st0 x*/);

	extern const double fpconst_half;
	#define _half fpconst_half

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fld1                                ; Load real number 1
		fld1                                ; Load real number 1
		fld     st(2)                       ; Duplicate st2
		fadd                                ; Increment
		fxch    st(2)                       ; Exchange st, st(2)
		fsub                                ; Compute 1 - x
		fdiv                                ; Compute (1 + x) / (1 - x)
		call    _CIlog                      ; Compute the natural log(st)
		fmul    qword ptr [_half]           ; Compute st * 0.5
		ret
	}
}
