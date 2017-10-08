.486
.model flat, c

public _CIasin

.code

align 16

_CIasin proc near

	fld     st(0)                   ; Load x
	fld     st(0)                   ; Load x
	fmul                            ; Multiply (x squared)
	fld1                            ; Load 1
	fsubr                           ; 1 - (x squared)
	fsqrt                           ; Square root of (1 - x squared)
	fpatan                          ; This gives the arc sine !
	ret

_CIasin endp

end
