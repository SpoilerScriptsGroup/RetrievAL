.486
.model flat, c

extrn TSSGCtrl_Open@16:proc

public Caller_TSSGCtrl_Open

.code

align 16

Caller_TSSGCtrl_Open proc near

	mov     edx, dword ptr [esp +  4]
	mov     ecx, dword ptr [esp +  8]
	mov     eax, dword ptr [esp + 12]
	and     edx, 0FFH
	mov     dword ptr [esp +  4], ecx
	mov     ecx, dword ptr [esp + 16]
	mov     edx, dword ptr [eax + edx]
	mov     dword ptr [esp +  8], eax
	mov     dword ptr [esp + 12], ecx
	mov     dword ptr [esp + 16], edx
	jmp     TSSGCtrl_Open@16

Caller_TSSGCtrl_Open endp

end
