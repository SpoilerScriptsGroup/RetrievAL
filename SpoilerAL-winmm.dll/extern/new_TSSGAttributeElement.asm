.486
.model flat, c
assume fs:nothing

extrn bcb6_global_operator_new:dword
extrn F005D54CC:dword

public new_TSSGAttributeElement

.const

data1 label byte
	dword   004D7D54H
	dword   00002007H, -4
	dword   00000000H

data2 label byte
	dword   00000000H, -40
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data1

.code

align 16

;EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement()
;{
;	return new TSSGAttributeElement();
;}
new_TSSGAttributeElement proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [F005D54CC]
	push    8
	call    dword ptr [bcb6_global_operator_new]
	test    eax, eax
	jz      L1
	mov     dword ptr [eax], 006151C0H
	mov     dword ptr [eax + 4], 0
L1:
	mov     ecx, dword ptr [ebp - 40]
	pop     edx
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret

new_TSSGAttributeElement endp

end
