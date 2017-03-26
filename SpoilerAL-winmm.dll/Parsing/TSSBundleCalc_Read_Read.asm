.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSBundleCalc_Read_Read

.code

align 16

_TSSBundleCalc_Read_Read proc near

	TSSGCtrl_Read equ 0051C3A0H

	push    TSSGCtrl_Read
	mov     ecx, dword ptr [ebp - 7CH]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	lea     ecx, [ebp - 7CH]
	jmp     @bcb6_std_string_dtor@4

_TSSBundleCalc_Read_Read endp

end
