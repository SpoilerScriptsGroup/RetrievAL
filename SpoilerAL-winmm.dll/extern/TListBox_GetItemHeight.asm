.486
.model flat

public @TListBox_GetItemHeight@4

.code

align 16

;EXTERN_C int __msfastcall TListBox_GetItemHeight(TListBox *ListBox)
;{
;	return ListBox->ItemHeight;
;}
@TListBox_GetItemHeight@4 proc near

	mov     edx, 00545B74H
	mov     eax, ecx
	jmp     edx

@TListBox_GetItemHeight@4 endp

end
