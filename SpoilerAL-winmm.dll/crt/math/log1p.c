#pragma function(log)

__declspec(naked) double __cdecl log1p(double x)
{
	// 1 - sqrt(2) / 2
	static const double range = 0.292893218813452475599155637895150960715164062311525963411;

	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     qword ptr [range]       ; Load range
		fcomp                           ; 1 - sqrt(2) / 2 <= x  ?
		fstsw   ax                      ; Get the FPU status word
		sahf                            ; Set flags based on test
		jbe     L1                      ; Re-direct if 1 - sqrt(2) / 2 <= x
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
