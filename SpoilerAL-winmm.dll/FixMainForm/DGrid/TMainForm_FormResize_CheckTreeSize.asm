.486
.model flat, c

public TMainForm_FormResize_CheckTreeSize

.code

align 16

TMainForm_FormResize_CheckTreeSize proc near

	NextCallAddress        equ 00446E88H
	DGrid_SetWidth         equ 00580430H
	AllWidthCheck          equ <esp + 8>
	MainForm               equ ebx
	DGrid                  equ <ebx + 3A4H>
	Splitter               equ <ebx + 2FCH>
	offsetof_Splitter_Left equ 40H

	mov     dword ptr [AllWidthCheck], 1
	mov     ecx, DGrid_SetWidth
	mov     edx, dword ptr [Splitter]
	mov     eax, dword ptr [DGrid]
	mov     edx, dword ptr [edx + offsetof_Splitter_Left]
	push    NextCallAddress
	jmp     ecx

TMainForm_FormResize_CheckTreeSize endp

end
