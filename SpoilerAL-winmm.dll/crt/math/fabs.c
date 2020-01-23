#pragma function(fabs)

__declspec(naked) double __cdecl fabs(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fabs                                    ; Take the absolute value
		ret
	}
}
