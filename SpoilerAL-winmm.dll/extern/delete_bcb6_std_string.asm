.486
.model flat
assume fs:nothing

extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_global_operator_delete:dword
extrn _F005D54CC:dword

public @delete_bcb6_std_string@4

.const

data1 label byte
	dword   0040261CH
	dword   0000400FH, -4
	dword   00000000H

data2 label byte
	dword   00000000H, -40
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data1
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H

.code

align 16

;EXTERN_C void __msfastcall delete_bcb6_std_string(string *s)
;{
;	delete s;
;}
@delete_bcb6_std_string@4 proc near

	test    ecx, ecx
	jz      L1
	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 44
	push    ebx
	mov     ebx, ecx
	call    dword ptr [_F005D54CC]
	mov     ecx, ebx
	call    @bcb6_std_string_dtor@4
	push    ebx
	lea     eax, [ebx + 8]
	mov     dword ptr [ebp - 2CH], eax
	call    dword ptr [_bcb6_global_operator_delete]
	mov     ecx, dword ptr [ebp - 40]
	mov     ebx, dword ptr [esp + 4]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
L1:
	ret

@delete_bcb6_std_string@4 endp

end
