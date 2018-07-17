
__declspec(naked) void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName()
{
	__asm
	{
		#define this                        edi
		#define offsetof_TMainForm_MultiLBox 3E4H
		#define offsetof_TMainForm_ListLBox 3E8H
		#define offsetof_TListBox_Font      68H
		#define TFont_SetName               0055D578H

		push    eax
		push    edx
		mov     eax, dword ptr [this + offsetof_TMainForm_MultiLBox]
		mov     eax, [eax + offsetof_TListBox_Font]
		mov     ecx, TFont_SetName
		call    ecx
		mov     edx, [esp]
		mov     eax, dword ptr [this + offsetof_TMainForm_ListLBox]
		mov     eax, [eax + offsetof_TListBox_Font]
		mov     ecx, TFont_SetName
		call    ecx
		pop     edx
		pop     eax
		mov     ecx, TFont_SetName
		jmp     ecx

		#undef this
		#undef offsetof_TMainForm_ListLBox
		#undef offsetof_TMainForm_MultiLBox
		#undef offsetof_TListBox_Font
		#undef TFont_SetName
	}
}
