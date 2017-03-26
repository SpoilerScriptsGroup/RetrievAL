.486
.model flat, c

public Caller_AdditionalTags

extrn AdditionalTags@16:proc

.code

align 16

Caller_AdditionalTags proc near

	ReturnAddress equ 004EB508H
	SSGCtrl       equ <ebp +  8H>
	ParentStack   equ <ebp + 10H>
	tmpS          equ <ebp - 50H>
	LineS         equ <ebp - 38H>

	lea     eax, [LineS]
	push    eax
	lea     ecx, [tmpS]
	push    ecx
	mov     eax, dword ptr [ParentStack]
	mov     ecx, dword ptr [eax + 16]
	mov     edx, dword ptr [eax + 20]
	cmp     ecx, edx
	jne     L1
	mov     ecx, dword ptr [eax + 28]
	mov     ecx, dword ptr [ecx - 4]
	add     ecx, 4
L1:
	mov     eax, dword ptr [ecx - 4]
	mov     ecx, dword ptr [SSGCtrl]
	push    eax
	push    ecx
	push    ReturnAddress
	jmp     AdditionalTags@16

Caller_AdditionalTags endp

end
