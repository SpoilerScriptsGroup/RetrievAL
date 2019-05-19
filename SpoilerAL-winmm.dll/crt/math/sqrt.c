#pragma function(sqrt)

__declspec(naked) double __cdecl sqrt(double x)
{
	double __cdecl _CIsqrt(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		jmp     _CIsqrt
	}
}

__declspec(naked) double __cdecl _CIsqrt(/*st0 x*/)
{
	__asm
	{
		fsqrt                               ; Take the square root
		ret
	}
}
