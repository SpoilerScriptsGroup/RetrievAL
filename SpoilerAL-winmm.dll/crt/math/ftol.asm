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

	fnstcw  word ptr [esp - 4]
	mov     ax, word ptr [esp - 4]
	or      ax, 0C00h
	mov     word ptr [esp - 8], ax
	fldcw   word ptr [esp - 8]
	fistp   qword ptr [esp - 16]
	fldcw   word ptr [esp - 4]
	mov     eax, dword ptr [esp - 16]
	mov     edx, dword ptr [esp - 12]
	ret

_ftol endp

end
