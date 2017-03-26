.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSCopy_Write_OneRead

.code

align 16

_TSSCopy_Write_OneRead proc near

	TSSGCtrl_OneRead equ 0051C4D8H

	push    TSSGCtrl_OneRead
	mov     ecx, dword ptr [ebx + 98H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSCopy_Write_OneRead endp

end
