.486
.model flat

public @TCanvas_Lock@4

.code

align 16

;EXTERN_C void __msfastcall TCanvas_Lock(TCanvas *Canvas)
;{
;	Canvas->Lock();
;}
@TCanvas_Lock@4 proc near

	mov     edx, 0055E284H
	mov     eax, ecx
	jmp     edx

@TCanvas_Lock@4 endp

end
