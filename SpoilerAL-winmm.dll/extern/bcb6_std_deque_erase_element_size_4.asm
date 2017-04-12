.486
.model flat, c

extrn bcb6_std_allocator_deallocate:dword

public bcb6_std_deque_erase_element_size_4@8

.code

align 16

;void __stdcall bcb6_std_deque_erase_element_size_4(deque<DWORD> *_deque, deque<DWORD>::iterator *it)
;{
;	_deque->erase(*it);
;}
bcb6_std_deque_erase_element_size_4@8 proc near

	push    ebp
	mov     ebp, esp
	sub     esp, 376
	push    ebx
	push    esi
	mov     edx, dword ptr [ebp + 0CH]
	mov     ebx, dword ptr [ebp + 8H]
	mov     eax, dword ptr [edx + 4H]
	mov     ecx, dword ptr [edx + 8H]
	mov     dword ptr [ebp - 74H], eax
	mov     dword ptr [ebp - 70H], ecx
	mov     dword ptr [ebp - 64H], eax
	mov     dword ptr [ebp - 60H], ecx
	mov     eax, dword ptr [edx]
	mov     ecx, dword ptr [edx + 0CH]
	mov     dword ptr [ebp - 6CH], ecx
	mov     dword ptr [ebp - 68H], eax
	add     eax, 4
	mov     dword ptr [ebp - 5CH], ecx
	add     ecx, 4
	mov     dword ptr [ebp - 78H], eax
	mov     edx, dword ptr [ebp - 70H]
	cmp     eax, edx
	jne     L1
	mov     eax, dword ptr [ecx]
	mov     dword ptr [ebp - 4H], ecx
	mov     dword ptr [ebp - 78H], eax
	lea     edx, [eax + 4]
	mov     dword ptr [ebp - 74H], eax
	mov     dword ptr [ebp - 70H], edx
	mov     dword ptr [ebp - 6CH], ecx
L1:
	mov     dword ptr [ebp - 0CH], ebx
	mov     eax, dword ptr [ebp - 5CH]
	mov     edx, dword ptr [ebx + 0CH]
	mov     ecx, eax
	sub     eax, edx
	sub     ecx, edx
	sar     ecx, 31
	mov     edx, dword ptr [ebp - 68H]
	and     ecx, 3
	mov     esi, dword ptr [ebp - 64H]
	add     eax, ecx
	sar     eax, 2
	mov     ecx, edx
	sub     edx, esi
	sub     ecx, esi
	sar     ecx, 31
	mov     esi, dword ptr [ebp - 0CH]
	and     ecx, 3
	add     edx, ecx
	mov     ecx, dword ptr [esi + 8H]
	sar     edx, 2
	sub     ecx, dword ptr [esi]
	add     eax, edx
	mov     edx, ecx
	sar     edx, 31
	and     edx, 3
	add     ecx, edx
	lea     edx, [ebx + 10H]
	sar     ecx, 2
	lea     eax, [eax + ecx - 1]
	mov     dword ptr [ebp - 14H], edx
	mov     dword ptr [ebp - 10H], ebx
	mov     dword ptr [ebp - 8H], eax
	mov     eax, dword ptr [ebp - 14H]
	mov     ecx, dword ptr [ebp - 10H]
	mov     edx, dword ptr [eax + 0CH]
	mov     ecx, dword ptr [ecx + 0CH]
	sub     edx, ecx
	mov     eax, edx
	sar     eax, 31
	and     eax, 3
	add     edx, eax
	mov     eax, dword ptr [ebp - 14H]
	sar     edx, 2
	mov     ecx, dword ptr [eax]
	mov     esi, dword ptr [eax + 4H]
	mov     eax, ecx
	sub     ecx, esi
	sub     eax, esi
	sar     eax, 31
	mov     esi, dword ptr [ebp - 10H]
	and     eax, 3
	add     ecx, eax
	mov     eax, dword ptr [esi + 8H]
	sar     ecx, 2
	mov     esi, dword ptr [esi]
	add     edx, ecx
	mov     ecx, eax
	sub     eax, esi
	sub     ecx, esi
	sar     ecx, 31
	and     ecx, 3
	add     eax, ecx
	sar     eax, 2
	lea     edx, [edx + eax - 1]
	shr     edx, 1
	cmp     edx, dword ptr [ebp - 8H]
	jbe     L8
	sub     esp, 48
	mov     eax, dword ptr [ebx]
	mov     ecx, dword ptr [ebx + 4H]
	mov     dword ptr [esp], eax
	mov     dword ptr [esp + 4H], ecx
	mov     dword ptr [ebp - 0D8H], eax
	mov     dword ptr [ebp - 0D4H], ecx
	mov     dword ptr [ebp - 0A8H], eax
	mov     dword ptr [ebp - 0A4H], ecx
	mov     eax, dword ptr [ebx + 8H]
	mov     ecx, dword ptr [ebx + 0CH]
	mov     dword ptr [esp + 8H], eax
	mov     dword ptr [esp + 0CH], ecx
	mov     dword ptr [ebp - 0D0H], eax
	mov     dword ptr [ebp - 0CCH], ecx
	mov     dword ptr [ebp - 0A0H], eax
	mov     dword ptr [ebp - 9CH], ecx
	mov     eax, dword ptr [ebp - 68H]
	mov     ecx, dword ptr [ebp - 64H]
	mov     dword ptr [esp + 10H], eax
	mov     dword ptr [esp + 14H], ecx
	mov     dword ptr [ebp - 0C8H], eax
	mov     dword ptr [ebp - 0C4H], ecx
	mov     dword ptr [ebp - 98H], eax
	mov     dword ptr [ebp - 94H], ecx
	mov     eax, dword ptr [ebp - 60H]
	mov     ecx, dword ptr [ebp - 5CH]
	mov     dword ptr [esp + 18H], eax
	mov     dword ptr [esp + 1CH], ecx
	mov     dword ptr [ebp - 0C0H], eax
	mov     dword ptr [ebp - 0BCH], ecx
	mov     dword ptr [ebp - 90H], eax
	mov     dword ptr [ebp - 8CH], ecx
	mov     eax, dword ptr [ebp - 78H]
	mov     ecx, dword ptr [ebp - 74H]
	mov     dword ptr [esp + 20H], eax
	mov     dword ptr [esp + 24H], ecx
	mov     dword ptr [ebp - 0B8H], eax
	mov     dword ptr [ebp - 0B4H], ecx
	mov     dword ptr [ebp - 88H], eax
	mov     dword ptr [ebp - 84H], ecx
	mov     eax, dword ptr [ebp - 70H]
	mov     ecx, dword ptr [ebp - 6CH]
	mov     dword ptr [esp + 28H], eax
	mov     dword ptr [esp + 2CH], ecx
	mov     dword ptr [ebp - 0B0H], eax
	mov     dword ptr [ebp - 0ACH], ecx
	mov     dword ptr [ebp - 80H], eax
	mov     dword ptr [ebp - 7CH], ecx
	mov     eax, dword ptr [ebp - 24H]
	mov     ecx, dword ptr [ebp - 20H]
	mov     dword ptr [ebp - 1CH], eax
	mov     dword ptr [ebp - 18H], ecx
	mov     ecx, dword ptr [esp + 1CH]
	mov     edx, dword ptr [esp + 1CH]
	mov     eax, ecx
	sub     ecx, edx
	sub     eax, edx
	sar     eax, 31
	mov     esi, dword ptr [esp + 14H]
	and     eax, 3
	add     ecx, eax
	mov     eax, dword ptr [esp + 10H]
	sar     ecx, 2
	mov     edx, eax
	sub     eax, esi
	sub     edx, esi
	sar     edx, 31
	mov     esi, dword ptr [esp]
	and     edx, 3
	add     eax, edx
	mov     edx, dword ptr [esp + 8H]
	sar     eax, 2
	add     ecx, eax
	sub     edx, esi
	mov     eax, edx
	sar     eax, 31
	and     eax, 3
	add     edx, eax
	sar     edx, 2
	lea     ecx, [ecx + edx - 1]
	test    ecx, ecx
	jle     L5
L2:
	mov     eax, dword ptr [esp + 10H]
	mov     edx, dword ptr [esp + 14H]
	cmp     eax, edx
	jne     L3
	mov     eax, dword ptr [esp + 1CH]
	sub     eax, 4
	mov     dword ptr [esp + 1CH], eax
	mov     eax, dword ptr [eax]
	mov     dword ptr [esp + 14H], eax
	add     eax, 4
	mov     dword ptr [esp + 10H], eax
L3:
	sub     eax, 4
	mov     dword ptr [esp + 10H], eax
	mov     edx, dword ptr [eax]
	mov     eax, dword ptr [esp + 20H]
	mov     esi, dword ptr [esp + 24H]
	cmp     eax, esi
	jne     L4
	mov     eax, dword ptr [esp + 2CH]
	sub     eax, 4
	mov     dword ptr [esp + 2CH], eax
	mov     eax, dword ptr [eax]
	mov     dword ptr [esp + 24H], eax
	add     eax, 4
	mov     dword ptr [esp + 28H], eax
	mov     dword ptr [esp + 20H], eax
L4:
	sub     eax, 4
	mov     dword ptr [esp + 20H], eax
	mov     dword ptr [eax], edx
	dec     ecx
	test    ecx, ecx
	jg      L2
L5:
	mov     eax, dword ptr [esp + 20H]
	mov     ecx, dword ptr [esp + 24H]
	mov     dword ptr [ebp - 0E8H], eax
	mov     dword ptr [ebp - 0E4H], ecx
	mov     eax, dword ptr [esp + 28H]
	mov     ecx, dword ptr [esp + 2CH]
	mov     dword ptr [ebp - 0E0H], eax
	mov     dword ptr [ebp - 0DCH], ecx
	add     esp, 48
	mov     eax, dword ptr [ebx]
	mov     ecx, dword ptr [ebx + 8H]
	add     eax, 4
	cmp     ecx, eax
	je      L6
	mov     dword ptr [ebx], eax
	jmp     L10
L6:
	mov     eax, dword ptr [ebx + 4H]
	test    eax, eax
	jz      L7
	push    4
	push    eax
	call    dword ptr [bcb6_std_allocator_deallocate]
	add     esp, 8
L7:
	mov     eax, dword ptr [ebx + 0CH]
	add     eax, 4
	mov     dword ptr [ebx + 0CH], eax
	mov     ecx, dword ptr [eax]
	mov     dword ptr [ebx], ecx
	lea     edx, [ecx + 4]
	mov     dword ptr [ebx + 4H], ecx
	mov     dword ptr [ebx + 8H], edx
	jmp     L10
L8:
	lea     eax, [ebp - 48H]
	push    0
	push    eax
	sub     esp, 48
	mov     eax, dword ptr [ebp - 78H]
	mov     ecx, dword ptr [ebp - 74H]
	mov     dword ptr [esp], eax
	mov     dword ptr [esp + 4H], ecx
	mov     dword ptr [ebp - 148H], eax
	mov     dword ptr [ebp - 144H], ecx
	mov     dword ptr [ebp - 118H], eax
	mov     dword ptr [ebp - 114H], ecx
	mov     eax, dword ptr [ebp - 70H]
	mov     ecx, dword ptr [ebp - 6CH]
	mov     dword ptr [esp + 8H], eax
	mov     dword ptr [esp + 0CH], ecx
	mov     dword ptr [ebp - 140H], eax
	mov     dword ptr [ebp - 13CH], ecx
	mov     dword ptr [ebp - 110H], eax
	mov     dword ptr [ebp - 10CH], ecx
	mov     eax, dword ptr [ebx + 10H]
	mov     ecx, dword ptr [ebx + 14H]
	mov     dword ptr [esp + 10H], eax
	mov     dword ptr [esp + 14H], ecx
	mov     dword ptr [ebp - 138H], eax
	mov     dword ptr [ebp - 134H], ecx
	mov     dword ptr [ebp - 108H], eax
	mov     dword ptr [ebp - 104H], ecx
	mov     eax, dword ptr [ebx + 18H]
	mov     ecx, dword ptr [ebx + 1CH]
	mov     dword ptr [esp + 18H], eax
	mov     dword ptr [esp + 1CH], ecx
	mov     dword ptr [ebp - 130H], eax
	mov     dword ptr [ebp - 12CH], ecx
	mov     dword ptr [ebp - 100H], eax
	mov     dword ptr [ebp - 0FCH], ecx
	mov     eax, dword ptr [ebp - 68H]
	mov     ecx, dword ptr [ebp - 64H]
	mov     dword ptr [esp + 20H], eax
	mov     dword ptr [esp + 24H], ecx
	mov     dword ptr [ebp - 128H], eax
	mov     dword ptr [ebp - 124H], ecx
	mov     dword ptr [ebp - 0F8H], eax
	mov     dword ptr [ebp - 0F4H], ecx
	mov     eax, dword ptr [ebp - 60H]
	mov     ecx, dword ptr [ebp - 5CH]
	mov     dword ptr [esp + 28H], eax
	mov     dword ptr [esp + 2CH], ecx
	mov     dword ptr [ebp - 120H], eax
	mov     dword ptr [ebp - 11CH], ecx
	mov     dword ptr [ebp - 0F0H], eax
	mov     dword ptr [ebp - 0ECH], ecx
	mov     eax, dword ptr [ebp - 40H]
	mov     ecx, dword ptr [ebp - 3CH]
	mov     dword ptr [ebp - 38H], eax
	lea     eax, [ebx + 10H]
	mov     dword ptr [ebp - 34H], ecx
	lea     ecx, [ebp - 158H]
	mov     dword ptr [ebp - 30H], eax
	mov     eax, 004D4A10H
	push    ecx
	call    eax
	add     esp, 60
	mov     eax, dword ptr [ebx + 10H]
	mov     ecx, dword ptr [ebx + 14H]
	cmp     eax, ecx
	je      L9
	sub     eax, 4
	mov     dword ptr [ebx + 10H], eax
	jmp     L10
L9:
	push    ebx
	mov     eax, 004D4064H
	call    eax
	pop     ecx
L10:
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret     8

bcb6_std_deque_erase_element_size_4@8 endp

end
