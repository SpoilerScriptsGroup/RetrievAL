#pragma function(cos)

__declspec(naked) double __cdecl cos(double x)
{
	double __cdecl _CIcos(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		jmp     _CIcos
	}
}

__declspec(naked) double __cdecl _CIcos(/*st0 x*/)
{
	__asm
	{
		fcos                                    ; Take the cosine
		ret
	}
}
