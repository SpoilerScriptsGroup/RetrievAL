#pragma function(sin)

__declspec(naked) double __cdecl sin(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fsin                            ; Take the sine
		ret                             ; Return to caller
	}
}
