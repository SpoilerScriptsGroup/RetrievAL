#pragma function(atan)

__declspec(naked) double __cdecl atan(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fld1                            ; Load constant 1
		fpatan                          ; Take the arctangent
		ret
	}
}
