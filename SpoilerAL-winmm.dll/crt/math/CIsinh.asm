.486
.model flat, c

extrn sinh:proc

public _CIsinh

.code

align 16

_CIsinh proc near

	sub     esp, 8                  ; Allocate stack space for x
	fstp    qword ptr [esp]         ; Copy x onto stack
	call    sinh                    ; Call sinh
	add     esp, 8                  ; Remove x from stack
	ret

_CIsinh endp

end
