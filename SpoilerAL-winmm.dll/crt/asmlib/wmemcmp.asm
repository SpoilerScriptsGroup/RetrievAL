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
	mov     eax, -2                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, eax                                    ; number of bytes
	cmp     ecx, 40H
	jbe     L820
	cmp     ecx, 80H
	jbe     L800

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
	sub     esi, edx                                    ; esi = -size of aligned blocks
	sub     edi, esi
	sub     ecx, eax
	sub     edx, eax

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
	vpcompressd zmm0{k1}{z},zmm0                        ; get first differing dword to position 0
	vpcompressd zmm1{k1}{z},zmm1                        ; get first differing dword to position 0
	vmovd   ecx, xmm0
	vmovd   edx, xmm1
	mov     eax, ecx
	xor     ecx, edx                                    ; difference
	bsf     ecx, ecx                                    ; position of lowest differing bit
	and     ecx, -16                                    ; round down to word boundary
	shr     eax, cl                                     ; first differing word in ax
	shr     edx, cl                                     ; first differing word in dx
	movzx   eax, ax                                     ; zero-extend bytes
	movzx   edx, dx
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
	mov     eax, -2                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, eax                                    ; number of bytes
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
	mov     eax, -2                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, eax                                    ; number of bytes

A001 label near
	; entry from above
	add     esi, ecx                                    ; use negative index from end of memory block
	add     edi, ecx
	neg     ecx
	jz      A900
	mov     edx, 0FFFFH
	cmp     ecx, -32
	ja      A100

A000:
	; loop comparing 32 bytes
	vmovdqu   ymm1, ymmword ptr [esi + ecx]
	vpcmpeqw  ymm0, ymm1, ymmword ptr [edi + ecx]       ; compare 32 bytes
	vpmovmskb eax, ymm0                                 ; get byte mask
	xor     eax, -1                                     ; not eax would not set flags
	jnz     A700                                        ; difference found
	add     ecx, 32
	jz      A900                                        ; finished, equal
	cmp     ecx, -32
	jna     A000                                        ; next 32 bytes
	vzeroupper                                          ; end ymm state

A100:
	; less than 32 bytes left
	cmp     ecx, -16
	ja      A200
	movdqu  xmm1, xmmword ptr [esi + ecx]
	movdqu  xmm2, xmmword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 16 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A701                                        ; difference found
	add     ecx, 16
	jz      A901                                        ; finished, equal

A200:
	; less than 16 bytes left
	cmp     ecx, -8
	ja      A300
	; compare 8 bytes
	movq    xmm1, qword ptr [esi + ecx]
	movq    xmm2, qword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 8 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A701                                        ; difference found
	add     ecx, 8
	jz      A901

A300:
	; less than 8 bytes left
	cmp     ecx, -4
	ja      A400
	; compare 4 bytes
	movd    xmm1, dword ptr [esi + ecx]
	movd    xmm2, dword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 4 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     A701                                        ; difference found
	add     ecx, 4
	jz      A901

A400:
	; less than 4 bytes left
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result
	pop     edi
	pop     esi
	ret
	align   16

A700:
	; difference found. find position
	vzeroupper

A701:
	bsf     eax, eax
	add     ecx, eax
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result
	pop     edi
	pop     esi
	ret
	align   16

A900:
	; equal
	vzeroupper

A901:
	xor     eax, eax
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
	mov     eax, -2                                     ; maximum bytes
	add     ecx, ecx                                    ; count * 2
	cmovc   ecx, eax                                    ; number of bytes
	add     esi, ecx                                    ; use negative index from end of memory block
	add     edi, ecx
	neg     ecx
	jz      S900
	mov     edx, 0FFFFH
	cmp     ecx, -16
	ja      S200

S100:
	; loop comparing 16 bytes
	movdqu  xmm1, xmmword ptr [esi + ecx]
	movdqu  xmm2, xmmword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 16 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S700                                        ; difference found
	add     ecx, 16
	jz      S900                                        ; finished, equal
	cmp     ecx, -16
	jna     S100                                        ; next 16 bytes

S200:
	; less than 16 bytes left
	cmp     ecx, -8
	ja      S300
	; compare 8 bytes
	movq    xmm1, qword ptr [esi + ecx]
	movq    xmm2, qword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 8 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S700                                        ; difference found
	add     ecx, 8
	jz      S900

S300:
	; less than 8 bytes left
	cmp     ecx, -4
	ja      S400
	; compare 4 bytes
	movd    xmm1, dword ptr [esi + ecx]
	movd    xmm2, dword ptr [edi + ecx]
	pcmpeqw xmm1, xmm2                                  ; compare 4 bytes
	pmovmskb eax, xmm1                                  ; get byte mask
	xor     eax, edx                                    ; not ax
	jnz     S700                                        ; difference found
	add     ecx, 4
	jz      S900

S400:
	; less than 4 bytes left
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result
	pop     edi
	pop     esi
	ret
	align   16

S700:
	; difference found. find position
	bsf     eax, eax
	add     ecx, eax
	movzx   eax, word ptr [esi + ecx]
	movzx   edx, word ptr [edi + ecx]
	sub     eax, edx                                    ; return result
	pop     edi
	pop     esi
	ret
	align   16

S900:
	; equal
	xor     eax, eax
	pop     edi
	pop     esi
	ret
wmemcmpSSE2 endp

; Generic version version. Use 32 bit registers
align 16
wmemcmp386 proc near
	; This is not perfectly optimized because it is unlikely to ever be used
	push    esi
	push    edi
	mov     ecx, dword ptr [esp + 20]                   ; count
	mov     edi, dword ptr [esp + 16]                   ; ptr2
	mov     esi, dword ptr [esp + 12]                   ; ptr1
	mov     eax, ecx
	shr     ecx, 1                                      ; count/2 = number of dwords
	jz      M600
	repe    cmpsd                                       ; compare dwords
	je      M600                                        ; equal
	; dwords differ. search in last 4 bytes
	xor     eax, eax
	xor     edx, edx
	mov     ax, word ptr [esi - 4]
	mov     dx, word ptr [edi - 4]
	sub     eax, edx                                    ; compare 1st word
	jnz     M700                                        ; not equal
	mov     ax, word ptr [esi - 2]
	mov     dx, word ptr [edi - 2]
	sub     eax, edx                                    ; compare 2nd word
	jmp     M700
	align   16

M600:
	and     eax, 1                                      ; remainder
	jz      M700
	xor     eax, eax
	xor     edx, edx
	mov     ax, word ptr [esi]
	mov     dx, word ptr [edi]
	sub     eax, edx
M700:
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
