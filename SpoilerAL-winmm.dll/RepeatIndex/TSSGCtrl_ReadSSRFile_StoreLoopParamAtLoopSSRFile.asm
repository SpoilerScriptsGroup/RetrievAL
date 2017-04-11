.486
.model flat, c

public TSSGCtrl_ReadSSRFile_StoreLoopParamAtLoopSSRFile

.code

align 16

TSSGCtrl_ReadSSRFile_StoreLoopParamAtLoopSSRFile proc near

	ElementSize equ <ebp + 14H>
	LoopBegin   equ <ebp + 18H>
	LoopStep    equ <ebp + 1CH>

	sar     edx, 4
	push    eax
	mov     eax, dword ptr [ElementSize]
	mov     ecx, dword ptr [LoopBegin]
	mov     dword ptr [eax], edx
	mov     eax, dword ptr [LoopStep]
	mov     dword ptr [ecx], 0
	mov     dword ptr [eax], 1
	pop     eax
	imul    edx
	ret

TSSGCtrl_ReadSSRFile_StoreLoopParamAtLoopSSRFile endp

end
