.486
.model flat, c

includelib dwmapi.lib

extrn _imp__GetModuleHandleA@4:dword
extrn _imp__GetProcAddress@8:dword

public hDwmAPI
public _imp__DwmGetWindowAttribute@16

.data

hDwmAPI                        dword 0
_imp__DwmGetWindowAttribute@16 dword DwmGetWindowAttribute@16$initializer$

.const

lpProcName byte 'DwmGetWindowAttribute', 0

.code

E_NOTIMPL equ 80004001H

align 16

DwmGetWindowAttribute@16$initializer$ proc near

	mov     eax, dword ptr [hDwmAPI]
	test    eax, eax
	jz      short L1
	push    offset lpProcName
	push    eax
	call    dword ptr [_imp__GetProcAddress@8]
	test    eax, eax
	jnz     L2
L1:
	mov     eax, offset DwmGetWindowAttributeStub@16
L2:
	mov     dword ptr [_imp__DwmGetWindowAttribute@16], eax
	jmp     eax

DwmGetWindowAttribute@16$initializer$ endp

align 16

DwmGetWindowAttributeStub@16 proc near

	mov     eax, E_NOTIMPL
	ret     16

DwmGetWindowAttributeStub@16 endp

end
