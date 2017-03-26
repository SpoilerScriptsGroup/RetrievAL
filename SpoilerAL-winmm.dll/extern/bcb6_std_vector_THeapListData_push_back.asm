.486
.model flat

public @bcb6_std_vector_THeapListData_push_back@8

.data

data1 label byte
	dword   004A46ACH
	dword   00002005H, -4
	dword   00000000H

data2 label byte
	dword   00000000H, -40
	dword   00050000H, 00000000H
	dword   00000000H, 00050008H
	dword   00000000H
	dword   data1

.code

align 16

;void __msfastcall bcb6_std_vector_THeapListData_push_back(vector<THeapListData> *heapList, THeapListData *heapListData)
;{
;	heapList->push_back(*heapListData);
;}
@bcb6_std_vector_THeapListData_push_back@8 proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 52
	push    ebx
	push    esi
	mov     ebx, ecx
	mov     esi, edx
	mov     ecx, 005D54CCH
	call    ecx
	mov     edx, dword ptr [ebx +  4H]
	mov     ecx, dword ptr [ebx + 10H]
	cmp     edx, ecx
	je      L2
	test    edx, edx
	jz      L1
	mov     ecx, dword ptr [esi      ]
	mov     eax, dword ptr [esi +  4H]
	mov     dword ptr [edx      ], ecx
	mov     dword ptr [edx +  4H], eax
	mov     ecx, dword ptr [esi +  8H]
	mov     eax, dword ptr [esi + 0CH]
	mov     dword ptr [edx +  8H], ecx
	mov     dword ptr [edx + 0CH], eax
	mov     ecx, dword ptr [esi + 10H]
	mov     eax, dword ptr [esi + 14H]
	mov     dword ptr [edx + 10H], ecx
	mov     dword ptr [edx + 14H], eax
L1:
	add     dword ptr [ebx +  4H], 24
	jmp     L3
L2:
	mov     eax, 004A4258H
	lea     ecx, [ebp - 34H]
	push    1
	push    1
	push    ecx
	push    esi
	push    edx
	push    ebx
	call    eax
	add     esp, 24
L3:
	pop     esi
	pop     ebx
	mov     esp, ebp
	pop     ebp
	ret

@bcb6_std_vector_THeapListData_push_back@8 endp

end
