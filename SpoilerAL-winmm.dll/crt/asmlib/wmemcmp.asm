.686
.xmm
.model flat

public _wmemcmp

extern _InstructionSet: near

.data

wmemcmpDispatch dd wmemcmpCPUDispatch

.code

; Function entry:
align 16
_wmemcmp proc near
	jmp     dword ptr [wmemcmpDispatch]                 ; Go to appropriate version, depending on instruction set
_wmemcmp endp

; AVX512BW Version. Use zmm register
align 16
wmemcmpAVX512BW proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     ecx, dword ptr [esp + 20]                   ; count
	mov     edx, -2                                     ; maximum bytes
	xor     eax, eax
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes
	cmp     ecx, 80H
	ja      L010
	cmp     ecx, 40H
	ja      L800
	test    ecx, ecx
	jnz     L820
	pop     edi
	pop     esi
	ret

	align   16
	; count >= 80H
L010 label near
	; entry from wmemcmpAVX512F
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [edi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 40H bytes for dwords not equal
	kortestw k1, k1
	jnz     L500                                        ; difference found

	; find 40H boundaries
	mov     eax, esi
	inc     esi
	and     eax, 1
	and     esi, -2
	lea     edx, dword ptr [esi + ecx]                  ; end of string 1
	mov     ecx, esi
	add     esi, 40H
	and     esi, -40H                                   ; first aligned boundary for esi
	sub     ecx, esi                                    ; -offset
	sub     edi, ecx                                    ; same offset to edi
	mov     ecx, edx
	and     edx, -40H                                   ; last aligned boundary for esi
	sub     ecx, eax
	sub     esi, edx                                    ; esi = -size of aligned blocks
	sub     edx, eax
	sub     edi, esi

	align   16
L100:
	; main loop
	vmovdqu64 zmm0, zmmword ptr [edx + esi]
	vmovdqu64 zmm1, zmmword ptr [edi + esi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 40H bytes for not equal
	kortestw k1, k1
	jnz     L500                                        ; difference found
	add     esi, 40H
	jnz     L100

	; remaining 0-3FH bytes. Overlap with previous block
	add     edi, ecx
	sub     edi, edx
	vmovdqu64 zmm0, zmmword ptr [ecx - 40H]
	vmovdqu64 zmm1, zmmword ptr [edi - 40H]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 40H bytes for not equal
	kortestw k1, k1
	jnz     L500                                        ; difference found

	; finished. no difference found
	xor     eax, eax
	vzeroupper
	pop     edi
	pop     esi
	ret

	align   16
L500:
	; the two strings are different
	vpcompressd zmm0{k1}{z}, zmm0                       ; get first differing dword to position 0
	vpcompressd zmm1{k1}{z}, zmm1                       ; get first differing dword to position 0
	vmovd   ecx, xmm0
	vmovd   edx, xmm1
	mov     eax, ecx
	xor     ecx, edx                                    ; difference
	bsf     ecx, ecx                                    ; position of lowest differing bit
	and     ecx, -16                                    ; round down to word boundary
	shr     eax, cl                                     ; first differing word in ax
	shr     edx, cl                                     ; first differing word in dx
	and     eax, 0FFFFH                                 ; zero-extend bytes
	and     edx, 0FFFFH
	sub     eax, edx                                    ; signed difference between unsigned words
	vzeroupper
	pop     edi
	pop     esi
	ret

	align   16
L800:
	; size = 41H - 80H
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [edi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 40H bytes for not equal
	kortestw k1, k1
	jnz     L500                                        ; difference found
	add     esi, 40H
	add     edi, 40H
	sub     ecx, 40H

L820:
	; size = 00H - 40H
	; (this is the only part that requires AVX512BW)
	or      eax, -1                                     ; if count = 1-31: |  if count = 32-63:
	bzhi    eax, eax, ecx                               ; -----------------|--------------------
	kmovd   k1, eax                                     ;       count 1's  |  all 1's
	xor     eax, eax                                    ;                  |
	sub     ecx, 32                                     ;                  |
	cmovb   ecx, eax                                    ;               0  |  count-32
	dec     eax                                         ;                  |
	bzhi    eax, eax, ecx                               ;                  |
	kmovd   k2, eax                                     ;               0  |  count-32 1's
	kunpckdq k3, k2, k1                                 ; low 32 bits from k1, high 32 bits from k2. total = count 1's

	vmovdqu8 zmm0{k3}{z}, zmmword ptr [esi]
	vmovdqu8 zmm1{k3}{z}, zmmword ptr [edi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare
	kortestw k1, k1
	jnz     L500                                        ; difference found
	xor     eax, eax                                    ; no difference found
	vzeroupper
	pop     edi
	pop     esi
	ret
wmemcmpAVX512BW endp

; AVX512F Version. Use zmm register
align 16
wmemcmpAVX512F proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     ecx, dword ptr [esp + 20]                   ; count
	mov     edx, -2                                     ; maximum bytes
	xor     eax, eax
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes
	cmp     ecx, 80H                                    ; size
	jae     L010                                        ; continue in wmemcmpAVX512BW
	jmp     A001                                        ; continue in wmemcmpAVX2 if less than 80H bytes
wmemcmpAVX512F endp

; AVX2 Version. Use ymm register
align 16
wmemcmpAVX2 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     ecx, dword ptr [esp + 20]                   ; count
	mov     edx, -2                                     ; maximum bytes
	xor     eax, eax
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes

A001 label near
	; entry from above
	add     esi, ecx                                    ; use negative index from end of memory block
	add     edi, ecx
	xor     ecx, -1
	mov     edx, 0FFFFH
	inc     ecx
	jnz     A110
	jmp     A530

	align   16
A100:
	; loop comparing 32 bytes
	vmovdqu   ymm1, ymmword ptr [esi + ecx]
	vpcmpeqw  ymm0, ymm1, ymmword ptr [edi + ecx]       ; compare 32 bytes
	vpmovmskb eax, ymm0                                 ; get byte mask
	xor     eax, -1                                     ; not eax would not set flags
	jnz     A500                                        ; difference found
	add     ecx, 32
	jz      A300                                        ; finished, equal

A110:
	cmp     ecx, -32
	jna     A100                                        ; next 32 bytes

	vzeroupper                                          ; end ymm state
	; less than 32 bytes left
	cmp     ecx, -16
	ja      A200
	movdqu  xmm1, xmmword ptr [esi + ecx]
	movdqu  xmm2, xmmword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 16 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A510                                        ; difference found
	add     ecx, 16
	jz      A530                                        ; finished, equal

A200:
	; less than 16 bytes left
	cmp     ecx, -8
	ja      A210
	; compare 8 bytes
	movq    xmm1, qword ptr [esi + ecx]
	movq    xmm2, qword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 8 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A510                                        ; difference found
	add     ecx, 8
	jz      A530

A210:
	; less than 8 bytes left
	cmp     ecx, -4
	ja      A520
	; compare 4 bytes
	movd    xmm1, dword ptr [esi + ecx]
	movd    xmm2, dword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 4 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A510                                        ; difference found
	add     ecx, 4
	jnz     A520                                        ; less than 4 bytes left
	jmp     A530

	align   16
A300:
	; equal
	vzeroupper
	jmp     A530

	align   16
A500:
	; difference found. find position
	vzeroupper

A510:
	bsf     eax, eax
	add     ecx, eax

A520:
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result

A530:
	pop     edi
	pop     esi
	ret
wmemcmpAVX2 endp

; SSE2 version. Use xmm register
align 16
wmemcmpSSE2 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     ecx, dword ptr [esp + 20]                   ; count
	mov     edx, -2                                     ; maximum bytes
	xor     eax, eax
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes
	add     esi, ecx                                    ; use negative index from end of memory block
	add     edi, ecx
	xor     ecx, -1
	mov     edx, 0FFFFH
	inc     ecx
	jnz     S110
	jmp     S530

	align   16
S100:
	; loop comparing 16 bytes
	movdqu  xmm1, xmmword ptr [esi + ecx]
	movdqu  xmm2, xmmword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 16 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S510                                        ; difference found
	add     ecx, 16
	jz      S530                                        ; finished, equal

S110:
	cmp     ecx, -16
	jna     S100                                        ; next 16 bytes

	; less than 16 bytes left
	cmp     ecx, -8
	ja      S210
	; compare 8 bytes
	movq    xmm1, qword ptr [esi + ecx]
	movq    xmm2, qword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 8 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S510                                        ; difference found
	add     ecx, 8
	jz      S530

S210:
	; less than 8 bytes left
	cmp     ecx, -4
	ja      S520
	; compare 4 bytes
	movd    xmm1, dword ptr [esi + ecx]
	movd    xmm2, dword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 4 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S510                                        ; difference found
	add     ecx, 4
	jz      S530
	jmp     S520                                        ; less than 4 bytes left

	align   16
S510:
	; difference found. find position
	bsf     eax, eax
	add     ecx, eax

S520:
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result

S530:
	pop     edi
	pop     esi
	ret
wmemcmpSSE2 endp

; Generic version version. Use 32 bit registers
align 16
wmemcmp386 proc near
	push    esi
	push    edi
	mov     ecx, dword ptr [esp + 20]                   ; ecx = count
	mov     edi, dword ptr [esp + 16]                   ; edi = buffer2
	mov     esi, dword ptr [esp + 12]                   ; esi = buffer1
	lea     edi, [edi + ecx * 2]                        ; edi = end of buffer2
	lea     esi, [esi + ecx * 2]                        ; esi = end of buffer1
	xor     ecx, -1                                     ; ecx = -count - 1

dword_loop:
	add     ecx, 2
	jc      compare_words
	mov     eax, dword ptr [esi + ecx * 2 - 2]
	mov     edx, dword ptr [edi + ecx * 2 - 2]
	cmp     eax, edx
	je      dword_loop

compare_words:
	xor     eax, eax
	xor     edx, edx
	dec     ecx
	jz      epilogue

word_loop:
	mov     ax, word ptr [esi + ecx * 2]
	mov     dx, word ptr [edi + ecx * 2]
	sub     eax, edx
	jnz     epilogue
	inc     ecx
	jnz     word_loop

epilogue:
	pop     edi
	pop     esi
	ret
wmemcmp386 endp

; CPU dispatching for wmemcmp. This is executed only once
align 16
wmemcmpCPUDispatch proc near
	call    _InstructionSet                             ; get supported instruction set
	; Point to generic version of wmemcmp
	mov     dword ptr [wmemcmpDispatch], offset wmemcmp386
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	mov     dword ptr [wmemcmpDispatch], offset wmemcmpSSE2
	cmp     eax, 13                                     ; check AVX2
	jb      Q100
	; AVX2 supported
	mov     dword ptr [wmemcmpDispatch], offset wmemcmpAVX2
	cmp     eax, 15                                     ; check AVX512F
	jb      Q100
	; AVX512F supported
	mov     dword ptr [wmemcmpDispatch], offset wmemcmpAVX512F
	cmp     eax, 16                                     ; check AVX512BW
	jb      Q100
	; AVX512BW supported
	mov     dword ptr [wmemcmpDispatch], offset wmemcmpAVX512BW

Q100:
	; Continue in appropriate version of wmemcmp
	jmp     dword ptr [wmemcmpDispatch]
wmemcmpCPUDispatch endp

end
