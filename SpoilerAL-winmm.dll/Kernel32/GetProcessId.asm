.486
.model flat, c

includelib kernel32.lib

extrn _imp__GetModuleHandleA@4:dword
extrn _imp__GetProcAddress@8:dword
extrn GetProcessIdCompatible@4:proc

public _imp__GetProcessId@4
public GetProcessId@4

.data

_imp__GetProcessId@4 dword GetProcessId@4$initializer$

.const

lpModuleName byte 'kernel32.dll', 0
lpProcName byte 'GetProcessId', 0

.code

align 16

GetProcessId@4$initializer$ proc near

	push    offset lpModuleName
	call    dword ptr [_imp__GetModuleHandleA@4]
	test    eax, eax
	jz      short L1
	push    offset lpProcName
	push    eax
	call    dword ptr [_imp__GetProcAddress@8]
	test    eax, eax
	jnz     L2
L1:
	mov     eax, offset GetProcessIdCompatible@4
L2:
	mov     dword ptr [_imp__GetProcessId@4], eax
	jmp     eax

GetProcessId@4$initializer$ endp

align 16

GetProcessId@4 proc near

	jmp     dword ptr [_imp__GetProcessId@4]

GetProcessId@4 endp

end
