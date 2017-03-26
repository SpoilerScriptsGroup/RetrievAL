.486
.model flat

extrn @bcb6_std_vector_string_clear@4:proc

public _TSSGCtrl_GetSSGDataFile_ClearAtBreak

.code

align 16

_TSSGCtrl_GetSSGDataFile_ClearAtBreak proc near

	tmpV equ <ebp - 30H>

	push    004EF423H
	lea     ecx, [tmpV]
	jmp     @bcb6_std_vector_string_clear@4

_TSSGCtrl_GetSSGDataFile_ClearAtBreak endp

end
