.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSDoubleToggle_Write_Write2

.code

align 16

_TSSDoubleToggle_Write_Write2 proc near

	TSSGCtrl_Write equ 0051C408H

	push    TSSGCtrl_Write
	mov     ecx, dword ptr [ebp - 15CH]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	lea     ecx, [ebp - 15CH]
	jmp     @bcb6_std_string_dtor@4

_TSSDoubleToggle_Write_Write2 endp

end
