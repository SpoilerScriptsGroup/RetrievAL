.486
.model flat, c

public TSSBundleList_Write_ErrorSkip

.code

align 16

TSSBundleList_Write_ErrorSkip proc near

	ReturnAddress         equ 004BF89EH
	AT_ERRORSKIP          equ 2000H
	SSGC                  equ <ebp + 12>
	_this                 equ esi
	TSSGCtrl_GetAttribute equ 005038E8H

	mov     eax, 0051C540H
	mov     dword ptr [esp], offset L1
	jmp     eax
L1:
	push    ReturnAddress
	test    al, al
	jnz     L2
	mov     eax, TSSGCtrl_GetAttribute
	mov     ecx, dword ptr [SSGC]
	push    AT_ERRORSKIP
	push    _this
	push    ecx
	call    eax
	add     esp, 12
	test    eax, eax
	setnz   al
L2:
	ret

TSSBundleList_Write_ErrorSkip endp

end
