.486
.model flat, c

extrn EnableParserFix:dword
extrn FixTraceAndCopySetting@8:proc

public Caller_FixCopySetting

.code

align 16

Caller_FixCopySetting proc near

	NextCallAddress equ 00415F90H
	SSGC            equ <ebp + 0CH>
	_this           equ ebx

	mov     eax, dword ptr [EnableParserFix]
	mov     ecx, 00415F90H
	test    eax, eax
	jz      L1
    push    dword ptr [SSGC]
    push    _this
	push    NextCallAddress
	jmp     FixTraceAndCopySetting@8
L1:
	jmp     ecx

Caller_FixCopySetting endp

end
