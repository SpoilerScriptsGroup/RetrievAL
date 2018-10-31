#pragma function(atan)

__declspec(naked) double __cdecl atan(double x)
{
	double __cdecl _CIatan(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		jmp     _CIatan
	}
}

__declspec(naked) double __cdecl _CIatan(/*st0 x*/)
{
	__asm
	{
		fld1                            ; Load constant 1
		fpatan                          ; Take the arctangent
		ret
	}
}
