.486
.model flat

public @TApplication_ProcessMessages@4

.code

align 16

;EXTERN_C void __msfastcall TApplication_ProcessMessages(TApplication *Application)
;{
;	Application->ProcessMessages();
;}
@TApplication_ProcessMessages@4 proc near

	mov     eax, ecx
	mov     ecx, 00571120H
	jmp     ecx

@TApplication_ProcessMessages@4 endp

end
