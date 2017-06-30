.486
.model flat

extrn _TSSGScriptStruct_GetTitleTextWidth_CtorData:ptr
extrn __imp__GetTextExtentPoint32A@16:dword
extrn @bcb6_std_string_dtor@4:proc
extrn _TSSGSubject_GetSubjectName:dword
extrn _F005D54CC:dword

public _TSSGSubject_GetSubjectNameTextWidth@12

.code

align 16

_TSSGSubject_GetSubjectNameTextWidth@12 proc near

	push    ebp
	mov     eax, offset _TSSGScriptStruct_GetTitleTextWidth_CtorData
	mov     ebp, esp
	sub     esp, 68
	call    dword ptr [_F005D54CC]
	mov     ecx, dword ptr [ebp + 10H]
	mov     eax, dword ptr [ebp + 8H]
	push    ecx
	lea     ecx, [ebp - 18H]
	push    eax
	push    ecx
	call    dword ptr [_TSSGSubject_GetSubjectName]
	add     esp, 12
	lea     eax, [ebp - 44H]
	mov     ecx, dword ptr [ebp - 18H]
	mov     edx, dword ptr [ebp - 14H]
	sub     edx, ecx
	push    eax
	mov     eax, dword ptr [ebp + 0CH]
	push    edx
	push    ecx
	push    eax
	call    dword ptr [__imp__GetTextExtentPoint32A@16]
	lea     ecx, [ebp - 18H]
	call    @bcb6_std_string_dtor@4
	mov     eax, dword ptr [ebp - 44H]
	mov     esp, ebp
	pop     ebp
	ret     12

_TSSGSubject_GetSubjectNameTextWidth@12 endp

end
