__declspec(naked) long long __cdecl llround(double x)
{
	extern const unsigned int fpconst_x0363;
	#define _x0363 fpconst_x0363

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fstcw   word ptr [esp - 4]              ; Save control word
		fclex                                   ; Clear exceptions
		fldcw   word ptr [_x0363]               ; Set new rounding control
		fistp   qword ptr [esp - 12]            ; Round to integer
		mov     eax, dword ptr [esp - 12]       ;
		mov     edx, dword ptr [esp - 8]        ;
		fclex                                   ; Clear exceptions
		fldcw   word ptr [esp - 4]              ; Restore control word
		ret
	}

	#undef _x0363
}
