.486
.model flat, c

public TSSGCtrl_SetSSGDataFile_IgnoreEmptyData

.code

align 16

TSSGCtrl_SetSSGDataFile_IgnoreEmptyData proc near

	mov     eax, dword ptr [esp + 8]
	mov     ecx, dword ptr [eax + 4]
	mov     eax, dword ptr [eax    ]
	cmp     eax, ecx
	je      L1
	push    ebp
	mov     eax, 004F0A51H
	mov     ebp, esp
	sub     esp, 1152
	jmp     eax
L1:
	ret

TSSGCtrl_SetSSGDataFile_IgnoreEmptyData endp

end
