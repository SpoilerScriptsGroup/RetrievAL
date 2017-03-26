.486
.model flat, c

extrn EnableParserFix:dword
extrn TSSGCtrl_LoopSSRFile_ReplaceDefine@20:proc

public Caller_TSSGCtrl_LoopSSRFile_ReplaceDefine

.code

align 16

Caller_TSSGCtrl_LoopSSRFile_ReplaceDefine proc near

	ReturnAddress equ 00502676H
	_this         equ <ebp + 8H>
	tmpS          equ <ebp - 78H>
	_Str          equ <ebp - 110H>
	File          equ esi
	Index         equ eax

	cmp     dword ptr [EnableParserFix], 0
	je      L1
	push    eax
	lea     edx, [_Str]
	push    esi
	lea     ecx, [tmpS]
	mov     eax, dword ptr [_this]
	push    edx
	push    ecx
	push    eax
	push    ReturnAddress
	jmp     TSSGCtrl_LoopSSRFile_ReplaceDefine@20
L1:
	mov     ecx, 0050213AH
	mov     dword ptr [ebp - 1E8H], eax
	jmp     ecx

Caller_TSSGCtrl_LoopSSRFile_ReplaceDefine endp

end
