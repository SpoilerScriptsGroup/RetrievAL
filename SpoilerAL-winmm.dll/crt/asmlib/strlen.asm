.686
.xmm
.model flat

include align.inc

public _strlen

extern InstructionSet: near

.data

strlenDispatch dd strlenCPUDispatch

.code

$align 16
_strlen proc near
	jmp     dword ptr [strlenDispatch]                  ; Go to appropriate version, depending on instruction set
_strlen endp

; SSE2 version
$align 16
strlenSSE2 proc near
	mov      eax,  dword ptr [esp + 4]                  ; get pointer to string
	mov      ecx,  eax                                  ; copy pointer
	pxor     xmm0, xmm0                                 ; set to zero
	and      ecx,  0FH                                  ; lower 4 bits indicate misalignment
	and      eax,  -10H                                 ; align pointer by 16
	movdqa   xmm1, xmmword ptr [eax]                    ; read from nearest preceding boundary
	pcmpeqb  xmm1, xmm0                                 ; compare 16 bytes with zero
	pmovmskb edx,  xmm1                                 ; get one bit for each byte result
	shr      edx,  cl                                   ; shift out false bits
	shl      edx,  cl                                   ; shift back again
	bsf      edx,  edx                                  ; find first 1-bit
	jnz      A200                                       ; found

	; Main loop, search 16 bytes at a time
A100:
	add      eax,  10H                                  ; increment pointer by 16
	movdqa   xmm1, xmmword ptr [eax]                    ; read 16 bytes aligned
	pcmpeqb  xmm1, xmm0                                 ; compare 16 bytes with zero
	pmovmskb edx,  xmm1                                 ; get one bit for each byte result
	bsf      edx,  edx                                  ; find first 1-bit
	; (moving the bsf out of the loop and using test here would be faster for long strings on old processors,
	;  but we are assuming that most strings are short, and newer processors have higher priority)
	jz       A100                                       ; loop if not found

A200:
	; Zero-byte found. Compute string length
	sub      eax,  dword ptr [esp + 4]                  ; subtract start address
	add      eax,  edx                                  ; add byte index
	ret
strlenSSE2 endp

; 80386 version
$align 16
strlen386 proc near
	push    ebx
	mov     ecx, dword ptr [esp + 8]                    ; get pointer to string
	mov     eax, ecx                                    ; copy pointer
	and     ecx, 3                                      ; lower 2 bits of address, check alignment
	jz      L2                                          ; string is aligned by 4. Go to loop
	and     eax, -4                                     ; align pointer by 4
	mov     ebx, dword ptr [eax]                        ; read from nearest preceding boundary
	shl     ecx, 3                                      ; mul by 8 = displacement in bits
	mov     edx, -1
	shl     edx, cl                                     ; make byte mask
	not     edx                                         ; mask = 0FFH for false bytes
	or      ebx, edx                                    ; mask out false bytes

	; check first four bytes for zero
	lea     ecx, [ebx - 01010101H]                      ; subtract 1 from each byte
	not     ebx                                         ; invert all bytes
	and     ecx, ebx                                    ; and these two
	and     ecx, 80808080H                              ; test all sign bits
	jnz     L3                                          ; zero-byte found

	; Main loop, read 4 bytes aligned
L1:
	add     eax, 4                                      ; increment pointer by 4
L2:
	mov     ebx, dword ptr [eax]                        ; read 4 bytes of string
	lea     ecx, [ebx - 01010101H]                      ; subtract 1 from each byte
	not     ebx                                         ; invert all bytes
	and     ecx, ebx                                    ; and these two
	and     ecx, 80808080H                              ; test all sign bits
	jz      L1                                          ; no zero bytes, continue loop

L3:
	bsf     ecx, ecx                                    ; find right-most 1-bit
	shr     ecx, 3                                      ; divide by 8 = byte index
	sub     eax, dword ptr [esp + 8]                    ; subtract start address
	add     eax, ecx                                    ; add index to byte
	pop     ebx
	ret
strlen386 endp

; CPU dispatching for strlen. This is executed only once
$align 16
strlenCPUDispatch proc near
	pushad
	call    InstructionSet
	; Point to generic version of strlen
	mov     dword ptr [strlenDispatch], offset strlen386
	cmp     eax, 4                                      ; check SSE2
	jb      M100
	; SSE2 supported
	; Point to SSE2 version of strlen
	mov     dword ptr [strlenDispatch], offset strlenSSE2

M100:
	popad
	; Continue in appropriate version of strlen
	jmp     dword ptr [strlenDispatch]
strlenCPUDispatch endp

end
