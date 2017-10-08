#pragma function(cos)

__declspec(naked) double __cdecl cos(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fcos                            ; Take the cosine
		ret
	}
}
