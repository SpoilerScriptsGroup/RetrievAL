.486
.model flat, c

extrn EnableParserFix:dword
extrn FixTraceAndCopySetting@8:proc

public Caller_FixTraceSetting

.code

align 16

Caller_FixTraceSetting proc near

	SSGC                              equ <ebp + 0CH>
	_this                             equ ebx
	bcb6_std_vector_string_deallocate equ 00415F90H

	mov     eax, dword ptr [EnableParserFix]
	mov     ecx, bcb6_std_vector_string_deallocate
	test    eax, eax
	jz      L1
    push    dword ptr [SSGC]
    push    _this
	push    ecx
	jmp     FixTraceAndCopySetting@8
L1:
	jmp     ecx

Caller_FixTraceSetting endp

end
