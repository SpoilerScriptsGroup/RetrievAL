.486
.model flat, c

public _CIacos

.code

align 16

_CIacos proc near

	fld     st(0)                   ; Load x
	fld     st(0)                   ; Load x
	fmul                            ; Multiply (x squared)
	fld1                            ; Load 1
	fsubr                           ; 1 - (x squared)
	fsqrt                           ; Square root of (1 - x squared)
	fxch                            ; Exchange st, st(1)
	fpatan                          ; This gives the arc cosine !
	ret

_CIacos endp

end
