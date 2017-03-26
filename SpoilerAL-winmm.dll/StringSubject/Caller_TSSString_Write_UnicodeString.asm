.486
.model flat, c

extrn TSSString_Write_UnicodeString@12:proc

public Caller_TSSString_Write_UnicodeString

.code

align 16

Caller_TSSString_Write_UnicodeString proc near

	NextCallAddress            equ 0051C408H
	TSSString                  equ ecx
	offsetof_TSSString_endWord equ 152
	CompareVec                 equ <ebp - 4CH>
	_Str                       equ <ebp - 1CH>

	add     ecx, offsetof_TSSString_endWord
	lea     edx, [_Str]
	push    edx
	lea     eax, [CompareVec]
	push    eax
	push    ecx
	push    NextCallAddress
	jmp     TSSString_Write_UnicodeString@12

Caller_TSSString_Write_UnicodeString endp

end
