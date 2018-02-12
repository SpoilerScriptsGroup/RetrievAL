.486
.model flat, c

includelib kernel32.lib

extrn MsImg32Handle:dword
extrn _imp__GetProcAddress@8:dword
extrn AlphaBlendCompatible@44:proc

public _imp__AlphaBlend@44
;public AlphaBlend@44

.data

_imp__AlphaBlend@44 dword AlphaBlend@44$initializer$

.const

lpProcName byte 'AlphaBlend', 0

.code

align 16

AlphaBlend@44$initializer$ proc near

	mov     eax, dword ptr [MsImg32Handle]
	test    eax, eax
	jz      short L1
	push    offset lpProcName
	push    eax
	call    dword ptr [_imp__GetProcAddress@8]
	test    eax, eax
	jnz     L2
L1:
	mov     eax, offset AlphaBlendCompatible@44
L2:
	mov     dword ptr [_imp__AlphaBlend@44], eax
	jmp     eax

AlphaBlend@44$initializer$ endp
;
;align 16
;
;AlphaBlend@44 proc near
;
;	jmp     dword ptr [_imp__AlphaBlend@44]
;
;AlphaBlend@44 endp

end
