#pragma function(exp)

__declspec(naked) double __cdecl expm1(double x)
{
	__asm
	{
		fld1                                ; Load real number 1
		fld     qword ptr [esp + 4]         ; Load real from stack
		fabs                                ; Take the absolute value
		fcompp                              ; x >= 1 ?
		fstsw   ax                          ; Get the FPU status word
		sahf                                ; Set flags based on test
		jae     L1                          ; Re-direct if x >= 1
		fld     qword ptr [esp + 4]         ; Load real from stack
		fldl2e                              ; Load log base 2(e)
		fmulp   st(1), st(0)                ; Multiply x * log base 2(e)
		f2xm1                               ; Compute 2 to the (x - 1)
		ret
	L1:
		mov     edx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    eax
		call    exp                         ; Call exp function
		add     esp, 8
		fld1                                ; Load real number 1
		fsubp   st(1), st(0)                ; Subtract
		ret
	}
}
