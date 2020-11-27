__declspec(naked) double __cdecl fdim(double x, double y)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load x
		fld     qword ptr [esp + 12]            ; Load y
		fcom    st(1)                           ; Cmompare y with x
		fstsw   ax                              ; Get the FPU status word
		test    ah, 45H                         ; y > x ?
		jz      L1                              ; Re-direct if y > x
		fsub                                    ; Compute x - y
		ret

		align   16
	L1:
		fstp    st(0)                           ; Set new top of stack
		fstp    st(0)                           ; Set new top of stack
		fldz                                    ; Set result to zero
		ret
	}
}
