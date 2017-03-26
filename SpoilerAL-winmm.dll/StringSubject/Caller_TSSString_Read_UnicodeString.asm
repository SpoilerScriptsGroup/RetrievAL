.486
.model flat, c

extrn TSSString_Read_UnicodeString@16:proc

public Caller_TSSString_Read_UnicodeString

.code

align 16

Caller_TSSString_Read_UnicodeString proc near

	NextCallAddress equ 005D4494H
	_size           equ <ebx + 78H>
	endWord         equ <ebx + 98H>
	Data            equ <ebp - 1CH>
	tmpC            equ edi

	mov     eax, dword ptr [_size]
	lea     edx, [Data]
	push    tmpC
	lea     ecx, [endWord]
	push    edx
	push    ecx
	push    eax
	push    NextCallAddress
	jmp     TSSString_Read_UnicodeString@16

Caller_TSSString_Read_UnicodeString endp

end
