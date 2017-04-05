.486
.model flat, c

extrn TSSGCtrl_GetAttribute:dword

public TSSGCtrl_Write_ErrorSkip

.code

align 16

TSSGCtrl_Write_ErrorSkip proc near

	_this                       equ ebx
	SSGS                        equ <ebp + 0CH>
	ssgActionListner            equ <ebx + 54H>
	OldAddress                  equ edi
	result                      equ esi
	AT_ERRORSKIP                equ 2000H
	MainForm                    equ 0064CE2CH
	offsetof_TMainForm_userMode equ 448H

	mov     ecx, dword ptr [SSGS]
	push    AT_ERRORSKIP
	push    ecx
	push    _this
	call    dword ptr [TSSGCtrl_GetAttribute]
	add     esp, 12
	test    eax, eax
	mov     eax, MainForm
	jz      L1
	mov     eax, dword ptr [eax]
	xor     esi, esi
	cmp     dword ptr [eax + offsetof_TMainForm_userMode], 3
	je      L2
L1:
	mov     eax, 0051C45AH
	mov     edx, dword ptr [SSGS]
	push    OldAddress
	push    edx
	jmp     eax
L2:
	mov     eax, 0051C466H
	jmp     eax

TSSGCtrl_Write_ErrorSkip endp

end
