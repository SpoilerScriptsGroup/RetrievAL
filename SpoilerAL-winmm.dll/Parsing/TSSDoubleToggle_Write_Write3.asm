.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSDoubleToggle_Write_Write3

.code

align 16

_TSSDoubleToggle_Write_Write3 proc near

	TSSGCtrl_Write equ 0051C408H

	mov     ecx, dword ptr [ebp - 1ACH]
	push    TSSGCtrl_Write
	mov     ecx, dword ptr [ecx]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSDoubleToggle_Write_Write3 endp

end
