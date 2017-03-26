.486
.model flat, c

public new_TIO_FEPAttribute

.data

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

;TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute()
;{
;	return new TIO_FEPAttribute();
;}
new_TIO_FEPAttribute proc near

	push    ebp
	mov     ecx, 005D54CCH
	mov     ebp, esp
	sub     esp, 40
	mov     eax, offset data2
	call    ecx
	mov     eax, 005D44B8H
	push    56
	call    eax
	test    eax, eax
	jz      L1
	pop     ecx
	mov     dword ptr [ebp - 4], eax
	mov     dword ptr [eax + 4], 0
	inc     dword ptr [ebp - 12]
	mov     dword ptr [eax], offset 00640324H
	add     eax, 8
	mov     ecx, 00402590H
	push    eax
	call    ecx
	pop     ecx
	mov     eax, dword ptr [ebp - 12]
	mov     ecx, dword ptr [ebp - 4]
	add     eax, 4
	add     ecx, 32
	mov     dword ptr [ebp - 12], eax
	push    ecx
	mov     eax, 00402590H
	call    eax
	mov     eax, dword ptr [ebp - 4]
	mov     dword ptr [eax + 4], 32
L1:
	mov     esp, ebp
	pop     ebp
	ret

new_TIO_FEPAttribute endp

end
