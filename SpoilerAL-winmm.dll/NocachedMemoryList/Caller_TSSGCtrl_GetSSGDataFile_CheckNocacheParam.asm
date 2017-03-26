.486
.model flat

extrn @TSSGCtrl_GetSSGDataFile_CheckNocacheParam@4:proc

public _Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam

.code

align 16

_Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam proc near

	lea     ecx, [ebp - 30H]
	call    @TSSGCtrl_GetSSGDataFile_CheckNocacheParam@4
	mov     ecx, 00402590H
	mov     dword ptr [esp + 8], eax
	jmp     ecx

_Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam endp

end
