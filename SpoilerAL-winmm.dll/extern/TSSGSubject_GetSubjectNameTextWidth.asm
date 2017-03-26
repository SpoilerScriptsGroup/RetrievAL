.486
.model flat

extrn _TSSGScriptStruct_GetTitleTextWidth_CtorData:ptr
extrn __imp__GetTextExtentPoint32A@16:dword
extrn @bcb6_std_string_dtor@4:proc

public _TSSGSubject_GetSubjectNameTextWidth@12

.code

align 16

_TSSGSubject_GetSubjectNameTextWidth@12 proc near

	TSSGSubject_GetSubjectName equ 0052CF6CH

	push    ebp
	mov     ecx, 005D54CCH
	mov     ebp, esp
	sub     esp, 68
	mov     eax, offset _TSSGScriptStruct_GetTitleTextWidth_CtorData
	call    ecx
	mov     ecx, dword ptr [ebp + 10H]
	mov     eax, dword ptr [ebp + 8H]
	push    ecx
	push    eax
	mov     eax, TSSGSubject_GetSubjectName
	lea     ecx, [ebp - 18H]
	push    ecx
	call    eax
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
