#pragma function(fmod)

__declspec(naked) double __cdecl fmod(double x, double y)
{
	__asm
	{
		fld     qword ptr [esp + 12]    ; Load real from stack
		fld     qword ptr [esp + 4]     ; Load real from stack
	L1:
		fprem                           ; Get the partial remainder
		fstsw   ax                      ; Get coprocessor status
		test    ax, 0400H               ; Complete remainder ?
		jnz     L1                      ; No, go get next remainder
		fstp    st(1)                   ; Set new top of stack
		ret
	}
}
