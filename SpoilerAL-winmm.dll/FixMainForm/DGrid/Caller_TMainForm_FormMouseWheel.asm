.486
.model flat

extrn @_TMainForm_FormMouseWheel@24:proc

public _Caller_TMainForm_FormMouseWheel

.code

align 16

_Caller_TMainForm_FormMouseWheel proc near

	push    eax
	push    eax
	mov     eax, dword ptr [esp + 8]
	mov     dword ptr [esp + 8], ecx
	mov     dword ptr [esp + 4], eax
	pop     ecx
	jmp     @_TMainForm_FormMouseWheel@24

_Caller_TMainForm_FormMouseWheel endp

end
