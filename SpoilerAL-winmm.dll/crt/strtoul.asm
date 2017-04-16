.486
.model flat, c

EINVAL equ 22
ERANGE equ 34

extrn _errno:proc

public strtoul

.code

align 16

; unsigned long __cdecl strtoul(const char *nptr, char **endptr, int base)
strtoul proc near

	push    ebx                         ; store register
	push    esi

	nptr   equ <esp + 4 * (1 + 3)>
	endptr equ <esp + 4 * (2 + 3)>
	base   equ <esp + 4 * (3 + 3)>
	sign   equ <esp + 4 * (3 + 3)>
	errno  equ edi

	call    _errno
	push    edi                         ; store register
	mov     errno, eax
	mov     esi, dword ptr [nptr]       ; esi is our scanning pointer
	xor     eax, eax                    ; start with zero
	mov     cl, byte ptr [esi]          ; read char
	mov     dword ptr [errno], eax
	mov     ebx, dword ptr [base]
	jmp     L2

	align   16
L1:
	mov     cl, byte ptr [esi + 1]      ; skip whitespace
	inc     esi
L2:
	cmp     cl, ' '
	je      L1
	cmp     cl, 0DH
	ja      L3
	cmp     cl, 09H
	jae     L1
	jmp     L110

L3:
	mov     byte ptr [sign], cl         ; store sign char
	and     ecx, 0FFH
	cmp     cl, '-'                     ; skip sign
	je      L4
	cmp     cl, '+'
	jne     L5
L4:
	mov     cl, byte ptr [esi + 1]
	inc     esi

L5:
	cmp     ebx, 1
	jae     L6
	cmp     cl, '0'                     ; determine base free-lance, based on first two chars of string
	jne     L10
	mov     cl, byte ptr [esi + 1]
	inc     esi
	cmp     cl, 'x'
	je      L31
	cmp     cl, 'X'
	je      L31
	jmp     L63

L6:
	je      L7
	cmp     ebx, 10
	je      L10
	cmp     ebx, 16
	je      L30
	cmp     ebx, 8
	je      L60
	cmp     ebx, 10 + 'Z' - 'A' + 1
	jbe     L80
L7:
	mov     dword ptr [errno], EINVAL   ; bad base!
	jmp     L110

	align   16
L10:
	sub     cl, '0'                     ; base == 10
	jl      L11
	cmp     cl, '9' - '0'
	jbe     L13
L11:
	jmp     L110                        ; no number there; return 0 and point to beginning of string
L12:
	jmp     L112

	align   16
L13:
	lea     eax, [eax + eax * 4]
	inc     esi
	lea     eax, [ecx + eax * 2]
	mov     cl, byte ptr [esi]          ; read next char
	sub     cl, '0'                     ; check and convert char to value
	jl      L12
	cmp     cl, '9' - '0'
	ja      L12
	cmp     eax, 19999999H
	jb      L13
	jne     L20
	cmp     cl, 5
	jbe     L13

L20:
	mov     edi, dword ptr [endptr]     ; overflow there
	mov     eax, 0FFFFFFFFH
	test    edi, edi
	mov     dword ptr [errno], ERANGE
	jz      L114

	align   16
L21:
	mov     cl, byte ptr [esi + 1]      ; point to end of string
	inc     esi
	cmp     cl, '0'
	jl      L22
	cmp     cl, '9'
	jbe     L21
L22:
	jmp     L111

	align   16
L30:
	sub     cl, '0'                     ; base == 16
	jne     L32
	mov     cl, byte ptr [esi + 1]
	inc     esi
	cmp     cl, 'x'
	je      L31
	cmp     cl, 'X'
	jne     L37

	align   16
L31:
	mov     cl, byte ptr [esi + 1]
	inc     esi
	sub     cl, '0'
L32:
	jl      L34
	cmp     cl, '9' - '0'
	jbe     L36
	sub     cl, 'A' - '0'
	jb      L34
	cmp     cl, 'F' - 'A'
	jbe     L33
	sub     cl, 'a' - 'A'
	jb      L34
	cmp     cl, 'f' - 'a'
	ja      L34
L33:
	add     cl, 10
	jmp     L36
L34:
	jmp     L110                        ; no number there; return 0 and point to beginning of string
L35:
	jmp     L112

	align   16
L36:
	shl     eax, 4
	inc     esi
	or      eax, ecx
	mov     cl, byte ptr [esi]          ; read next char
L37:
	sub     cl, '0'                     ; check and convert char to value
	jl      L35
	cmp     cl, '9' - '0'
	jbe     L39
	sub     cl, 'A' - '0'
	jb      L35
	cmp     cl, 'F' - 'A'
	jbe     L38
	sub     cl, 'a' - 'A'
	jb      L35
	cmp     cl, 'f' - 'a'
	ja      L35
L38:
	add     cl, 10
L39:
	test    eax, 0F0000000H
	jz      L36

L50:
	mov     edi, dword ptr [endptr]     ; overflow there
	mov     eax, 0FFFFFFFFH
	test    edi, edi
	mov     dword ptr [errno], ERANGE
	jz      L114

	align   16
L51:
	mov     cl, byte ptr [esi + 1]      ; point to end of string
	inc     esi
	cmp     cl, '0'
	jl      L52
	cmp     cl, '9'
	jbe     L51
	cmp     cl, 'A'
	jb      L52
	cmp     cl, 'F'
	jbe     L51
	cmp     cl, 'a'
	jb      L52
	cmp     cl, 'f'
	jbe     L51
L52:
	jmp     L111

	align   16
L60:
	sub     cl, '0'                     ; base == 8
	jl      L61
	cmp     cl, '7' - '0'
	jbe     L62
L61:
	jmp     L110                        ; no number there; return 0 and point to beginning of string

	align   16
L62:
	shl     eax, 3
	inc     esi
	or      eax, ecx
	mov     cl, byte ptr [esi]          ; read next char
L63:
	sub     cl, '0'                     ; check and convert char to value
	jl      L73
	cmp     cl, '7' - '0'
	ja      L73
	test    eax, 0E0000000H
	jz      L62

L70:
	mov     edi, dword ptr [endptr]     ; overflow there
	mov     eax, 0FFFFFFFFH
	test    edi, edi
	mov     dword ptr [errno], ERANGE
	jz      L114

	align   16
L71:
	mov     cl, byte ptr [esi + 1]      ; point to end of string
	inc     esi
	cmp     cl, '0'
	jl      L72
	cmp     cl, '7'
	jbe     L71
L72:
	jmp     L111
L73:
	jmp     L112

	align   16
L80:
	sub     cl, '0'                     ; base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
	jl      L83
	cmp     cl, '9' - '0'
	jbe     L82
	sub     cl, 'A' - '0'
	jb      L83
	cmp     cl, 'Z' - 'A'
	jbe     L81
	sub     cl, 'a' - 'A'
	jb      L83
	cmp     cl, 'z' - 'a'
	ja      L83
L81:
	add     cl, 10
L82:
	cmp     cl, bl
	jb      L84
L83:
	jmp     L110                        ; no number there; return 0 and point to beginning of string

	align   16
L84:
	mul     ebx
	jc      L100
	add     eax, ecx
	jc      L100
	mov     cl, byte ptr [esi + 1]      ; read next char
	inc     esi
	sub     cl, '0'                     ; check and convert char to value
	jl      L90
	cmp     cl, '9' - '0'
	jbe     L86
	sub     cl, 'A' - '0'
	jb      L90
	cmp     cl, 'Z' - 'A'
	jbe     L85
	sub     cl, 'a' - 'A'
	jb      L90
	cmp     cl, 'z' - 'a'
	ja      L90
L85:
	add     cl, 10
L86:
	cmp     cl, bl
	jb      L84
L90:
	jmp     L112

L100:
	mov     edi, dword ptr [endptr]     ; overflow there
	mov     eax, 0FFFFFFFFH
	test    edi, edi
	mov     dword ptr [errno], ERANGE
	jz      L114

	align   16
L101:
	mov     cl, byte ptr [esi + 1]      ; point to end of string
	inc     esi
	sub     cl, '0'
	jl      L111
	cmp     cl, '9' - '0'
	jbe     L103
	sub     cl, 'A' - '0'
	jb      L111
	cmp     cl, 'Z' - 'A'
	jbe     L102
	sub     cl, 'a' - 'A'
	jb      L111
	cmp     cl, 'z' - 'a'
	ja      L111
L102:
	add     cl, 10
L103:
	cmp     cl, bl
	jb      L101
	jmp     L111

	align   16
L110:
	mov     edx, dword ptr [endptr]     ; store beginning of string in endptr
	mov     esi, dword ptr [nptr]       ; return 0
	test    edx, edx
	jz      L114
L111:
	mov     dword ptr [edx], esi
	jmp     L114
L112:
	mov     edx, dword ptr [endptr]
	test    edx, edx
	jz      L113
	mov     dword ptr [edx], esi        ; store pointer to char that stopped the scan
L113:
	cmp     byte ptr [sign], '-'
	jne     L114
	neg     eax                         ; negate result if there was a neg sign
L114:
	pop     edi                         ; restore register
	pop     esi
	pop     ebx
	ret

strtoul endp

end
