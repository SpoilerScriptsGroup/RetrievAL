#pragma function(atan2)

__declspec(naked) double __cdecl atan2(double y, double x)
{
	double __cdecl _CIatan2(/*st1 y, st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fld     qword ptr [esp + 12]            ; Load real from stack
		jmp     _CIatan2
	}
}

__declspec(naked) double __cdecl _CIatan2(/*st1 y, st0 x*/)
{
	__asm
	{
		fpatan                                  ; Take the arctangent
		ret
	}
}
