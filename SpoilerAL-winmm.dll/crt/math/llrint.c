__declspec(naked) __int64 __cdecl llrint(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		sub     esp, 8
		fistp   qword ptr [esp]
		pop     eax
		pop     edx
		ret
	}
}
