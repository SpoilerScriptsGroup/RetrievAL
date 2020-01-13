__declspec(naked) double __cdecl _chgsign(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fchs                                ; Negate the result
		ret
	}
}
