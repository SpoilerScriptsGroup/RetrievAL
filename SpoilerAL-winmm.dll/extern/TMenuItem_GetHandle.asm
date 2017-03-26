.486
.model flat

public @TMenuItem_GetHandle@4

.code

align 16

;EXTERN_C HMENU __msfastcall TMenuItem_GetHandle(const TMenuItem *MenuItem)
;{
;	return MenuItem->Handle;
;}
@TMenuItem_GetHandle@4 proc near

	mov     edx, 0054A0DCH
	mov     eax, ecx
	jmp     edx

@TMenuItem_GetHandle@4 endp

end
