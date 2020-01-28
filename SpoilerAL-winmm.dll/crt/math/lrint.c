__declspec(naked) long __cdecl lrint(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fistp   dword ptr [esp - 4]
		mov     eax, dword ptr [esp - 4]
		ret
	}
}
