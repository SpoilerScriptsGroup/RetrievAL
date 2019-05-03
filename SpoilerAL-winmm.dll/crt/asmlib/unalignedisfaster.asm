.386
.model flat

public UnalignedIsFaster

extern CpuType: near

.code

UnalignedIsFaster proc near
	push    ebx
	push    0                      ; vendor
	push    0                      ; family
	push    0                      ; model
	mov     eax, esp
	push    eax                    ; &model
	add     eax, 4
	push    eax                    ; &family
	add     eax, 4
	push    eax                    ; &vendor
	call    CpuType                ; get vendor, family, model
	add     esp, 12
	pop     edx                    ; model
	pop     ecx                    ; family
	pop     ebx                    ; vendor
	xor     eax, eax               ; return value
	dec     ebx
	jz      Intel
	dec     ebx
	jz      AMD
	dec     ebx
	jz      VIA
	; unknown vendor
	inc     eax
	jmp     Uend

Intel:
	; Unaligned read is faster on Intel Nehalem and later, but not Atom
	; Nehalem  = family 6, model 1AH
	; Atom     = family 6, model 1CH
	; Netburst = family 0FH
	; Future models are likely to be family 6, mayby > 6, model > 1C
	cmp     ecx, 6
	jb      Uend                   ; old Pentium 1, etc
	cmp     ecx, 0FH
	je      Uend                   ; old Netburst architecture
	cmp     edx, 1AH
	jb      Uend                   ; earlier than Nehalem
	cmp     edx, 1CH
	je      Uend                   ; Intel Atom
	or      eax, 2                 ; Intel Nehalem and later, except Atom
	jmp     Uend

AMD:
	; AMD processors:
	; The PALIGNR instruction is slow on AMD Bobcat but fast on Jaguar
	; K10/Opteron = family 10H     ; Use unaligned
	; Bobcat = family 14H          ; PALIGNR is very slow. Use unaligned
	; Piledriver = family 15H      ; Use unaligned
	; Jaguar = family 16H          ; PALIGNR is fast. Use aligned (aligned is faster in most cases, but not all)
	cmp     ecx, 10H               ; AMD K8 or earlier: use aligned
	jb      Uend
	cmp     ecx, 16H               ; Jaguar: use aligned
	je      Uend
	or      eax, 2                 ; AMD K10 or later: use unaligned
	jmp     Uend

VIA:
	; Unaligned read is not faster than PALIGNR on VIA Nano 2000 and 3000
	cmp     ecx, 0FH
	jna     Uend                   ; VIA Nano
	inc     eax                    ; Future versions: unknown
	;jmp     Uend

Uend:
	pop     ebx
	ret
UnalignedIsFaster endp

end
