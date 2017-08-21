.486
.model flat

extrn @bcb6_std_string_ctor_assign_cstr@8:proc

public _TSSGActionListner_OnParsingDoubleProcess61@20

.code

align 16

;void __stdcall TSSGActionListner_OnParsingDoubleProcess61(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal)
;{
;	ssgActionListner->OnParsingDoubleProcess(SSGS, Code, TopVal);
;}
_TSSGActionListner_OnParsingDoubleProcess61@20 proc near

	ssgActionListner equ <esp +  4>
	SSGS             equ <esp +  8>
	Code             equ <esp + 12>
	TopVal           equ <esp + 16>

	mov     eax, dword ptr [TopVal + 4]
	mov     ecx, dword ptr [TopVal]
	push    eax
	push    ecx
	mov     edx, dword ptr [Code + 8]
	sub     esp, 24
	mov     ecx, esp
	call    @bcb6_std_string_ctor_assign_cstr@8
	mov     ecx, dword ptr [ssgActionListner + 32]
	mov     edx, dword ptr [SSGS + 32]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 48]
	add     esp, 40
	ret     20

_TSSGActionListner_OnParsingDoubleProcess61@20 endp

end
