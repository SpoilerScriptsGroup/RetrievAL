.486
.model flat, c

public Caller_OnSSGCtrlCleared

extrn OnSSGCtrlCleared:proc

.code

align 16

Caller_OnSSGCtrlCleared proc near

	pop     ebx
	pop     ebp
	jmp     OnSSGCtrlCleared

Caller_OnSSGCtrlCleared endp

end
