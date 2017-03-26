.486
.model flat, c

extrn TMainForm_DrawTree@20:proc

public Caller_TMainForm_DrawTree

.code

align 16

Caller_TMainForm_DrawTree proc near

	FALSE      equ 0
	_this      equ <esp +  4>
	DestCanvas equ <esp +  8>
	LeftOffset equ <esp + 12>
	TopOffset  equ <esp + 16>

	mov     eax, dword ptr [TopOffset ]
	mov     edx, dword ptr [LeftOffset]
	push    FALSE
	push    eax
	mov     ecx, dword ptr [DestCanvas + 8]
	mov     eax, dword ptr [_this      + 8]
	push    edx
	push    ecx
	push    eax
	call    TMainForm_DrawTree@20
	ret

Caller_TMainForm_DrawTree endp

end
