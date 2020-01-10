__declspec(naked) double __cdecl remquo(double x, double y, int *quo)
{
	__asm
	{
		fld     qword ptr [esp + 12]        ; Load y
		fld     qword ptr [esp + 4]         ; Load x
	L1:
		fprem1
		fstsw   ax                          ; Get the FPU status word
		sahf                                ; Store AH into Flags
		jp      L1
		fstp    st(1)                       ; Set new stack top and pop
		shr     eax, 6                      ; Extract the three low-order bits of the quotient from C0, C3, C1.
		mov     ecx, eax
		and     eax, 0108H
		ror     eax, 7
		or      ecx, eax
		rol     eax, 4
		or      eax, ecx
		mov     ecx, dword ptr [esp + 16]   ; Negate the quotient bits if x*y<0.  Avoid using an unpredictable branch.
		and     eax, 7
		xor     ecx, dword ptr [esp + 8]
		sar     ecx, 32
		xor     eax, ecx
		and     ecx, 1
		add     eax, ecx
		mov     ecx, dword ptr [esp + 20]   ; Store the quotient and return.
		mov     dword ptr [ecx], eax
		ret
	}
}
