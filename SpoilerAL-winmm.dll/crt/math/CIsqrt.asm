.486
.model flat, c

public _CIsqrt

.code

align 16

_CIsqrt proc near

	fsqrt                           ; Take the square root
	ret

_CIsqrt endp

end
