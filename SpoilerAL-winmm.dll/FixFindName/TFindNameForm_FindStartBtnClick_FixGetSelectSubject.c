
__declspec(naked) void * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(void *MainForm)
{
	__asm
	{
		#define MainForm      ecx
		#define offset_stNONE 0x0048413C

		mov     eax, dword ptr [ecx + 524H]
		test    eax, eax
		jnz     L1
		mov     eax, offset_stNONE - 5
	L1:
		ret

		#undef MainForm
		#undef offset_stNONE
	}
}
