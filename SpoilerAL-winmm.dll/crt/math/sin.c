#pragma function(sin)

__declspec(naked) double __cdecl sin(double x)
{
	double __cdecl _CIsin(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIsin
	}
}

__declspec(naked) double __cdecl _CIsin(/*st0 x*/)
{
	__asm
	{
		fsin                                ; Take the sine
		ret                                 ; Return to caller
	}
}
