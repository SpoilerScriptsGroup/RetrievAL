#pragma function(tan)

__declspec(naked) double __cdecl tan(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]     ; Load real from stack
		fptan                           ; Take the tangent
		fstp    st(0)                   ; Throw away the constant 1
		ret
	}
}
