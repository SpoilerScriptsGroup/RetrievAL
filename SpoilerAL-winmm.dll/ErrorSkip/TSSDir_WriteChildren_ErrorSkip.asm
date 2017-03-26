.486
.model flat, c

public TSSDir_WriteChildren_ErrorSkip

.code

align 16

TSSDir_WriteChildren_ErrorSkip proc near

	ReturnAddress         equ 004C32A4H
	AT_ERRORSKIP          equ 2000H
	SSGC                  equ edi
	_this                 equ <ebp + 8>
	TSSGCtrl_GetAttribute equ 005038E8H

	mov     eax, dword ptr [ecx + 12]
	mov     dword ptr [esp], offset L1
	jmp     eax
L1:
	mov     dword ptr [esp + 8], ReturnAddress
	add     esp, 8
	test    eax, eax
	jz      L4
	push    eax
	mov     eax, TSSGCtrl_GetAttribute
	mov     ecx, dword ptr [_this]
	push    AT_ERRORSKIP
	push    ecx
	push    SSGC
	call    eax
	test    eax, eax
	jz      L2
	xor     eax, eax
	jmp     L3
L2:
	mov     eax, dword ptr [esp + 12]
L3:
	add     esp, 16
L4:
	ret

TSSDir_WriteChildren_ErrorSkip endp

end
