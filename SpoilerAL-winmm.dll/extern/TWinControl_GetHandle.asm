.486
.model flat

public @TWinControl_GetHandle@4

.code

align 16

;EXTERN_C HWND __msfastcall TWinControl_GetHandle(TWinControl *WinControl)
;{
;	return WinControl->Handle;
;}
@TWinControl_GetHandle@4 proc near

	mov     edx, 0058750CH
	mov     eax, ecx
	jmp     edx

@TWinControl_GetHandle@4 endp

end
