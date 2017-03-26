.486
.model flat

extrn _EnableParserFix:dword
extrn @bcb6_std_string_assign@8:proc
extrn _ReplaceDefine@8:proc

public _TSSGCtrl_ReadSSRFile_ReplaceDefine

.code

align 16

_TSSGCtrl_ReadSSRFile_ReplaceDefine proc near

	CallAddress                         equ 004AE6C0H
	_this                               equ edi
	offsetof_TSSGCtrl_attributeSelector equ 32
	VIt                                 equ esi
	tmpS                                equ <ebp - 68H>
	Src                                 equ <esp + 4H>

	mov     eax, dword ptr [_EnableParserFix]
	push    CallAddress
	test    eax, eax
	jz      L1
	mov     edx, VIt
	lea     ecx, [tmpS]
	mov     dword ptr [Src + 4], ecx
	lea     eax, [_this + offsetof_TSSGCtrl_attributeSelector]
	push    ecx
	push    eax
	call    @bcb6_std_string_assign@8
	call    _ReplaceDefine@8
L1:
	ret

_TSSGCtrl_ReadSSRFile_ReplaceDefine endp

end
