.486
.model flat

public @TListBox_GetTopIndex@4

.code

align 16

;EXTERN_C int __msfastcall TListBox_GetTopIndex(TListBox *ListBox)
;{
;	return ListBox->TopIndex;
;}
@TListBox_GetTopIndex@4 proc near

	mov     edx, 00545D60H
	mov     eax, ecx
	jmp     edx

@TListBox_GetTopIndex@4 endp

end
