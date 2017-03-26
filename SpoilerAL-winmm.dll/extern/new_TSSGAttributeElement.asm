.486
.model flat, c

public new_TSSGAttributeElement

.data

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

;TSSGAttributeElement * __cdecl new_TSSGAttributeElement()
;{
;	return new TSSGAttributeElement();
;}
new_TSSGAttributeElement proc near

	push    ebp
	mov     ecx, 005D54CCH
	mov     ebp, esp
	sub     esp, 40
	mov     eax, offset data2
	call    ecx
	mov     eax, 005D44B8H
	push    8
	call    eax
	test    eax, eax
	jz      L1
	mov     dword ptr [eax], 006151C0H
	mov     dword ptr [eax + 4], 0
L1:
	pop     ecx
	mov     esp, ebp
	pop     ebp
	ret

new_TSSGAttributeElement endp

end
