.486
.model flat, c

extrn A_memcpy:proc
extrn A_memmove:proc
extrn TSSGCtrl_TrimString:proc

public TSSGCtrl_EnumReadSSR_SwitchTmpS_0

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

TSSGCtrl_EnumReadSSR_SwitchTmpS_0 proc near

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
	call    TSSGCtrl_TrimString
	add     esp, 44
	mov     byte ptr [ebp - 0C5H], 32
	mov     byte ptr [ebp - 78H], 32
	lea     edx, [ebp - 58H]
	lea     ecx, [ebp - 70H]
	mov     eax, 004166F0H
	mov     dword ptr [ebp - 0C4H], edx
	push    edx
	push    ecx
	call    eax
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
	mov     edx, dword ptr [ebp - 60H]
	mov     ecx, dword ptr [ebp - 70H]
	sub     edx, ecx
	mov     dword ptr [ebp - 0DCH], eax
	mov     dword ptr [ebp - 0E0H], edx
	test    ecx, ecx
	jz      L3
	cmp     edx, 128
	jbe     L2
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     edx
	jmp     L3
L2:
	mov     eax, 005F47A0H
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L3:
	mov     edx, dword ptr [ebp - 48H]
	mov     ecx, dword ptr [ebp - 58H]
	sub     edx, ecx
	mov     dword ptr [ebp - 0E4H], edx
	test    ecx, ecx
	jz      L5
	cmp     edx, 128
	jbe     L4
	push    ecx
	mov     eax, 005D4484H
	call    eax
	pop     ecx
	jmp     L5
L4:
	mov     eax, 005F47A0H
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L5:
	mov     edx, dword ptr [ebp - 30H]
	mov     ecx, dword ptr [ebp - 40H]
	sub     edx, ecx
	mov     dword ptr [ebp - 0E8H], edx
	test    ecx, ecx
	jz      L7
	cmp     edx, 128
	jbe     L6
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     ecx
	jmp     L7
L6:
	mov     eax, 005F47A0H
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L7:
	mov     edx, dword ptr [ebp - 18H]
	mov     ecx, dword ptr [ebp - 28H]
	sub     edx, ecx
	mov     dword ptr [ebp - 0ECH], edx
	test    ecx, ecx
	jz      L9
	cmp     edx, 128
	jbe     L8
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     ecx
	jmp     L9
L8:
	mov     eax, 005F47A0H
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L9:
	mov     eax, dword ptr [esi]
	jmp     L11
L10:
	inc     eax
L11:
	mov     dl, byte ptr [eax]
	cmp     dl, 32
	je      L10
	cmp     dl, 9
	jb      L12
	cmp     dl, 13
	jbe     L10
L12:
	mov     edx, dword ptr [esi + 4H]
	dec     edx
	jmp     L14
L13:
	dec     edx
L14:
	cmp     eax, edx
	jae     L15
	mov     cl, byte ptr [edx]
	cmp     cl, 32
	je      L13
	cmp     cl, 9
	jb      L15
	cmp     cl, 13
	jbe     L13
L15:
	inc     edx
	mov     ecx, dword ptr [esi]
	cmp     edx, dword ptr [esi + 4H]
	jne     L16
	cmp     eax, ecx
	je      L19
L16:
	sub     edx, eax
	mov     dword ptr [ebp - 0F0H], edx
	cmp     eax, ecx
	je      L17
	push    edx
	push    eax
	push    ecx
	call    A_memcpy
	add     esp, 12
L17:
	mov     eax, dword ptr [esi + 4H]
	mov     ecx, dword ptr [esi]
	sub     eax, ecx
	mov     dword ptr [ebp - 0F4H], edx
	cmp     eax, edx
	jb      L18
	mov     eax, dword ptr [esi + 4H]
	add     ecx, edx
	mov     dword ptr [ebp - 0F8H], eax
	mov     dword ptr [ebp - 0FCH], ecx
	cmp     ecx, eax
	je      L19
	mov     dword ptr [ebp - 100H], 1
	push    1
	push    eax
	push    ecx
	call    A_memmove
	mov     eax, dword ptr [esi + 4H]
	mov     ecx, dword ptr [ebp - 0F8H]
	sub     eax, ecx
	mov     ecx, dword ptr [ebp - 0FCH]
	add     eax, ecx
	add     esp, 12
	mov     dword ptr [ebp - 104H], eax
	mov     dword ptr [esi + 4H], eax
	jmp     L19
L18:
	mov     eax, dword ptr [esi + 4H]
	push    0
	sub     edx, eax
	mov     eax, 00462FFCH
	add     edx, ecx
	push    edx
	push    esi
	call    eax
	add     esp, 12
L19:
	mov     edx, 00641AE4H
	lea     ecx, [ebp - 90H]
	mov     dl, byte ptr [edx]
	mov     eax, 004166F0H
	mov     byte ptr [ebp - 98H], dl
	push    esi
	push    ecx
	call    eax
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
	mov     edx, dword ptr [ebp - 80H]
	mov     ecx, dword ptr [ebp - 90H]
	sub     edx, ecx
	mov     dword ptr [ebp - 11CH], eax
	test    ecx, ecx
	jz      L21
	cmp     edx, 128
	jbe     L20
	mov     eax, 005D4484H
	push    ecx
	call    eax
	pop     ecx
	jmp     L21
L20:
	mov     eax, 005F47A0H
	push    edx
	push    ecx
	call    eax
	add     esp, 8
L21:
	pop     esi
	pop     ebx
	mov     esp, ebp
	mov     eax, 00500DFFH
	pop     ebp
	jmp     eax

TSSGCtrl_EnumReadSSR_SwitchTmpS_0 endp

end
