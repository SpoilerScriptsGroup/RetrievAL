.486
.model flat, c

extrn cosh:proc

public _CIcosh

.code

align 16

_CIcosh proc near

	sub     esp, 8                  ; Allocate stack space for x
	fstp    qword ptr [esp]         ; Copy x onto stack
	call    cosh                    ; Call cosh
	add     esp, 8                  ; Remove x from stack
	ret

_CIcosh endp

end
