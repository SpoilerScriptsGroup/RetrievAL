.486
.model flat
assume fs:nothing

extrn __imp__GetTextExtentPoint32A@16:dword
extrn @bcb6_std_string_dtor@4:proc
extrn _F005D54CC:dword
extrn _TSSGScriptStruct_GetDistinction:dword

public _TSSGScriptStruct_GetTitleTextWidth_CtorData
public _TSSGScriptStruct_GetTitleTextWidth@8

.const

data1 label byte
	dword   00416274H
	dword   00000004H, -24
	dword   00000000H

data2 label byte
	dword   00416274H
	dword   00000004H, -24
	dword   00000000H

_TSSGScriptStruct_GetTitleTextWidth_CtorData label byte
	dword   00000000H, -60
	dword   00050000H, 00000000H
	dword   data2
	dword   00050000H, 00000000H
	dword   data1
	dword   00050014H, 00000000H
	dword   00000000H, 00050014H
	dword   00000000H, 00000000H

.code

align 16

_TSSGScriptStruct_GetTitleTextWidth@8 proc near

	lpszTitle equ 006030FFH

	push    ebp
	mov     eax, offset _TSSGScriptStruct_GetTitleTextWidth_CtorData
	mov     ebp, esp
	sub     esp, 68
	call    dword ptr [_F005D54CC]
	mov     edx, dword ptr [ebp + 8H]
	lea     ecx, [ebp - 18H]
	push    lpszTitle
	push    edx
	push    ecx
	call    dword ptr [_TSSGScriptStruct_GetDistinction]
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
	mov     ecx, dword ptr [ebp - 60]
	mov     eax, dword ptr [ebp - 44H]
	mov     dword ptr fs:[0], ecx
	mov     esp, ebp
	pop     ebp
	ret     8

_TSSGScriptStruct_GetTitleTextWidth@8 endp

end
