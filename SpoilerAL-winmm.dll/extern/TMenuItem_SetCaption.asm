.486
.model flat, c

extrn F005D54CC:dword
extrn F005E0D50:dword
extrn F0054B87C:dword
extrn F005E0EA8:dword

public TMenuItem_SetCaption@8

.const

data1 label byte
	dword   00401834H
	dword   00000004H, -4
	dword   00000000H

data2 label byte
	dword   00000000H, -40
	dword   00050000H, 00000000H
	dword   data1

.code

align 16

;EXTERN_C void __stdcall TMenuItem_SetCaption(TMenuItem *MenuItem, LPCSTR lpText)
;{
;	MenuItem->Caption = lpText;
;}
TMenuItem_SetCaption@8 proc near

	push    ebp
	mov     eax, offset data2
	mov     ebp, esp
	sub     esp, 40
	call    dword ptr [F005D54CC]
	mov     edx, dword ptr [ebp + 0CH]
	lea     eax, [ebp - 4H]
	call    dword ptr [F005E0D50]
	mov     edx, dword ptr [eax]
	mov     eax, dword ptr [ebp + 8H]
	call    dword ptr [F0054B87C]
	mov     edx, 2
	lea     eax, [ebp - 4H]
	call    dword ptr [F005E0EA8]
	mov     esp, ebp
	pop     ebp
	ret     8

TMenuItem_SetCaption@8 endp

end
