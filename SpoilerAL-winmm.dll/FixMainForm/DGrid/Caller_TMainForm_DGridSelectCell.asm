.486
.model flat

extrn @_TMainForm_DGridSelectCell@20:proc

public _Caller_TMainForm_DGridSelectCell

.code

align 16

_Caller_TMainForm_DGridSelectCell proc near

	push    eax
	push    eax
	mov     eax, dword ptr [esp + 8]
	mov     dword ptr [esp + 8], ecx
	mov     dword ptr [esp + 4], eax
	pop     ecx
	jmp     @_TMainForm_DGridSelectCell@20

_Caller_TMainForm_DGridSelectCell endp

end
