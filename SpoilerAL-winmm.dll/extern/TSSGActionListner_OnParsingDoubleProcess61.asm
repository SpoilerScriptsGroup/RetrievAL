.486
.model flat, c

extrn _bcb6_std_string_ctor_assign_cstr:dword
public TSSGActionListner_OnParsingDoubleProcess61@20

.code

align 16

;void __stdcall TSSGActionListner_OnParsingDoubleProcess61(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal)
;{
;	ssgActionListner->OnParsingDoubleProcess(SSGS, Code, TopVal);
;}
TSSGActionListner_OnParsingDoubleProcess61@20 proc near

	TopVal           equ <esp + 16>

	mov     eax, dword ptr [TopVal + 4]
	mov     ecx, dword ptr [TopVal]
	push    eax
	push    ecx

	Code             equ <esp + 20>

	mov     eax, esp
	mov     edx, dword ptr [Code]
	sub     esp, 32
	sub     eax, 8
	mov     ecx, esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign_cstr]

	ssgActionListner equ <esp + 56>

	mov     ecx, dword ptr [ssgActionListner]
	add     esp, 12

	SSGS             equ <esp + 48>

	mov     edx, dword ptr [SSGS]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 48]
	add     esp, 48
	ret     20

TSSGActionListner_OnParsingDoubleProcess61@20 endp

end
