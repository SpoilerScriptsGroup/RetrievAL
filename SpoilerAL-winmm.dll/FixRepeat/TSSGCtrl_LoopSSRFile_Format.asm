.486
.model flat

extrn _Caller_ParsingWithVal:proc
extrn __snprintf:proc
extrn @bcb6_std_string_append_range@12:proc

public _TSSGCtrl_LoopSSRFile_Format
public _TSSGCtrl_LoopSSRFile_LineListLoopContinue

.code

align 16

_TSSGCtrl_LoopSSRFile_Format:

	RT_FORMAT equ 20H
	_this     equ <ebp + 8H>
	LoopVal   equ <ebp + 14H>
	VIt       equ <ebp - 180H>
	tmpS      equ <ebp - 78H>
	SSGS      equ <ebp - 60H>
	_Type     equ <edi>

	cmp     _Type, RT_FORMAT
	jne     _TSSGCtrl_LoopSSRFile_LineListLoopContinue
	sub     esp, 256 + 8

	buffer      equ <esp>
	outReserved equ <esp + 256>

	mov     eax, dword ptr [LoopVal]
	mov     edx, dword ptr [VIt]
	push    eax
	add     edx, 16
	lea     ecx, [SSGS]
	mov     eax, dword ptr [_this]
	push    edx
	push    ecx
	push    eax
	call    _Caller_ParsingWithVal
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
	call    __snprintf
	add     esp, 16
	cmp     eax, 256
	mov     edx, buffer
	jb      L2
	test    eax, eax
	js      L1
	mov     eax, 256 - 1
	jmp     L2
L1:
	xor     eax, eax
L2:
	add     eax, buffer
	lea     ecx, [tmpS]
	push    eax
	call    @bcb6_std_string_append_range@12
	add     esp, 256 + 8

_TSSGCtrl_LoopSSRFile_LineListLoopContinue:
	mov     ecx, 0050267CH
	mov     eax, dword ptr [VIt]
	jmp     ecx

end
