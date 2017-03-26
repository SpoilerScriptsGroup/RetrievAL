.486
.model flat, c

extrn TMainForm_DrawTreeCell_FixDefaultColWidth@16:proc

public Caller_TMainForm_DrawTreeCell_FixDefaultColWidth

.code

align 16

Caller_TMainForm_DrawTreeCell_FixDefaultColWidth proc near

	TCanvas_InternalTextOut equ 0055E418H
	Canvas                  equ eax
	X                       equ edx
	Y                       equ ecx
	SSGS                    equ edi
	DrawStr                 equ <ebp - 18H>

	push    TCanvas_InternalTextOut
	push    ecx
	mov     ecx, ebp
	push    eax
	sub     ecx, 24
	push    edx

	push    ecx
	push    X
	push    Canvas
	push    SSGS
	call    TMainForm_DrawTreeCell_FixDefaultColWidth@16

	pop     edx
	pop     eax
	pop     ecx
	ret

Caller_TMainForm_DrawTreeCell_FixDefaultColWidth endp

end
