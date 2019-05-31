.386
.model flat

include align.inc

public _strcat

extern _memcpy: near
extern _strlen: near

.code

$align 16
_strcat proc near
	mov     eax, dword ptr [esp + 8]                    ; src
	push    eax
	call    _strlen                                     ; length of src
	inc     eax                                         ; include terminating zero in length
	push    eax                                         ; strlen(src)+1
	mov     edx, dword ptr [esp + 4 + 8]                ; dest
	push    edx
	call    _strlen                                     ; length of dest
	pop     edx                                         ; dest. Assume unchanged by _A_strlen
	add     edx, eax                                    ; dest+strlen(dest)
	mov     ecx, dword ptr [esp + 8 + 8]                ; src
	                                                    ; strlen(src)+1 is on stack
	push    ecx                                         ; src
	push    edx                                         ; dest+strlen(dest)
	call    _memcpy                                     ; copy
	add     esp, 16                                     ; clean up stack
	mov     eax, dword ptr [esp + 4]                    ; return dest
	ret
_strcat endp

end
