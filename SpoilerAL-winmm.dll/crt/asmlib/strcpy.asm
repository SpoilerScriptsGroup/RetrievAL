.386
.model flat

public _strcpy

extern _memcpy: near
extern _strlen: near

.code

_strcpy proc near
	mov     eax, dword ptr [esp + 8]    ; src
	push    eax
	call    _strlen                     ; length of src
	pop     ecx                         ; ecx = src. Assume unchanged by _A_strlen
	inc     eax                         ; include terminating zero in length
	mov     edx, dword ptr [esp + 4]    ; dest
	push    eax                         ; length+1
	push    ecx                         ; src
	push    edx                         ; dest
	call    _memcpy                     ; copy
	add     esp, 12                     ; clean up stack
	ret
_strcpy endp

end
