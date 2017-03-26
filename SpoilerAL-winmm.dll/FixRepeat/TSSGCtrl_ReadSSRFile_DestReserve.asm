.486
.model flat, c

public TSSGCtrl_ReadSSRFile_DestReserve

.code

align 16

TSSGCtrl_ReadSSRFile_DestReserve proc near

	signedLoop equ <esp>
	Step       equ <ebp - 0F8H>

	cmp     dword ptr [signedLoop], 0
	jne     L1
	div     dword ptr [Step]
	jmp     L2
L1:
	idiv    dword ptr [Step]
	test    eax, eax
	jns     L2
	neg     eax
L2:
	mov     ecx, 004FF2C0H
	jmp     ecx

TSSGCtrl_ReadSSRFile_DestReserve endp

end
