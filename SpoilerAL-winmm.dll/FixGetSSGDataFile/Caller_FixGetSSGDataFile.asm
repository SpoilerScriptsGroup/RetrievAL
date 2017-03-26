.486
.model flat, c

public Caller_GetSSGDataFile

extrn FixGetSSGDataFile@4:proc

.code

align 16

Caller_GetSSGDataFile proc near

	NextCallAddress equ 005D5258H
	FileName        equ <ebp - 1A8H>

	lea     eax, [FileName]
	push    eax
	push    NextCallAddress
	jmp     FixGetSSGDataFile@4

Caller_GetSSGDataFile endp

end
