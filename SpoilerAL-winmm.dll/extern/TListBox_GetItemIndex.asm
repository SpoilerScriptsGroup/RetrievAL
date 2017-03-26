.486
.model flat

public @TListBox_GetItemIndex@4

.code

align 16

;EXTERN_C int __msfastcall TListBox_GetItemIndex(TListBox *ListBox)
;{
;	return ListBox->ItemIndex;
;}
@TListBox_GetItemIndex@4 proc near

	mov     edx, dword ptr [ecx]
	mov     eax, ecx
	jmp     dword ptr [edx + 0C8H]

@TListBox_GetItemIndex@4 endp

end
