.486
.model flat

public @TEdit_SetFocus@4

.code

align 16

;EXTERN_C void __msfastcall TEdit_SetFocus(TEdit *Edit)
;{
;	Edit->SetFocus();
;}
@TEdit_SetFocus@4 proc near

	mov     eax, ecx
	mov     ecx, dword ptr [ecx]
	jmp     dword ptr [ecx + 0C0H]

@TEdit_SetFocus@4 endp

end
