.486
.model flat, c

includelib user32.lib

extrn _imp__SendMessageA@16:dword

public TGuideForm_ctor

.const

lpszString byte '→ SSGデバッグモード - マウスオーバーを除く', 0

.code

align 16

TGuideForm_ctor proc near

	_this                              equ <ebp - 4H>
	offsetof_TGuideForm_UserModeCmbBox equ 764
	TWinControl_GetHandle              equ 0058750CH
	CB_ADDSTRING                       equ 143H
	ReturnAddress                      equ 0048C247H

	pop     ecx
	add     dword ptr [ebp - 0CH], 4

	push    offset lpszString
	push    0
	push    CB_ADDSTRING
	mov     eax, dword ptr [_this]
	mov     ecx, TWinControl_GetHandle
	mov     eax, dword ptr [eax + offsetof_TGuideForm_UserModeCmbBox]
	call    ecx
	push    eax
	push    ReturnAddress
	jmp     dword ptr [_imp__SendMessageA@16]

TGuideForm_ctor endp

end
