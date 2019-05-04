.686
.xmm
.model flat

include align.inc

public _strcspn

extern InstructionSet: near

.data

strcspnDispatch dd strcspnCPUDispatch

.code

; function dispatching
_strcspn proc near
	jmp     dword ptr [strcspnDispatch] ; Go to appropriate version, depending on instruction set
	$align  16
_strcspn endp

; SSE4.2 version
strcspnSSE42 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]   ; str
	mov     edi, dword ptr [esp + 16]   ; set
	xor     ecx, ecx                    ; span counter

str_next2:
	movdqu  xmm2, xmmword ptr [esi]     ; str
	movdqu  xmm1, xmmword ptr [edi]     ; set
	pcmpistrm xmm1, xmm2, 00110000b     ; find in set, invert valid bits, return bit mask in xmm0
	movd    eax, xmm0
	jns     set_extends2

set_finished2:
	cmp     ax, -1
	jne     str_finished2
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add     esi, 16                     ; next 16 bytes of str
	add     ecx, 16                     ; count span
	jmp     str_next2

str_finished2:
	not     eax
	bsf     eax, eax
	add     eax, ecx
	pop     edi
	pop     esi
	ret

set_loop2:
	and     eax, edx                    ; accumulate matches

set_extends2:
	; the set is more than 16 bytes
	add     edi, 16
	movdqu  xmm1, xmmword ptr [edi]     ; next part of set
	pcmpistrm xmm1, xmm2, 00110000b     ; find in set, invert valid bits, return bit mask in xmm0
	movd    edx, xmm0
	jns     set_loop2
	mov     edi, dword ptr [esp + 16]   ; restore set pointer
	and     eax, edx                    ; accumulate matches
	jmp     set_finished2
	$align  16
strcspnSSE42 endp

; Generic version
strcspnGeneric proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]   ; str pointer

str_next20:
	mov     edi, dword ptr [esp + 16]   ; set pointer
	mov     al, byte ptr [esi]          ; read one byte from str
	test    al, al
	jz      str_finished20              ; str finished

set_next20:
	mov     dl, byte ptr [edi]
	test    dl, dl
	jz      set_finished20
	inc     edi
	cmp     al, dl
	jne     set_next20
	; character match found, stop search
	jmp     str_finished20

set_finished20:
	; end of set, mismatch found
	inc     esi
	jmp     str_next20

str_finished20:
	; end of str, all match
	sub     esi, dword ptr [esp + 12]   ; calculate position
	mov     eax, esi
	pop     edi
	pop     esi
	ret
strcspnGeneric endp

strcspnCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strcspnGeneric
	cmp     eax, 10                     ; check SSE4.2
	jb      Q200
	; SSE4.2 supported
	; Point to SSE4.2 version of strstr
	mov     ecx, offset strcspnSSE42
Q200:
	mov     dword ptr [strcspnDispatch], ecx
	; Continue in appropriate version 
	jmp     ecx
	$align  16
strcspnCPUDispatch endp

end
