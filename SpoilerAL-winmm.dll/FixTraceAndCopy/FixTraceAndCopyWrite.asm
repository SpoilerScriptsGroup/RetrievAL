.486
.model flat, c

extrn EnableParserFix:dword

public FixTraceAndCopyWrite

.code

align 16

FixTraceAndCopyWrite proc near

	mov     eax, dword ptr [EnableParserFix]
	mov     ecx, dword ptr [ebx]
	test    eax, eax
	jz      L1
	ret
L1:
	jmp     dword ptr [ecx + 4]

FixTraceAndCopyWrite endp

end
