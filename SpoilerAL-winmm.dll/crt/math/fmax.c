__declspec(naked) double __cdecl fmax(double x, double y)
{
	__asm
	{
		fld     qword ptr [esp + 12]            ; Load y
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		fld     qword ptr [esp + 4]             ; Load x
		and     ah, 45H                         ; Isolate C0, C2 and C3
		cmp     ah, 01H                         ; x is NaN ?
		je      L1                              ; Re-direct if x is NaN
		fucom   st(1)                           ; Cmompare x with y
		fstsw   ax                              ; Get the FPU status word
		test    ah, 01H                         ; x >= y ?
		jz      L1                              ; Re-direct if x >= y
		fxch                                    ; Exchange st, st(1)
	L1:
		fstp    st(1)                           ; Set new stack top and pop
		ret
	}
}
