.486
.model flat, c

includelib user32.lib

extrn _imp__SetWindowTextA@8:dword
extrn F00401FD8:dword
extrn _TWinControl_GetHandle:dword

public TGetSearchRangeForm_ctor

.const

data1 label byte
	byte    '対象プロセスの、ヒープ領域を取得', 0

.code

align 16

TGetSearchRangeForm_ctor proc near

	call    dword ptr [F00401FD8]
	add     dword ptr [ebp - 0CH], 16
	push    offset data1
	mov     edx, dword ptr [ebp - 4H]
	mov     eax, dword ptr [edx + 31CH]
	call    dword ptr [_TWinControl_GetHandle]
	push    eax
	push    0048B1BFH
	jmp     dword ptr [_imp__SetWindowTextA@8]

TGetSearchRangeForm_ctor endp

end
