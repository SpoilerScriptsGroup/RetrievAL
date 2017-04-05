.486
.model flat, c

extrn Caller_ParsingWithVal:proc
extrn A_strlen:proc
extrn bcb6__snprintf:dword

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
	push    256 - 1
	push    edx
	call    dword ptr [bcb6__snprintf]
	add     esp, 16
	push    buffer
	call    A_strlen
	lea     edx, [buffer + 4]
	pop     ecx
	lea     ecx, [edx + 256]
	add     eax, edx
	push    ecx
	push    eax
	lea     ecx, [tmpS]
	mov     eax, 00418610H
	push    edx
	push    ecx
	call    eax
	add     esp, 16 + 256 + 8

align 16

TSSGCtrl_LoopSSRFile_LineListLoopContinue:
	mov     ecx, 0050267CH
	mov     eax, dword ptr [ebp - 180H]
	jmp     ecx

end
