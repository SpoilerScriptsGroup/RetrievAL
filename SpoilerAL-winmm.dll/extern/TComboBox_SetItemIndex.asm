.486
.model flat

public @TComboBox_SetItemIndex@8

.code

align 16

;EXTERN_C void __msfastcall TComboBox_SetItemIndex(const TComboBox *ComboBox, const int Index)
;{
;	ComboBox->ItemIndex = Index;
;}
@TComboBox_SetItemIndex@8 proc near

	mov     eax, ecx
	mov     ecx, dword ptr [ecx]
	jmp     dword ptr [ecx + 204]

@TComboBox_SetItemIndex@8 endp

end
