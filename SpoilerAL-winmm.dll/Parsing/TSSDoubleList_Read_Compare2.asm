.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSDoubleList_Read_Compare2

.code

align 16

_TSSDoubleList_Read_Compare2 proc near

	TSSGCtrl_Compare equ 0051C470H

	push    TSSGCtrl_Compare
	mov     ecx, dword ptr [ebp - 0F4H]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	lea     ecx, [ebp - 0F4H]
	jmp     @bcb6_std_string_dtor@4

_TSSDoubleList_Read_Compare2 endp

end
