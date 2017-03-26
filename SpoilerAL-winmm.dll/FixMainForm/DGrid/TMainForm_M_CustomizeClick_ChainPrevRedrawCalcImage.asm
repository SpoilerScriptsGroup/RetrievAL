.486
.model flat, c

public TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage

.code

align 16

TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage proc near

	mov     eax, dword ptr [ebp - 1F4H]
	mov     ecx, 00415948H
	push    eax
	push    0044B312H
	jmp     ecx

TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage endp

end
