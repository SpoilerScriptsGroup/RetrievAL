.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowTextA@8:dword

public TMemorySettingForm_ctor

.const

data1 label byte
	byte    'ƒtƒ@ƒCƒ‹‚ÌCRC32‚ðŽæ“¾', 0

.code

align 16

TMemorySettingForm_ctor proc near

	push    offset L1
	push    00401FD8H
	ret
	align   16
L1:
	add     dword ptr [ebp - 0CH], 16
	push    offset data1
	mov     edx, dword ptr [ebp - 4H]
	mov     ecx, 0058750CH
	mov     eax, dword ptr [edx + 308H]
	call    ecx
	push    eax
	push    0047B483H
	jmp     dword ptr [_imp__SetWindowTextA@8]

TMemorySettingForm_ctor endp

end
