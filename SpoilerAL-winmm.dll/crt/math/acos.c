#pragma function(acos)

__declspec(naked) double __cdecl acos(double x)
{
	__asm
	{
		emms
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld     st(0)                   ; Load x
		fld     st(0)                   ; Load x
		fmul                            ; Multiply (x squared)
		fld1                            ; Load 1
		fsubr                           ; 1 - (x squared)
		fsqrt                           ; Square root of (1 - x squared)
		fxch                            ; Exchange st, st(1)
		fpatan                          ; This gives the arc cosine !
		ret
	}
}
