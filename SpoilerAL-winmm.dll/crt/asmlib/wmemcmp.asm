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
	xor     eax, eax
	or      edx, -1                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes
	cmp     ecx, 128
	ja      wmemcmpAVX512BW_entry
	cmp     ecx, 64
	ja      less_than_128_bytes_left
	test    ecx, ecx
	jnz     less_than_64_bytes_left
	pop     edi
	pop     esi
	ret

	align   16
	; count >= 128
wmemcmpAVX512BW_entry label near
	; entry from wmemcmpAVX512F
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [edi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 64 bytes for dwords not equal
	kortestw k1, k1
	jnz     difference_zmmword                          ; difference found

	; find 64 boundaries
	mov     eax, esi
	inc     esi
	and     eax, 1
	and     esi, -2
	lea     edx, dword ptr [esi + ecx]                  ; end of string 1
	mov     ecx, esi
	add     esi, 64
	and     esi, -64                                    ; first aligned boundary for esi
	sub     ecx, esi                                    ; -offset
	sub     edi, ecx                                    ; same offset to edi
	mov     ecx, edx
	and     edx, -64                                    ; last aligned boundary for esi
	sub     ecx, eax
	sub     esi, edx                                    ; esi = -size of aligned blocks
	sub     edx, eax
	sub     edi, esi

	align   16
loop_begin:
	; main loop
	vmovdqu64 zmm0, zmmword ptr [edx + esi]
	vmovdqu64 zmm1, zmmword ptr [edi + esi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 64 bytes for not equal
	kortestw k1, k1
	jnz     difference_zmmword                          ; difference found
	add     esi, 64
	jnz     loop_begin

	; remaining 0-3FH bytes. Overlap with previous block
	add     edi, ecx
	sub     edi, edx
	vmovdqu64 zmm0, zmmword ptr [ecx - 64]
	vmovdqu64 zmm1, zmmword ptr [edi - 64]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 64 bytes for not equal
	kortestw k1, k1
	jnz     difference_zmmword                          ; difference found

	; finished. no difference found
	xor     eax, eax
	vzeroupper
	pop     edi
	pop     esi
	ret

	align   16
difference_zmmword:
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
	and     eax, 0FFFFH                                 ; zero-extend words
	and     edx, 0FFFFH
	sub     eax, edx                                    ; signed difference between unsigned words
	vzeroupper
	pop     edi
	pop     esi
	ret

	align   16
less_than_128_bytes_left:
	; size = 65 - 128
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [edi]
	vpcmpd  k1, zmm0, zmm1, 4                           ; compare first 64 bytes for not equal
	kortestw k1, k1
	jnz     difference_zmmword                          ; difference found
	add     esi, 64
	add     edi, 64
	sub     ecx, 64

less_than_64_bytes_left:
	; size = 1 - 64
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
	jnz     difference_zmmword                          ; difference found
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
	xor     eax, eax
	cmp     ecx, 64                                     ; size
	jb      wmemcmpAVX2_entry                           ; continue in wmemcmpAVX2 if less than 128 bytes
	or      edx, -1                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, edx                                    ; number of bytes
	jmp     wmemcmpAVX512BW_entry                       ; continue in wmemcmpAVX512BW
wmemcmpAVX512F endp

; AVX2 Version. Use ymm register
align 16
wmemcmpAVX2 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     ecx, dword ptr [esp + 20]                   ; count
	xor     eax, eax

wmemcmpAVX2_entry label near
	; entry from above
	lea     esi, [esi + ecx * 2]                        ; use negative index from end of memory block
	lea     edi, [edi + ecx * 2]
	xor     ecx, -1
	mov     edx, 0FFFFH
	inc     ecx
	jnz     loop_entry
	jmp     epilogue

	align   16
loop_begin:
	; loop comparing 32 bytes
	vmovdqu ymm1, ymmword ptr [esi + ecx * 2]
	vpcmpeqw ymm0, ymm1, ymmword ptr [edi + ecx * 2]    ; compare 32 bytes
	vpmovmskb eax, ymm0                                 ; get byte mask
	xor     eax, -1                                     ; not eax would not set flags
	jnz     difference_ymmword                          ; difference found
	add     ecx, 16
	jz      epilogue_avx                                ; finished, equal
loop_entry:
	cmp     ecx, -16
	jna     loop_begin                                  ; next 32 bytes

	; less than 32 bytes left
	vzeroupper                                          ; end ymm state
	cmp     ecx, -8
	ja      less_than_16_bytes_left
	movdqu  xmm0, xmmword ptr [esi + ecx * 2]
	movdqu  xmm1, xmmword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 16 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 8
	jz      epilogue                                    ; finished, equal

less_than_16_bytes_left:
	; less than 16 bytes left
	cmp     ecx, -4
	ja      less_than_8_bytes_left
	; compare 8 bytes
	movq    xmm0, qword ptr [esi + ecx * 2]
	movq    xmm1, qword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 8 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 4
	jz      epilogue

less_than_8_bytes_left:
	; less than 8 bytes left
	cmp     ecx, -2
	ja      difference_word
	; compare 4 bytes
	movd    xmm0, dword ptr [esi + ecx * 2]
	movd    xmm1, dword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 4 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 2
	jnz     difference_word                             ; one word left
	jmp     epilogue                                    ; no bytes left

	align   16
difference_ymmword:
	; difference found. find position
	vzeroupper

difference_xmmword:
	bsf     eax, eax
	shr     eax, 1
	add     ecx, eax

difference_word:
	movzx   eax, word ptr [esi + ecx * 2]
	movzx   edx, word ptr [edi + ecx * 2]
	sub     eax, edx                                    ; signed difference between unsigned words
	jmp     epilogue

	align   16
epilogue_avx:
	; equal
	vzeroupper

epilogue:
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
	xor     eax, eax
	lea     esi, [esi + ecx * 2]                        ; use negative index from end of memory block
	lea     edi, [edi + ecx * 2]
	xor     ecx, -1
	mov     edx, 0FFFFH
	inc     ecx
	jnz     loop_entry
	jmp     epilogue

	align   16
loop_begin:
	; loop comparing 16 bytes
	movdqu  xmm0, xmmword ptr [esi + ecx * 2]
	movdqu  xmm1, xmmword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 16 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 8
	jz      epilogue                                    ; finished, equal
loop_entry:
	cmp     ecx, -8
	jna     loop_begin                                  ; next 16 bytes

	; less than 16 bytes left
	cmp     ecx, -4
	ja      less_than_8_bytes_left
	; compare 8 bytes
	movq    xmm0, qword ptr [esi + ecx * 2]
	movq    xmm1, qword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 8 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 4
	jz      epilogue

less_than_8_bytes_left:
	; less than 8 bytes left
	cmp     ecx, -2
	ja      difference_word
	; compare 4 bytes
	movd    xmm0, dword ptr [esi + ecx * 2]
	movd    xmm1, dword ptr [edi + ecx * 2]
	pcmpeqw xmm0, xmm1                                  ; compare 4 bytes
	pmovmskb eax, xmm0                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     difference_xmmword                          ; difference found
	add     ecx, 2
	jnz     difference_word                             ; one word left
	jmp     epilogue                                    ; no bytes left

	align   16
difference_xmmword:
	; difference found. find position
	bsf     eax, eax
	shr     eax, 1
	add     ecx, eax

difference_word:
	movzx   eax, word ptr [esi + ecx * 2]
	movzx   edx, word ptr [edi + ecx * 2]
	sub     eax, edx                                    ; signed difference between unsigned words

epilogue:
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
	add     ecx, 2
	jnc     dword_loop
	jmp     compare_words

	align   16
dword_loop:
	mov     eax, dword ptr [esi + ecx * 2 - 2]
	mov     edx, dword ptr [edi + ecx * 2 - 2]
	cmp     eax, edx
	jne     compare_words
	add     ecx, 2
	jnc     dword_loop

compare_words:
	xor     eax, eax
	xor     edx, edx
	dec     ecx
	jnz     word_loop
	jmp     epilogue

	align   16
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
