.486
.model flat, c

extrn Caller_ParsingWithVal:proc
extrn bcb6__snprintf:dword
extrn _bcb6_std_string_append_range:dword

public TSSGCtrl_LoopSSRFile_Format
public TSSGCtrl_LoopSSRFile_LineListLoopContinue

.code

align 16

TSSGCtrl_LoopSSRFile_Format:

	RT_FORMAT equ 20H
	_this     equ <ebp + 8H>
	LoopVal   equ <ebp + 14H>
	VIt       equ <ebp - 180H>
	tmpS      equ <ebp - 78H>
	SSGS      equ <ebp - 60H>
	_Type     equ <edi>

	cmp     _Type, RT_FORMAT
	jne     TSSGCtrl_LoopSSRFile_LineListLoopContinue
	sub     esp, 256 + 8

	buffer equ <esp>
	Val    equ <esp + 256>

	mov     eax, dword ptr [LoopVal]
	mov     edx, dword ptr [VIt]
	push    eax
	add     edx, 16
	lea     ecx, [SSGS]
	mov     eax, dword ptr [_this]
	push    edx
	push    ecx
	push    eax
	call    Caller_ParsingWithVal
	mov     ecx, dword ptr [VIt]
	add     esp, 16
	mov     ecx, dword ptr [ecx]
	mov     edx, buffer
	mov     dword ptr [VIt], ecx
	mov     ecx, dword ptr [ecx + 16]
	push    eax
	push    ecx
	push    256
	push    edx
	call    dword ptr [bcb6__snprintf]
	cmp     eax, 256
	jb      L2
	test    eax, eax
	js      L1
	mov     eax, 256 - 1
	jmp     L2
L1:
	xor     eax, eax
L2:
	add     esp, 16
	lea     edx, [buffer + 4]
	add     eax, edx
	lea     ecx, [edx + 256]
	push    ecx
	lea     ecx, [tmpS]
	push    eax
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_append_range]
	add     esp, 16 + 256 + 8

align 16

TSSGCtrl_LoopSSRFile_LineListLoopContinue:
	mov     ecx, 0050267CH
	mov     eax, dword ptr [ebp - 180H]
	jmp     ecx

end
