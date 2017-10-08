.486
.model flat, c

extrn tanh:proc

public _CItanh

.code

align 16

_CItanh proc near

	sub     esp, 8                  ; Allocate stack space for x
	fstp    qword ptr [esp]         ; Copy x onto stack
	call    tanh                    ; Call tanh
	add     esp, 8                  ; Remove x from stack
	ret

_CItanh endp

end
