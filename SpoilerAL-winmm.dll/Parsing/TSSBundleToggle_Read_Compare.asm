.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword

public _TSSBundleToggle_Read_Compare

.code

align 16

_TSSBundleToggle_Read_Compare proc near

	TSSGCtrl_Compare equ 0051C470H

	mov     ecx, dword ptr [ebp - 34H]
	push    TSSGCtrl_Compare
	mov     ecx, dword ptr [ecx]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	ret

_TSSBundleToggle_Read_Compare endp

end
