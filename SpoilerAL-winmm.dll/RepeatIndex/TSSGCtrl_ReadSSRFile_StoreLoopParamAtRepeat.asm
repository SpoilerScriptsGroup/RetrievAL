.486
.model flat, c

public TSSGCtrl_ReadSSRFile_StoreLoopParamAtRepeat

.code

align 16

TSSGCtrl_ReadSSRFile_StoreLoopParamAtRepeat proc near

	FormatVec   equ <ebp - 30H>
	ElementSize equ <ebp + 14H>
	LoopBegin   equ <ebp + 18H>
	LoopStep    equ <ebp + 1CH>
	Begin       equ <ebp - 0F0H>
	_End        equ <ebp - 0F4H>
	Step        equ <ebp - 0F8H>

	mov     ecx, dword ptr [FormatVec    ]
	mov     eax, dword ptr [FormatVec + 4]
	sub     eax, ecx
	jns     L1
	add     eax, 15
L1:
	sar     eax, 4
	mov     ecx, dword ptr [ElementSize]
	mov     dword ptr [ecx], eax
	mov     eax, dword ptr [LoopBegin]
	mov     ecx, dword ptr [Begin]
	mov     dword ptr [eax], ecx
	mov     eax, dword ptr [LoopStep]
	mov     ecx, dword ptr [Step]
	mov     dword ptr [eax], ecx
	mov     eax, dword ptr [_End]
	ret

TSSGCtrl_ReadSSRFile_StoreLoopParamAtRepeat endp

end
