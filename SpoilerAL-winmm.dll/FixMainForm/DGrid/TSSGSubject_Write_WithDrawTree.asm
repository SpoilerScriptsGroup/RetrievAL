.486
.model flat, c

SHOW_ERROR_MESSAGE equ 1

extrn TMainForm_DrawTree@20:proc
if SHOW_ERROR_MESSAGE
extrn dwErrorMessageId:dword
endif

public TSSGSubject_Write_WithDrawTree

.code

align 16

TSSGSubject_Write_WithDrawTree proc near

	NULL     equ 0
	TRUE     equ 1
	MainForm equ 0064CE2CH

if SHOW_ERROR_MESSAGE
	mov     dword ptr [dwErrorMessageId], 0
	mov     ecx, dword ptr [ecx + 12]
endif
	mov     edx, dword ptr [esp + 12]
	mov     eax, dword ptr [esp +  8]
	push    edx
	push    eax
	mov     edx, dword ptr [esp +  8]
	mov     eax, dword ptr [esp + 12]
	mov     dword ptr [esp + 20], edx
	push    eax
if SHOW_ERROR_MESSAGE
	call    ecx
else
	call    dword ptr [ecx + 12]
endif
	mov     dword ptr [esp + 20], eax
	mov     eax, MainForm
	mov     dword ptr [esp + 16], TRUE
	mov     eax, dword ptr [eax]
	mov     dword ptr [esp + 12], 0
	mov     dword ptr [esp +  8], 0
	mov     dword ptr [esp +  4], NULL
	mov     dword ptr [esp     ], eax
	call    TMainForm_DrawTree@20
	pop     eax
	ret

TSSGSubject_Write_WithDrawTree endp

end
