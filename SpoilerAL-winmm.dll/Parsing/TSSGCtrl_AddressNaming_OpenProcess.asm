.486
.model flat, c

extrn TSSGCtrl_OpenProcess@12:proc

public TSSGCtrl_AddressNaming_OpenProcess

.code

align 16

TSSGCtrl_AddressNaming_OpenProcess proc near

	ReturnAddress equ 00504F76H

	mov     ecx, dword ptr [esi]
	lea     eax, [edi + 1B0H]
	push    dword ptr [ecx + 18H]
	push    16
	push    eax
	push    ReturnAddress
	jmp     TSSGCtrl_OpenProcess@12

TSSGCtrl_AddressNaming_OpenProcess endp

end
