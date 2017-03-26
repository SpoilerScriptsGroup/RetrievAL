.486
.model flat, c

public TMainForm_LoadSetting_ListLBox_Font_SetName

.code

align 16

TMainForm_LoadSetting_ListLBox_Font_SetName proc near

	_this                       equ edi
	offsetof_TMainForm_ListLBox equ 3E8H
	offsetof_TListBox_Font      equ 68H
	TFont_SetName               equ 0055D578H

	push    TFont_SetName
	push    edx
	push    eax
	mov     eax, dword ptr [_this + offsetof_TMainForm_ListLBox]
	mov     ecx, TFont_SetName
	mov     eax, [eax + offsetof_TListBox_Font]
	call    ecx
	pop     eax
	pop     edx
	ret

TMainForm_LoadSetting_ListLBox_Font_SetName endp

end
