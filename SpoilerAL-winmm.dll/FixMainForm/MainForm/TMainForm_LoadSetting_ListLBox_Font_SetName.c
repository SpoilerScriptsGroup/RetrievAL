
__declspec(naked) void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName()
{
	__asm
	{
		#define this                        edi
		#define offsetof_TMainForm_ListLBox 3E8H
		#define offsetof_TListBox_Font      68H
		#define TFont_SetName               0055D578H

		push    TFont_SetName
		push    edx
		push    eax
		mov     eax, dword ptr [this + offsetof_TMainForm_ListLBox]
		mov     ecx, TFont_SetName
		mov     eax, [eax + offsetof_TListBox_Font]
		call    ecx
		pop     eax
		pop     edx
		ret

		#undef this
		#undef offsetof_TMainForm_ListLBox
		#undef offsetof_TListBox_Font
		#undef TFont_SetName
	}
}
