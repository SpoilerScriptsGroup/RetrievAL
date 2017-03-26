.486
.model flat, c

extrn TMainForm_HotKeyEditKeyDown_Up@4:proc

public Caller_TMainForm_HotKeyEditKeyDown_Up

.code

align 16

Caller_TMainForm_HotKeyEditKeyDown_Up proc near

	ReturnAddress equ 00443484H
	_this         equ ebx

	push    _this
	push    ReturnAddress
	jmp     TMainForm_HotKeyEditKeyDown_Up@4

Caller_TMainForm_HotKeyEditKeyDown_Up endp

end
