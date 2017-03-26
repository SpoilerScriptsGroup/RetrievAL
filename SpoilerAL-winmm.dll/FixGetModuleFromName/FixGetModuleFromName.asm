.486
.model flat, c

public FixGetModuleFromName

.code

align 16

FixGetModuleFromName proc near

	ProcessCtrl                              equ ecx
	offsetof_ProcessCtrl_entry_th32ProcessID equ 200
	ProcessCtrl_LoadModuleList               equ 004A4E4CH
	ProcessCtrl_Attach                       equ 004A610CH

	mov     ecx, dword ptr [ProcessCtrl + offsetof_ProcessCtrl_entry_th32ProcessID]
	mov     eax, ProcessCtrl_LoadModuleList
	test    ecx, ecx
	jz      L1
	jmp     eax
L1:
	mov     eax, ProcessCtrl_Attach
	jmp     eax

FixGetModuleFromName endp

end
