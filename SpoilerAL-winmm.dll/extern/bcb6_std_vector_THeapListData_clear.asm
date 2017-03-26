.486
.model flat

public @bcb6_std_vector_THeapListData_clear@4

.code

align 16

;void __msfastcall bcb6_std_vector_THeapListData_clear(vector<THeapListData> *heapList)
;{
;	heapList->clear();
;}
@bcb6_std_vector_THeapListData_clear@4 proc near

	push    ebx
	push    esi
	sub     esp, 16
	mov     ebx, ecx
	mov     eax, 004A34B8H
	mov     edx, esp
	mov     ecx, dword ptr [ebx]
	mov     esi, dword ptr [ebx + 4]
	push    0
	push    edx
	push    ecx
	push    esi
	push    esi
	call    eax
	add     esp, 20
	mov     esi, eax
	mov     eax, 004694E4H
	lea     edx, [esp + 8]
	mov     ecx, dword ptr [ebx + 4]
	push    edx
	push    ecx
	push    esi
	call    eax
	add     esp, 12 + 16
	mov     dword ptr [ebx + 4], esi
	pop     esi
	pop     ebx
	ret

@bcb6_std_vector_THeapListData_clear@4 endp

end
