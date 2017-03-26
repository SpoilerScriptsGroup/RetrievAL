.486
.model flat, c

extrn UpdateUserModeMenu:proc

public TMainForm_LoadSetting_SetUserMode

.code

align 16

TMainForm_LoadSetting_SetUserMode proc near

	dec     dword ptr [ebx + 1CH]
	dec     dword ptr [ebx + 1CH]
	dec     dword ptr [ebx + 1CH]
	push    00437E31H
	jmp     UpdateUserModeMenu

TMainForm_LoadSetting_SetUserMode endp

end
