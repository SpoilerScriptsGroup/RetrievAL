.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSDoubleToggle_Read_Compare2

.code

align 16

_TSSDoubleToggle_Read_Compare2 proc near

	TSSGCtrl_Compare equ 0051C470H

	mov     ecx, dword ptr [ebp - 144H]
	push    TSSGCtrl_Compare
	mov     ecx, dword ptr [ecx]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSDoubleToggle_Read_Compare2 endp

end
