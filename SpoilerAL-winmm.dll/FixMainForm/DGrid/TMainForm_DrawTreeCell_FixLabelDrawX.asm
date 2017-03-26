.486
.model flat, c

public TMainForm_DrawTreeCell_FixLabelDrawX

.code

align 16

TMainForm_DrawTreeCell_FixLabelDrawX proc near

	mov     dl, byte ptr [edi + 60H]
	mov     eax, 00444E2DH
	cmp     dl, 3
	jne     L1
	mov     eax, 00444E79H
L1:
	mov     edx, edi
	jmp     eax

TMainForm_DrawTreeCell_FixLabelDrawX endp

end
