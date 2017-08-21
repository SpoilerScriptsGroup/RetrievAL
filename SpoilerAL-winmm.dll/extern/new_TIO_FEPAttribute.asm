.486
.model flat
assume fs:nothing

extrn @bcb6_std_string_ctor@4:proc
extrn _bcb6_global_operator_new:dword
extrn _F005D54CC:dword

public _new_TIO_FEPAttribute

.const

data1 label byte
	dword   004EBA38H
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

;EXTERN_C TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute()
;{
;	return new TIO_FEPAttribute();
;}
_new_TIO_FEPAttribute proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [_F005D54CC]
	push    56
	call    dword ptr [_bcb6_global_operator_new]
	test    eax, eax
	jz      L1
	pop     ecx
	mov     dword ptr [ebp - 4], eax
	mov     dword ptr [eax + 4], 0
	inc     dword ptr [ebp - 12]
	mov     dword ptr [eax], offset 00640324H
	lea     ecx, [eax + 8]
	call    @bcb6_std_string_ctor@4
	mov     eax, dword ptr [ebp - 12]
	mov     ecx, dword ptr [ebp - 4]
	add     eax, 4
	add     ecx, 32
	mov     dword ptr [ebp - 12], eax
	call    @bcb6_std_string_ctor@4
	mov     eax, dword ptr [ebp - 4]
	mov     dword ptr [eax + 4], 32
L1:
	mov     ecx, dword ptr [ebp - 40]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret

_new_TIO_FEPAttribute endp

end
