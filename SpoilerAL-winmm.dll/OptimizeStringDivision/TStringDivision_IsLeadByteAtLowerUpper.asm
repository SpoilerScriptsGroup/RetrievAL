.486
.model flat, c

CODEPAGE_SUPPORT equ 0

public TStringDivision_IsLeadByteAtLowerUpper

if CODEPAGE_SUPPORT
extrn _imp__IsDBCSLeadByte@4:dword
endif

.code

align 16

TStringDivision_IsLeadByteAtLowerUpper proc near

if CODEPAGE_SUPPORT
	movzx   edx, byte ptr [ebx]
	pop     eax
	push    edx
	push    eax
	jmp     dword ptr [_imp__IsDBCSLeadByte@4]
else
	mov     dl, byte ptr [ebx]
	xor     eax, eax
	cmp     dl, 81H
	jb      L2
	cmp     dl, 9FH
	jbe     L1
	cmp     dl, 0E0H
	jb      L2
	cmp     dl, 0FCH
	ja      L2
L1:
	inc     eax
L2:
	ret
endif

TStringDivision_IsLeadByteAtLowerUpper endp

end
