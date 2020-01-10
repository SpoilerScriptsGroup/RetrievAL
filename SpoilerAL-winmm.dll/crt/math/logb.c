__declspec(naked) double __cdecl logb(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fxtract                             ; Get exponent and significand
		fstp    st(0)                       ; Set new top of stack
		ret
	}
}
