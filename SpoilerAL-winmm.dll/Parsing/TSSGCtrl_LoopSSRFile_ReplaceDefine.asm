.486
.model flat

extrn _EnableParserFix:dword
extrn @bcb6_std_string_ctor_assign_range@12:proc
extrn @bcb6_std_string_dtor@4:proc
extrn _ReplaceDefine@8:proc

public _TSSGCtrl_LoopSSRFile_ReplaceDefine
public _TSSGCtrl_LoopSSRFile_ReplaceDefine_Release

.code

align 16

_TSSGCtrl_LoopSSRFile_ReplaceDefine proc near

	CallAddress                         equ 0043CC08H
	_this                               equ <ebp + 8H>
	offsetof_TSSGCtrl_attributeSelector equ 32
	_Str                                equ <ebp - 110H>
	begin                               equ <ebp - 1F8H>
	_end                                equ <ebp - 1F4H>

	mov     ecx, dword ptr [_EnableParserFix]
	mov     eax, dword ptr [_end]
	test    ecx, ecx
	jz      L1

	push    eax
	lea     ecx, [_Str]
	call    @bcb6_std_string_ctor_assign_range@12

	mov     eax, dword ptr [_this]
	lea     ecx, [_Str]
	add     eax, offsetof_TSSGCtrl_attributeSelector
	push    ecx
	push    eax
	call    _ReplaceDefine@8

	mov     ecx, dword ptr [_Str]
	mov     eax, dword ptr [_Str + 4]
	mov     dword ptr [begin], ecx
L1:
	ret

_TSSGCtrl_LoopSSRFile_ReplaceDefine endp

align 16

_TSSGCtrl_LoopSSRFile_ReplaceDefine_Release proc near

	_Str equ <ebp - 110H>

	mov     eax, dword ptr [_EnableParserFix]
	lea     ecx, [_Str]
	test    eax, eax
	jz      L1
	call    @bcb6_std_string_dtor@4
L1:
	mov     word ptr [ebx + 10H], 20
	ret

_TSSGCtrl_LoopSSRFile_ReplaceDefine_Release endp

end
