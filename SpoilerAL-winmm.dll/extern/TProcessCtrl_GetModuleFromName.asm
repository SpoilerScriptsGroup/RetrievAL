.486
.model flat, c

public TProcessCtrl_GetModuleFromName@8

.code

align 16

;LPMODULEENTRY32A __stdcall TProcessCtrl_GetModuleFromName(TProcessCtrl *processCtrl, LPCSTR Name)
;{
;	return processCtrl->GetModuleFromName(Name);
;}
TProcessCtrl_GetModuleFromName@8 proc near

	_Name       equ <esp + 8>

	mov     eax, esp
	mov     edx, dword ptr [_Name]
	sub     esp, 32
	sub     eax, 8
	mov     ecx, esp
	push    eax
	mov     eax, 004165E8H
	push    edx
	push    ecx
	call    eax

	processCtrl equ <esp + 48>

	mov     ecx, dword ptr [processCtrl]
	add     esp, 12
	mov     eax, 004A61F8H
	push    ecx
	call    eax
	add     esp, 36
	ret     8

TProcessCtrl_GetModuleFromName@8 endp

end
