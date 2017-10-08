#pragma function(log)

__declspec(naked) double __cdecl log1p(double x)
{
	// 1.0 - sqrt(2) / 2.0
	static const double range = 0.292893218813452475599155637895150960715164062311525963411;

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     qword ptr [range]       ; Load range (1.0 - sqrt(2) / 2.0)
		fld     st(1)                   ; Duplicate stack top
		fabs                            ; Take the absolute value
		fcompp                          ; x > range ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		ja      L1                      ; Re-direct if x > range
		fldln2                          ; Load log base e of 2
		fxch                            ; Exchange st, st(1)
		fyl2xp1                         ; Compute the logarithm epsilon
		ret
	L1:
		fld1                            ; Load real number 1
		faddp   st(1), st(0)            ; Compute x+1
		fstp    qword ptr [esp + 4]     ; Copy x onto stack
		jmp     log                     ; Jump log function
	}
}
