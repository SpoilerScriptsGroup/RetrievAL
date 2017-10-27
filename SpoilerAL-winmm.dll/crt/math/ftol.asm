;-----------------------------------------------------------------------------
; ftol.asm - floating point to integer conversion
;-----------------------------------------------------------------------------
.486
.model flat, c

public _ftol

alias <_ftol2> = <_ftol>
alias <_ftol2_sse> = <_ftol>

.code

align 16

_ftol proc near

	fnstcw  word ptr [esp - 2]
	mov     ax, word ptr [esp - 2]
	or      ax, 0C00h
	mov     word ptr [esp - 4], ax
	fldcw   word ptr [esp - 4]
	fistp   qword ptr [esp - 12]
	fldcw   word ptr [esp - 2]
	mov     eax, dword ptr [esp - 12]
	mov     edx, dword ptr [esp - 8]
	ret

_ftol endp

end
