.486
.model flat

extrn _EnableParserFix:dword
extrn _ReplaceDefine@8:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _bcb6_std_string_ctor_assign:dword
extrn _strtoul:proc

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
	jz      L2
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
	push    eax
	mov     eax, dword ptr [tmpS]
	mov     ecx, esp
	push    0
	push    ecx
	push    eax
	call    _strtoul
	add     esp, 12
	mov     ecx, dword ptr [tmpS]
	pop     edx
	cmp     ecx, edx
	jne     L1
	mov     dword ptr [esp + 8], 004FF399H
	add     esp, 8
L1:
	push    eax
	lea     ecx, [tmpS]
	call    @bcb6_std_string_dtor@4
	pop     eax
	ret

	align   16
L2:
	jmp     ecx

_TSSGCtrl_ReadSSRFile_ReplaceDefine endp

end
