.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSDoubleList_Read_OneRead

.code

align 16

_TSSDoubleList_Read_OneRead proc near

	TSSGCtrl_OneRead equ 0051C4D8H

	push    TSSGCtrl_OneRead
	mov     ecx, dword ptr [edi + 98H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSDoubleList_Read_OneRead endp

end
