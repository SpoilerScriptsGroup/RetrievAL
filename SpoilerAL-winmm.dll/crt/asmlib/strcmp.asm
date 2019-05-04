.686
.xmm
.model flat

include align.inc

public _strcmp

extern InstructionSet: near

.data

strcmpDispatch dd strcmpCPUDispatch

.code

; strcmp function
_strcmp proc near
	jmp     dword ptr [strcmpDispatch]                  ; Go to appropriate version, depending on instruction set
	$align  16
_strcmp endp

strcmpSSE42 proc near
	mov     eax, dword ptr [esp + 4]                    ; string 1
	mov     edx, dword ptr [esp + 8]                    ; string 2
	push    ebx
	mov     ebx, -16                                    ; offset counter

compareloop:
	add     ebx, 16                                     ; increment offset
	movdqu  xmm1, xmmword ptr [eax + ebx]               ; read 16 bytes of string 1
	pcmpistri xmm1, xmmword ptr [edx + ebx], 00011000B  ; unsigned bytes, equal each, invert. returns index in ecx
	jnbe    compareloop                                 ; jump if not carry flag and not zero flag
	jnc     equal

notequal:
	; strings are not equal
	add     ecx, ebx                                    ; offset to first differing byte
	movzx   eax, byte ptr [eax + ecx]                   ; compare bytes
	movzx   edx, byte ptr [edx + ecx]
	sub     eax, edx
	pop     ebx
	ret

equal:
	xor     eax, eax                                    ; strings are equal
	pop     ebx
	ret
strcmpSSE42 endp

; generic version
strcmpGeneric proc near
	; This is a very simple solution. There is not much gained by using SSE2 or anything complicated
	mov     ecx, dword ptr [esp + 4]                    ; string 1
	mov     edx, dword ptr [esp + 8]                    ; string 2

compareloop:
	mov     al, byte ptr [ecx]
	cmp     al, byte ptr [edx]
	jne     notequal
	test    al, al
	jz      equal
	inc     ecx
	inc     edx
	jmp     compareloop

equal:
	xor     eax, eax                                    ; strings are equal
	ret

notequal:
	; strings are not equal
	movzx   eax, byte ptr [ecx]                         ; compare first differing byte
	movzx   edx, byte ptr [edx]
	sub     eax, edx
	ret
strcmpGeneric endp

; CPU dispatching for strcmp. This is executed only once
strcmpCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strcmp
	mov     ecx, offset strcmpGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q100
	; SSE4.2 supported
	; Point to SSE4.2 version of strcmp
	mov     ecx, offset strcmpSSE42

Q100:
	mov     dword ptr [strcmpDispatch], ecx
	; Continue in appropriate version of strcmp
	jmp     ecx
	$align  16
strcmpCPUDispatch endp

end
