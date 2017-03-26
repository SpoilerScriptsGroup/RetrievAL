.486
.model flat, c

extrn TSSString_ToByteCode_UnicodeString@12:proc

public Caller_TSSString_ToByteCode_UnicodeString

.code

align 16

Caller_TSSString_ToByteCode_UnicodeString proc near

	NextCallAddress            equ 004462DCH
	TSSString                  equ ecx
	offsetof_TSSString_endWord equ 152
	Val                        equ <ebp - 18H>
	tmpS                       equ edx

	add     ecx, offsetof_TSSString_endWord
	lea     eax, [Val]
	push    tmpS
	push    eax
	push    ecx
	push    NextCallAddress
	jmp     TSSString_ToByteCode_UnicodeString@12

Caller_TSSString_ToByteCode_UnicodeString endp

end
