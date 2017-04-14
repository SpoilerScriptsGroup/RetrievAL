.486
.model flat

extrn _RepeatDepth:dword
extrn _TSSGCtrl_EnumReadSSG:dword
extrn _TSSGCtrl_ReadSSRFile:dword
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

	push    ebx
	push    esi
	push    edi
	sub     esp, 12
	lea     edx, [LineS]
	mov     eax, esp
	mov     ecx, dword ptr [_this]
	push    eax
	add     eax, 4
	push    eax
	add     eax, 4
	push    eax
	lea     eax, [tmpV]
	push    edx
	push    ecx
	push    eax
	call    dword ptr [_TSSGCtrl_ReadSSRFile]
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
	push    edx
	lea     ecx, [SSGFile]
	mov     edx, dword ptr [ParentStack]
	mov     eax, dword ptr [_this]
	push    edx
	push    ecx
	push    eax
	call    dword ptr [_TSSGCtrl_EnumReadSSG]
	add     esp, 24
	mov     eax, dword ptr [tmpV + 4H]
	cmp     it, eax
	jb      L2
	mov     edx, dword ptr [_RepeatDepth]
	dec     edx
	mov     dword ptr [_RepeatDepth], edx
	lea     ecx, [SSGFile]
	call    @bcb6_std_vector_string_dtor@4
	jmp     L5
L4:
	mov     ecx, dword ptr [ParentRepeat]
	mov     eax, dword ptr [RepeatIndex]
	push    ecx
	push    eax
	mov     ecx, dword ptr [ADJElem]
	mov     eax, dword ptr [ParentStack]
	push    ecx
	push    eax
	mov     eax, dword ptr [_this]
	lea     ecx, [tmpV]
	push    ecx
	push    eax
	call    dword ptr [_TSSGCtrl_EnumReadSSG]
	add     esp, 24
L5:
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
