#pragma function(fmod)

__declspec(naked) double __cdecl fmod(double x, double y)
{
	double __cdecl _CIfmod(/*st1 x, st0 y*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     qword ptr [esp + 12]    ; Load real from stack
		jmp     _CIfmod
	}
}

__declspec(naked) double __cdecl _CIfmod(/*st1 x, st0 y*/)
{
	__asm
	{
		fxch                            ; Swap arguments
	L1:
		fprem                           ; Get the partial remainder
		fstsw   ax                      ; Get coprocessor status
		test    ax, 0400H               ; Complete remainder ?
		jnz     L1                      ; No, go get next remainder
		fstp    st(1)                   ; Set new stack top and pop
		ret
	}
}
