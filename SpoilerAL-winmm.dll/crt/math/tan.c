#pragma function(tan)

__declspec(naked) double __cdecl tan(double x)
{
	double __cdecl _CItan(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		jmp     _CItan
	}
}

__declspec(naked) double __cdecl _CItan(/*st0 x*/)
{
	__asm
	{
		fptan                           ; Take the tangent
		fstp    st(0)                   ; Throw away the constant 1
		ret
	}
}
