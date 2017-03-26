.486
.model flat

public @TMainMenu_GetHandle@4

.code

align 16

;EXTERN_C HMENU __msfastcall TMainMenu_GetHandle(const TMainMenu *MainMenu)
;{
;	return MainMenu->Handle;
;}
@TMainMenu_GetHandle@4 proc near

	mov     edx, dword ptr [ecx]
	mov     eax, ecx
	jmp     dword ptr [edx + 52]

@TMainMenu_GetHandle@4 endp

end
