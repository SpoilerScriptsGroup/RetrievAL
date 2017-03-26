.486
.model flat

public @TCanvas_GetHandle@4

.code

align 16

;EXTERN_C HDC __msfastcall TCanvas_GetHandle(TCanvas *Canvas)
;{
;	return Canvas->Handle;
;}
@TCanvas_GetHandle@4 proc near

	mov     edx, 0055E74CH
	mov     eax, ecx
	jmp     edx

@TCanvas_GetHandle@4 endp

end
