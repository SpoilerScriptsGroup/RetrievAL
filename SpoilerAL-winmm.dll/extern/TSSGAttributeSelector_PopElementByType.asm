.486
.model flat, c

public TSSGAttributeSelector_PopElementByType

.code

align 16

;void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *attributeSelector, unsigned long type);

TSSGAttributeSelector_PopElementByType proc near

	push    ebp
	mov     eax, 004D3E46H
	mov     ebp, esp
	sub     esp, 84
	push    ebx
	push    esi
	push    edi
	mov     edi, dword ptr [ebp + 8]
	mov     edx, dword ptr [ebp + 12]
	jmp     eax

TSSGAttributeSelector_PopElementByType endp

end
