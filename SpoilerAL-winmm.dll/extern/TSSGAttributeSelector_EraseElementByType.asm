.486
.model flat, c

public TSSGAttributeSelector_EraseElementByType

.code

align 16

;void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *attributeSelector, unsigned long type);

TSSGAttributeSelector_EraseElementByType proc near

	push    ebp
	mov     ecx, 004D40B2H
	mov     ebp, esp
	sub     esp, 64
	push    ebx
	push    esi
	push    edi
	lea     ebx, [ebp - 64]
	mov     eax, dword ptr [ebp + 12]
	jmp     ecx

TSSGAttributeSelector_EraseElementByType endp

end
