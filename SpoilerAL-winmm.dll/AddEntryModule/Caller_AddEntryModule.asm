.486
.model flat, c

public Caller_AddEntryModule

extrn AddEntryModule@8:proc

.code

align 16

Caller_AddEntryModule proc near

	mov     eax, dword ptr [ebp + 8]
	sub     dword ptr [ebp - 56], 2

	_this                                     equ eax
	offsetof_TProcessCtrl_entry               equ 192
	offsetof_TProcessCtrl_entry_th32ProcessID equ 200
	offsetof_TProcessCtrl_moduleList          equ 512
	ReturnAddress                             equ 004A50DDH

	mov     ecx, dword ptr [_this + offsetof_TProcessCtrl_entry_th32ProcessID]
	add     eax, offsetof_TProcessCtrl_moduleList
	push    ecx
	push    eax
	push    ReturnAddress
	jmp     AddEntryModule@8

Caller_AddEntryModule endp

end
