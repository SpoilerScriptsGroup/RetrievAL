__declspec(naked) double __cdecl rint(double x)
{
	__asm
	{
		push    eax                     ; Allocate temporary space
		fld     qword ptr [esp + 8]     ; Load real from stack
		fstcw   word ptr [esp + 2]      ; Save control word
		fclex                           ; Clear exceptions
		mov     ax, word ptr [esp + 2]  ; Control word
		or      ax, 20H                 ;
		mov     word ptr [esp], ax      ;
		fldcw   word ptr [esp]          ; Set new control word
		frndint                         ; Round to integer
		fclex                           ; Clear exceptions
		fldcw   word ptr [esp + 2]      ; Restore control word
		pop     eax                     ; Deallocate temporary space
		ret
	}
}
