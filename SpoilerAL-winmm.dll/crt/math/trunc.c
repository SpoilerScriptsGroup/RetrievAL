
__declspec(naked) double __cdecl trunc(double x)
{
	__asm
	{
		push    eax                     ; Allocate temporary space
		fld     qword ptr [esp + 8]     ; Load real from stack
		fstcw   word ptr [esp + 2]      ; Save control word
		fclex                           ; Clear exceptions
		mov     word ptr [esp], 0F63H   ; Rounding control word
		fldcw   word ptr [esp]          ; Set new rounding control
		frndint                         ; Round to integer
		fclex                           ; Clear exceptions
		fldcw   word ptr [esp + 2]      ; Restore control word
		pop     eax                     ; Deallocate temporary space
		ret
	}
}
