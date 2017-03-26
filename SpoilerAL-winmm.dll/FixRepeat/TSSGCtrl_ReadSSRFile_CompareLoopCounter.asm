.486
.model flat, c

public TSSGCtrl_ReadSSRFile_CompareLoopCounter

.code

align 16

TSSGCtrl_ReadSSRFile_CompareLoopCounter proc near

	signedLoop equ <esp>
	Step       equ <ebp - 0F8H>
	_End       equ <ebp - 0F4H>
	i          equ <esi>

	mov     eax, dword ptr [signedLoop]
	mov     ecx, dword ptr [_End]
	test    eax, eax
	mov     edx, dword ptr [Step]
	mov     eax, 004FF2F1H
	jnz     L2
L1:
	cmp     i, ecx
	jb      L5
	jmp     L4
L2:
	test    edx, edx
	js      L3
	cmp     i, ecx
	jl      L5
	jmp     L4
L3:
	cmp     i, ecx
	jg      L5
L4:
	mov     eax, 004FF3A4H
L5:
	jmp     eax

TSSGCtrl_ReadSSRFile_CompareLoopCounter endp

end
