.486
.model flat

public @TMainForm_HotKeyEditKeyDown@16

.code

align 16

;EXTERN_C void __msfastcall TMainForm_HotKeyEditKeyDown(TMainForm *_this, TObject *Sender, WORD &Key, TShiftState Shift)
;{
;	_this->HotKeyEditKeyDown(Sender, Key, Shift);
;}
@TMainForm_HotKeyEditKeyDown@16 proc near

	mov     eax, dword ptr [esp]
	push    ecx
	mov     ecx, dword ptr [esp + 8]
	mov     dword ptr [esp + 4], 00443054H
	mov     dword ptr [esp + 8], eax
	pop     eax
	ret

@TMainForm_HotKeyEditKeyDown@16 endp

end
