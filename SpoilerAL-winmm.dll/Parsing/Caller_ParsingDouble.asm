.486
.model flat, c

public Caller_ParsingDouble

extrn EnableParserFix:dword
extrn ParsingDouble:proc

.code

align 16

Caller_ParsingDouble proc near

	cmp     dword ptr [EnableParserFix], 0
	jne     ParsingDouble
	push    ebp
	mov     eax, 005174C0H + 9
	mov     ebp, esp
	sub     esp, 2288
	jmp     eax

Caller_ParsingDouble endp

end
