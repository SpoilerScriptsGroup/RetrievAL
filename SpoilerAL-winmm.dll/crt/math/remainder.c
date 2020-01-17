__declspec(naked) double __cdecl remainder(double x, double y)
{
	__asm
	{
		fld     qword ptr [esp + 12]
		fld     qword ptr [esp + 4]
	L1:
		fprem1
		fstsw   ax
		sahf
		jp      L1
		fstp    st(1)
		ret
	}
}
