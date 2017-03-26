.486
.model flat, c

public VectorPushModuleEntry32A@8

.code

align 16

;void __stdcall VectorPushModuleEntry32A(vector<MODULEENTRY32A> *vector, LPMODULEENTRY32A element)
;{
;	vector->push_back(*element);
;}
VectorPushModuleEntry32A@8 proc near

	sizeof_MODULEENTRY32 equ 548

	mov     eax, dword ptr [esp +  4]
	mov     edx, dword ptr [eax +  4]
	mov     ecx, dword ptr [eax + 16]
	cmp     ecx, edx
	je      L1
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 16]
	mov     edi, edx
	mov     ecx, sizeof_MODULEENTRY32 / 4
	add     edx, sizeof_MODULEENTRY32
	rep movsd
	pop     edi
	pop     esi
	mov     dword ptr [eax +  4], edx
	ret     8
L1:
	mov     ecx, dword ptr [esp + 8]
	sub     esp, 24
	mov     dword ptr [esp     ], eax
	mov     dword ptr [esp +  4], edx
	lea     eax, [esp + 24]
	mov     dword ptr [esp +  8], ecx
	mov     dword ptr [esp + 12], eax
	mov     dword ptr [esp + 16], 1
	mov     eax, 004A5184H
	mov     dword ptr [esp + 20], 1
	call    eax
	add     esp, 24
	ret     8

VectorPushModuleEntry32A@8 endp

end
