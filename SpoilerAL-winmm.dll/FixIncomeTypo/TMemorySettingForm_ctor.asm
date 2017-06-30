.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowTextA@8:dword
extrn F00401FD8:dword
extrn _TWinControl_GetHandle:dword

public TMemorySettingForm_ctor

.const

data1 label byte
	byte    'ƒtƒ@ƒCƒ‹‚ÌCRC32‚ðŽæ“¾', 0

.code

align 16

TMemorySettingForm_ctor proc near

	call    dword ptr [F00401FD8]
	add     dword ptr [ebp - 0CH], 16
	push    offset data1
	mov     edx, dword ptr [ebp - 4H]
	mov     eax, dword ptr [edx + 308H]
	call    dword ptr [_TWinControl_GetHandle]
	push    eax
	push    0047B483H
	jmp     dword ptr [_imp__SetWindowTextA@8]

TMemorySettingForm_ctor endp

end
