__declspec(naked) double __cdecl _scalb(double x, long exp)
{
	__asm
	{
		fild    dword ptr [esp + 12]        ; Load exp as integer
		fld     qword ptr [esp + 4]         ; Load real from stack
		fscale                              ; Scale by power of 2
		fstp    st(1)                       ; Set new stack top and pop
	}
}
