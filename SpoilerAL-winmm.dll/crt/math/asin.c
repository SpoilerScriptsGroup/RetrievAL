#pragma function(asin)

__declspec(naked) double __cdecl asin(double x)
{
	double __cdecl _CIasin(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		jmp     _CIasin
	}
}

__declspec(naked) double __cdecl _CIasin(/*st0 x*/)
{
	__asm
	{
		fld     st(0)                   ; Load x
		fld     st(0)                   ; Load x
		fmul                            ; Multiply (x squared)
		fld1                            ; Load 1
		fsubr                           ; 1 - (x squared)
		fsqrt                           ; Square root of (1 - x squared)
		fpatan                          ; This gives the arc sine !
		ret
	}
}
