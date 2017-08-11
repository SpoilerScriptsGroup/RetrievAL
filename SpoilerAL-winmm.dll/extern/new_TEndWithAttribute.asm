.486
.model flat, c
assume fs:nothing

extrn bcb6_std_string_ctor:dword
extrn bcb6_global_operator_new:dword
extrn F005D54CC:dword

public new_TEndWithAttribute

.const

data1 label byte
	dword   004EBAC4H
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

;EXTERN_C TEndWithAttribute * __cdecl new_TEndWithAttribute()
;{
;	return new TEndWithAttribute();
;}
new_TEndWithAttribute proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [F005D54CC]
	push    32
	call    dword ptr [bcb6_global_operator_new]
	test    eax, eax
	jz      L1
	pop     ecx
	mov     dword ptr [ebp - 4], eax
	mov     dword ptr [eax + 4], 0
	inc     dword ptr [ebp - 12]
	mov     dword ptr [eax], 0064030CH
	add     eax, 8
	push    eax
	call    dword ptr [bcb6_std_string_ctor]
	mov     eax, dword ptr [ebp - 4]
	mov     dword ptr [eax + 4], 64
L1:
	mov     ecx, dword ptr [ebp - 40]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret

new_TEndWithAttribute endp

end
