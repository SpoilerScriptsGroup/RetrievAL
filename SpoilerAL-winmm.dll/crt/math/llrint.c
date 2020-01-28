__declspec(naked) __int64 __cdecl llrint(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fistp   qword ptr [esp - 8]
		mov     eax, dword ptr [esp - 8]
		mov     edx, dword ptr [esp - 4]
		ret
	}
}
