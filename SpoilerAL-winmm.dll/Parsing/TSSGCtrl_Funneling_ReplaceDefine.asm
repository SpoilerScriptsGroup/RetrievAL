.486
.model flat

extrn _EnableParserFix:dword
extrn _TStringDivision_List:proc
extrn _ReplaceDefine@8:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_std_string_ctor_assign:dword

public _TSSGCtrl_Funneling_ReplaceDefine

.code

align 16

_TSSGCtrl_Funneling_ReplaceDefine proc near

	_this                               equ edi
	Src                                 equ eax
	offsetof_TSSGCtrl_attributeSelector equ 32

	cmp     dword ptr [_EnableParserFix], 0
	je      L1
	sub     esp, 24
	mov     edx, esp
	push    Src
	push    edx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	add     esp, 8
	lea     eax, [_this + offsetof_TSSGCtrl_attributeSelector]
	push    esp
	push    eax
	call    _ReplaceDefine@8
	mov     eax, dword ptr [esp + 24 + 40]
	mov     ecx, dword ptr [esp + 24 + 36]
	push    eax
	push    ecx
	mov     eax, dword ptr [esp + 24 + 40]
	mov     ecx, dword ptr [esp + 24 + 36]
	push    eax
	push    ecx
	mov     eax, dword ptr [esp + 24 + 40]
	mov     ecx, dword ptr [esp + 24 + 36]
	push    eax
	push    ecx
	mov     eax, dword ptr [esp + 24 + 40]
	mov     ecx, dword ptr [esp + 24 + 36]
	push    eax
	push    ecx
	mov     ecx, dword ptr [esp + 24 + 36]
	lea     eax, [esp + 32]
	push    eax
	push    ecx
	call    _TStringDivision_List
	add     esp, 40
	mov     ecx, esp
	push    eax
	call    @bcb6_std_string_dtor@4
	mov     eax, dword ptr [esp]
	add     esp, 4 + 24
	ret

	align   16
L1:
	jmp     _TStringDivision_List

_TSSGCtrl_Funneling_ReplaceDefine endp

end
