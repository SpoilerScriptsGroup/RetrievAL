.486
.model flat
assume fs:nothing

extrn @bcb6_std_string_ctor@4:proc
extrn _bcb6_global_operator_new:dword
extrn _F005D54CC:dword

public _new_bcb6_std_string

.const

data1 label byte
	dword   0040261CH
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

;EXTERN_C string * __cdecl new_bcb6_std_string()
;{
;	return new string();
;}
_new_bcb6_std_string proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [_F005D54CC]
	push    24
	call    dword ptr [_bcb6_global_operator_new]
	pop     ecx
	test    eax, eax
	jz      L1
	mov     ecx, eax
	call    @bcb6_std_string_ctor@4
L1:
	mov     ecx, dword ptr [ebp - 40]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret

_new_bcb6_std_string endp

end
