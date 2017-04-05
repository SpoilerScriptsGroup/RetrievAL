.486
.model flat

extrn _EnableParserFix:dword
extrn _ReplaceDefine@8:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_std_string_ctor_assign:dword

public _TSSGCtrl_ReadSSRFile_ReplaceDefine

.code

align 16

_TSSGCtrl_ReadSSRFile_ReplaceDefine proc near

	TStringDivision_ToULongDef          equ 004AE6C0H
	_this                               equ edi
	offsetof_TSSGCtrl_attributeSelector equ 32
	VIt                                 equ esi
	tmpS                                equ <ebp - 68H>

	mov     eax, dword ptr [_EnableParserFix]
	mov     ecx, TStringDivision_ToULongDef
	test    eax, eax
	jz      L1
	push    VIt
	lea     ecx, [tmpS]
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	add     esp, 8
	lea     eax, [tmpS]
	push    eax
	lea     eax, [_this + offsetof_TSSGCtrl_attributeSelector]
	push    eax
	call    _ReplaceDefine@8
	mov     eax, TStringDivision_ToULongDef
	lea     ecx, [tmpS]
	push    0
	push    ecx
	call    eax
	add     esp, 8
	lea     ecx, [tmpS]
	push    eax
	call    @bcb6_std_string_dtor@4
	pop     eax
	ret

	align   16
L1:
	jmp     ecx

_TSSGCtrl_ReadSSRFile_ReplaceDefine endp

end
