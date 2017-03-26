.486
.model flat

public @TListBox_ItemAtPos@12

.code

align 16

;EXTERN_C int __msfastcall TListBox_ItemAtPos(TListBox *ListBox, TPoint &Pos, Boolean Existing)
;{
;	return ListBox->ItemAtPos(Pos, Existing);
;}
@TListBox_ItemAtPos@12 proc near

	mov     eax, dword ptr [esp]
	push    ecx
	mov     ecx, dword ptr [esp + 8]
	mov     dword ptr [esp + 4], 00545F58H
	mov     dword ptr [esp + 8], eax
	pop     eax
	ret

@TListBox_ItemAtPos@12 endp

end
