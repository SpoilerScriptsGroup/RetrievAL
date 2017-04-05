.486
.model flat, c

extrn bcb6_std_string_ctor_assign:dword

public FixLoopByteArray@32

.data

data1 label byte
	dword   004667A8H
	dword   00000004H, -24
	dword   00000000H

data2 label byte
	dword   004667A8H
	dword   00000004H, -48
	dword   0047066CH
	dword   00000004H, -56
	dword   004667A8H
	dword   00000004H, -24
	dword   00000000H

data3 label byte
	dword   00416274H
	dword   00000004H, -536
	dword   00000000H

data4 label byte
	dword   00416274H
	dword   00000004H, -80
	dword   00000000H

data5 label byte
	dword   00416274H
	dword   00000004H, -80
	dword   00000000H

data6 label byte
	dword   004667A8H
	dword   00000004H, -104
	dword   00000000H

data7 label byte
	dword   004667A8H
	dword   00000004H, -128
	dword   0047066CH
	dword   00000004H, -136
	dword   004667A8H
	dword   00000004H, -104
	dword   00000000H

data8 label byte
	dword   00416274H
	dword   00000004H, -536
	dword   00000000H

data9 label byte
	dword   00000000H, -172
	dword   00050000H, 00000000H
	dword   data1
	dword   00050008H, 00000000H
	dword   data2
	dword   00050014H, 00000003H
	dword   data3
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H
	dword   00050008H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H, 00000000H
	dword   00050000H, 00000000H
	dword   data4
	dword   00050074H, 00000000H
	dword   data5
	dword   00050074H, 00000004H
	dword   data6
	dword   0005008CH, 00000004H
	dword   data7
	dword   00050098H, 00000003H
	dword   data8
	dword   00050098H, 00000000H
	dword   00000000H, 00050098H
	dword   00000000H, 00000000H
	dword   00050098H, 00000000H
	dword   00000000H, 00050098H
	dword   00000000H, 00000000H
	dword   0005008CH, 00000000H
	dword   00000000H, 0005008CH
	dword   00000000H, 00000000H
	dword   0005008CH, 00000000H
	dword   00000000H, 0005008CH
	dword   00000000H, 00000000H
	dword   0005008CH, 00000000H
	dword   00000000H, 0005008CH
	dword   00000000H, 00000000H

.code

BSWAP32 macro value
	local result
	result = ((value shr 24) and 0FFH) or ((value shr 8) and 0FF00H) or ((value shl 8) and 000FF0000H) or ((value shl 24) and 0FF000000H)
	exitm %result
endm

align 16

FixLoopByteArray@32 proc near

	push    ebp
	mov     ebp, esp
	sub     esp, 496
	mov     eax, offset data9
	push    ebx
	mov     ecx, 005D54CCH
	push    esi
	call    ecx
	mov     edx, dword ptr [ebp + 10H]
	mov     ecx, dword ptr [ebp + 1CH]
	mov     edx, dword ptr [edx]
	xor     eax, eax
	lea     ecx, [ecx + edx + 6]
	add     edx, dword ptr [ebp + 18H]
	cmp     ecx, edx
	jae     L6
	sub     ecx, 4
	sub     edx, 4
L1:
	mov     eax, dword ptr [ecx]
	inc     ecx
	cmp     eax, BSWAP32('$Rel')
	jne     L3
	mov     ah, byte ptr [ecx + 3]
	add     ecx, 3
	test    ah, ah
	js      L3
	cmp     ah, '0'
	jb      L2
	cmp     ah, '9'
	jbe     L5
	cmp     ah, 'A'
	jb      L2
	cmp     ah, 'Z'
	jbe     L5
	cmp     ah, '_'
	je      L5
	cmp     ah, 'a'
	jb      L2
	cmp     ah, 'z'
	jbe     L5
L2:
	jmp     L13
L3:
	cmp     al, 81H
	jb      L5
	cmp     al, 9FH
	jbe     L4
	cmp     al, 0E0H
	jb      L5
	cmp     al, 0FCH
	ja      L5
L4:
	inc     ecx
L5:
	cmp     ecx, edx
	jb      L1
L6:
	mov     ecx, dword ptr [ebp + 1CH]
	mov     eax, dword ptr [ebp + 18H]
	add     ecx, 2
	sub     esp, 24
	sub     eax, ecx
	mov     edx, dword ptr [ebp + 10H]
	push    0
	push    eax
	push    ecx
	push    edx
	lea     ecx, [esp + 0CH]
	mov     eax, 00439D1CH
	push    ecx
	call    eax
	mov     eax, dword ptr [ebp + 0CH]
	mov     ecx, dword ptr [ebp + 8H]
	lea     edx, [ebp - 30H]
	mov     dword ptr [esp + 12], eax
	mov     dword ptr [esp +  8], ecx
	mov     dword ptr [esp +  4], edx
	mov     dword ptr [esp     ], offset L7
	push    ebp
	mov     eax, 00506F81H
	mov     ebp, esp
	sub     esp, 4092
	jmp     eax
L7:
	lea     ecx, [ebp - 30H]
	lea     edx, [ebp - 38H]
	mov     dword ptr [ebp - 0B8H], ecx
	mov     dword ptr [ebp - 0BCH], edx
	mov     edx, dword ptr [ecx]
	mov     ecx, dword ptr [ecx + 4H]
	add     esp, 40
	sub     ecx, edx
	mov     edx, ecx
	xor     eax, eax
	sar     edx, 31
	mov     dword ptr [ebp - 18H], eax
	and     edx, 3
	mov     dword ptr [ebp - 14H], eax
	add     ecx, edx
	mov     dword ptr [ebp - 8H], eax
	sar     ecx, 2
	mov     dword ptr [ebp - 0C0H], ecx
	jz      L9
	shl     ecx, 2
	mov     eax, 005D44B8H
	mov     dword ptr [ebp - 0C4H], ecx
	push    ecx
	cmp     ecx, 128
	ja      L8
	mov     eax, 005F43F0H
L8:
	call    eax
	pop     ecx
L9:
	mov     ecx, dword ptr [ebp - 0C0H]
	mov     dword ptr [ebp - 18H], eax
	shl     ecx, 2
	mov     dword ptr [ebp - 14H], eax
	add     ecx, eax
	mov     dword ptr [ebp - 0C8H], eax
	mov     dword ptr [ebp - 8H], ecx
	mov     eax, dword ptr [ebp - 0B8H]
	mov     ecx, dword ptr [eax + 4H]
	mov     eax, dword ptr [eax]
	mov     dword ptr [ebp - 0CCH], ecx
	mov     dword ptr [ebp - 0D0H], eax
	mov     ecx, dword ptr [ebp - 0E0H]
	mov     eax, dword ptr [ebp - 0DCH]
	mov     dword ptr [ebp - 0D8H], ecx
	mov     dword ptr [ebp - 0D4H], eax
	mov     ecx, dword ptr [ebp - 0F0H]
	mov     eax, dword ptr [ebp - 0ECH]
	mov     dword ptr [ebp - 0E8H], ecx
	mov     dword ptr [ebp - 0E4H], eax
	lea     ecx, [ebp - 0F8H]
	mov     eax, 0048E0D8H
	push    ecx
	call    eax
	lea     ecx, [ebp - 100H]
	mov     dword ptr [esp], 0
	push    ecx
	mov     eax, dword ptr [ebp - 0C8H]
	mov     ecx, dword ptr [ebp - 0CCH]
	mov     edx, dword ptr [ebp - 0D0H]
	push    eax
	push    ecx
	mov     eax, 00490434H
	push    edx
	call    eax
	mov     dword ptr [ebp - 14H], eax
	mov     ecx, dword ptr [ebp - 30H]
	mov     eax, dword ptr [ebp - 20H]
	mov     dword ptr [ebp - 108H], ecx
	sub     eax, ecx
	mov     edx, dword ptr [ebp - 2CH]
	mov     ecx, eax
	mov     dword ptr [ebp - 104H], edx
	sar     ecx, 31
	add     esp, 20
	and     ecx, 3
	mov     edx, dword ptr [ebp - 30H]
	add     eax, ecx
	mov     dword ptr [ebp - 110H], edx
	sar     eax, 2
	mov     dword ptr [ebp - 10CH], eax
	jz      L11
	mov     ecx, eax
	shl     eax, 2
	mov     dword ptr [ebp - 114H], eax
	cmp     eax, 128
	jbe     L10
	mov     eax, 005D4484H
	push    edx
	call    eax
	pop     ecx
	jmp     L11
L10:
	push    eax
	mov     eax, 005F47A0H
	push    edx
	call    eax
	add     esp, 8
L11:
	lea     eax, [ebp - 18H]
	push    1
	push    eax
	mov     edx, dword ptr [ebp + 20H]
	mov     ecx, dword ptr [ebp + 14H]
	mov     eax, 0048FA48H
	push    edx
	push    ecx
	call    eax
	mov     ecx, dword ptr [ebp - 8H]
	mov     eax, dword ptr [ebp - 18H]
	sub     ecx, eax
	mov     dword ptr [ebp - 11CH], eax
	mov     eax, ecx
	add     esp, 16
	sar     eax, 31
	mov     edx, dword ptr [ebp - 14H]
	and     eax, 3
	mov     dword ptr [ebp - 118H], edx
	add     ecx, eax
	mov     eax, dword ptr [ebp - 18H]
	sar     ecx, 2
	mov     dword ptr [ebp - 124H], eax
	mov     dword ptr [ebp - 120H], ecx
	test    eax, eax
	jz      L29
	shl     ecx, 2
	mov     dword ptr [ebp - 128H], ecx
	cmp     ecx, 128
	jbe     L12
	push    eax
	mov     eax, 005D4484H
	call    eax
	pop     ecx
	jmp     L29
L12:
	push    ecx
	push    eax
	mov     eax, 005F47A0H
	call    eax
	add     esp, 8
	jmp     L29
L13:
	cmp     dword ptr [ebp + 20H], 0
	je      L29
	mov     ecx, dword ptr [ebp + 1CH]
	mov     eax, dword ptr [ebp + 18H]
	add     ecx, 2
	mov     edx, dword ptr [ebp + 10H]
	sub     eax, ecx
	lea     esi, [ebp - 50H]
	push    eax
	push    ecx
	push    edx
	mov     eax, 00439D1CH
	push    esi
	call    eax
	add     esp, 16
	mov     dword ptr [ebp - 12CH], 0
L14:
	mov     ecx, dword ptr [ebp - 12CH]
	lea     edx, [ebp - 50H]
	push    ecx
	lea     ecx, [esp - 24]
	sub     esp, 24
	push    edx
	push    ecx
	call    dword ptr [bcb6_std_string_ctor_assign]
	mov     eax, dword ptr [ebp + 0CH]
	mov     ecx, dword ptr [ebp + 8H]
	lea     edx, [ebp - 80H]
	mov     dword ptr [esp +  4], eax
	mov     dword ptr [esp     ], ecx
	push    edx
	push    offset L15
	push    ebp
	mov     eax, 00506F81H
	mov     ebp, esp
	sub     esp, 4092
	jmp     eax
	align   16
L15:
	lea     ebx, [ebp - 80H]
	add     esp, 40
	mov     eax, dword ptr [ebx]
	mov     ecx, dword ptr [ebx + 4H]
	sub     ecx, eax
	lea     edx, [ebp - 88H]
	mov     eax, ecx
	mov     dword ptr [ebp - 130H], edx
	sar     ecx, 31
	mov     dword ptr [ebp - 68H], 0
	and     ecx, 3
	mov     dword ptr [ebp - 64H], 0
	add     eax, ecx
	mov     dword ptr [ebp - 58H], 0
	sar     eax, 2
	mov     esi, eax
	jz      L17
	shl     eax, 2
	mov     ecx, 005D44B8H
	mov     dword ptr [ebp - 134H], eax
	push    eax
	cmp     eax, 128
	ja      L16
	mov     ecx, 005F43F0H
L16:
	call    ecx
	pop     ecx
L17:
	shl     esi, 2
	mov     dword ptr [ebp - 68H], eax
	add     esi, eax
	mov     dword ptr [ebp - 64H], eax
	mov     dword ptr [ebp - 58H], esi
	mov     dword ptr [ebp - 138H], eax
	mov     ecx, dword ptr [ebx + 4H]
	mov     eax, dword ptr [ebx]
	mov     dword ptr [ebp - 13CH], ecx
	mov     dword ptr [ebp - 140H], eax
	mov     ecx, dword ptr [ebp - 150H]
	mov     eax, dword ptr [ebp - 14CH]
	mov     dword ptr [ebp - 148H], ecx
	mov     dword ptr [ebp - 144H], eax
	mov     ecx, dword ptr [ebp - 160H]
	mov     eax, dword ptr [ebp - 15CH]
	mov     dword ptr [ebp - 158H], ecx
	mov     dword ptr [ebp - 154H], eax
	lea     ecx, [ebp - 168H]
	mov     eax, 0048E0D8H
	push    ecx
	call    eax
	lea     ecx, [ebp - 170H]
	mov     dword ptr [esp], 0
	push    ecx
	mov     eax, dword ptr [ebp - 138H]
	mov     ecx, dword ptr [ebp - 13CH]
	mov     edx, dword ptr [ebp - 140H]
	push    eax
	push    ecx
	mov     eax, 00490434H
	push    edx
	call    eax
	mov     dword ptr [ebp - 64H], eax
	mov     ecx, dword ptr [ebp - 80H]
	mov     eax, dword ptr [ebp - 70H]
	mov     dword ptr [ebp - 178H], ecx
	sub     eax, ecx
	add     esp, 20
	mov     ecx, eax
	mov     edx, dword ptr [ebp - 7CH]
	sar     ecx, 31
	mov     dword ptr [ebp - 174H], edx
	and     ecx, 3
	mov     edx, dword ptr [ebp - 80H]
	add     eax, ecx
	mov     dword ptr [ebp - 180H], edx
	sar     eax, 2
	mov     dword ptr [ebp - 17CH], eax
	test    edx, edx
	jz      L19
	shl     eax, 2
	mov     dword ptr [ebp - 184H], eax
	cmp     eax, 128
	jbe     L18
	mov     eax, 005D4484H
	push    edx
	call    eax
	pop     ecx
	jmp     L19
L18:
	push    eax
	mov     eax, 005F47A0H
	push    edx
	call    eax
	add     esp, 8
L19:
	mov     ebx, dword ptr [ebp - 68H]
	jmp     L27
L20:
	mov     eax, dword ptr [ebx]
	push    1
	mov     ecx, dword ptr [eax]
	push    eax
	call    dword ptr [ecx + 4H]
	mov     edx, dword ptr [ebp - 12CH]
	mov     ecx, dword ptr [ebp + 20H]
	add     eax, edx
	add     esp, 8
	cmp     eax, ecx
	jae     L21
	mov     dword ptr [ebp - 12CH], eax
	mov     eax, dword ptr [ebp + 24H]
	mov     eax, dword ptr [eax + 8H]
	mov     dword ptr [ebp - 188H], ebx
	mov     dword ptr [ebp - 18CH], eax
	mov     ecx, dword ptr [ebp + 24H]
	mov     eax, 0050E758H
	push    ebx
	push    ecx
	call    eax
	mov     ecx, dword ptr [ebp - 18CH]
	add     esp, 8
	mov     dword ptr [ebp - 190H], ecx
	mov     edx, dword ptr [ecx + 4H]
	mov     dword ptr [ebp - 194H], edx
	mov     dword ptr [eax], ecx
	mov     dword ptr [eax + 4H], edx
	mov     dword ptr [edx], eax
	mov     dword ptr [ecx + 4H], eax
	mov     dword ptr [ebp - 198H], eax
	mov     dword ptr [ebp - 19CH], eax
	jmp     L26
L21:
	je      L22
	mov     ecx, dword ptr [ebp + 20H]
	mov     eax, dword ptr [ebp - 12CH]
	sub     ecx, eax
	mov     eax, dword ptr [ebx]
	push    1
	push    ecx
	mov     ecx, dword ptr [eax]
	push    eax
	call    dword ptr [ecx + 8H]
	add     esp, 12
L22:
	mov     ecx, dword ptr [ebp + 24H]
	mov     dword ptr [ebp - 1A0H], ebx
	mov     esi, dword ptr [ecx + 8H]
	push    ebx
	mov     dword ptr [ebp - 1A4H], esi
	mov     eax, 0050E758H
	push    ecx
	call    eax
	mov     edx, dword ptr [esi + 4H]
	add     esp, 8
	mov     dword ptr [esi + 4H], eax
	mov     dword ptr [eax], esi
	mov     dword ptr [eax + 4H], edx
	mov     dword ptr [edx], eax
	mov     dword ptr [ebp - 1ACH], eax
	mov     ebx, dword ptr [ebp - 68H]
	mov     ecx, dword ptr [ebp - 58H]
	mov     dword ptr [ebp - 1B8H], ebx
	sub     ecx, ebx
	mov     dword ptr [ebp - 1A8H], edx
	mov     eax, ecx
	mov     edx, dword ptr [ebp - 1ACH]
	sar     ecx, 31
	mov     dword ptr [ebp - 1B0H], edx
	and     ecx, 3
	add     eax, ecx
	mov     ecx, dword ptr [ebp - 64H]
	sar     eax, 2
	mov     dword ptr [ebp - 1BCH], eax
	mov     dword ptr [ebp - 1B4H], ecx
	test    ebx, ebx
	jz      L24
	shl     eax, 2
	mov     dword ptr [ebp - 1C0H], eax
	cmp     eax, 128
	jbe     L23
	mov     eax, 005D4484H
	push    ebx
	call    eax
	pop     ecx
	jmp     L24
L23:
	push    eax
	mov     eax, 005F47A0H
	push    ebx
	call    eax
	add     esp, 8
L24:
	mov     eax, dword ptr [ebp - 40H]
	mov     esi, dword ptr [ebp - 50H]
	sub     eax, esi
	test    esi, esi
	mov     dword ptr [ebp - 1C4H], eax
	jz      L29
	cmp     eax, 128
	jbe     L25
	mov     eax, 005D4484H
	push    esi
	call    eax
	pop     ecx
	jmp     L29
L25:
	push    eax
	mov     eax, 005F47A0H
	push    esi
	call    eax
	add     esp, 8
	jmp     L29
L26:
	add     ebx, 4
L27:
	cmp     ebx, dword ptr [ebp - 64H]
	jne     L20
	mov     edx, dword ptr [ebp - 68H]
	mov     eax, dword ptr [ebp - 58H]
	sub     eax, edx
	mov     dword ptr [ebp - 1CCH], edx
	mov     ecx, eax
	mov     dword ptr [ebp - 1C8H], ebx
	sar     eax, 31
	mov     dword ptr [ebp - 1D4H], edx
	and     eax, 3
	add     ecx, eax
	sar     ecx, 2
	mov     dword ptr [ebp - 1D0H], ecx
	test    edx, edx
	jz      L14
	shl     ecx, 2
	mov     dword ptr [ebp - 1D8H], ecx
	cmp     ecx, 128
	jbe     L28
	mov     eax, 005D4484H
	push    edx
	call    eax
	pop     ecx
	jmp     L14
L28:
	push    ecx
	mov     eax, 005F47A0H
	push    edx
	call    eax
	add     esp, 8
	jmp     L14
L29:
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     32

FixLoopByteArray@32 endp

end
