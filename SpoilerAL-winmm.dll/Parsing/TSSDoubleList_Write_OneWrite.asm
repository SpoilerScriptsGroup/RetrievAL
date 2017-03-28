.486
.model flat

IO_FEP_SUPPORT equ 1

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
if IO_FEP_SUPPORT
extrn _TSSGCtrl_OneWrite_with_CheckIO_FEP:proc
endif

public _TSSDoubleList_Write_OneWrite

.code

align 16

_TSSDoubleList_Write_OneWrite proc near

	TSSGCtrl_OneWrite equ 0051C540H

if IO_FEP_SUPPORT
	push    offset _TSSGCtrl_OneWrite_with_CheckIO_FEP
else
	push    TSSGCtrl_OneWrite
endif
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
