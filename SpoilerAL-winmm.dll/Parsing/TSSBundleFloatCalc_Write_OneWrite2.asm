.486
.model flat

includelib kernel32.lib

extrn @TSSGCtrl_IsRemoteProcess@4:proc
extrn __imp__GetCurrentProcess@0:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSBundleFloatCalc_Write_OneWrite2

.code

align 16

_TSSBundleFloatCalc_Write_OneWrite2 proc near

	TSSGCtrl_OneWrite equ 0051C540H

	push    TSSGCtrl_OneWrite
	mov     ecx, dword ptr [ebp - 4CH]
	call    @TSSGCtrl_IsRemoteProcess@4
	test    al, al
	jnz     L1
	call    dword ptr [__imp__GetCurrentProcess@0]
	mov     dword ptr [esp + 16], eax
L1:
	lea     ecx, [ebp - 4CH]
	jmp     @bcb6_std_string_dtor@4

_TSSBundleFloatCalc_Write_OneWrite2 endp

end
