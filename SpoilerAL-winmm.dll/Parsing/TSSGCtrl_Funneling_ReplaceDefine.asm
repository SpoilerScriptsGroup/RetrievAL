.486
.model flat

extrn _EnableParserFix:dword
extrn _ReplaceDefine@8:proc
extrn @bcb6_std_string_dtor@4:proc

public _TSSGCtrl_Funneling_ReplaceDefine

.data

data1 label byte
	dword   00416274H
	dword   00000004H, -24
	dword   00000000H

data2 label byte
	dword   00416274H
	dword   00000004H, -24
	dword   00000000H

data3 label byte
	dword   00402634H
	dword   00000004H, -32
	dword   00000000H

data4 label byte
	dword   00402634H
	dword   00000004H, -32
	dword   00416274H
	dword   00000004H, -112
	dword   00000000H

data5 label byte
	dword   00000000H, -68
	dword   00050000H, 00000000H
	dword   data2
	dword   00050000H, 00000000H
	dword   data1
	dword   00050014H, 00000004H
	dword   data3
	dword   00050020H, 00000004H
	dword   data4
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H

.code

align 16

_TSSGCtrl_Funneling_ReplaceDefine proc near

	ReturnAddress equ 005104ACH
	_this         equ edi
	List          equ esi
	i             equ <ebp - 14CH>
	tmpV          equ <ebp - 34H>

	mov     eax, dword ptr [_EnableParserFix]
	lea     ecx, [tmpV]
	test    eax, eax
	jz      L2
	push    ecx
	push    ReturnAddress
	push    ebp
	mov     eax, dword ptr [i]
	mov     ebp, esp
	sub     esp, 68
	mov     ecx, 005D54CCH
	push    ebx
	mov     ebx, eax
	mov     eax, offset data5
	call    ecx
	mov     eax, dword ptr [esi + 4H]
	mov     ecx, dword ptr [esi]
	sub     eax, ecx
	mov     ecx, 0AAAAAAABH
	mul     ecx
	shr     edx, 4
	cmp     ebx, edx
	jb      L1
	mov     eax, 005F4804H
	push    00631D34H
	call    eax
	pop     ecx
L1:
	shl     ebx, 3
	mov     ecx, dword ptr [esi]
	mov     edx, 004166F0H
	lea     eax, [ebx + ebx * 2]
	add     eax, ecx
	lea     ecx, [ebp - 18H]
	push    eax
	push    ecx
	call    edx
	add     esp, 8
	lea     ecx, [ebp - 18H]
	push    ecx
	lea     eax, [edi + 20H]
	push    eax
	call    _ReplaceDefine@8
	mov     ecx, dword ptr [ebp + 8H]
	lea     edx, [ebp - 20H]
	push    0
	push    ecx
	sub     esp, 24
	mov     eax, 004165E8H
	mov     ecx, esp
	push    edx
	push    00631D32H
	push    ecx
	call    eax
	add     esp, 12
	lea     edx, [ebp - 18H]
	mov     eax, 004ACECCH
	lea     ecx, [edi + 3E0H]
	push    edx
	push    ecx
	call    eax
	add     esp, 40
	lea     ecx, [ebp - 18H]
	call    @bcb6_std_string_dtor@4
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     4
L2:
	mov     eax, 0051042EH
	push    0
	lea     ecx, [ebp - 34H]
	jmp     eax

_TSSGCtrl_Funneling_ReplaceDefine endp

end
