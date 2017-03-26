.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowTextA@8:dword

public TGetSearchRangeForm_ctor

.const

data1 label byte
	byte    '対象プロセスの、ヒープ領域を取得', 0

.code

align 16

TGetSearchRangeForm_ctor proc near

	push    offset L1
	push    00401FD8H
	ret
	align   16
L1:
	add     dword ptr [ebp - 0CH], 16
	push    offset data1
	mov     edx, dword ptr [ebp - 4H]
	mov     ecx, 0058750CH
	mov     eax, dword ptr [edx + 31CH]
	call    ecx
	push    eax
	push    0048B1BFH
	jmp     dword ptr [_imp__SetWindowTextA@8]

TGetSearchRangeForm_ctor endp

end
