.486
.model flat

public @TCanvas_Unlock@4

.code

align 16

;EXTERN_C void __msfastcall TCanvas_Unlock(TCanvas *Canvas)
;{
;	Canvas->Unlock();
;}
@TCanvas_Unlock@4 proc near

	mov     edx, 0055E620H
	mov     eax, ecx
	jmp     edx

@TCanvas_Unlock@4 endp

end
