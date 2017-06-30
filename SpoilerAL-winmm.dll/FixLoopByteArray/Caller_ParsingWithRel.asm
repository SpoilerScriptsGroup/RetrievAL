.486
.model flat, c

public Caller_ParsingWithRel

extrn EnableParserFix:dword
extrn Parsing:proc
extrn F00439D1C:dword

.const

lpszRel byte 'Rel', 0

.code

align 16

Caller_ParsingWithRel proc near

	cmp     dword ptr [EnableParserFix], 0
	je      L1

	ReturnAddress equ 0050B51AH
	Rel           equ <ebp + 44>

	mov     eax, dword ptr [Rel]
	mov     ecx, dword ptr [ebp - 0D60H]
	push    0
	push    0
	push    eax
	push    offset lpszRel
	push    3
	lea     eax, [esi + 2]
	lea     edx, [ebp - 3E8H]
	sub     ecx, eax
	push    edx
	push    ecx
	lea     ecx, [ebp + 14H]
	push    eax
	push    ecx
	push    edx
	call    dword ptr [F00439D1C]
	add     esp, 16
	mov     ecx, dword ptr [ebp + 10H]
	mov     eax, dword ptr [ebp + 0CH]
	push    ecx
	push    eax
	call    Parsing
	mov     ecx, ReturnAddress
	add     esp, 32
	jmp     ecx
	align   16
L1:
	push    0
	mov     ecx, 0050B4D9H
	lea     eax, [esi + 2H]
	jmp     ecx

Caller_ParsingWithRel endp

end
