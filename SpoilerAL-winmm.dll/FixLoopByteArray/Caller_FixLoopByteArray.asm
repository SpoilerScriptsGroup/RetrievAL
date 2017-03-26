.486
.model flat, c

public Caller_FixLoopByteArray

extrn EnableParserFix:dword
extrn FixLoopByteArray@32:proc

.code

align 16

Caller_FixLoopByteArray proc near

	cmp     dword ptr [EnableParserFix], 0
	je      L2
	lea     ecx, [ebp - 10H]
	mov     eax, dword ptr [ebp - 0C80H]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ebp - 0C7CH]
	mov     eax, dword ptr [ebp - 0C78H]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ebp - 85CH]
	lea     eax, [ebp + 14H]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ebp + 10H]
	mov     eax, dword ptr [ebp + 0CH]
	push    ecx
	push    eax
	call    FixLoopByteArray@32
	mov     ecx, dword ptr [ebp - 344H]
	mov     eax, dword ptr [ebp - 334H]
	sub     eax, ecx
	mov     esi, dword ptr [ebp - 0C78H]
	mov     dword ptr [ebp - 0D04H], ecx
	mov     dword ptr [ebp - 0D00H], eax
	test    ecx, ecx
	jz      L1
	mov     eax, 0050B2F4H
	jmp     eax
L1:
	mov     eax, 0050B325H
	jmp     eax
	align   16
L2:
	dec     dword ptr [ebx + 1CH]
	dec     dword ptr [ebx + 1CH]
	mov     eax, 0050AFA2H
	jmp     eax

Caller_FixLoopByteArray endp

end
