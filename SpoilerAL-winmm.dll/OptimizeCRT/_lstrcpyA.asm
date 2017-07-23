.486
.model flat, c

public _lstrcpyA@8

extrn strcpy:proc

.code

align 16

_lstrcpyA@8 proc near

	mov     edx, dword ptr [esp + 8]
	mov     ecx, dword ptr [esp + 4]
	push    edx
	push    ecx
	call    strcpy
	add     esp, 8
	ret     8

_lstrcpyA@8 endp

end
