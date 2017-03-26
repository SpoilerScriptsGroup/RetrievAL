.486
.model flat

extrn @TrimString@4:proc

public _TSSGCtrl_GetSSGDataFile_TrimString

.code

align 16

_TSSGCtrl_GetSSGDataFile_TrimString proc near

	push    004166F0H
	jmp     @TrimString@4

_TSSGCtrl_GetSSGDataFile_TrimString endp

end
