.486
.model flat

extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_global_operator_delete:dword

public @delete_bcb6_std_string@4

.data

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

;void __msfastcall delete_bcb6_std_string(string *str)
;{
;	delete str;
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
	mov     ecx, 005D54CCH
	call    ecx
	mov     ecx, ebx
	call    @bcb6_std_string_dtor@4
	push    ebx
	lea     eax, [ebx + 8]
	mov     dword ptr [ebp - 2CH], eax
	call    dword ptr [_bcb6_global_operator_delete]
	mov     ebx, dword ptr [esp + 4]
	mov     esp, ebp
	pop     ebp
L1:
	ret

@delete_bcb6_std_string@4 endp

end
