.486
.model flat

CODEPAGE_SUPPORT equ 0

if CODEPAGE_SUPPORT
extrn __imp__IsDBCSLeadByte@4:dword
endif
extrn @bcb6_std_vector_push_back@8:proc

public _TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar

.code

align 16

_TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar proc near

if CODEPAGE_SUPPORT
	movzx   eax, byte ptr [esi]
	push    005072DCH
	push    eax
	call    dword ptr [__imp__IsDBCSLeadByte@4]
	test    eax, eax
	jz      L2
	mov     dl, byte ptr [esi]
else
	mov     dl, byte ptr [esi]
	push    005072DCH
	cmp     dl, 81H
	jb      L2
	cmp     dl, 9FH
	jbe     L1
	cmp     dl, 0E0H
	jb      L2
	cmp     dl, 0FCH
	ja      L2
L1:
endif
	mov     ecx, dword ptr [ebp - 618H]
	call    @bcb6_std_vector_push_back@8
	inc     esi
L2:
	ret

_TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar endp

end
