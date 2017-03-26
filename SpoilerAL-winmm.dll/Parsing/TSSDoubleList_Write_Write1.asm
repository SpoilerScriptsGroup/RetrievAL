.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSDoubleList_Write_Write1

.code

align 16

_TSSDoubleList_Write_Write1 proc near

	TSSGCtrl_Write equ 0051C408H

	push    TSSGCtrl_Write
	mov     ecx, dword ptr [ebp - 74H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	lea     ecx, [ebp - 74H]
	jmp     @bcb6_std_string_dtor@4

_TSSDoubleList_Write_Write1 endp

end
