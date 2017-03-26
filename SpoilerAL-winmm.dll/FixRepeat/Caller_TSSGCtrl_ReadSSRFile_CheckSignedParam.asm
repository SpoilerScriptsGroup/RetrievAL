.486
.model flat

extrn @TSSGCtrl_ReadSSRFile_CheckSignedParam@4:proc

public _Caller_TSSGCtrl_ReadSSRFile_CheckSignedParam

.code

align 16

_Caller_TSSGCtrl_ReadSSRFile_CheckSignedParam proc near

	tmpV equ esi

	mov     ecx, tmpV
	call    @TSSGCtrl_ReadSSRFile_CheckSignedParam@4
	push    eax
	mov     eax, 004FEBD0H
	mov     word ptr [ebx + 10H], 104
	jmp     eax

_Caller_TSSGCtrl_ReadSSRFile_CheckSignedParam endp

end
