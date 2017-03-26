.486
.model flat, c

public _lstrlenA@4

extrn A_strlen:proc

.code

align 16

_lstrlenA@4 proc near

	pop     eax
	pop     ecx
	push    eax
	push    ecx
	call    A_strlen
	pop     ecx
	ret

_lstrlenA@4 endp

end
