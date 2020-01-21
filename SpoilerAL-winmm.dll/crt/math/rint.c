__declspec(naked) double __cdecl rint(double x)
{
	__asm
	{
		fld     qword ptr [esp + 4]         ; Load real from stack
		fstcw   word ptr [esp + 4]          ; Save control word
		fclex                               ; Clear exceptions
		mov     ax, word ptr [esp + 4]      ; Control word
		and     ax, not 20H                 ;
		mov     word ptr [esp + 8], ax      ;
		fldcw   word ptr [esp + 8]          ; Set new control word
		frndint                             ; Round to integer
		fldcw   word ptr [esp + 4]          ; Restore control word
		ret
	}
}
