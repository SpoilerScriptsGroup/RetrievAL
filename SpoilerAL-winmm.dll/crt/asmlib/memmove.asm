.686
.xmm
.model flat

public _memmove

extern SetMemcpyCacheLimit1: near
extern _Store256BitIsFaster: near
extern _UnalignedIsFaster: near
extern _InstructionSet: near
extern memcpyAVX512BW: near
extern memcpyAVX512F: near
extern memcpyU256: near
extern memcpyU: near
extern memcpySSSE3: near
extern memcpySSE2: near
extern memcpy386: near

.const

; Jump tables for alignments 0 - 15:
; The CPU dispatcher replaces MAlignmentDispatchSSE2 with
; MAlignmentDispatchSupSSE3 if Suppl-SSE3 is supported

MAlignmentDispatchSSE2 label dword
	dd C100, D101, D102, D103, D104, D105, D106, D107
	dd D108, D109, D10A, D10B, D10C, D10D, D10E, D10F

MAlignmentDispatchSSSE3 label dword
	dd C100, E101, E102, E103, E104, E105, E106, E107
	dd E108, E109, E10A, E10B, E10C, E10D, E10E, E10F

MAlignmentDispatchNT label dword
	dd F100, F101, F102, F103, F104, F105, F106, F107
	dd F108, F109, F10A, F10B, F10C, F10D, F10E, F10F

.data

; Pointer to appropriate version.
; This initially points to memcpyCPUDispatch. memcpyCPUDispatch will
; change this to the appropriate version of memcpy, so that
; memcpyCPUDispatch is only executed once:
memmoveDispatch dd memmoveCPUDispatch

; Bypass cache by using non-temporal moves if count > CacheBypassLimit
; The optimal value of CacheBypassLimit is difficult to estimate, but
; a reasonable value is half the size of the largest cache:
CacheBypassLimit dd 0

.code

; Define prolog for this function
; Parameter 1 is forward function label
PROLOGM macro label:req
	; Check if dest overlaps src
	mov     eax, dword ptr [esp + 4]                    ; dest
	sub     eax, dword ptr [esp + 8]                    ; src
	cmp     eax, dword ptr [esp + 12]                   ; count
	; We can avoid testing for dest < src by using unsigned compare:
	; (Assume that the memory block cannot span across address 0)
	; Must move backwards if unsigned(dest-src) < count
	jae     label                                       ; Jump to memcpy if we can move forwards

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
_memmove proc near
	jmp     dword ptr [memmoveDispatch]                 ; Go to appropriate version, depending on instruction set
_memmove endp

	align   16

	; short versions
L000:
	; 41H - 80H bytes
	vmovdqu64 zmm0, zmmword ptr [esi + ecx - 40H]
	vmovdqu64 zmmword ptr [edi + ecx - 40H], zmm0
	sub     ecx, 40H

L010:
	; 0 - 40H bytes
	mov     eax, -1                                     ; if count = 1-31: |  if count = 32-63:
	bzhi    eax, eax, ecx                               ; -----------------|--------------------
	kmovd   k1, eax                                     ;       count 1's  |  all 1's
	xor     eax, eax                                    ;
	sub     ecx, 32                                     ;
	cmovb   ecx, eax                                    ;               0  |  count-32
	dec     eax                                         ;
	bzhi    eax, eax, ecx                               ;
	kmovd   k2, eax                                     ;               0  |  count-32 1's
	kunpckdq k3, k2, k1                                 ; low 32 bits from k1, high 32 bits from k2. total = count 1's
	vmovdqu8 zmm0{k3}{z}, zmmword ptr [esi]
	vmovdqu8 zmmword ptr [edi]{k3}, zmm0
	vzeroupper
	EPILOGM

; AVX512BW Version for processors with fast unaligned read and fast 512 bit write
align 16
memmoveAVX512BW proc near
	PROLOGM memcpyAVX512BW                              ; jump to memcpyAVX512BW if copying forwards
	cmp     ecx, 40H
	jbe     L010                                        ; Use simpler code if count <= 64
	cmp     ecx, 80H
	jbe     L000                                        ; Use simpler code if count <= 128

L100 label near
	; count >= 80H                                      ; Entry from memmoveAVX512F
	vmovdqu64 zmm1, zmmword ptr [esi]                   ; save first possibly unaligned block to after main loop
	vmovdqu64 zmm2, zmmword ptr [esi + ecx - 40H]       ; save last  possibly unaligned block to after main loop

	; Align destination by 40H
	mov     edx, edi                                    ; destination
	add     edi, 40H
	and     edi, -40H                                   ; first 40H boundary of destination
	lea     eax, [edx + ecx]                            ; end of destination
	sub     edx, edi                                    ; -offset
	sub     esi, edx                                    ; add same offset to source
	add     ecx, edx                                    ; size minus first unaligned part
	and     ecx, -40H                                   ; round down size to multiple of 40H
	add     edx, edi                                    ; restore destination

	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      L800                                        ; Use non-temporal store if count > CacheBypassLimit

	align   16
L200:
	; Main move loop
	sub      ecx, 40H
	vmovdqu64 zmm0, zmmword ptr [esi + ecx]
	vmovdqa64 zmmword ptr [edi + ecx], zmm0
	jnz      L200

L300:
	; insert remaining parts in beginning and end, possibly overwriting what has already been stored
	vmovdqu64 zmmword ptr [eax - 40H], zmm2
	vmovdqu64 zmmword ptr [edx], zmm1
	vzeroupper
	EPILOGM

L800:
	; move loop, bypass cache
	sub      ecx, 40H
	vmovdqu64 zmm0, zmmword ptr [esi + ecx]
	vmovntdq zmmword ptr [edi + ecx], zmm0
	jnz      L800
	sfence
	jmp      L300
memmoveAVX512BW endp

; AVX512F Version for processors with fast unaligned read and fast 512 bit write
align 16
memmoveAVX512F proc near
	PROLOGM memcpyAVX512F                               ; jump to memcpyAVX512F if copying forwards
	cmp     ecx, 80H
	jnb     L100                                        ; use same code as memmoveAVX512BW if count >= 80H
	cmp     ecx, 40H
	jb      A1000                                       ; use code below if count < 40H
	; count = 40H - 80H. Make two partially overlapping 40H blocks
	vmovdqu64 zmm0, zmmword ptr [esi]
	vmovdqu64 zmm1, zmmword ptr [esi + ecx - 40H]
	vmovdqu64 zmmword ptr [edi], zmm0
	vmovdqu64 zmmword ptr [edi + ecx - 40H], zmm1
	vzeroupper
	EPILOGM
memmoveAVX512F endp

; AVX Version for processors with fast unaligned read and fast 256 bits write
align 16
memmoveU256 proc near
	PROLOGM memcpyU256

	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; Note: this part will not always work if count < 64
	; Calculate size of last block after last regular boundary of dest
	lea     edx, [edi + ecx]                            ; end of dext
	and     edx, 1FH
	jz      B4300                                       ; Skip if end of dest aligned by 32

	; edx = size of last partial block, 1 - 32 bytes
	test    dl, 3
	jz      B4210
	test    dl, 1
	jz      B4201                                       ; B4200 if we haven't tested edx,3
	; move 1 byte
	dec     ecx
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

B4200:
	test    dl, 2
	jz      B4210

B4201:
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

B4210:
	test    dl, 4
	jz      B4220
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax

B4220:
	test    dl, 8
	jz      B4230
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

B4230:
	test    dl, 16
	jz      B4300
	; move 16 bytes
	sub     ecx, 16
	movups  xmm0, xmmword ptr [esi + ecx]
	movaps  xmmword ptr [edi + ecx], xmm0

B4300:
	; Now end of dest is aligned by 32. Any partial block has been moved
	mov     edx, ecx
	and     ecx, 1FH                                    ; remaining size after 32 bytes blocks moved
	and     edx, -20H                                   ; number of 32 bytes blocks
	jz      H4100
	add     esi, ecx
	add     edi, ecx

	; Check if count very big
	cmp     edx, dword ptr [CacheBypassLimit]
	ja      H4800                                       ; Use non-temporal store if count > CacheBypassLimit

	align   16
H4000:
	; 32 bytes move loop
	vmovups  ymm0, ymmword ptr [esi + edx - 20H]
	vmovaps  ymmword ptr [edi + edx - 20H], ymm0
	sub      edx, 20H
	jnz      H4000
	vzeroupper

H4090:
	sub      esi, ecx
	sub      edi, ecx

H4100:
	; remaining 0-31 bytes
	test    ecx, ecx
	jz      H4600
	test    cl, 10H
	jz      H4200
	; move 16 bytes
	sub     ecx, 10H
	movups  xmm0, xmmword ptr [esi + ecx]
	movaps  xmmword ptr [edi + ecx], xmm0
	jz      H4600                                       ; early out if count divisible by 16

H4200:
	test    cl, 8
	jz      H4300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

H4300:
	test    cl, 4
	jz      H4400
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	jz      H4600                                       ; early out if count divisible by 4

H4400:
	test    cl, 2
	jz      H4500
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

H4500:
	test    cl, 1
	jz      H4600
	; move 1 byte
	movzx   eax, byte ptr [esi]                         ; ecx-1 = 0
	mov     byte ptr [edi], al

H4600:
	; finished
	EPILOGM

H4800:
	; 32 bytes move loop, bypass cache
	vmovups  ymm0, ymmword ptr [esi + edx - 20H]
	vmovntps ymmword ptr [edi + edx - 20H], ymm0
	sub      edx, 20H
	jnz      H4800
	vzeroupper
	sfence
	jmp      H4090
	align   16

	; count < 64. Move 32-16-8-4-2-1 bytes
	; multiple CPU versions, SSSE3 and later
A1000 label near
	test    cl, 20H
	jz      A1100
	; move 32 bytes
	; movups is faster than 64-bit moves on processors with SSSE3
	sub     ecx, 20H
	movups  xmm0, xmmword ptr [esi + ecx + 10H]
	movups  xmm1, xmmword ptr [esi + ecx]
	movups  xmmword ptr [edi + ecx + 10H], xmm0
	movups  xmmword ptr [edi + ecx], xmm1

A1100:
	test    cl, 10H
	jz      A1200
	; move 16 bytes
	sub     ecx, 10H
	movups  xmm0, xmmword ptr [esi + ecx]
	movups  xmmword ptr [edi + ecx], xmm0

A1200:
	test    cl, 8
	jz      A1300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

A1300:
	test    cl, 4
	jz      A1400
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	jz      A1900                                       ; early out if count divisible by 4

A1400:
	test    cl, 2
	jz      A1500
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

A1500:
	test    cl, 1
	jz      A1900
	; move 1 byte
	movzx   eax, byte ptr [esi]                         ; ecx-1 = 0
	mov     byte ptr [edi], al

A1900:
	; finished
	EPILOGM
memmoveU256 endp

;  Version for processors with fast unaligned read and fast 16 bytes write
align 16
memmoveU proc near
	PROLOGM memcpyU

	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; Note: this part will not always work if count < 64
	; Calculate size of last block after last regular boundary of dest
	lea     edx, [edi + ecx]                            ; end of dext
	and     edx, 0FH
	jz      B3300                                       ; Skip if end of dest aligned by 16

	; edx = size of last partial block, 1 - 15 bytes
	test    dl, 3
	jz      B3210
	test    dl, 1
	jz      B3201                                       ; B3200 if we haven't tested edx,3
	; move 1 byte
	dec     ecx
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

B3200:
	test    dl, 2
	jz      B3210

B3201:
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

B3210:
	test    dl, 4
	jz      B3220
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax

B3220:
	test    dl, 8
	jz      B3300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

B3300:
	; Now end of dest is aligned by 16. Any partial block has been moved
	mov      edx, ecx
	and      ecx, 1FH                                   ; remaining size after 32 bytes blocks moved
	and      edx, -20H                                  ; number of 32 bytes blocks
	jz       H1100
	add      esi, ecx
	add      edi, ecx

	; Check if count very big
	cmp     edx, dword ptr [CacheBypassLimit]
	ja      H1800                                       ; Use non-temporal store if count > CacheBypassLimit

	align   16
H1000:
	; 32 bytes move loop
	movups   xmm1, xmmword ptr [esi + edx - 20H]
	movups   xmm0, xmmword ptr [esi + edx - 10H]
	movaps   xmmword ptr [edi + edx - 20H], xmm1
	movaps   xmmword ptr [edi + edx - 10H], xmm0
	sub      edx, 20H
	jnz      H1000

H1090:
	sub      esi, ecx
	sub      edi, ecx

H1100:
	; remaining 0-31 bytes
	test    ecx, ecx
	jz      H1600
	test    cl, 10H
	jz      H1200
	; move 16 bytes
	sub     ecx, 10H
	movups  xmm0, xmmword ptr [esi + ecx]
	movaps  xmmword ptr [edi + ecx], xmm0
	jz      H1600                                       ; early out if count divisible by 16

H1200:
	test    cl, 8
	jz      H1300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

H1300:
	test    cl, 4
	jz      H1400
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	jz      H1600                                       ; early out if count divisible by 4

H1400:
	test    cl, 2
	jz      H1500
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

H1500:
	test    cl, 1
	jz      H1600
	; move 1 byte
	movzx   eax, byte ptr [esi]                         ; ecx-1 = 0
	mov     byte ptr [edi], al

H1600:
	; finished
	EPILOGM

H1800:
	; 32 bytes move loop, bypass cache
	movups   xmm1, xmmword ptr [esi + edx - 20H]
	movups   xmm0, xmmword ptr [esi + edx - 10H]
	movntps  xmmword ptr [edi + edx - 20H], xmm1
	movntps  xmmword ptr [edi + edx - 10H], xmm0
	sub      edx, 20H
	jnz      H1800
	sfence
	jmp      H1090
memmoveU endp

;  Version for processors with SSSE3. Aligned read + shift + aligned write
align 16
memmoveSSSE3 proc near
	PROLOGM    memcpySSSE3

	cmp     ecx, 40H
	jb      A1000                                       ; Use simpler code if count < 64

	; count >= 64
	; Note: this part will not always work if count < 64
	; Calculate size of last block after last regular boundary of dest
	lea     edx, [edi + ecx]                            ; end of dext
	and     edx, 0FH
	jz      B1300                                       ; Skip if end of dest aligned by 16

	; edx = size of last partial block, 1 - 15 bytes
	test    dl, 3
	jz      B1210
	test    dl, 1
	jz      B1201                                       ; B1200 if we haven't tested edx,3
	; move 1 byte
	dec     ecx
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

B1200:
	test    dl, 2
	jz      B1210

B1201:
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

B1210:
	test    dl, 4
	jz      B1220
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax

B1220:
	test    dl, 8
	jz      B1300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

B1300:
	; Now end of dest is aligned by 16. Any partial block has been moved
	; Find alignment of end of src modulo 16 at this point:
	lea     eax, [esi + ecx]
	and     eax, 0FH

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	sub     edx, ecx                                    ; Remaining data after loop
	sub     esi, eax                                    ; Nearest preceding aligned block of src
	; Add the same to esi and edi as we have subtracted from ecx
	add     esi, edx
	add     edi, edx

	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      B1400                                       ; Use non-temporal store if count > CacheBypassLimit

	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [MAlignmentDispatchSSSE3 + eax * 4]
	align   16

B1400:
	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [MAlignmentDispatchNT + eax * 4]
	align   16

C100 label near
	; Code for aligned src. SSE2 or later instruction set
	; The nice case, src and dest have same alignment.

	; Loop. ecx has positive index from the beginning, counting down to zero
	movaps  xmm0, xmmword ptr [esi + ecx - 10H]
	movaps  xmm1, xmmword ptr [esi + ecx - 20H]
	movaps  xmmword ptr [edi + ecx - 10H], xmm0
	movaps  xmmword ptr [edi + ecx - 20H], xmm1
	sub     ecx, 20H
	jnz     C100

	; Move the remaining edx bytes (0 - 31):
	; move 16-8-4-2-1 bytes, aligned
	test    edx, edx
	jz      C500                                        ; Early out if no more data
	test    dl, 10H
	jz      C200
	; move 16 bytes
	sub     ecx, 10H
	movaps  xmm0, xmmword ptr [esi + ecx]
	movaps  xmmword ptr [edi + ecx], xmm0

C200 label near
	; Other branches come in here, ecx may contain arbitrary offset
	test    edx, edx
	jz      C500                                        ; Early out if no more data
	test    dl, 8
	jz      C210
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

C210:
	test    dl, 4
	jz      C220
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	jz      C500                                        ; Early out if count divisible by 4

C220:
	test    dl, 2
	jz      C230
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

C230:
	test    dl, 1
	jz      C500
	; move 1 byte
	movzx   eax, byte ptr [esi + ecx - 1]               ; ecx-1 not always 0
	mov     byte ptr [edi + ecx - 1], al

C500:
	; finished
	EPILOGM
memmoveSSSE3 endp

;  Version for processors with SSE2. Aligned read + shift + aligned write
align 16
memmoveSSE2 proc near
	PROLOGM memcpySSE2

	cmp     ecx, 40H
	jae     B0100                                       ; Use simpler code if count < 64

	; count < 64. Move 32-16-8-4-2-1 bytes
	test    cl, 20H
	jz      A100
	; move 32 bytes
	; movq is faster than movdqu on Intel Pentium M and Core 1
	; movdqu is faster on later processors
	sub     ecx, 20H
	movq    xmm0, qword ptr [esi + ecx + 18H]
	movq    xmm1, qword ptr [esi + ecx + 10H]
	movq    xmm2, qword ptr [esi + ecx + 8]
	movq    xmm3, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx + 18H], xmm0
	movq    qword ptr [edi + ecx + 10H], xmm1
	movq    qword ptr [edi + ecx + 8], xmm2
	movq    qword ptr [edi + ecx], xmm3

A100:
	test    cl, 10H
	jz      A200
	; move 16 bytes
	sub     ecx, 10H
	movq    xmm0, qword ptr [esi + ecx + 8]
	movq    xmm1, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx + 8], xmm0
	movq    qword ptr [edi + ecx], xmm1

A200:
	test    cl, 8
	jz      A300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

A300:
	test    cl, 4
	jz      A400
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax
	jz      A900                                        ; early out if count divisible by 4

A400:
	test    cl, 2
	jz      A500
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

A500:
	test    cl, 1
	jz      A900
	; move 1 byte
	movzx   eax, byte ptr [esi]                         ; ecx-1 = 0
	mov     byte ptr [edi], al

A900:
	; finished
	EPILOGM

B0100:
	; count >= 64
	; This part will not always work if count < 64
	; Calculate size of last block after last regular boundary of dest
	lea     edx, [edi + ecx]                            ; end of dest
	and     edx, 0FH
	jz      B0300                                       ; Skip if end of dest aligned by 16

	; edx = size of last partial block, 1 - 15 bytes
	test    dl, 3
	jz      B0210
	test    dl, 1
	jz      B0201                                       ; B0200 if we haven't tested edx,3
	; move 1 byte
	dec     ecx
	movzx   eax, byte ptr [esi + ecx]
	mov     byte ptr [edi + ecx], al

B0200:
	test    dl, 2
	jz      B0210

B0201:
	; move 2 bytes
	sub     ecx, 2
	movzx   eax, word ptr [esi + ecx]
	mov     word ptr [edi + ecx], ax

B0210:
	test    dl, 4
	jz      B0220
	; move 4 bytes
	sub     ecx, 4
	mov     eax, dword ptr [esi + ecx]
	mov     dword ptr [edi + ecx], eax

B0220:
	test    dl, 8
	jz      B0300
	; move 8 bytes
	sub     ecx, 8
	movq    xmm0, qword ptr [esi + ecx]
	movq    qword ptr [edi + ecx], xmm0

B0300:
	; Now end of dest is aligned by 16. Any partial block has been moved
	; Find alignment of end of src modulo 16 at this point:
	lea     eax, [esi + ecx]
	and     eax, 0FH

	; Set up for loop moving 32 bytes per iteration:
	mov     edx, ecx                                    ; Save count
	and     ecx, -20H                                   ; Round down to nearest multiple of 32
	sub     edx, ecx                                    ; Remaining data after loop
	sub     esi, eax                                    ; Nearest preceding aligned block of src
	; Add the same to esi and edi as we have subtracted from ecx
	add     esi, edx
	add     edi, edx

	; Check if count very big
	cmp     ecx, dword ptr [CacheBypassLimit]
	ja      B0400                                       ; Use non-temporal store if count > CacheBypassLimit

	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [MAlignmentDispatchSSE2 + eax * 4]
	align   16

B0400:
	; Dispatch to different codes depending on src alignment
	jmp     dword ptr [MAlignmentDispatchNT + eax * 4]
	align   16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  Macros and alignment jump tables
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Macros for each src alignment, SSE2 instruction set:
; Make separate code for each alignment u because the shift instructions
; have the shift count as a constant:

MOVE_REVERSE_UNALIGNED_SSE2 macro MODULO:req, NON_TEMPORAL:req
; Move ecx + edx bytes of data
; Source is misaligned. (src-dest) modulo 16 = MODULO
; NON_TEMPORAL != 0 if non-temporal store desired
; eax = MODULO
; esi = src - MODULO = nearest preceding 16-bytes boundary
; edi = dest (aligned)
; ecx = count rounded down to nearest divisible by 32
; edx = remaining bytes to move after loop
	local L1, L2

	movdqa  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest following 16B boundary

L1:
	; Loop. ecx has positive index from the beginning, counting down to zero
	sub     ecx, 20H
	movdqa  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movdqa  xmm2, xmmword ptr [esi + ecx]
	movdqa  xmm3, xmm1                                  ; Copy because used twice
	pslldq  xmm0, 16 - MODULO                           ; shift left
	psrldq  xmm1, MODULO                                ; shift right
	por     xmm0, xmm1                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + ecx + 10H], xmm0         ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + ecx + 10H], xmm0         ; Save aligned
	ENDIF
	movdqa  xmm0, xmm2                                  ; Save for next iteration
	pslldq  xmm3, 16 - MODULO                           ; shift left
	psrldq  xmm2, MODULO                                ; shift right
	por     xmm3, xmm2                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + ecx], xmm3               ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + ecx], xmm3               ; Save aligned
	ENDIF
	jnz     L1

	; Move edx remaining bytes
	test    dl, 10H
	jz      L2
	; One more 16-bytes block to move
	sub     ecx, 10H
	movdqa  xmm1, xmmword ptr [esi + ecx]
	pslldq  xmm0, 16 - MODULO                           ; shift left
	psrldq  xmm1, MODULO                                ; shift right
	por     xmm0, xmm1                                  ; combine blocks
	IF NON_TEMPORAL EQ 0
	movdqa  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntdq xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ENDIF

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

MOVE_REVERSE_UNALIGNED_SSE2_4 macro NON_TEMPORAL:req
; Special case: u = 4
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest following 16B boundary

L1:
	; Loop. ecx has positive index from the beginning, counting down to zero
	sub     ecx, 20H
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	movaps  xmm2, xmmword ptr [esi + ecx]
	movaps  xmm3, xmm0
	movaps  xmm0, xmm2
	movss   xmm2, xmm1
	shufps  xmm2, xmm2, 00111001B                       ; Rotate right
	movss   xmm1, xmm3
	shufps  xmm1, xmm1, 00111001B                       ; Rotate right
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx + 10H], xmm1         ; Save aligned
	movaps  xmmword ptr [edi + ecx], xmm2               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx + 10H], xmm1         ; Non-temporal save
	movntps xmmword ptr [edi + ecx], xmm2               ; Non-temporal save
	ENDIF
	jnz     L1

	; Move edx remaining bytes
	test    dl, 10H
	jz      L2
	; One more 16-bytes block to move
	sub     ecx, 10H
	movaps  xmm1, xmmword ptr [esi + ecx]
	movss   xmm1, xmm0
	shufps  xmm1, xmm1, 00111001B                       ; Rotate right
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm1               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm1               ; Non-temporal save
	ENDIF

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

MOVE_REVERSE_UNALIGNED_SSE2_8 macro NON_TEMPORAL:req
; Special case: u = 8
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest following 16B boundary
	shufps  xmm0, xmm0, 01001110B                       ; Rotate

L1:
	; Loop. ecx has positive index from the beginning, counting down to zero
	sub     ecx, 20H
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	shufps  xmm1, xmm1, 01001110B                       ; Rotate
	movsd   xmm0, xmm1
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx + 10H], xmm0         ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx + 10H], xmm0         ; Non-temporal save
	ENDIF
	movaps  xmm0, xmmword ptr [esi + ecx]
	shufps  xmm0, xmm0, 01001110B                       ; Rotate
	movsd   xmm1, xmm0
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm1               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm1               ; Non-temporal save
	ENDIF
	jnz     L1

	; Move edx remaining bytes
	test    dl, 10H
	jz      L2
	; One more 16-bytes block to move
	sub     ecx, 10H
	movaps  xmm1, xmmword ptr [esi + ecx]
	shufps  xmm1, xmm1, 01001110B                       ; Rotate
	movsd   xmm0, xmm1
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm0               ; Non-temporal save
	ENDIF

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

MOVE_REVERSE_UNALIGNED_SSE2_12 macro NON_TEMPORAL:req
; Special case: u = 12
	local L1, L2

	movaps  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest following 16B boundary
	shufps  xmm0, xmm0, 10010011B                       ; Rotate right

L1:
	; Loop. ecx has positive index from the beginning, counting down to zero
	sub     ecx, 20H
	movaps  xmm1, xmmword ptr [esi + ecx + 10H]         ; Read next two blocks aligned
	shufps  xmm1, xmm1, 10010011B                       ; Rotate left
	movss   xmm0, xmm1
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx + 10H], xmm0         ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx + 10H], xmm0         ; Non-temporal save
	ENDIF
	movaps  xmm0, xmmword ptr [esi + ecx]
	shufps  xmm0, xmm0, 10010011B                       ; Rotate left
	movss   xmm1, xmm0
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm1               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm1               ; Non-temporal save
	ENDIF
	jnz     L1

	; Move edx remaining bytes
	test    dl, 10H
	jz      L2
	; One more 16-bytes block to move
	sub     ecx, 10H
	movaps  xmm1, xmmword ptr [esi + ecx]
	shufps  xmm1, xmm1, 10010011B                       ; Rotate left
	movss   xmm0, xmm1
	IF NON_TEMPORAL EQ 0
	movaps  xmmword ptr [edi + ecx], xmm0               ; Save aligned
	ELSE
	movntps xmmword ptr [edi + ecx], xmm0               ; Non-temporal save
	ENDIF

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
; Code for unaligned src, Suppl.SSE3 instruction set.
; Make separate code for each alignment u because the palignr instruction
; has the shift count as a constant:

MOVE_REVERSE_UNALIGNED_SSSE3 macro MODULO:req
; Move ecx + edx bytes of data
; Source is misaligned. (src-dest) modulo 16 = MODULO
; eax = MODULO
; esi = src - MODULO = nearest preceding 16-bytes boundary
; edi = dest (aligned)
; ecx = - (count rounded down to nearest divisible by 32)
; edx = remaining bytes to move after loop
	local L1, L2

	movdqa  xmm0, xmmword ptr [esi + ecx]               ; Read from nearest following 16B boundary

L1:
	; Loop. ecx has positive index from the beginning, counting down to zero
	movdqa  xmm1, xmmword ptr [esi + ecx - 10H]         ; Read next two blocks
	palignr xmm0, xmm1, MODULO                          ; Combine parts into aligned block
	movdqa  xmmword ptr [edi + ecx - 10H], xmm0         ; Save aligned
	movdqa  xmm0, xmmword ptr [esi + ecx - 20H]
	palignr xmm1, xmm0, MODULO                          ; Combine parts into aligned block
	movdqa  xmmword ptr [edi + ecx - 20H], xmm1         ; Save aligned
	sub     ecx, 20H
	jnz     L1

	; Set up for edx remaining bytes
	test    dl, 10H
	jz      L2
	; One more 16-bytes block to move
	sub     ecx, 10H
	movdqa  xmm1, xmmword ptr [esi + ecx]               ; Read next two blocks
	palignr xmm0, xmm1, MODULO                          ; Combine parts into aligned block
	movdqa  xmmword ptr [edi + ecx], xmm0               ; Save aligned

L2:
	; Get src pointer back to misaligned state
	add     esi, eax
	; Move remaining 0 - 15 bytes
	jmp     C200
	align   16
endm

; Make 15 instances of SSE2 macro for each value of the alignment u.
; These are pointed to by the jump table MAlignmentDispatchSSE2 below
; (aligns and fillers are inserted manually to minimize the
;  number of 16-bytes boundaries inside loops)

D104 label near
	MOVE_REVERSE_UNALIGNED_SSE2_4    0
D108 label near
	MOVE_REVERSE_UNALIGNED_SSE2_8    0
D10C label near
	MOVE_REVERSE_UNALIGNED_SSE2_12   0
D101 label near
	MOVE_REVERSE_UNALIGNED_SSE2 1,   0
D102 label near
	MOVE_REVERSE_UNALIGNED_SSE2 2,   0
D103 label near
	MOVE_REVERSE_UNALIGNED_SSE2 3,   0
D105 label near
	MOVE_REVERSE_UNALIGNED_SSE2 5,   0
D106 label near
	MOVE_REVERSE_UNALIGNED_SSE2 6,   0
D107 label near
	MOVE_REVERSE_UNALIGNED_SSE2 7,   0
D109 label near
	MOVE_REVERSE_UNALIGNED_SSE2 9,   0
D10A label near
	MOVE_REVERSE_UNALIGNED_SSE2 0AH, 0
D10B label near
	MOVE_REVERSE_UNALIGNED_SSE2 0BH, 0
D10D label near
	MOVE_REVERSE_UNALIGNED_SSE2 0DH, 0
D10E label near
	MOVE_REVERSE_UNALIGNED_SSE2 0EH, 0
D10F label near
	MOVE_REVERSE_UNALIGNED_SSE2 0FH, 0

; Make 15 instances of Sup.SSE3 macro for each value of the alignment u.
; These are pointed to by the jump table MAlignmentDispatchSupSSE3 below

E104 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 4
E108 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 8
E10C label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0CH
E101 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 1
E102 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 2
E103 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 3
E105 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 5
E106 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 6
E107 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 7
E109 label near
	MOVE_REVERSE_UNALIGNED_SSSE3 9
E10A label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0AH
E10B label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0BH
E10D label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0DH
E10E label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0EH
E10F label near
	MOVE_REVERSE_UNALIGNED_SSSE3 0FH

F100 label near
	; Non-temporal move, src and dest have same alignment.
	; Loop. ecx has positive index from the beginning, counting down to zero
	sub     ecx, 20H
	movaps  xmm0, xmmword ptr [esi + ecx + 10H]
	movaps  xmm1, xmmword ptr [esi + ecx]
	movntps xmmword ptr [edi + ecx + 10H], xmm0
	movntps xmmword ptr [edi + ecx], xmm1
	jnz     F100

	; Move the remaining edx bytes (0 - 31):
	; move 16-8-4-2-1 bytes, aligned
	test    dl, 10H
	jz      C200
	; move 16 bytes
	sub     ecx, 10H
	movaps  xmm0, xmmword ptr [esi + ecx]
	movntps  xmmword ptr [edi + ecx], xmm0
	sfence
	; move the remaining 0 - 15 bytes
	jmp     C200
	align   16

; Non-temporal move, src and dest have different alignment.
; Make 15 instances of SSE2 macro for each value of the alignment u.
; These are pointed to by the jump table MAlignmentDispatchNT below

F104 label near
	MOVE_REVERSE_UNALIGNED_SSE2_4    1
F108 label near
	MOVE_REVERSE_UNALIGNED_SSE2_8    1
F10C label near
	MOVE_REVERSE_UNALIGNED_SSE2_12   1
F101 label near
	MOVE_REVERSE_UNALIGNED_SSE2 1,   1
F102 label near
	MOVE_REVERSE_UNALIGNED_SSE2 2,   1
F103 label near
	MOVE_REVERSE_UNALIGNED_SSE2 3,   1
F105 label near
	MOVE_REVERSE_UNALIGNED_SSE2 5,   1
F106 label near
	MOVE_REVERSE_UNALIGNED_SSE2 6,   1
F107 label near
	MOVE_REVERSE_UNALIGNED_SSE2 7,   1
F109 label near
	MOVE_REVERSE_UNALIGNED_SSE2 9,   1
F10A label near
	MOVE_REVERSE_UNALIGNED_SSE2 0AH, 1
F10B label near
	MOVE_REVERSE_UNALIGNED_SSE2 0BH, 1
F10D label near
	MOVE_REVERSE_UNALIGNED_SSE2 0DH, 1
F10E label near
	MOVE_REVERSE_UNALIGNED_SSE2 0EH, 1
F10F label near
	MOVE_REVERSE_UNALIGNED_SSE2 0FH, 1
memmoveSSE2 endp

; 80386 version used when SSE2 not supported:
align 16
memmove386 proc near
	PROLOGM memcpy386
; edi = dest
; esi = src
; ecx = count
	std                                                 ; Move backwards
	lea     edi, [edi + ecx - 1]                        ; Point to last byte of dest
	lea     esi, [esi + ecx - 1]                        ; Point to last byte of src
	cmp     ecx, 8
	jb      G500

G100:
	test    edi, 3                                      ; Test if unaligned
	jz      G200
	movsb
	dec     ecx
	jmp     G100                                        ; Repeat while edi unaligned
	align   16

G200:
	; edi is aligned now. Move 4 bytes at a time
	sub     edi, 3                                      ; Point to last dword of dest
	sub     esi, 3                                      ; Point to last dword of src
	mov     edx, ecx
	shr     ecx, 2
	rep     movsd                                       ; move 4 bytes at a time
	mov     ecx, edx
	and     ecx, 3
	add     edi, 3                                      ; Point to last byte of dest
	add     esi, 3                                      ; Point to last byte of src
	rep     movsb                                       ; move remaining 0-3 bytes
	cld
	EPILOGM

G500:
	; count < 8. Move one byte at a time
	rep     movsb                                       ; move count bytes
	cld
	EPILOGM
memmove386 endp

; CPU dispatching for memmove. This is executed only once
align 16
memmoveCPUDispatch proc near
	pushad
	; set CacheBypassLimit to half the size of the largest level cache
	push    0
	call    SetMemcpyCacheLimit
	pop     ecx
	call    _InstructionSet
	mov     ebx, eax
	mov     esi, offset memmove386                      ; Point to generic version of memmove
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	mov     esi, offset memmoveSSE2                     ; Point to SSE2 version of memmove
	cmp     eax, 6                                      ; check Suppl-SSE3
	jb      Q100
	; Suppl-SSE3 supported
	mov     esi, offset memmoveSSSE3                    ; Point to SSSE3 version of memmove
	call    _UnalignedIsFaster
	test    eax, eax
	jz      Q100
	mov     esi, offset memmoveU                        ; Point to unaligned version of memmove
	call    _Store256BitIsFaster
	test    eax, eax
	jz      Q100
	mov     esi, offset memmoveU256                     ; Point to 256 bit move version of memmove
	cmp     ebx, 15
	jb      Q100
	mov     esi, offset memmoveAVX512F
	cmp     ebx, 16
	jb      Q100
	mov     esi, offset memmoveAVX512BW

Q100:
	mov     dword ptr [memmoveDispatch], esi
	popad
	; Continue in appropriate version of memmove
	jmp     dword ptr [memmoveDispatch]
memmoveCPUDispatch endp

align 16
SetMemcpyCacheLimit proc near
	mov     eax, dword ptr [esp + 4]
	push    eax
	call    SetMemcpyCacheLimit1
	pop     ecx
	mov     dword ptr [CacheBypassLimit], eax
	ret
SetMemcpyCacheLimit endp

end
