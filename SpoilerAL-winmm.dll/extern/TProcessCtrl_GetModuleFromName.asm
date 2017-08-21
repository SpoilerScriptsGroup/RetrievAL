.486
.model flat

extrn @bcb6_std_string_ctor_assign_cstr@8:proc
extrn _F004A61F8:dword

public @TProcessCtrl_GetModuleFromName@8

.code

align 16

;LPMODULEENTRY32A __msfastcall TProcessCtrl_GetModuleFromName(TProcessCtrl *processCtrl, LPCSTR Name)
;{
;	return processCtrl->GetModuleFromName(Name);
;}
@TProcessCtrl_GetModuleFromName@8 proc near

	sizeof_string equ 24

	sub     esp, sizeof_string
	mov     eax, ecx
	mov     ecx, esp
	push    eax
	call    @bcb6_std_string_ctor_assign_cstr@8
	call    dword ptr [_F004A61F8]
	add     esp, 4 + sizeof_string
	ret

@TProcessCtrl_GetModuleFromName@8 endp

end
