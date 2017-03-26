.486
.model flat, c

public Caller_Parsing

extrn EnableParserFix:dword
extrn Parsing:proc
;extrn Parsing61:proc

.code

align 16

Caller_Parsing proc near

	cmp     dword ptr [EnableParserFix], 0
	jne     Parsing
if 0
	jmp     Parsing61
else
	mov     eax, 005113F8H
	jmp     eax
endif

Caller_Parsing endp

end
