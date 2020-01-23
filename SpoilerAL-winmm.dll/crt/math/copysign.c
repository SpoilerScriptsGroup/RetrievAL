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
		push    eax
		push    edx
		fld     qword ptr [esp]                 ; Load real from stack
		add     esp, 8                          ; Deallocate temporary space
		ret

		#undef x
		#undef y
	}
}
