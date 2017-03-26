.486
.model flat

extrn @TSSGCtrl_EnumReadSSG_SetSubjectProperty@12:proc

public _Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty2

.code

align 16

_Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty2 proc near

	SSGS         equ <ebp - 0EF4H>
	RepeatIndex  equ <ebp + 18H>
	ParentRepeat equ <ebp + 1CH>

	mov     eax, dword ptr [ParentRepeat]
	mov     edx, dword ptr [RepeatIndex]
	mov     ecx, dword ptr [SSGS]
	push    eax
	push    004EB508H
	jmp     @TSSGCtrl_EnumReadSSG_SetSubjectProperty@12

_Caller_TSSGCtrl_EnumReadSSG_SetSubjectProperty2 endp

end
