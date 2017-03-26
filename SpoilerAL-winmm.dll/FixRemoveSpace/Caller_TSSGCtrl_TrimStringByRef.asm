.486
.model flat

public _Caller_TSSGCtrl_TrimStringByRef

extrn @TrimString@4:proc

.code

align 16

_Caller_TSSGCtrl_TrimStringByRef proc near

	lea     ecx, [ebp - 18H]
	call    @TrimString@4
	mov     eax, 004FFDA2H
	dec     dword ptr [ebx + 1CH]
	dec     dword ptr [ebx + 1CH]
	jmp     eax

_Caller_TSSGCtrl_TrimStringByRef endp

end

