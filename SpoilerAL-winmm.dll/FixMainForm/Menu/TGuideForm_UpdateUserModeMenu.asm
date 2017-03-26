.486
.model flat, c

extrn UpdateUserModeMenu:proc

public TGuideForm_UpdateUserModeMenu

.code

align 16

TGuideForm_UpdateUserModeMenu proc near

	mov     dword ptr [edx + 448H], eax
	push    0048D178H
	jmp     UpdateUserModeMenu

TGuideForm_UpdateUserModeMenu endp

end
