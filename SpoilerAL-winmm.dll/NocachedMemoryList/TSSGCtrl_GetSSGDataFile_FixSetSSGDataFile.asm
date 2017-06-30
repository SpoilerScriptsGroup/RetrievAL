.486
.model flat

extrn _bcb6_std_allocator_allocate:dword
extrn _bcb6_std_allocator_deallocate:dword
extrn @bcb6_std_node_alloc_allocate@4:proc
extrn _bcb6_std_string_ctor_assign:dword
extrn @bcb6_std_string_dtor@4:proc
extrn @bcb6_std_vector_string_dtor@4:proc
extrn _A_memcmp:proc
extrn _TSSGCtrl_SetSSGDataFile:dword
extrn __bcb6_std_string_ctor_assign_cstr:dword
extrn _F005D54CC:dword
extrn _F004AE4AC:dword
extrn _F004F21F8:dword
extrn _F004F32FC:dword
extrn _F004185E8:dword
extrn _F004185FC:dword
extrn _F004F3410:dword
extrn _F00469A5C:dword
extrn _F004F20E4:dword
extrn _F0044CCD4:dword
extrn _F004F2424:dword
extrn _F0044CA38:dword

public _TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile

.const

data1 label byte
	dword   00402634H
	dword   00000004H, -8
	dword   00416274H
	dword   00000004H, -32
	dword   00416274H
	dword   00000004H, -56
	dword   00000000H

data2 label byte
	dword   00416274H
	dword   00000004H, -648
	dword   00000000H

data3 label byte
	dword   0046ED10H
	dword   00000004H, -88
	dword   00469F34H
	dword   00000004H, -144
	dword   00000000H

data4 label byte
	dword   004F22C8H
	dword   00000004H, -152
	dword   00000000H

data5 label byte
	dword   0047111CH
	dword   00000004H, -160
	dword   00000000H

data6 label byte
	dword   004F22C8H
	dword   00000004H, -168
	dword   00000000H

data7 label byte
	dword   0047111CH
	dword   00000004H, -176
	dword   00000000H

data8 label byte
	dword   00416510H
	dword   00000004H, -184
	dword   00416414H
	dword   00000004H, -208
	dword   00469DDCH
	dword   00000004H, -256
	dword   00000000H

data9 label byte
	dword   00416510H
	dword   00000004H, -264
	dword   00000000H

data10 label byte
	dword   00000000H, -300
	dword   00050000H, 00000000H
	dword   data1
	dword   00050008H, 00000005H
	dword   data2
	dword   00050008H, 00000009H
	dword   data3
	dword   00050020H, 00000000H
	dword   data4
	dword   0005002CH, 00000000H
	dword   data5
	dword   00050020H, 00000000H
	dword   00000000H, 00050020H
	dword   00000000H, 00000000H
	dword   00050050H, 00000000H
	dword   data6
	dword   0005005CH, 00000000H
	dword   data7
	dword   0005005CH, 00000000H
	dword   00000000H, 00050020H
	dword   00000000H, 00000000H
	dword   00050020H, 00000000H
	dword   00000000H, 00050020H
	dword   00000000H, 00000000H
	dword   00050020H, 00000000H
	dword   00000000H, 00050020H
	dword   00000000H, 00000000H
	dword   00050020H, 00000000H
	dword   00000000H, 00050008H
	dword   00000009H
	dword   data8
	dword   000500C8H, 00000000H
	dword   00000000H, 000500C8H
	dword   00000000H, 00000000H
	dword   000500C8H, 00000004H
	dword   data9
	dword   000500ECH, 00000000H
	dword   00000000H, 000500ECH
	dword   00000000H, 00000000H
	dword   000500C8H, 00000000H
	dword   00000000H, 000500C8H
	dword   00000000H, 00000000H
	dword   000500C8H, 00000000H
	dword   00000000H, 000500C8H
	dword   00000000H, 00000000H
	dword   000500C8H, 00000000H
	dword   00000000H, 000500C8H
	dword   00000000H, 00000000H
	dword   00050000H, 00000000H
	dword   00000000H, 00050000H
	dword   00000000H, 00000000H
	dword   00050000H, 00000000H
	dword   00000000H, 00050000H
	dword   00000000H, 00000000H

.code

align 16

_TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile proc near

	IsNocache equ <esp + 10H>

	cmp     dword ptr [IsNocache], 0
	jne     L1
	jmp     dword ptr [_TSSGCtrl_SetSSGDataFile]
L1:
	_this     equ <ebp + 8H>
	Data      equ <ebp + 0CH>
	FileName  equ <ebp + 10H>
	IsNocache equ <ebp + 14H>

	push    ebp
	mov     eax, offset data10
	mov     ebp, esp
	sub     esp, 600
	push    ebx
	push    esi
	lea     ebx, [ebp - 90H]
	call    dword ptr [_F005D54CC]
	mov     ecx, dword ptr [Data]
	push    ecx
	lea     ecx, [ebp - 8H]
	push    ecx
	lea     ecx, [ebp - 20H]
	push    00631A92H
	push    ecx
	call    dword ptr [__bcb6_std_string_ctor_assign_cstr]
	add     esp, 12
	mov     ecx, dword ptr [FileName]
	push    -1
	push    0
	sub     esp, 24
	mov     eax, esp
	push    ecx
	push    eax
	call    dword ptr [_bcb6_std_string_ctor_assign]
	mov     ecx, dword ptr [_this]
	add     esp, 8
	add     ecx, 992
	push    ecx
	lea     ecx, [ebp - 38H]
	push    ecx
	call    dword ptr [_F004AE4AC]
	add     esp, 40
	mov     ecx, dword ptr [_this]
	lea     edx, [ebp - 38H]
	add     ecx, 392
	mov     dword ptr [ebp - 130H], edx
	mov     dword ptr [ebp - 134H], ecx
	push    edx
	push    ecx
	call    dword ptr [_F004F21F8]
	mov     ecx, dword ptr [ebp - 134H]
	add     esp, 8
	mov     ecx, dword ptr [ecx + 8H]
	mov     dword ptr [ebp - 138H], eax
	mov     dword ptr [ebp - 13CH], eax
	mov     dword ptr [ebp - 140H], ecx
	mov     dword ptr [ebp - 144H], ecx
	cmp     ecx, eax
	je      L5
	mov     ecx, dword ptr [eax + 14H]
	add     eax, 16
	mov     edx, dword ptr [eax]
	mov     dword ptr [ebp - 148H], eax
	mov     dword ptr [ebp - 154H], ecx
	mov     dword ptr [ebp - 158H], edx
	sub     ecx, edx
	mov     eax, dword ptr [ebp - 134H]
	mov     dword ptr [ebp - 168H], ecx
	mov     eax, dword ptr [ebp - 130H]
	mov     ecx, dword ptr [eax + 18H]
	mov     edx, dword ptr [eax + 1CH]
	mov     dword ptr [ebp - 150H], ecx
	mov     dword ptr [ebp - 14CH], edx
	mov     ecx, dword ptr [eax + 4H]
	mov     edx, dword ptr [eax]
	mov     dword ptr [ebp - 15CH], ecx
	mov     dword ptr [ebp - 160H], edx
	sub     ecx, edx
	mov     eax, dword ptr [ebp - 168H]
	mov     dword ptr [ebp - 164H], ecx
	cmp     eax, ecx
	jge     L2
	jmp     L3
L2:
	mov     eax, ecx
L3:
	mov     dword ptr [ebp - 170H], eax
	push    eax
	mov     ecx, dword ptr [ebp - 158H]
	mov     edx, dword ptr [ebp - 160H]
	push    ecx
	push    edx
	call    _A_memcmp
	add     esp, 12
	mov     dword ptr [ebp - 16CH], eax
	test    eax, eax
	jz      L4
	jl      L5
	jmp     L11
L4:
	mov     eax, dword ptr [ebp - 164H]
	mov     ecx, dword ptr [ebp - 168H]
	cmp     eax, ecx
	jg      L11
L5:
	mov     dword ptr [ebp - 50H], 0
	lea     eax, [ebp - 98H]
	mov     dword ptr [ebp - 17CH], eax
	push    64
	call    dword ptr [_bcb6_std_allocator_allocate]
	mov     dword ptr [ebp - 50H], eax
	pop     ecx
	mov     byte ptr [eax], 0
	mov     dword ptr [eax + 4H], 0
	mov     dword ptr [eax + 8H], eax
	mov     dword ptr [eax + 0CH], eax
	mov     ecx, dword ptr [ebp - 184H]
	mov     eax, dword ptr [ebp - 180H]
	mov     dword ptr [ebp - 48H], 0
	mov     dword ptr [ebp - 40H], ecx
	mov     dword ptr [ebp - 3CH], eax
	lea     edx, [ebp - 58H]
	mov     ecx, dword ptr [ebp - 130H]
	mov     dword ptr [ebp - 178H], edx
	push    ecx
	push    ebx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	add     esp, 8
	lea     eax, [ebp - 0A8H]
	mov     dword ptr [ebx + 20H], 0
	mov     dword ptr [ebp - 188H], eax
	push    64
	call    dword ptr [_bcb6_std_allocator_allocate]
	pop     ecx
	mov     dword ptr [ebx + 20H], eax
	mov     eax, dword ptr [ebp - 178H]
	mov     dword ptr [ebx + 28H], 0
	mov     ecx, dword ptr [eax + 18H]
	mov     edx, dword ptr [eax + 1CH]
	mov     eax, dword ptr [eax + 8H]
	mov     dword ptr [ebx + 30H], ecx
	mov     dword ptr [ebx + 34H], edx
	mov     eax, dword ptr [eax + 4H]
	test    eax, eax
	jnz     L6
	mov     eax, dword ptr [ebx + 20H]
	mov     byte ptr [eax], 0
	mov     dword ptr [eax + 4H], 0
	mov     dword ptr [eax + 8H], eax
	mov     dword ptr [eax + 0CH], eax
	jmp     L7
L6:
	mov     ecx, dword ptr [ebp - 178H]
	mov     edx, dword ptr [ebx + 20H]
	mov     ecx, dword ptr [ecx + 8H]
	mov     byte ptr [edx], 0
	mov     ecx, dword ptr [ecx + 4H]
	push    edx
	lea     edx, [ebx + 18H]
	push    ecx
	push    edx
	call    dword ptr [_F004F32FC]
	mov     ecx, dword ptr [ebx + 20H]
	add     esp, 12
	mov     dword ptr [ecx + 4H], ecx
	mov     dword ptr [ebp - 18CH], ecx
	push    ecx
	call    dword ptr [_F004185E8]
	mov     ecx, dword ptr [ebx + 20H]
	mov     dword ptr [ecx + 8H], ecx
	mov     ecx, dword ptr [ecx + 4H]
	mov     dword ptr [ebp - 190H], ecx
	mov     dword ptr [esp], ecx
	call    dword ptr [_F004185FC]
	pop     ecx
	mov     ecx, dword ptr [ebx + 20H]
	mov     dword ptr [ecx + 0CH], eax
L7:
	mov     ecx, dword ptr [ebp - 178H]
	mov     edx, dword ptr [ebp - 13CH]
	mov     ecx, dword ptr [ecx + 10H]
	mov     dword ptr [ebp - 194H], edx
	mov     dword ptr [ebx + 28H], ecx
	mov     dword ptr [ebp - 174H], ebx
	push    ebx
	push    ebx
	mov     ecx, dword ptr [ebp - 194H]
	mov     edx, dword ptr [ebp - 134H]
	mov     dword ptr [esp], ecx
	push    edx
	lea     ecx, [ebp - 198H]
	push    ecx
	call    dword ptr [_F004F3410]
	add     esp, 16
	lea     eax, [ebp - 198H]
	mov     ecx, dword ptr [eax]
	mov     eax, dword ptr [ebx + 28H]
	mov     dword ptr [ebp - 13CH], ecx
	test    eax, eax
	jz      L8
	mov     ecx, dword ptr [ebx + 20H]
	lea     edx, [ebx + 18H]
	mov     ecx, dword ptr [ecx + 4H]
	push    ecx
	push    edx
	call    dword ptr [_F00469A5C]
	mov     eax, dword ptr [ebx + 20H]
	add     esp, 8
	mov     dword ptr [eax + 4H], 0
	mov     dword ptr [eax + 8H], eax
	mov     dword ptr [eax + 0CH], eax
	mov     dword ptr [ebx + 28H], 0
L8:
	mov     eax, dword ptr [ebx + 20H]
	mov     dword ptr [ebp - 19CH], eax
	test    eax, eax
	jz      L9
	mov     eax, dword ptr [ebp - 19CH]
	push    64
	push    eax
	call    dword ptr [_bcb6_std_allocator_deallocate]
	add     esp, 8
L9:
	mov     ecx, ebx
	call    @bcb6_std_string_dtor@4
	cmp     dword ptr [ebp - 48H], 0
	jz      L10
	mov     edx, dword ptr [ebp - 50H]
	mov     edx, dword ptr [edx + 4H]
	lea     ecx, [ebp - 58H]
	push    edx
	push    ecx
	call    dword ptr [_F00469A5C]
	mov     eax, dword ptr [ebp - 50H]
	add     esp, 8
	mov     dword ptr [eax + 4H], 0
	mov     dword ptr [eax + 8H], eax
	mov     dword ptr [eax + 0CH], eax
	mov     dword ptr [ebp - 48H], 0
L10:
	mov     ecx, dword ptr [ebp - 50H]
	mov     dword ptr [ebp - 1A8H], ecx
	test    ecx, ecx
	jz      L11
	mov     eax, dword ptr [ebp - 1A8H]
	push    64
	push    eax
	call    dword ptr [_bcb6_std_allocator_deallocate]
	add     esp, 8
L11:
	mov     edx, dword ptr [ebp - 13CH]
	add     edx, 40
	lea     ecx, [ebp - 20H]
	mov     dword ptr [ebp - 1ACH], edx
	push    ecx
	push    edx
	call    dword ptr [_F004F20E4]
	add     esp, 8
	mov     ecx, dword ptr [ebp - 1ACH]
	mov     dword ptr [ebp - 1B0H], eax
	mov     ecx, dword ptr [ecx + 8H]
	mov     dword ptr [ebp - 1B4H], eax
	mov     dword ptr [ebp - 1B8H], ecx
	mov     dword ptr [ebp - 1BCH], ecx
	cmp     ecx, eax
	je      L13
	mov     eax, dword ptr [ebp - 1B4H]
	add     eax, 16
	mov     ecx, dword ptr [eax + 4H]
	mov     edx, dword ptr [eax]
	mov     dword ptr [ebp - 1C0H], eax
	mov     dword ptr [ebp - 1CCH], ecx
	mov     eax, dword ptr [ebp - 1ACH]
	mov     dword ptr [ebp - 1D0H], edx
	mov     ecx, dword ptr [eax + 18H]
	mov     eax, dword ptr [eax + 1CH]
	mov     dword ptr [ebp - 1C8H], ecx
	mov     dword ptr [ebp - 1C4H], eax
	mov     ecx, dword ptr [ebp - 1CH]
	mov     eax, dword ptr [ebp - 20H]
	mov     dword ptr [ebp - 1D4H], ecx
	mov     dword ptr [ebp - 1D8H], eax
	sub     ecx, eax
	mov     edx, dword ptr [ebp - 1D0H]
	mov     dword ptr [ebp - 1DCH], ecx
	mov     eax, dword ptr [ebp - 1CCH]
	sub     eax, edx
	mov     dword ptr [ebp - 1E0H], eax
	cmp     eax, ecx
	jl      L12
	mov     eax, ecx
L12:
	mov     dword ptr [ebp - 1E8H], eax
	push    eax
	mov     ecx, dword ptr [ebp - 1D0H]
	mov     edx, dword ptr [ebp - 1D8H]
	push    ecx
	push    edx
	call    _A_memcmp
	add     esp, 12
	mov     dword ptr [ebp - 1E4H], eax
	test    eax, eax
	jnz     L14
	mov     eax, dword ptr [ebp - 1DCH]
	mov     ecx, dword ptr [ebp - 1E0H]
	cmp     eax, ecx
	jl      L14
	jmp     L15
L13:
	mov     dword ptr [ebp - 0D0H], 0
	lea     ecx, [ebp - 0B8H]
	mov     dword ptr [ebp - 1F4H], ecx
	lea     ecx, [ebp - 0D0H]
	mov     dword ptr [ebp - 1F0H], ecx
	lea     ecx, [ebp - 20H]
	mov     dword ptr [ebp - 0CCH], 0
	lea     edx, [ebp - 100H]
	mov     dword ptr [ebp - 0C0H], 0
	push    ecx
	push    edx
	call    dword ptr [_bcb6_std_string_ctor_assign]
	add     esp, 8
	lea     eax, [ebp - 108H]
	mov     ecx, dword ptr [ebp - 1F0H]
	mov     dword ptr [ebp - 1F8H], eax
	mov     eax, dword ptr [ecx + 4H]
	mov     ecx, dword ptr [ecx]
	sub     eax, ecx
	mov     ecx, 0AAAAAAABH
	mul     ecx
	mov     ecx, edx
	xor     eax, eax
	shr     ecx, 4
	mov     dword ptr [ebp - 0E8H], eax
	mov     dword ptr [ebp - 0E4H], eax
	mov     dword ptr [ebp - 0D8H], eax
	mov     dword ptr [ebp - 1FCH], ecx
	jz      L14
	shl     ecx, 3
	lea     ecx, [ecx + ecx * 2]
	mov     dword ptr [ebp - 200H], ecx
	call    @bcb6_std_node_alloc_allocate@4
L14:
	mov     ecx, dword ptr [ebp - 1FCH]
	mov     dword ptr [ebp - 0E8H], eax
	shl     ecx, 3
	lea     edx, [ebp - 208H]
	mov     dword ptr [ebp - 0E4H], edx
	lea     ecx, [ecx + ecx * 2]
	push    edx
	add     ecx, eax
	mov     edx, dword ptr [ebp - 1F0H]
	mov     dword ptr [ebp - 0D8H], ecx
	push    eax
	mov     ecx, dword ptr [edx + 4H]
	mov     edx, dword ptr [edx]
	push    ecx
	push    edx
	call    dword ptr [_F0044CCD4]
	mov     dword ptr [ebp - 0E4H], eax
	lea     eax, [ebp - 100H]
	add     esp, 16
	mov     edx, dword ptr [ebp - 1B4H]
	push    eax
	push    eax
	mov     dword ptr [ebp - 1ECH], eax
	mov     dword ptr [ebp - 20CH], edx
	mov     dword ptr [esp], edx
	mov     edx, dword ptr [ebp - 1ACH]
	lea     ecx, [ebp - 210H]
	push    edx
	push    ecx
	call    dword ptr [_F004F2424]
	add     esp, 16
	lea     edx, [ebp - 210H]
	mov     edx, dword ptr [edx]
	mov     dword ptr [ebp - 1B4H], edx
	lea     ecx, [ebp - 0E8H]
	call    @bcb6_std_vector_string_dtor@4
	lea     ecx, [ebp - 100H]
	call    @bcb6_std_string_dtor@4
	lea     ecx, [ebp - 0D0H]
	call    @bcb6_std_vector_string_dtor@4
L15:
	mov     ecx, dword ptr [ebp - 1B4H]
	add     ecx, 40
	push    ecx
	call    dword ptr [_F0044CA38]
	add     esp, 8
	lea     ecx, [ebp - 20H]
	call    @bcb6_std_string_dtor@4
	lea     ecx, [ebp - 38H]
	call    @bcb6_std_string_dtor@4
L16:
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret

_TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile endp

end
