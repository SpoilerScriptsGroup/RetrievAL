.486
.model flat, c

extrn _bcb6_std_string_ctor_assign_cstr:dword
extrn F004A61F8:dword

public TProcessCtrl_GetModuleFromName@8

.code

align 16

;LPMODULEENTRY32A __stdcall TProcessCtrl_GetModuleFromName(TProcessCtrl *processCtrl, LPCSTR Name)
;{
;	return processCtrl->GetModuleFromName(Name);
;}
TProcessCtrl_GetModuleFromName@8 proc near

	processCtrl equ <esp + 4>
	_Name       equ <esp + 8>

	mov     eax, esp
	mov     edx, dword ptr [_Name]
	sub     esp, 32
	sub     eax, 8
	mov     ecx, esp
	push    eax
	push    edx
	push    ecx
	call    dword ptr [_bcb6_std_string_ctor_assign_cstr]
	mov     ecx, dword ptr [processCtrl + 44]
	add     esp, 12
	push    ecx
	call    dword ptr [F004A61F8]
	add     esp, 36
	ret     8

TProcessCtrl_GetModuleFromName@8 endp

end
