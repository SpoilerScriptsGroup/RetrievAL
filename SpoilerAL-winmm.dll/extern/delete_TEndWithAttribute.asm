.486
.model flat

public @delete_TEndWithAttribute@4

.data

data1 label byte
	dword   00000008H, 00100400H
	dword   004EBAC4H
	dword   00000002H
	byte    'TEndWithAttribute *[2]', 0, 0

data2 label byte
	dword   data1
	dword   0000500FH, -8
	dword   00000000H

data3 label byte
	dword   00000000H, -44
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data2

.code

align 16

;EXTERN_C void __msfastcall delete_TEndWithAttribute(TEndWithAttribute *attribute)
;{
;	delete attribute;
;}
@delete_TEndWithAttribute@4 proc near

	test    ecx, ecx
	jz      L1
	push    ebp
	mov     eax, offset data3
	mov     ebp, esp
	sub     esp, 44
	mov     dword ptr [ebp - 8], ecx
	mov     ecx, 005D54CCH
	call    ecx
	mov     eax, dword ptr [ebp - 8]
	push    3
	mov     ecx, dword ptr [eax]
	push    eax
	mov     dword ptr [ebp - 4], ecx
	call    dword ptr [ecx]
	mov     esp, ebp
	pop     ebp
L1:
	ret

@delete_TEndWithAttribute@4 endp

end
