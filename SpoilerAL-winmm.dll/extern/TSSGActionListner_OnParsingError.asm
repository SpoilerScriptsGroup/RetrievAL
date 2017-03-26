.486
.model flat, c

public TSSGActionListner_OnParsingError@12

.code

align 16

;void __stdcall TSSGActionListner_OnParsingError(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code)
;{
;	ssgActionListner->OnParsingError(SSGS, Code);
;}
TSSGActionListner_OnParsingError@12 proc near

	Code             equ <esp + 12>

	mov     eax, esp
	mov     edx, dword ptr [Code]
	sub     esp, 32
	sub     eax, 8
	mov     ecx, esp
	push    eax
	mov     eax, 004165E8H
	push    edx
	push    ecx
	call    eax

	ssgActionListner equ <esp + 48>

	mov     ecx, dword ptr [ssgActionListner]
	add     esp, 12

	SSGS             equ <esp + 40>

	mov     edx, dword ptr [SSGS]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 40]
	add     esp, 40
	ret     12

TSSGActionListner_OnParsingError@12 endp

end
