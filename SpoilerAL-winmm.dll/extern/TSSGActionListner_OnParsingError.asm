.486
.model flat

extrn @bcb6_std_string_ctor_assign_cstr@8:proc

public _TSSGActionListner_OnParsingError@12

.code

align 16

;void __stdcall TSSGActionListner_OnParsingError(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code)
;{
;	ssgActionListner->OnParsingError(SSGS, Code);
;}
_TSSGActionListner_OnParsingError@12 proc near

	ssgActionListner equ <esp +  4>
	SSGS             equ <esp +  8>
	Code             equ <esp + 12>

	mov     edx, dword ptr [Code]
	sub     esp, 24
	mov     ecx, esp
	call    @bcb6_std_string_ctor_assign_cstr@8
	mov     ecx, dword ptr [ssgActionListner + 24]
	mov     edx, dword ptr [SSGS + 24]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 40]
	add     esp, 32
	ret     12

_TSSGActionListner_OnParsingError@12 endp

end
