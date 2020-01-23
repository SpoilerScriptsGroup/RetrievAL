__declspec(naked) long __cdecl lrint(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		sub     esp, 4
		fistp   dword ptr [esp]
		pop     eax
		ret
	}
}
