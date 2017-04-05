.486
.model flat

IO_FEP_SUPPORT equ 1

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
if IO_FEP_SUPPORT
extrn _TSSGCtrl_OneWrite_with_CheckIO_FEP:proc
else
extrn _TSSGCtrl_OneWrite:dword
endif

public _TSSDoubleList_Write_OneWrite

.code

align 16

_TSSDoubleList_Write_OneWrite proc near

	mov     ecx, dword ptr [esi + 98H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 12], eax
L1:
if IO_FEP_SUPPORT
	jmp     _TSSGCtrl_OneWrite_with_CheckIO_FEP
else
	jmp     dword ptr [_TSSGCtrl_OneWrite]
endif

_TSSDoubleList_Write_OneWrite endp

end
