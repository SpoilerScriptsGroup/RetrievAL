
__declspec(naked) void * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(void *MainForm)
{
	__asm
	{
		#define MainForm                         ecx
		#define offsetof_TMainForm_selectSubject 0x0524
		#define offset_stNONE                    0x0048413C
		#define offsetof_TSSGSubject_type        5

		mov     eax, dword ptr [ecx + 524H]
		test    eax, eax
		jnz     L1
		mov     eax, offset_stNONE - 5
	L1:
		ret

		#undef MainForm
		#undef offsetof_TMainForm_selectSubject
		#undef offset_stNONE
		#undef offsetof_TSSGSubject_type
	}
}
