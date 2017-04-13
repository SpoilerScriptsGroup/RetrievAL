.486
.model flat

extrn _bcb6_std_string_ctor_assign:dword
extrn _bcb6_std_string_append:dword
extrn @bcb6_std_string_dtor@4:dword
extrn _A_memcpy:proc
extrn _A_memmove:proc
extrn _TSSGCtrl_TrimString:proc

public _TSSGCtrl_EnumReadSSR_SwitchTmpS_0

.data

data1 label byte
	dd 00402634H
	dd 00000004H, -8
	dd 00402634H
	dd 00000004H, -16
	dd 00416274H
	dd 00000004H, -40
	dd 00416274H
	dd 00000004H, -64
	dd 00416274H
	dd 00000004H, -88
	dd 00501888H
	dd 00000004H, -120
	dd 00000000H

data2 label byte
	dd 00402634H
	dd 00000004H, -8
	dd 00416274H
	dd 00000004H, -328
	dd 00402634H
	dd 00000004H, -16
	dd 00416274H
	dd 00000004H, -40
	dd 00416274H
	dd 00000004H, -64
	dd 00000000H

data3 label byte
	dd 00402634H
	dd 00000004H, -16
	dd 00416274H
	dd 00000004H, -368
	dd 00000000H

data4 label byte
	dd 00501888H
	dd 00000004H, -152
	dd 00000000H

data5 label byte
	dd 00000000H, -188
	dd 00050000H, 00000000H
	dd data1
	dd 00050008H, 00000000H
	dd data2
	dd 00050014H, 00000005H
	dd data3
	dd 00050008H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H, 00000000H
	dd 00050000H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H, 00000000H
	dd 00050000H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H, 00000000H
	dd 00050000H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H, 00000000H
	dd 00050000H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H
	dd data4
	dd 00050098H, 00000000H
	dd 00000000H, 00050000H
	dd 00000000H, 00000000H
	dd 00050000H, 00000000H
	dd 00000000H

.code

align 16

_TSSGCtrl_EnumReadSSR_SwitchTmpS_0 proc near

	sub     al, '@' - '#'
	je      L1
	cmp     al, 'F' - '@'
	je      FormatPrefix
	mov     ecx, 004FFDC5H
	jmp     ecx
L1:
	mov     ecx, 005009A5H
	jmp     ecx

FormatPrefix:

	_this                  equ <ebp + 5A8H + 8H>
	offsetof_TSSGCtrl_strD equ 992
	LineList               equ <ebp + 5A8H - 40H>
	tmpS                   equ <ebp + 5A8H - 18H>

	push    ebp
	mov     ebp, esp
	sub     esp, 284
	mov     ecx, 005D54CCH
	push    ebx
	push    esi
	mov     ebx, dword ptr [_this]
	lea     esi, [tmpS]
	add     ebx, offsetof_TSSGCtrl_strD
	mov     eax, offset data5
	call    ecx
	push    0
	push    0
	sub     esp, 24
	lea     edx, [ebp - 8H]
	mov     ecx, esp
	mov     eax, 004165E8H
	push    edx
	push    00631BFDH
	push    ecx
	call    eax
	mov     dword ptr [esp + 8], -1
	mov     dword ptr [esp + 4], 1
	mov     dword ptr [esp    ], 0
	push    0
	sub     esp, 24
	lea     edx, [ebp - 10H]
	mov     ecx, esp
	mov     eax, 004165E8H
	push    edx
	push    00631BFBH
	push    ecx
	call    eax
	lea     ecx, [ebp - 28H]
	mov     eax, 004AC3ECH
	mov     dword ptr [esp + 8], esi
	mov     dword ptr [esp + 4], ebx
	mov     dword ptr [esp    ], ecx
	call    eax
	add     esp, 44
	lea     edx, [ebp - 28H]
	lea     ecx, [ebp - 40H]
	mov     eax, 00439D1CH
	push    edx
	push    ecx
	call    eax
	add     esp, 16
	lea     ecx, [ebp - 40H]
	lea     eax, [ebp - 58H]
	push    ecx
	push    ebx
	push    eax
	call    _TSSGCtrl_TrimString
	add     esp, 44
	mov     byte ptr [ebp - 0C5H], 32
	lea     edx, [ebp - 58H]
	mov     byte ptr [ebp - 78H], 32
	lea     ecx, [ebp - 70H]
	mov     dword ptr [ebp - 0C4H], edx
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	lea     eax, [LineList]
	lea     edx, [ebp - 78H]
	mov     eax, dword ptr [eax + 8H]
	mov     dword ptr [ebp - 0C0H], edx
	mov     dword ptr [ebp - 0CCH], eax
	lea     ecx, [LineList]
	mov     eax, 0050123CH
	mov     dword ptr [esp + 4], edx
	mov     dword ptr [esp    ], ecx
	call    eax
	mov     edx, dword ptr [ebp - 0CCH]
	add     esp, 8
	mov     dword ptr [ebp - 0D0H], edx
	mov     ecx, dword ptr [edx + 4H]
	mov     dword ptr [ebp - 0D4H], ecx
	mov     dword ptr [eax], edx
	mov     dword ptr [eax + 4H], ecx
	mov     dword ptr [ecx], eax
	mov     dword ptr [edx + 4H], eax
	mov     dword ptr [ebp - 0D8H], eax
	lea     ecx, [ebp - 70H]
	call    @bcb6_std_string_dtor@4
	lea     ecx, [ebp - 58H]
	call    @bcb6_std_string_dtor@4
	lea     ecx, [ebp - 40H]
	call    @bcb6_std_string_dtor@4
	lea     ecx, [ebp - 28H]
	call    @bcb6_std_string_dtor@4
	mov     eax, dword ptr [esi]
	jmp     L3
L2:
	inc     eax
L3:
	mov     dl, byte ptr [eax]
	cmp     dl, 32
	je      L2
	cmp     dl, 9
	jb      L4
	cmp     dl, 13
	jbe     L2
L4:
	mov     edx, dword ptr [esi + 4H]
	dec     edx
	jmp     L6
L5:
	dec     edx
L6:
	cmp     eax, edx
	jae     L7
	mov     cl, byte ptr [edx]
	cmp     cl, 32
	je      L5
	cmp     cl, 9
	jb      L7
	cmp     cl, 13
	jbe     L5
L7:
	inc     edx
	mov     ecx, dword ptr [esi]
	cmp     edx, dword ptr [esi + 4H]
	jne     L8
	cmp     eax, ecx
	je      L11
L8:
	sub     edx, eax
	mov     dword ptr [ebp - 0F0H], edx
	cmp     eax, ecx
	je      L9
	push    edx
	push    eax
	push    ecx
	call    _A_memcpy
	add     esp, 12
L9:
	mov     eax, dword ptr [esi + 4H]
	mov     ecx, dword ptr [esi]
	sub     eax, ecx
	mov     dword ptr [ebp - 0F4H], edx
	cmp     eax, edx
	jb      L10
	mov     eax, dword ptr [esi + 4H]
	add     ecx, edx
	mov     dword ptr [ebp - 0F8H], eax
	mov     dword ptr [ebp - 0FCH], ecx
	cmp     ecx, eax
	je      L11
	mov     dword ptr [ebp - 100H], 1
	push    1
	push    eax
	push    ecx
	call    _A_memmove
	mov     eax, dword ptr [esi + 4H]
	mov     ecx, dword ptr [ebp - 0F8H]
	sub     eax, ecx
	mov     ecx, dword ptr [ebp - 0FCH]
	add     eax, ecx
	add     esp, 12
	mov     dword ptr [ebp - 104H], eax
	mov     dword ptr [esi + 4H], eax
	jmp     L11
L10:
	mov     eax, dword ptr [esi + 4H]
	add     edx, ecx
	sub     edx, eax
	push    0
	push    edx
	push    esi
	call    dword ptr [_bcb6_std_string_append]
	add     esp, 12
L11:
	mov     edx, 00641AE4H
	lea     ecx, [ebp - 90H]
	mov     dl, byte ptr [edx]
	push    esi
	mov     byte ptr [ebp - 98H], dl
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	add     esp, 8
	lea     eax, [LineList]
	lea     edx, [ebp - 98H]
	mov     eax, dword ptr [eax + 8H]
	lea     ecx, [LineList]
	mov     dword ptr [ebp - 108H], edx
	mov     dword ptr [ebp - 10CH], eax
	mov     dword ptr [ebp - 110H], eax
	push    edx
	mov     eax, 0050123CH
	push    ecx
	call    eax
	mov     edx, dword ptr [ebp - 110H]
	add     esp, 8
	mov     ecx, dword ptr [edx + 4H]
	mov     dword ptr [ebp - 114H], ecx
	mov     dword ptr [eax], edx
	mov     dword ptr [eax + 4H], ecx
	mov     dword ptr [ecx], eax
	mov     dword ptr [edx + 4H], eax
	mov     dword ptr [ebp - 118H], eax
	lea     ecx, [ebp - 90H]
	call    @bcb6_std_string_dtor@4
	pop     esi
	pop     ebx
	mov     esp, ebp
	mov     eax, 00500DFFH
	pop     ebp
	jmp     eax

_TSSGCtrl_EnumReadSSR_SwitchTmpS_0 endp

end
