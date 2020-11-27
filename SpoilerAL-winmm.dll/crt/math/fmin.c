__declspec(naked) double __cdecl fmin(double x, double y)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load x
		fld     qword ptr [esp + 12]            ; Load y
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		and     ah, 45H                         ; Isolate C0, C2 and C3
		cmp     ah, 01H                         ; y is NaN ?
		je      L1                              ; Re-direct if y is NaN
		fucom   st(1)                           ; Cmompare y with x
		fstsw   ax                              ; Get the FPU status word
		test    ah, 01H                         ; y < x ?
		jnz     L2                              ; Re-direct if y < x
	L1:
		fxch                                    ; Exchange st, st(1)
	L2:
		fstp    st(1)                           ; Set new stack top and pop
		ret
	}
}
