.486
.model flat

extrn _RepeatDepth:dword
extrn @GrowSubjectProperty@4:proc
extrn @bcb6_std_vector_string_resize@8:proc
extrn @bcb6_std_string_assign@8:proc
extrn @bcb6_std_vector_string_dtor@4:proc

public _TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile

.code

align 16

_TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile proc near

	_this           equ <ebp +   8H>
	ParentStack     equ <ebp +  10H>
	ADJElem         equ <ebp +  14H>
	RepeatIndex     equ <ebp +  18H>
	ParentRepeat    equ <ebp +  1CH>
	LineS           equ <ebp -  38H>
	tmpV            equ <ebp - 2F4H>
	SSGFile         equ <ebp - 2DCH>
	esp_loopStep    equ <esp       >
	esp_loopIndex   equ <esp +   4H>
	esp_elementSize equ <esp +   8H>
	esp_parentIndex equ <esp +   8H>
	it              equ ebx
	elementSize     equ esi
	EnumReadSSG     equ 004E5090H
	ReadSSRFile     equ 004FEAECH

	push    ebx
	push    esi
	push    edi
	mov     eax, ReadSSRFile
	sub     esp, 12
	lea     edx, [LineS]
	mov     ecx, esp
	push    ecx
	add     ecx, 4
	push    ecx
	add     ecx, 4
	push    ecx
	mov     ecx, dword ptr [_this]
	push    edx
	lea     edx, [tmpV]
	push    ecx
	push    edx
	call    eax
	mov     eax, dword ptr [tmpV]
	mov     ecx, dword ptr [tmpV + 4H]
	add     esp, 24
	cmp     eax, ecx
	mov     elementSize, dword ptr [esp_elementSize]
	je      L5
	test    elementSize, elementSize
	jz      L4
	lea     ecx, [esp_parentIndex]
	call    @GrowSubjectProperty@4
	test    eax, eax
	jz      L1
	mov     ecx, dword ptr [_RepeatDepth]
	mov     edx, dword ptr [RepeatIndex]
	mov     dword ptr [eax +  4H], ecx
	mov     ecx, dword ptr [ParentRepeat]
	mov     dword ptr [eax +  8H], edx
	mov     dword ptr [eax + 0CH], ecx
L1:
	mov     eax, dword ptr [_RepeatDepth]
	lea     ecx, [SSGFile]
	inc     eax
	mov     edx, elementSize
	mov     dword ptr [_RepeatDepth], eax
	mov     dword ptr [ecx      ], 0
	mov     dword ptr [ecx +  4H], 0
	mov     dword ptr [ecx + 10H], 0
	call    @bcb6_std_vector_string_resize@8
	mov     it, dword ptr [tmpV]
	mov     edi, dword ptr [SSGFile + 4]
L2:
	mov     esi, dword ptr [SSGFile]
L3:
	mov     edx, it
	add     it, 24
	mov     ecx, esi
	add     esi, 24
	call    @bcb6_std_string_assign@8
	cmp     esi, edi
	jne     L3
	mov     edx, dword ptr [esp_loopStep]
	mov     eax, dword ptr [esp_loopIndex]
	mov     ecx, dword ptr [esp_parentIndex]
	add     edx, eax
	mov     dword ptr [esp_loopIndex], edx
	mov     edx, dword ptr [ADJElem]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ParentStack]
	mov     eax, EnumReadSSG
	push    edx
	push    ecx
	mov     ecx, dword ptr [_this]
	lea     edx, [SSGFile]
	push    edx
	push    ecx
	call    eax
	add     esp, 24
	mov     eax, dword ptr [tmpV + 4H]
	cmp     it, eax
	jb      L2
	mov     edx, dword ptr [_RepeatDepth]
	lea     eax, [ebp - 9ECH]
	dec     edx
	mov     ecx, dword ptr [SSGFile]
	mov     dword ptr [_RepeatDepth], edx
	mov     edx, dword ptr [SSGFile + 4H]
	mov     dword ptr [ebp - 9E4H], ecx
	mov     dword ptr [ebp - 9E0H], edx
	push    eax
	mov     eax, 00415F90H
	push    edx
	push    ecx
	call    eax
	add     esp, 12
	lea     ecx, [SSGFile]
	call    @bcb6_std_vector_string_dtor@4
	jmp     L5
L4:
	mov     eax, dword ptr [ADJElem]
	mov     ecx, dword ptr [ParentStack]
	push    -1
	push    0
	push    eax
	push    ecx
	mov     eax, EnumReadSSG
	lea     edx, [tmpV]
	mov     ecx, dword ptr [_this]
	push    edx
	push    ecx
	call    eax
	add     esp, 24
L5:
	lea     eax, [ebp - 9D0H]
	mov     ecx, dword ptr [tmpV]
	mov     edx, dword ptr [tmpV + 4H]
	mov     dword ptr [ebp - 9C8H], ecx
	mov     dword ptr [ebp - 9C4H], edx
	push    eax
	mov     eax, 00415F90H
	push    edx
	push    ecx
	call    eax
	add     esp, 12
	lea     ecx, [tmpV]
	call    @bcb6_std_vector_string_dtor@4
	mov     eax, 004EB508H
	add     esp, 12
	pop     edi
	pop     esi
	pop     ebx
	jmp     eax

_TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile endp

end
