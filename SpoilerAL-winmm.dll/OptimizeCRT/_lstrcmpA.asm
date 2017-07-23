.486
.model flat, c

public _lstrcmpA@8

extrn strcmp:proc

.code

align 16

_lstrcmpA@8 proc near

	mov     edx, dword ptr [esp + 8]
	mov     ecx, dword ptr [esp + 4]
	push    edx
	push    ecx
	call    strcmp
	add     esp, 8
	ret     8

_lstrcmpA@8 endp

end
