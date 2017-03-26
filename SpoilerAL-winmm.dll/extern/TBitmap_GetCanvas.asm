.486
.model flat

public @TBitmap_GetCanvas@4

.code

align 16

;EXTERN_C LPVOID __msfastcall TBitmap_GetCanvas(TBitmap *Bitmap)
;{
;	return Bitmap->Canvas;
;}
@TBitmap_GetCanvas@4 proc near

	mov     edx, 00562CF0H
	mov     eax, ecx
	jmp     edx

@TBitmap_GetCanvas@4 endp

end
