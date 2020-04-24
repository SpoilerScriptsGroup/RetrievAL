.686
.xmm
.model flat

public _memcpy
public memcpyAVX512BW
public memcpyAVX512F
public memcpyU256
public memcpyU
public memcpySSSE3
public memcpySSE2
public memcpy386
public SetMemcpyCacheLimit1

extern _DataCacheSize: near
extern _Store256BitIsFaster: near
extern _UnalignedIsFaster: near
extern _InstructionSet: near

.const

; Jump tables for alignments 0 - 15:
; The CPU dispatcher replaces AlignmentDispatchSSE2 with
; AlignmentDispatchSupSSE3 if Suppl-SSE3 is supported

; Code pointer for each alignment for SSE2 instruction set
AlignmentDispatchSSE2 label dword
	dd C100, D101, D102, D103, D104, D105, D106, D107
	dd D108, D109, D10A, D10B, D10C, D10D, D10E, D10F

; Code pointer for each alignment for Suppl.SSE3 instruction set
AlignmentDispatchSSSE3 label dword
	dd C100, E101, E102, E103, E104, E105, E106, E107
	dd E108, E109, E10A, E10B, E10C, E10D, E10E, E10F

; Code pointer for each alignment for non-temporal store
AlignmentDispatchNT label dword
	dd F100, F101, F102, F103, F104, F105, F106, F107
	dd F108, F109, F10A, F10B, F10C, F10D, F10E, F10F

.data

; Pointer to appropriate version.
; This initially points to memcpyCPUDispatch. memcpyCPUDispatch will
; change this to the appropriate version of memcpy, so that
; memcpyCPUDispatch is only executed once:
memcpyDispatch dd memcpyCPUDispatch

; Bypass cache by using non-temporal moves if count > CacheBypassLimit
; The optimal value of CacheBypassLimit is difficult to estimate, but
; a reasonable value is half the size of the largest cache:
CacheBypassLimit dd 0

.code

; Define prolog for this function
PROLOGM macro
	push    esi
	push    edi
	mov     edi, dword ptr [esp + 12]                   ; dest
	mov     esi, dword ptr [esp + 16]                   ; src
	mov     ecx, dword ptr [esp + 20]                   ; count
endm

; Define return from this function
EPILOGM macro
	pop     edi
	pop     esi
	mov     eax, dword ptr [esp + 4]                    ; Return value = dest
	ret
	align   16
endm

; Function entry:
align 16
_memcpy proc near
	jmp     dword ptr [memcpyDispatch]                  ; Go to appropriate version, depending on instruction set
_memcpy endp


	align   16
	; Version for size <= 40H. Requires AVX512BW and BMI2
L000:
	mov     eax, -1                                     ; if count = 1-31: |  if count = 32-63:
	bzhi    eax, eax, ecx                               ; -----------------|-------------------
	kmovd   k1, eax                                     ;       count 1's  |  all 1's
	xor     eax, eax                                    ;
	sub     ecx, 32                                     ;
	cmovb   ecx, eax                                    ;               0  |  count-32
	dec     eax                                         ;
	bzhi    eax, eax, ecx                               ;
	kmovd   k2, eax                                     ;               0  |  count-32 1's
	kunpckdq k3, k2, k1                                 ; low 32 bits from k1, high 32 bits from k2. total = count 1's
	vmovdqu8 zmm0{k3}{z}, zmmword ptr [esi]             ; move count bytes
	vmovdqu8 zmmword ptr [edi]{k3}, zmm0
	vzeroupper
	EPILOGM

	; Version for size = 40H - 80H
L010:
	; make two partially overlapping blocks
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [esi + ecx - 40H]
	vmovdqu64 zmmword ptr [edi], zmm0
	vmovdqu64 zmmword ptr [edi + ecx - 40H], zmm1
	vzeroupper
	EPILOGM

; AVX512BW Version for processors with fast unaligned read and fast 512 bits write
align 16
memcpyAVX512BW proc near
	PROLOGM
if 0
	cmp     ecx, 040H
	jbe     L000
	cmp     ecx, 080H
	jbe     L010
else
	cmp     ecx, 080H
	ja      L100
	cmp     ecx, 040H
	ja      L010
	test    ecx, ecx
	jnz     L000
	EPILOGM
endif

L100 label near
	; count > 80H                                       ; Entry from memcpyAVX512F
	vmovdqu64 zmm1, zmmword ptr [esi]                   ; save first possibly unaligned block to after main loop
	vmovdqu64 zmm2, zmmword ptr [esi + ecx - 40H]       ; save last  possibly unaligned block to after main loop

	mov     eax, edi                                    ; save destination
	add     edi, ecx                                    ; end of destination
	and     edi, -40H                                   ; round down to align by 40H
	mov     edx, edi
	sub     edx, eax
	add     esi, edx                                    ; end of main blocks of source
	and     edx, -40H                                   ; size of aligned blocks to copy

	; Check if count very big
	cmp     edx, dword ptr [CacheBypassLimit]
	ja      L500                                        ; Use non-temporal store if count > CacheBypassLimit

	neg     edx                                         ; negative index from end of aligned blocks

L200:
	; main loop. Move 40H bytes at a time
	vmovdqu64 zmm0, zmmword ptr [esi + edx]
	vmovdqa64 zmmword ptr [edi + edx], zmm0
	add     edx, 40H
	jnz     L200

L210:
	; insert remaining bytes at beginning and end, possibly overlapping main blocks
	vmovdqu64 zmmword ptr [eax], zmm1
	vmovdqu64 zmmword ptr [eax + ecx - 40H], zmm2
	vzeroupper
	EPILOGM

L500:
	; Move 40H bytes at a time, non-temporal
	neg     edx

L510:
	vmovdqu64 zmm0, zmmword ptr [esi + edx]
	vmovntdq zmmword ptr [edi + edx], zmm0
	add     edx, 40H
	jnz     L510
	sfence
	jmp     L210
memcpyAVX512BW endp

; AVX512F Version for processors with fast unaligned read and fast 512 bits write
align 16
memcpyAVX512F proc near
	PROLOGM

	; edi = dest
	; esi = src
	; ecx = count
	cmp     ecx, 080H
	ja      L100
	cmp     ecx, 040H
	jae     L010
	; count < 40H
	jmp     A1000
memcpyAVX512F endp

; AVX Version for processors with fast unaligned read and fast 256 bits write
align 16
memcpyU256 proc near
	PROLOGM
	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; Calculate size of first block up to first regular boundary of dest
	mov     edx, edi
	neg     edx
	and     edx, 1FH
	jz      B3100                                       ; Skip if dest aligned by 32

	; edx = size of first partial block, 1 - 31 bytes
	test    dl, 3
	jz      B3030
	test    dl, 1
	jz      B3020
	; move 1 byte
	movzx   eax, byte ptr [esi]
	mov     byte ptr [edi], al
	inc     esi
	inc     edi

B3020:
	test    dl, 2
	jz      B3030
	; move 2 bytes
	movzx   eax, word ptr [esi]
	mov     word ptr [edi], ax
	add     esi, 2
	add     edi, 2

B3030:
	test    dl, 4
	jz      B3040
	; move 4 bytes
	mov     eax, dword ptr [esi]
	mov     dword ptr [edi], eax
	add     esi, 4
	add     edi, 4

B3040:
	test    dl, 8
	jz      B3050
	; move 8 bytes
	movq    xmm0, qword ptr [esi]
	movq    qword ptr [edi], xmm0
	add     esi, 8
	add     edi, 8

B3050:
	test    dl, 16
	jz      B3060
	; move 16 bytes
	movups  xmm0, xmmword ptr [esi]
	movaps  xmmword ptr [edi], xmm0
	add     esi, 16
	add     edi, 16

B3060:
	sub     ecx, edx

B3100:
	; Now dest is aligned by 32. Any partial block has been moved

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	add     esi, ecx                                    ; Point to the end
	add     edi, ecx                                    ; Point to the end
	sub     edx, ecx                                    ; Remaining data after loop

	; Check if count very big
	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      I3100                                       ; Use non-temporal store if count > CacheBypassLimit
	neg     ecx                                         ; Negative index from the end

H3100:
	; copy -ecx bytes in blocks of 32 bytes.

	; Check for false memory dependence: The CPU may falsely assume
	; a partial overlap between the written destination and the following
	; read source if source is unaligned and
	; (src-dest) modulo 4096 is close to 4096
	test    esi, 1FH
	jz      H3110                                       ; aligned
	mov     eax, esi
	sub     eax, edi
	and     eax, 0FFFH                                  ; modulo 4096
	cmp     eax, 1000H - 200H
	ja      J3100

H3110:
	; main copy loop, 32 bytes at a time
	; ecx has negative index from the end, counting up to zero
	vmovups ymm0, ymmword ptr [esi + ecx]
	vmovaps ymmword ptr [edi + ecx], ymm0
	add     ecx, 20H
	jnz     H3110
	vzeroupper                                          ; end of AVX mode

	; Move the remaining edx bytes (0 - 31):
H3120:
	add     esi, edx
	add     edi, edx
	neg     edx
	jz      H3500                                       ; Skip if no more data
	; move 16-8-4-2-1 bytes, aligned
	cmp     edx, -10H
	jg      H3200
	; move 16 bytes
	movups  xmm0, xmmword ptr [esi + edx]
	movaps  xmmword ptr [edi + edx], xmm0
	add     edx, 10H

H3200:
	cmp     edx, -8
	jg      H3210
	; move 8 bytes
	movq    xmm0, qword ptr [esi + edx]
	movq    qword ptr [edi + edx], xmm0
	add     edx, 8
	jz      H3500                                       ; Early skip if count divisible by 8

H3210:
	cmp     edx, -4
	jg      H3220
	; move 4 bytes
	mov     eax, dword ptr [esi + edx]
	mov     dword ptr [edi + edx], eax
	add     edx, 4

H3220:
	cmp     edx, -2
	jg      H3230
	; move 2 bytes
	movzx   eax, word ptr [esi + edx]
	mov     word ptr [edi + edx], ax
	add     edx, 2

H3230:
	cmp     edx, -1
	jg      H3500
	; move 1 byte
	movzx   eax, byte ptr [esi + edx]
	mov     byte ptr [edi + edx], al

H3500:
	; finished
	EPILOGM

I3100:
	; non-temporal move
	neg     ecx                                         ; Negative index from the end

	align   16
I3110:
	; main copy loop, 32 bytes at a time
	; ecx has negative index from the end, counting up to zero
	vmovups ymm0, ymmword ptr [esi + ecx]
	vmovntps ymmword ptr [edi + ecx], ymm0
	add     ecx, 20H
	jnz     I3110
	sfence
	vzeroupper                                          ; end of AVX mode
	jmp     H3120                                       ; Move the remaining edx bytes (0 - 31):

	align   16
J3100:
	; There is a false memory dependence.
	; check if src and dest overlap, if not then it is safe
	; to copy backwards to avoid false memory dependence
	; Use this version if you want consistent behavior in the case
	; where dest > src and overlap. However, this case is undefined
	; anyway because part of src is overwritten before copying
	push    edx
	mov     eax, esi
	sub     eax, edi
	cdq
	xor     eax, edx
	sub     eax, edx                                    ; abs(src-dest)
	neg     ecx                                         ; size
	pop     edx                                         ; restore edx
	cmp     eax, ecx
	jnb     J3110
	neg     ecx                                         ; restore ecx
	jmp     H3110                                       ; overlap between src and dest. Can't copy backwards

	align   16
J3110:
	; copy backwards, ecx = size. esi, edi = end of src, dest
	push    esi
	push    edi
	sub     esi, ecx
	sub     edi, ecx

J3120:
	; loop backwards
	vmovups ymm1, ymmword ptr [esi + ecx - 20H]
	vmovaps ymmword ptr [edi + ecx - 20H], ymm1
	sub     ecx, 20H
	jnz     J3120
	vzeroupper
	pop     edi
	pop     esi
	jmp     H3120

	align   16
	; count < 64. Move 32-16-8-4-2-1 bytes
	; multiple CPU versions (SSSE3 and later)
A1000 label near
	add     esi, ecx                                    ; end of src
	add     edi, ecx                                    ; end of dest
	neg     ecx                                         ; negative index from the end
	cmp     ecx, -20H
	jg      A1100
	; move 32 bytes
	; movdqu is faster than movq on all processors with SSSE3
	movups  xmm0, oword ptr [esi + ecx]
	movups  xmm1, oword ptr [esi + ecx + 10H]
	movups  oword ptr xmmword ptr [edi + ecx], xmm0
	movups  oword ptr xmmword ptr [edi + ecx + 10H], xmm1
	add     ecx, 20H

A1100:
	cmp     ecx, -10H
	jg      A1200
	; move 16 bytes
	movups  xmm0, oword ptr [esi + ecx]
	movups  oword ptr xmmword ptr [edi + ecx], xmm0
	add     ecx, 10H

A1200:
	cmp     ecx, -8
	jg      A1300
	; move 8 bytes
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0
	add     ecx, 8

A1300:
	cmp     ecx, -4
	jg      A1400
	; move 4 bytes
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	add     ecx, 4
	jz      A1900                                       ; early out if count divisible by 4

A1400:
	cmp     ecx, -2
	jg      A1500
	; move 2 bytes
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax
	add     ecx, 2

A1500:
	cmp     ecx, -1
	jg      A1900
	; move 1 byte
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

A1900:
	; finished
	EPILOGM
memcpyU256 endp

;  Version for processors with fast unaligned read and fast 16 bytes write
align 16
memcpyU proc near
	PROLOGM
	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; Calculate size of first block up to first regular boundary of dest
	mov     edx, edi
	neg     edx
	and     edx, 0FH
	jz      B2100                                       ; Skip if dest aligned by 16

	; edx = size of first partial block, 1 - 15 bytes
	test    dl, 3
	jz      B2030
	test    dl, 1
	jz      B2020
	; move 1 byte
	movzx   eax, byte ptr [esi]
	mov     byte ptr [edi], al
	inc     esi
	inc     edi

B2020:
	test    dl, 2
	jz      B2030
	; move 2 bytes
	movzx   eax, word ptr [esi]
	mov     word ptr [edi], ax
	add     esi, 2
	add     edi, 2

B2030:
	test    dl, 4
	jz      B2040
	; move 4 bytes
	mov     eax, dword ptr [esi]
	mov     dword ptr [edi], eax
	add     esi, 4
	add     edi, 4

B2040:
	test    dl, 8
	jz      B2050
	; move 8 bytes
	movq    xmm0, qword ptr [esi]
	movq    qword ptr [edi], xmm0
	add     esi, 8
	add     edi, 8

B2050:
	sub     ecx, edx

B2100:
	; Now dest is aligned by 16. Any partial block has been moved

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	add     esi, ecx                                    ; Point to the end
	add     edi, ecx                                    ; Point to the end
	sub     edx, ecx                                    ; Remaining data after loop

	; Check if count very big
	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      I100                                        ; Use non-temporal store if count > CacheBypassLimit
	neg     ecx                                         ; Negative index from the end

H100:
	; copy -ecx bytes in blocks of 32 bytes.

	; Check for false memory dependence: The CPU may falsely assume
	; a partial overlap between the written destination and the following
	; read source if source is unaligned and
	; (src-dest) modulo 4096 is close to 4096
	test    esi, 0FH
	jz      H110                                        ; aligned
	mov     eax, esi
	sub     eax, edi
	and     eax, 0FFFH                                  ; modulo 4096
	cmp     eax, 1000H - 200H
	ja      J100

H110:
	; main copy loop, 32 bytes at a time
	; ecx has negative index from the end, counting up to zero
	movups  xmm0, xmmword ptr [esi + ecx]
	movups  xmm1, xmmword ptr [esi + ecx + 10H]
	movaps  xmmword ptr [edi + ecx], xmm0
	movaps  xmmword ptr [edi + ecx + 10H], xmm1
	add     ecx, 20H
	jnz     H110

	; Move the remaining edx bytes (0 - 31):
H120:
	add     esi, edx
	add     edi, edx
	neg     edx
	jz      H500                                        ; Skip if no more data
	; move 16-8-4-2-1 bytes, aligned
	cmp     edx, -10H
	jg      H200
	; move 16 bytes
	movups  xmm0, xmmword ptr [esi + edx]
	movaps  xmmword ptr [edi + edx], xmm0
	add     edx, 10H

H200:
	cmp     edx, -8
	jg      H210
	; move 8 bytes
	movq    xmm0, qword ptr [esi + edx]
	movq    qword ptr [edi + edx], xmm0
	add     edx, 8
	jz      H500                                        ; Early skip if count divisible by 8

H210:
	cmp     edx, -4
	jg      H220
	; move 4 bytes
	mov     eax, dword ptr [esi + edx]
	mov     dword ptr [edi + edx], eax
	add     edx, 4

H220:
	cmp     edx, -2
	jg      H230
	; move 2 bytes
	movzx   eax, word ptr [esi + edx]
	mov     word ptr [edi + edx], ax
	add     edx, 2

H230:
	cmp     edx, -1
	jg      H500
	; move 1 byte
	movzx   eax, byte ptr [esi + edx]
	mov     byte ptr [edi + edx], al

H500:
	; finished
	EPILOGM

I100:
	; non-temporal move
	neg     ecx                                         ; Negative index from the end

	align   16
I110:
	; main copy loop, 32 bytes at a time
	; ecx has negative index from the end, counting up to zero
	movups  xmm0, xmmword ptr [esi + ecx]
	movups  xmm1, xmmword ptr [esi + ecx + 10H]
	movntps xmmword ptr [edi + ecx], xmm0
	movntps xmmword ptr [edi + ecx + 10H], xmm1
	add     ecx, 20H
	jnz     I110
	sfence
	jmp     H120                                        ; Move the remaining edx bytes (0 - 31):

	align   16
J100:
	; There is a false memory dependence.
	; check if src and dest overlap, if not then it is safe
	; to copy backwards to avoid false memory dependence
	; Use this version if you want consistent behavior in the case
	; where dest > src and overlap. However, this case is undefined
	; anyway because part of src is overwritten before copying
	push    edx
	mov     eax, esi
	sub     eax, edi
	cdq
	xor     eax, edx
	sub     eax, edx                                    ; abs(src-dest)
	neg     ecx                                         ; size
	pop     edx                                         ; restore rdx
	cmp     eax, ecx
	jnb     J110
	neg     ecx                                         ; restore rcx
	jmp     H110                                        ; overlap between src and dest. Can't copy backwards

	align   16
J110:
	; copy backwards, ecx = size. esi, edi = end of src, dest
	push    esi
	push    edi
	sub     esi, ecx
	sub     edi, ecx

J120:
	; loop backwards
	movups  xmm1, xmmword ptr [esi + ecx - 20H]
	movups  xmm0, xmmword ptr [esi + ecx - 10H]
	movaps  xmmword ptr [edi + ecx - 20H], xmm1
	movaps  xmmword ptr [edi + ecx - 10H], xmm0
	sub     ecx, 20H
	jnz     J120
	pop     edi
	pop     esi
	jmp     H120
memcpyU endp

;  Version for processors with SSSE3. Aligned read + shift + aligned write
align 16
memcpySSSE3 proc near
	PROLOGM
	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; This part will not always work if count < 64
	; Calculate size of first block up to first regular boundary of dest
	mov     edx, edi
	neg     edx
	and     edx, 0FH
	jz      B1200                                       ; Skip if dest aligned by 16

	; edx = size of first partial block, 1 - 15 bytes
	test    dl, 3
	jz      B1120
	test    edx, 1
	jz      B1110
	; move 1 byte
	movzx   eax, byte ptr [esi]
	mov     byte ptr [edi], al
	inc     esi
	inc     edi

B1110:
	test    dl, 2
	jz      B1120
	; move 2 bytes
	movzx   eax, word ptr [esi]
	mov     word ptr [edi], ax
	add     esi, 2
	add     edi, 2

B1120:
	test    dl, 4
	jz      B1130
	; move 4 bytes
	mov     eax, dword ptr [esi]
	mov     dword ptr [edi], eax
	add     esi, 4
	add     edi, 4

B1130:
	test    dl, 8
	jz      B1140
	; move 8 bytes
	movq    xmm0, qword ptr [esi]
	movq    qword ptr [edi], xmm0
	add     esi, 8
	add     edi, 8

B1140:
	sub     ecx, edx

B1200:
	; Now dest is aligned by 16. Any partial block has been moved
	; Find alignment of src modulo 16 at this point:
	mov     eax, esi
	and     eax, 0FH

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	add     esi, ecx                                    ; Point to the end
	add     edi, ecx                                    ; Point to the end
	sub     edx, ecx                                    ; Remaining data after loop
	sub     esi, eax                                    ; Nearest preceding aligned block of src

	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      B1400                                       ; Use non-temporal store if count > CacheBypassLimit
	neg     ecx                                         ; Negative index from the end

	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [AlignmentDispatchSSSE3 + eax * 4]

	align   16
B1400:
	neg     ecx
	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [AlignmentDispatchNT + eax * 4]

	align   16
C100 label near
	; Code for aligned src. SSE2 and later instruction set
	; The nice case, src and dest have same alignment.

	; Loop. ecx has negative index from the end, counting up to zero
	movaps  xmm0, xmmword ptr [esi + ecx]
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]
	movaps  xmmword ptr [edi + ecx], xmm0
	movaps  xmmword ptr [edi + ecx + 10H], xmm1
	add     ecx, 20H
	jnz     C100

	; Move the remaining edx bytes (0 - 31):
	add     esi, edx
	add     edi, edx
	neg     edx
	jz      C500                                        ; Skip if no more data
	; move 16-8-4-2-1 bytes, aligned
	cmp     edx, -10H
	jg      C200
	; move 16 bytes
	movaps  xmm0, xmmword ptr [esi + edx]
	movaps  xmmword ptr [edi + edx], xmm0
	add     edx, 10H

C200 label near
	cmp     edx, -8
	jg      C210
	; move 8 bytes
	movq    xmm0, qword ptr [esi + edx]
	movq    qword ptr [edi + edx], xmm0
	add     edx, 8
	jz      C500                                        ; Early skip if count divisible by 8

C210:
	cmp     edx, -4
	jg      C220
	; move 4 bytes
	mov     eax, dword ptr [esi + edx]
	mov     dword ptr [edi + edx], eax
	add     edx, 4

C220:
	cmp     edx, -2
	jg      C230
	; move 2 bytes
	movzx   eax, word ptr [esi + edx]
	mov     word ptr [edi + edx], ax
	add     edx, 2

C230:
	cmp     edx, -1
	jg      C500
	; move 1 byte
	movzx   eax, byte ptr [esi + edx]
	mov     byte ptr [edi + edx], al

C500 label near
	; finished
	EPILOGM
memcpySSSE3 endp

;  Version for processors with SSE2. Aligned read + shift + aligned write
align 16
memcpySSE2 proc near
	PROLOGM
	cmp     ecx, 40H
	jae     B100                                        ; Use simpler code if count < 64

	; count < 64. Move 32-16-8-4-2-1 bytes
	add     esi, ecx                                    ; end of src
	add     edi, ecx                                    ; end of dest
	neg     ecx                                         ; negative index from the end
	cmp     ecx, -20H
	jg      A100
	; move 32 bytes
	; movq is faster than movdqu on Intel Pentium M and Core 1
	; movdqu is fast on Nehalem and later
	movq    xmm0, qword ptr [esi + ecx]
	movq    xmm1, qword ptr [esi + ecx + 8]
	movq    xmm2, qword ptr [esi + ecx + 10H]
	movq    xmm3, qword ptr [esi + ecx + 18H]
	movq    qword ptr [edi + ecx], xmm0
	movq    qword ptr [edi + ecx + 8], xmm1
	movq    qword ptr [edi + ecx + 10H], xmm2
	movq    qword ptr [edi + ecx + 18H], xmm3
	add     ecx, 20H

A100:
	cmp     ecx, -10H
	jg      A200
	; move 16 bytes
	movq    xmm0, qword ptr [esi + ecx]
	movq    xmm1, qword ptr [esi + ecx + 8]
	movq    qword ptr [edi + ecx], xmm0
	movq    qword ptr [edi + ecx + 8], xmm1
	add     ecx, 10H

A200:
	cmp     ecx, -8
	jg      A300
	; move 8 bytes
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0
	add     ecx, 8

A300:
	cmp     ecx, -4
	jg      A400
	; move 4 bytes
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	add     ecx, 4
	jz      A900                                        ; early out if count divisible by 4

A400:
	cmp     ecx, -2
	jg      A500
	; move 2 bytes
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax
	add     ecx, 2

A500:
	cmp     ecx, -1
	jg      A900
	; move 1 byte
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

A900:
	; finished
	EPILOGM

B100:
	; count >= 64
	; This part will not always work if count < 64
	; Calculate size of first block up to first regular boundary of dest
	mov     edx, edi
	neg     edx
	and     edx, 0FH
	jz      B200                                        ; Skip if dest aligned by 16

	; edx = size of first partial block, 1 - 15 bytes
	test    dl, 3
	jz      B120
	test    dl, 1
	jz      B110
	; move 1 byte
	movzx   eax, byte ptr [esi]
	mov     byte ptr [edi], al
	inc     esi
	inc     edi

B110:
	test    dl, 2
	jz      B120
	; move 2 bytes
	movzx   eax, word ptr [esi]
	mov     word ptr [edi], ax
	add     esi, 2
	add     edi, 2

B120:
	test    dl, 4
	jz      B130
	; move 4 bytes
	mov     eax, dword ptr [esi]
	mov     dword ptr [edi], eax
	add     esi, 4
	add     edi, 4

B130:
	test    dl, 8
	jz      B140
	; move 8 bytes
	movq    xmm0, qword ptr [esi]
	movq    qword ptr [edi], xmm0
	add     esi, 8
	add     edi, 8

B140:
	sub     ecx, edx

B200:
	; Now dest is aligned by 16. Any partial block has been moved
	; Find alignment of src modulo 16 at this point:
	mov     eax, esi
	and     eax, 0FH

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	add     esi, ecx                                    ; Point to the end
	add     edi, ecx                                    ; Point to the end
	sub     edx, ecx                                    ; Remaining data after loop
	sub     esi, eax                                    ; Nearest preceding aligned block of src

	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      B400                                        ; Use non-temporal store if count > CacheBypassLimit
	neg     ecx                                         ; Negative index from the end

	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [AlignmentDispatchSSE2 + eax * 4]

	align   16
B400:
	neg     ecx
	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [AlignmentDispatchNT + eax * 4]

	align   16
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  Macros and alignment jump tables
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Macros for each src alignment, SSE2 instruction set:
; Make separate code for each alignment u because the shift instructions
; have the shift count as a constant:

MOVE_UNALIGNED_SSE2 macro MODULO:req, NON_TEMPORAL:req
; Move ecx + edx bytes of data
; Source is misaligned. (src-dest) modulo 16 = MODULO
; NON_TEMPORAL != 0 if non-temporal store desired
; eax = MODULO
; esi = src - MODULO = nearest preceding 16-bytes boundary
; edi = dest (aligned)
; ecx = - (count rounded down to nearest divisible by 32)
; edx = remaining bytes to move after loop
	local L1, L2

	movdqa  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest preceding 16B boundary

L1:
	; Loop. ecx has negative index from the end, counting up to zero
	movdqa  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movdqa  xmm2, xmmword ptr [esi + ecx + 20H]
	movdqa  xmm3, xmm1                                  ; Copy because used twice
	psrldq  xmm0, MODULO                                ; shift right
	pslldq  xmm1, 16 - MODULO                           ; shift left
	por     xmm0, xmm1                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + ecx], xmm0               ; non-temporal save
	ENDIF
	movdqa  xmm0, xmm2                                  ; Save for next iteration
	psrldq  xmm3, MODULO                                ; shift right
	pslldq  xmm2, 16 - MODULO                           ; shift left
	por     xmm3, xmm2                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + ecx + 10H], xmm3         ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + ecx + 10H], xmm3         ; non-temporal save
	ENDIF
	add     ecx, 20H                                    ; Loop through negative values up to zero
	jnz     L1

	; Set up for edx remaining bytes
	add     esi, edx
	add     edi, edx
	neg     edx
	cmp     edx, -10H
	jg      L2
	; One more 16-bytes block to move
	movdqa  xmm1, xmmword ptr [esi + edx + 10H]
	psrldq  xmm0, MODULO                                ; shift right
	pslldq  xmm1, 16 - MODULO                           ; shift left
	por     xmm0, xmm1                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + edx], xmm0               ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + edx], xmm0               ; non-temporal save
	ENDIF
	add     edx, 10H

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	IF NON_TEMPORAL NE 0
	sfence
	ENDIF
	; Move remaining 0 - 15 bytes, unaligned
	jmp     C200
	align   16
endm

MOVE_UNALIGNED_SSE2_4 macro NON_TEMPORAL:req
; Special case for u = 4
; NON_TEMPORAL != 0 if non-temporal store desired
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest preceding 16B boundary

L1:
	; Loop. ecx has negative index from the end, counting up to zero
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movss   xmm0, xmm1                                  ; Moves 4 bytes, leaves remaining bytes unchanged
	;pshufd  xmm0, xmm0, 00111001B
	shufps  xmm0, xmm0, 00111001B
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm0               ; Non-temporal save
	ENDIF
	movaps  xmm0, xmmword ptr [esi + ecx + 20H]
	movss   xmm1, xmm0
	shufps  xmm1, xmm1, 00111001B
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx + 10H], xmm1         ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx + 10H], xmm1         ; Non-temporal save
	ENDIF
	add     ecx, 20H                                    ; Loop through negative values up to zero
	jnz     L1
	; Set up for edx remaining bytes
	add     esi, edx
	add     edi, edx
	neg     edx
	cmp     edx, -10H
	jg      L2
	; One more 16-bytes block to move
	movaps  xmm1, xmmword ptr [esi + edx + 10H]         ; Read next two blocks aligned
	movss   xmm0, xmm1
	shufps  xmm0, xmm0, 00111001B
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + edx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + edx], xmm0               ; Non-temporal save
	ENDIF
	add     edx, 10H

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	IF NON_TEMPORAL NE 0
	sfence
	ENDIF
	; Move remaining 0 - 15 bytes, unaligned
	jmp     C200
	align   16
endm

MOVE_UNALIGNED_SSE2_8 macro NON_TEMPORAL:req
; Special case for u = 8
; NON_TEMPORAL != 0 if non-temporal store desired
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest preceding 16B boundary

L1:
	; Loop. ecx has negative index from the end, counting up to zero
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movsd   xmm0, xmm1                                  ; Moves 8 bytes, leaves remaining bytes unchanged
	shufps  xmm0, xmm0, 01001110B                       ; Rotate
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm0               ; Non-temporal save
	ENDIF
	movaps  xmm0, xmmword ptr [esi + ecx + 20H]
	movsd   xmm1, xmm0
	shufps  xmm1, xmm1, 01001110B
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx + 10H], xmm1         ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx + 10H], xmm1         ; Non-temporal save
	ENDIF
	add     ecx, 20H                                    ; Loop through negative values up to zero
	jnz     L1
	; Set up for edx remaining bytes
	add     esi, edx
	add     edi, edx
	neg     edx
	cmp     edx, -10H
	jg      L2
	; One more 16-bytes block to move
	movaps  xmm1, xmmword ptr [esi + edx + 10H]         ; Read next two blocks aligned
	movsd   xmm0, xmm1
	shufps  xmm0, xmm0, 01001110B
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + edx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + edx], xmm0               ; Non-temporal save
	ENDIF
	add     edx, 10H

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	IF NON_TEMPORAL NE 0
	sfence
	ENDIF
	; Move remaining 0 - 15 bytes, unaligned
	jmp     C200
	align   16
endm

MOVE_UNALIGNED_SSE2_12 macro NON_TEMPORAL:req
; NON_TEMPORAL != 0 if non-temporal store desired
; Special case for u = 12
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest preceding 16B boundary
	shufps  xmm0, xmm0, 10010011B

L1:
	; Loop. ecx has negative index from the end, counting up to zero
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movaps  xmm2, xmmword ptr [esi + ecx + 20H]
	shufps  xmm1, xmm1, 10010011B
	shufps  xmm2, xmm2, 10010011B
	movaps  xmm3, xmm2
	movss   xmm2, xmm1                                  ; Moves 4 bytes, leaves remaining bytes unchanged
	movss   xmm1, xmm0                                  ; Moves 4 bytes, leaves remaining bytes unchanged
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm1               ; Save aligned
	movaps  xmmword ptr [edi + ecx + 10H], xmm2         ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm1               ; Non-temporal save
	movntps xmmword ptr [edi + ecx + 10H], xmm2         ; Non-temporal save
	ENDIF
	movaps  xmm0, xmm3                                  ; Save for next iteration
	add     ecx, 20H                                    ; Loop through negative values up to zero
	jnz     L1
	; Set up for edx remaining bytes
	add     esi, edx
	add     edi, edx
	neg     edx
	cmp     edx, -10H
	jg      L2
	; One more 16-bytes block to move
	movaps  xmm1, xmmword ptr [esi + edx + 10H]         ; Read next two blocks aligned
	shufps  xmm1, xmm1, 10010011B
	movss   xmm1, xmm0                                  ; Moves 4 bytes, leaves remaining bytes unchanged
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + edx], xmm1               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + edx], xmm1               ; Non-temporal save
	ENDIF
	add     edx, 10H

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	IF NON_TEMPORAL NE 0
	sfence
	ENDIF
	; Move remaining 0 - 15 bytes, unaligned
	jmp     C200
	align   16
endm

; Macros for each src alignment, Suppl.SSE3 instruction set:
; Make separate code for each alignment u because the palignr instruction
; has the shift count as a constant:

MOVE_UNALIGNED_SSSE3 macro MODULO:req
; Move ecx + edx bytes of data
; Source is misaligned. (src-dest) modulo 16 = MODULO
; eax = MODULO
; esi = src - MODULO = nearest preceding 16-bytes boundary
; edi = dest (aligned)
; ecx = - (count rounded down to nearest divisible by 32)
; edx = remaining bytes to move after loop
	local L1, L2

	movdqa  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest preceding 16B boundary

L1:
	; Loop. ecx has negative index from the end, counting up to zero
	movdqa  xmm2, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks
	movdqa  xmm3, xmmword ptr [esi + ecx + 20H]
	movdqa  xmm1, xmm0                                  ; Save xmm0
	movdqa  xmm0, xmm3                                  ; Save for next iteration
	palignr xmm3, xmm2, MODULO                          ; Combine parts into aligned block
	palignr xmm2, xmm1, MODULO                          ; Combine parts into aligned block
	movdqa  xmmword ptr [edi + ecx], xmm2               ; Save aligned
	movdqa  xmmword ptr [edi + ecx + 10H], xmm3         ; Save aligned
	add     ecx, 20H
	jnz     L1

	; Set up for edx remaining bytes
	add     esi, edx
	add     edi, edx
	neg     edx
	cmp     edx, -10H
	jg      L2
	; One more 16-bytes block to move
	movdqa  xmm2, xmmword ptr [esi + edx + 10H]
	palignr xmm2, xmm0, MODULO
	movdqa  xmmword ptr [edi + edx], xmm2
	add     edx, 10H

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	; Move remaining 0 - 15 bytes
	jmp     C200
	align   16
endm

; Make 15 instances of SSE2 macro for each value of the alignment u.
; These are pointed to by the jump table AlignmentDispatchSSE2 below

; (aligns are inserted manually to minimize the number of 16-bytes
; boundaries inside loops in the most common cases)

D104 label near
	MOVE_UNALIGNED_SSE2_4    0
D108 label near
	MOVE_UNALIGNED_SSE2_8    0
D10C label near
	MOVE_UNALIGNED_SSE2_12   0
D101 label near
	MOVE_UNALIGNED_SSE2 1,   0
D102 label near
	MOVE_UNALIGNED_SSE2 2,   0
D103 label near
	MOVE_UNALIGNED_SSE2 3,   0
D105 label near
	MOVE_UNALIGNED_SSE2 5,   0
D106 label near
	MOVE_UNALIGNED_SSE2 6,   0
D107 label near
	MOVE_UNALIGNED_SSE2 7,   0
D109 label near
	MOVE_UNALIGNED_SSE2 9,   0
D10A label near
	MOVE_UNALIGNED_SSE2 0AH, 0
D10B label near
	MOVE_UNALIGNED_SSE2 0BH, 0
D10D label near
	MOVE_UNALIGNED_SSE2 0DH, 0
D10E label near
	MOVE_UNALIGNED_SSE2 0EH, 0
D10F label near
	MOVE_UNALIGNED_SSE2 0FH, 0

; Make 15 instances of Suppl-SSE3 macro for each value of the alignment u.
; These are pointed to by the jump table AlignmentDispatchSupSSE3 below

E104 label near
	MOVE_UNALIGNED_SSSE3 4
E108 label near
	MOVE_UNALIGNED_SSSE3 8
E10C label near
	MOVE_UNALIGNED_SSSE3 0CH
E101 label near
	MOVE_UNALIGNED_SSSE3 1
E102 label near
	MOVE_UNALIGNED_SSSE3 2
E103 label near
	MOVE_UNALIGNED_SSSE3 3
E105 label near
	MOVE_UNALIGNED_SSSE3 5
E106 label near
	MOVE_UNALIGNED_SSSE3 6
E107 label near
	MOVE_UNALIGNED_SSSE3 7
E109 label near
	MOVE_UNALIGNED_SSSE3 9
E10A label near
	MOVE_UNALIGNED_SSSE3 0AH
E10B label near
	MOVE_UNALIGNED_SSSE3 0BH
E10D label near
	MOVE_UNALIGNED_SSSE3 0DH
E10E label near
	MOVE_UNALIGNED_SSSE3 0EH
E10F label near
	MOVE_UNALIGNED_SSSE3 0FH

; Codes for non-temporal move. Aligned case first

F100 label near
	; Non-temporal move, src and dest have same alignment.
	; Loop. ecx has negative index from the end, counting up to zero
	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]
	movntps xmmword ptr [edi + ecx], xmm0               ; Write non-temporal (bypass cache)
	movntps xmmword ptr [edi + ecx + 10H], xmm1
	add     ecx, 20H
	jnz     F100                                        ; Loop through negative ecx up to zero

	; Move the remaining edx bytes (0 - 31):
	add     esi, edx
	add     edi, edx
	neg     edx
	jz      C500                                        ; Skip if no more data
	; Check if we can more one more 16-bytes block
	cmp     edx, -10H
	jg      C200
	; move 16 bytes, aligned
	movaps  xmm0, xmmword ptr [esi + edx]
	movntps xmmword ptr [edi + edx], xmm0
	add     edx, 10H
	sfence
	; move the remaining 0 - 15 bytes
	jmp     C200

	align   16
; Make 15 instances of MOVE_UNALIGNED_SSE2 macro for each value of
; the alignment u.
; These are pointed to by the jump table AlignmentDispatchNT below

F104 label near
	MOVE_UNALIGNED_SSE2_4    1
F108 label near
	MOVE_UNALIGNED_SSE2_8    1
F10C label near
	MOVE_UNALIGNED_SSE2_12   1
F101 label near
	MOVE_UNALIGNED_SSE2 1,   1
F102 label near
	MOVE_UNALIGNED_SSE2 2,   1
F103 label near
	MOVE_UNALIGNED_SSE2 3,   1
F105 label near
	MOVE_UNALIGNED_SSE2 5,   1
F106 label near
	MOVE_UNALIGNED_SSE2 6,   1
F107 label near
	MOVE_UNALIGNED_SSE2 7,   1
F109 label near
	MOVE_UNALIGNED_SSE2 9,   1
F10A label near
	MOVE_UNALIGNED_SSE2 0AH, 1
F10B label near
	MOVE_UNALIGNED_SSE2 0BH, 1
F10D label near
	MOVE_UNALIGNED_SSE2 0DH, 1
F10E label near
	MOVE_UNALIGNED_SSE2 0EH, 1
F10F label near
	MOVE_UNALIGNED_SSE2 0FH, 1
memcpySSE2 endp

; 80386 version used when SSE2 not supported:
align 16
memcpy386 proc near
	PROLOGM
; edi = dest
; esi = src
; ecx = count
	cld
	cmp     ecx, 8
	jb      G500

G100:
	test    edi, 1
	jz      G200
	movsb
	dec     ecx

G200:
	test    edi, 2
	jz      G300
	movsw
	sub     ecx, 2

G300:
	; edi is aligned now
	mov     edx, ecx
	shr     ecx, 2
	rep     movsd                                       ; move 4 bytes at a time
	mov     ecx, edx
	and     ecx, 3
if 0
	rep     movsb                                       ; move remaining 0-3 bytes
	EPILOGM

G500:
	; count < 8. Move one byte at a time
	rep     movsb                                       ; move count bytes
	EPILOGM
else

G500:
	rep     movsb                                       ; move remaining bytes
	EPILOGM
endif
memcpy386 endp

; CPU dispatching for memcpy. This is executed only once
align 16
memcpyCPUDispatch proc near
	pushad
	; set CacheBypassLimit to half the size of the largest level cache
	call    GetMemcpyCacheLimit
	; get supported instruction set
	call    _InstructionSet
	; Point to generic version of memcpy
	mov     esi, offset memcpy386
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	; Point to SSE2 version of memcpy
	mov     esi, offset memcpySSE2
	cmp     eax, 6                                      ; check Suppl-SSE3
	jb      Q100
	; Suppl-SSE3 supported
	; Point to SSSE3 version of memcpy
	mov     esi, offset memcpySSSE3
	call    _UnalignedIsFaster                          ; Test if unaligned read is faster than aligned read and shift
	test    eax, eax
	jz      Q100
	; Point to unaligned version of memcpy
	mov     esi, offset memcpyU
	call    _Store256BitIsFaster                        ; Test if 256-bit read/write is available and faster than 128-bit read/write
	test    eax, eax
	jz      Q100
	mov     esi, offset memcpyU256
	call    _InstructionSet
	cmp     eax, 15
	jb      Q100
	mov     esi, offset memcpyAVX512F
	cmp     eax, 16
	jb      Q100
	mov     esi, offset memcpyAVX512BW

Q100:
	mov     dword ptr [memcpyDispatch], esi
	popad
	; Continue in appropriate version of memcpy
	jmp     dword ptr [memcpyDispatch]
memcpyCPUDispatch endp

align 16
GetMemcpyCacheLimit proc near
	push    ebx
	mov     ebx, offset CacheBypassLimit
	mov     eax, dword ptr [ebx]
	test    eax, eax
	jnz     U200
	; Get half the size of the largest level cache
	push    0                                           ; 0 means largest level cache
	call    _DataCacheSize                              ; get cache size
	pop     ecx
	shr     eax, 1                                      ; half the size
	jnz     U100
	mov     eax, 400000H                                ; cannot determine cache size. use 4 Mbytes

U100:
	mov     dword ptr [ebx], eax

U200:
	pop     ebx
	ret
GetMemcpyCacheLimit endp

align 16
SetMemcpyCacheLimit1 proc near
	push    ebx
	mov     ebx, offset CacheBypassLimit
	mov     eax, dword ptr [esp + 8]
	test    eax, eax
	jnz     U400
	; zero, means default
	mov     dword ptr [ebx], eax
	call    GetMemcpyCacheLimit

U400:
	mov     dword ptr [ebx], eax
	pop     ebx
	ret
SetMemcpyCacheLimit1 endp

end
