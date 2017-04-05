.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn _TSSGCtrl_OneRead:dword

public _TSSCopy_Write_OneRead

.code

align 16

_TSSCopy_Write_OneRead proc near

	mov     ecx, dword ptr [ebx + 98H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 12], eax
L1:
	jmp     dword ptr [_TSSGCtrl_OneRead]

_TSSCopy_Write_OneRead endp

end
