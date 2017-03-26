.486
.model flat, c

public TSSGActionListner_OnParsingProcess61@16

.code

align 16

;void __stdcall TSSGActionListner_OnParsingProcess61(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal)
;{
;	ssgActionListner->OnParsingProcess(SSGS, Code, TopVal);
;}
TSSGActionListner_OnParsingProcess61@16 proc near

	TopVal           equ <esp + 16>
	Code             equ <esp + 12>

	mov     ecx, dword ptr [TopVal]
	mov     edx, dword ptr [Code]
	mov     eax, esp
	push    ecx
	sub     eax, 12
	sub     esp, 32
	mov     ecx, esp
	push    eax
	mov     eax, 004165E8H
	push    edx
	push    ecx
	call    eax

	ssgActionListner equ <esp + 52>

	mov     ecx, dword ptr [ssgActionListner]
	add     esp, 12

	SSGS             equ <esp + 44>

	mov     edx, dword ptr [SSGS]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 44]
	add     esp, 44
	ret     16

TSSGActionListner_OnParsingProcess61@16 endp

end
