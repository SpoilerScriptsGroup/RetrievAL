.486
.model flat
assume fs:nothing

extrn @bcb6_std_allocator_allocate@4:proc
extrn @bcb6_std_allocator_deallocate@8:proc
extrn _bcb6_std_string_ctor_assign:dword
extrn @bcb6_std_string_dtor@4:proc
extrn _F005D54CC:dword
extrn _F00439D1C:dword
extrn _F0048E0D8:dword
extrn _F00490434:dword
extrn _F0048FA48:dword
extrn _F0050E758:dword

public _FixLoopByteArray@32

.const

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

_FixLoopByteArray@32 proc near

	push    ebp
	mov     eax, offset data9
	mov     ebp, esp
	sub     esp, 496
	push    ebx
	push    esi
	call    dword ptr [_F005D54CC]
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
	jmp     L8
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
	push    0
	mov     edx, esp
	push    eax
	mov     eax, dword ptr [ebp + 10H]
	push    ecx
	push    eax
	push    edx
	call    dword ptr [_F00439D1C]
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
	shl     ecx, 2
	mov     dword ptr [ebp - 0C4H], ecx
	call    @bcb6_std_allocator_allocate@4
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
	lea     ecx, [ebp - 0F8H]
	mov     dword ptr [ebp - 0E4H], eax
	push    ecx
	call    dword ptr [_F0048E0D8]
	mov     dword ptr [esp], 0
	lea     ecx, [ebp - 100H]
	push    ecx
	mov     eax, dword ptr [ebp - 0C8H]
	mov     ecx, dword ptr [ebp - 0CCH]
	mov     edx, dword ptr [ebp - 0D0H]
	push    eax
	push    ecx
	push    edx
	call    dword ptr [_F00490434]
	mov     dword ptr [ebp - 14H], eax
	mov     eax, dword ptr [ebp - 30H]
	mov     edx, dword ptr [ebp - 20H]
	mov     dword ptr [ebp - 108H], eax
	sub     edx, eax
	mov     ecx, dword ptr [ebp - 2CH]
	mov     eax, edx
	mov     dword ptr [ebp - 104H], ecx
	sar     eax, 31
	add     esp, 20
	and     eax, 3
	mov     ecx, dword ptr [ebp - 30H]
	add     edx, eax
	mov     dword ptr [ebp - 110H], ecx
	sar     edx, 2
	mov     dword ptr [ebp - 10CH], edx
	shl     edx, 2
	mov     dword ptr [ebp - 114H], edx
	call    @bcb6_std_allocator_deallocate@8
	push    1
	lea     eax, [ebp - 18H]
	push    eax
	mov     edx, dword ptr [ebp + 20H]
	mov     ecx, dword ptr [ebp + 14H]
	push    edx
	push    ecx
	call    dword ptr [_F0048FA48]
	mov     edx, dword ptr [ebp - 8H]
	mov     eax, dword ptr [ebp - 18H]
	sub     edx, eax
	mov     dword ptr [ebp - 11CH], eax
	mov     eax, edx
	add     esp, 16
	sar     eax, 31
	mov     ecx, dword ptr [ebp - 14H]
	and     eax, 3
	mov     dword ptr [ebp - 118H], ecx
	add     edx, eax
	mov     ecx, dword ptr [ebp - 18H]
	sar     edx, 2
	mov     dword ptr [ebp - 124H], ecx
	mov     dword ptr [ebp - 120H], edx
	shl     edx, 2
	mov     dword ptr [ebp - 128H], edx
	call    @bcb6_std_allocator_deallocate@8
	jmp     L16
L8:
	cmp     dword ptr [ebp + 20H], 0
	je      L16
	mov     ecx, dword ptr [ebp + 1CH]
	mov     eax, dword ptr [ebp + 18H]
	add     ecx, 2
	mov     edx, dword ptr [ebp + 10H]
	sub     eax, ecx
	lea     esi, [ebp - 50H]
	push    eax
	push    ecx
	push    edx
	push    esi
	call    dword ptr [_F00439D1C]
	add     esp, 16
	mov     dword ptr [ebp - 12CH], 0
L9:
	mov     ecx, dword ptr [ebp - 12CH]
	lea     edx, [ebp - 50H]
	push    ecx
	lea     ecx, [esp - 24]
	sub     esp, 24
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	mov     eax, dword ptr [ebp + 0CH]
	mov     ecx, dword ptr [ebp + 8H]
	lea     edx, [ebp - 80H]
	mov     dword ptr [esp +  4], eax
	mov     dword ptr [esp     ], ecx
	push    edx
	push    offset L10
	push    ebp
	mov     eax, 00506F81H
	mov     ebp, esp
	sub     esp, 4092
	jmp     eax
	align   16
L10:
	lea     ebx, [ebp - 80H]
	add     esp, 40
	mov     ecx, dword ptr [ebx]
	mov     eax, dword ptr [ebx + 4H]
	sub     eax, ecx
	lea     edx, [ebp - 88H]
	mov     ecx, eax
	mov     dword ptr [ebp - 130H], edx
	sar     eax, 31
	mov     dword ptr [ebp - 68H], 0
	and     eax, 3
	mov     dword ptr [ebp - 64H], 0
	add     ecx, eax
	mov     dword ptr [ebp - 58H], 0
	sar     ecx, 2
	mov     esi, ecx
	shl     ecx, 2
	mov     dword ptr [ebp - 134H], ecx
	call    @bcb6_std_allocator_allocate@4
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
	lea     ecx, [ebp - 168H]
	mov     dword ptr [ebp - 154H], eax
	push    ecx
	call    dword ptr [_F0048E0D8]
	mov     dword ptr [esp], 0
	lea     ecx, [ebp - 170H]
	push    ecx
	mov     eax, dword ptr [ebp - 138H]
	mov     ecx, dword ptr [ebp - 13CH]
	mov     edx, dword ptr [ebp - 140H]
	push    eax
	push    ecx
	push    edx
	call    dword ptr [_F00490434]
	mov     dword ptr [ebp - 64H], eax
	mov     eax, dword ptr [ebp - 80H]
	mov     edx, dword ptr [ebp - 70H]
	mov     dword ptr [ebp - 178H], eax
	sub     edx, eax
	add     esp, 20
	mov     eax, edx
	mov     ecx, dword ptr [ebp - 7CH]
	sar     eax, 31
	mov     dword ptr [ebp - 174H], ecx
	and     eax, 3
	mov     ecx, dword ptr [ebp - 80H]
	add     edx, eax
	mov     dword ptr [ebp - 180H], ecx
	sar     edx, 2
	mov     dword ptr [ebp - 17CH], edx
	shl     edx, 2
	mov     dword ptr [ebp - 184H], edx
	call    @bcb6_std_allocator_deallocate@8
	mov     ebx, dword ptr [ebp - 68H]
	jmp     L15
L11:
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
	jae     L12
	mov     dword ptr [ebp - 12CH], eax
	mov     eax, dword ptr [ebp + 24H]
	mov     eax, dword ptr [eax + 8H]
	mov     dword ptr [ebp - 188H], ebx
	mov     dword ptr [ebp - 18CH], eax
	mov     ecx, dword ptr [ebp + 24H]
	push    ebx
	push    ecx
	call    dword ptr [_F0050E758]
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
	jmp     L14
L12:
	je      L13
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
L13:
	mov     ecx, dword ptr [ebp + 24H]
	mov     dword ptr [ebp - 1A0H], ebx
	mov     esi, dword ptr [ecx + 8H]
	push    ebx
	mov     dword ptr [ebp - 1A4H], esi
	push    ecx
	call    dword ptr [_F0050E758]
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
	mov     edx, ecx
	mov     eax, dword ptr [ebp - 1ACH]
	sar     ecx, 31
	mov     dword ptr [ebp - 1B0H], eax
	and     ecx, 3
	add     edx, ecx
	mov     ecx, dword ptr [ebp - 64H]
	sar     edx, 2
	mov     dword ptr [ebp - 1BCH], edx
	mov     dword ptr [ebp - 1B4H], ecx
	shl     edx, 2
	mov     dword ptr [ebp - 1C0H], edx
	mov     ecx, ebx
	call    @bcb6_std_allocator_deallocate@8
	lea     ecx, [ebp - 50H]
	call    @bcb6_std_string_dtor@4
	jmp     L16
L14:
	add     ebx, 4
L15:
	cmp     ebx, dword ptr [ebp - 64H]
	jne     L11
	mov     ecx, dword ptr [ebp - 68H]
	mov     eax, dword ptr [ebp - 58H]
	sub     eax, ecx
	mov     dword ptr [ebp - 1CCH], ecx
	mov     edx, eax
	mov     dword ptr [ebp - 1C8H], ebx
	sar     eax, 31
	mov     dword ptr [ebp - 1D4H], ecx
	and     eax, 3
	add     edx, eax
	sar     edx, 2
	mov     dword ptr [ebp - 1D0H], edx
	shl     edx, 2
	mov     dword ptr [ebp - 1D8H], edx
	call    @bcb6_std_allocator_deallocate@8
	jmp     L9
L16:
	mov     ecx, dword ptr [ebp - 172]
	mov     dword ptr fs:[0], ecx
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     32

_FixLoopByteArray@32 endp

end
