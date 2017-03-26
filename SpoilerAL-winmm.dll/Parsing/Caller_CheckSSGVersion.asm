.486
.model flat, c

public Caller_CheckSSGVersion

extrn CheckSSGVersion@4:proc

.code

align 16

Caller_CheckSSGVersion proc near

	lpFileName      equ <esp + 8>
	NextCallAddress equ 004B65D0H

	push    dword ptr [lpFileName]
	push    NextCallAddress
	jmp     CheckSSGVersion@4

Caller_CheckSSGVersion endp

end
