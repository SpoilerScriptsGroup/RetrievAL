__declspec(naked) double __cdecl copysign(double x, double y)
{
	__asm
	{
		#define x (esp + 4)
		#define y (esp + 12)

		mov     eax, dword ptr [x + 4]
		mov     ecx, dword ptr [y + 4]
		and     eax, 7FFFFFFFH
		and     ecx, 80000000H
		mov     edx, dword ptr [x    ]
		or      eax, ecx
		mov     dword ptr [esp - 8], edx
		mov     dword ptr [esp - 4], eax
		fld     qword ptr [esp - 8]             ; Load real from stack
		ret

		#undef x
		#undef y
	}
}
