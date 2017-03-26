.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSDoubleList_Write_OneWrite

.code

align 16

_TSSDoubleList_Write_OneWrite proc near

	TSSGCtrl_OneWrite equ 0051C540H

	push    TSSGCtrl_OneWrite
	mov     ecx, dword ptr [esi + 98H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSDoubleList_Write_OneWrite endp

end
