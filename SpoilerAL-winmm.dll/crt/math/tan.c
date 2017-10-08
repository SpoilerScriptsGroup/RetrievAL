#pragma function(sqrt)

__declspec(naked) double __cdecl sqrt(double x)
{
	__asm
	{
		fld     qword [esp + 4]         ; Load real from stack
		fsqrt                           ; Take the square root
		ret
	}
}
