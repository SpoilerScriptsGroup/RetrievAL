.486
.model flat

extrn @bcb6_std_string_ctor_assign_cstr@8:proc

public _TSSGActionListner_OnParsingProcess61@16

.code

align 16

;void __stdcall TSSGActionListner_OnParsingProcess61(TSSGActionListnerBase *ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal)
;{
;	ssgActionListner->OnParsingProcess(SSGS, Code, TopVal);
;}
_TSSGActionListner_OnParsingProcess61@16 proc near

	ssgActionListner equ <esp +  4>
	SSGS             equ <esp +  8>
	Code             equ <esp + 12>
	TopVal           equ <esp + 16>

	mov     eax, dword ptr [TopVal]
	sub     esp, 28
	mov     edx, dword ptr [Code + 28]
	mov     dword ptr [esp + 24], eax
	mov     ecx, esp
	call    @bcb6_std_string_ctor_assign_cstr@8
	mov     ecx, dword ptr [ssgActionListner + 28]
	mov     edx, dword ptr [SSGS + 28]
	mov     eax, dword ptr [ecx]
	push    edx
	push    ecx
	call    dword ptr [eax + 44]
	add     esp, 36
	ret     16

_TSSGActionListner_OnParsingProcess61@16 endp

end
