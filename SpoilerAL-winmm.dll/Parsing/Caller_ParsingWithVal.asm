.486
.model flat, c

public Caller_ParsingWithVal

extrn EnableParserFix:dword
extrn Parsing:proc
;extrn Parsing61:proc

.const

lpszVal byte 'Val', 0

.code

align 16

Caller_ParsingWithVal proc near

	cmp     dword ptr [EnableParserFix], 0
	je      L1
	mov     eax, dword ptr [esp + 16]
	mov     edx, dword ptr [esp + 12]
	push    0
	push    0
	push    eax
	push    offset lpszVal
	push    3
	mov     ecx, dword ptr [esp + 28]
	mov     eax, dword ptr [esp + 24]
	push    edx
	push    ecx
	push    eax
	call    Parsing
	add     esp, 32
	ret
	align   16
L1:
if 0
	jmp     Parsing61
else
	mov     eax, 005113F8H
	jmp     eax
endif

Caller_ParsingWithVal endp

end
