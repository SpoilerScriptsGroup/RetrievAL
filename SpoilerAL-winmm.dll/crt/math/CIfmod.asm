.486
.model flat, c

public _CIfmod

.code

align 16

_CIfmod proc near

	fxch    st(1)                   ; Swap arguments
L1:
	fprem                           ; Get the partial remainder
	fstsw   ax                      ; Get coprocessor status
	test    ax, 0400H               ; Complete remainder ?
	jnz     L1                      ; No, go get next remainder
	fstp    st(1)                   ; Set new top of stack
	ret

_CIfmod endp

end
