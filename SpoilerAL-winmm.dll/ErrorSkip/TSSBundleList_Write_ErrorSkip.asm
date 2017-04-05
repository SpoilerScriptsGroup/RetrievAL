.486
.model flat, c

extrn TSSGCtrl_GetAttribute:dword
extrn TSSGCtrl_OneWrite:dword

public TSSBundleList_Write_ErrorSkip

.code

align 16

TSSBundleList_Write_ErrorSkip proc near

	ReturnAddress equ 004BF89EH
	AT_ERRORSKIP  equ 2000H
	SSGC          equ <ebp + 12>
	_this         equ esi

	mov     dword ptr [esp], offset L1
	jmp     dword ptr [TSSGCtrl_OneWrite]
L1:
	push    ReturnAddress
	test    al, al
	jnz     L2
	mov     ecx, dword ptr [SSGC]
	push    AT_ERRORSKIP
	push    _this
	push    ecx
	call    dword ptr [TSSGCtrl_GetAttribute]
	add     esp, 12
	test    eax, eax
	setnz   al
L2:
	ret

TSSBundleList_Write_ErrorSkip endp

end
