__declspec(naked) double __cdecl significand(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fxtract                             ; Get exponent and significand
		fstp    st(1)                       ; Set new stack top and pop
		ret
	}
}
