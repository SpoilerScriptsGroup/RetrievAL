.686
.xmm
.model flat

include align.inc

public _strstr

extern InstructionSet: near

.data

strstrDispatch dd strstrCPUDispatch

.code

; function dispatching
$align 16
_strstr proc near
	jmp     dword ptr [strstrDispatch]                  ; Go to appropriate version, depending on instruction set
_strstr endp

; SSE4.2 version
$align 16
strstrSSE42 proc near
	push    ebx
	push    esi
	mov     esi, dword ptr [esp + 12]                   ; haystack
	mov     eax, dword ptr [esp + 16]                   ; needle
	movdqu  xmm1, xmmword ptr [eax]                     ; needle

	align   16
haystacknext:
	; [esi] = haystack
	pcmpistrm xmm1, xmmword ptr [esi], 00001100b        ; unsigned byte search, equal ordered, return mask in xmm0
	jc      matchbegin                                  ; found beginning of a match
	jz      nomatch                                     ; end of haystack found, no match
	add     esi, 16
	jmp     haystacknext

matchbegin:
	jz      foundshort                                  ; haystack ends here, a short match is found
	movd    eax, xmm0                                   ; bit mask of possible matches

nextindexbit:
	bsf     ecx, eax                                    ; index of first bit in mask of possible matches

	; compare strings for full match
	lea     ebx, [esi + ecx]                            ; haystack + index
	mov     edx, dword ptr [esp + 16]                   ; needle

compareloop:
	; compare loop for long match
	movdqu  xmm2, [edx]                                 ; paragraph of needle
	pcmpistrm xmm2, xmmword ptr [ebx], 00001100B        ; unsigned bytes, equal ordered, modifies xmm0
	; (can't use "equal each, masked" because it inverts when past end of needle, but not when past end of both)

	jno     longmatchfail                               ; difference found after extending partial match
	js      longmatchsuccess                            ; end of needle found, and no difference
	add     edx, 16
	add     ebx, 16
	jmp     compareloop                                 ; loop to next 16 bytes

longmatchfail:
	; remove index bit of first partial match
	btr     eax, ecx
	test    eax, eax
	jnz     nextindexbit                                ; mask contains more index bits, loop to next bit in eax mask
	; mask exhausted for possible matches, continue to next haystack paragraph
	add     esi, 16
	jmp     haystacknext                                ; loop to next paragraph of haystack

longmatchsuccess:
	; match found over more than one paragraph
	lea     eax, [esi + ecx]                            ; haystack + index to begin of long match
	pop     esi
	pop     ebx
	ret

foundshort:
	; match found within single paragraph
	movd    eax, xmm0                                   ; bit mask of matches
	bsf     eax, eax                                    ; index of first match
	add     eax, esi                                    ; pointer to first match
	pop     esi
	pop     ebx
	ret

nomatch:
	; needle not found, return 0
	xor     eax, eax
	pop     esi
	pop     ebx
	ret
strstrSSE42 endp

; SSE2 version
$align 16
strstrSSE2 proc near
	mov     ecx, dword ptr [esp + 8]
	mov     eax, dword ptr [esp + 4]
	push    ebx
	push    esi
	push    edi
	movzx   edx, byte ptr [ecx]
	mov     ebx, edx
	shl     edx, 8
	or      edx, ebx
	jz      L9
	movd    xmm3, edx
	pshuflw xmm3, xmm3, 00H
	movlhps xmm3, xmm3
	pxor    xmm0, xmm0
	mov     esi, ecx
	or      edi, -1

L1:
	movzx   ebx, byte ptr [ecx]
	add     ecx, 1
	test    ebx, ebx
	jz      L2
	test    ecx, 0FH
	jnz     L1
	movdqa  xmm2, xmmword ptr [ecx]
	pcmpeqb xmm2, xmm0
	pmovmskb ebx, xmm2
	test    ebx, ebx
	jnz     L2
	mov     edi, 15

L2:
	movd    edx, xmm3

L3:
	mov     ebx, 0FFFH
	and     ebx, eax
	cmp     ebx, 0FF0H
	ja      L4
	movdqu  xmm1, xmmword ptr [eax]
	pxor    xmm2, xmm2
	pcmpeqb xmm2, xmm1
	pcmpeqb xmm1, xmm3
	por     xmm1, xmm2
	pmovmskb ebx, xmm1
	add     eax, 16
	test    ebx, ebx
	jz      L3
	bsf     ebx, ebx
	sub     eax, 16
	add     eax, ebx

L4:
	movzx   ebx, byte ptr [eax]
	test    ebx, ebx
	jz      L8
	add     eax, 1
	cmp     dl, bl
	jne     L3
	mov     edx, eax
	lea     ecx, [esi + 1]

L5:
	mov     ebx, 0FFFH
	test    edi, ecx
	jnz     L7
	and     ebx, edx
	cmp     ebx, 0FF0H
	ja      L7
	movdqu  xmm1, xmmword ptr [edx]
	movdqa  xmm2, xmmword ptr [ecx]
	pcmpeqb xmm1, xmm2
	pcmpeqb xmm2, xmm0
	pcmpeqb xmm1, xmm0
	por     xmm2, xmm1
	pmovmskb ebx, xmm2
	test    ebx, ebx
	jnz     L6
	add     edx, 16
	add     ecx, 16
	jmp     L5
	$align  16

L6:
	bsf     ebx, ebx
	add     edx, ebx
	add     ecx, ebx

L7:
	movzx   ebx, byte ptr [ecx]
	test    ebx, ebx
	jz      L10
	cmp     bl, byte ptr [edx]
	jne     L2
	add     edx, 1
	add     ecx, 1
	jmp     L5
	$align  16

L8:
	xor     eax, eax

L9:
	pop     edi
	pop     esi
	pop     ebx
	ret
	$align  16

L10:
	dec     eax
	pop     edi
	pop     esi
	pop     ebx
	ret
strstrSSE2 endp

; generic version
$align 16
strstrGeneric proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; haystack
	mov     edi, dword ptr [esp + 16]                   ; needle

	mov     ax, word ptr [edi]
	test    al, al
	jz      Found                                       ; a zero-length needle is always found
	test    ah, ah
	jz      SingleCharNeedle

SearchLoop:
	; search for first character match
	mov     cl, byte ptr [esi]
	test    cl, cl
	jz      NotFound                                    ; end of haystack reached without finding
	cmp     al, cl
	je      FirstCharMatch                              ; first character match

IncompleteMatch:
	inc     esi
	jmp     SearchLoop                                  ; loop through haystack

FirstCharMatch:
	mov     ecx, esi                                    ; begin of match position

MatchLoop:
	inc     ecx
	inc     edi
	mov     al, byte ptr [edi]
	test    al, al
	jz      Found                                       ; end of needle. match ok
	cmp     al, byte ptr [ecx]
	je      MatchLoop
	; match failed, recover and continue
	mov     edi, dword ptr [esp + 16]                   ; needle
	mov     al, byte ptr [edi]
	jmp     IncompleteMatch

NotFound:
	; needle not found. return 0
	xor     eax, eax
	pop     edi
	pop     esi
	ret

Found:
	; needle found. return pointer to position in haystack
	mov     eax, esi
	pop     edi
	pop     esi
	ret

SingleCharNeedle:
	; Needle is a single character
	movzx   ecx, byte ptr [esi]
	test    cl, cl
	jz      NotFound                                    ; end of haystack reached without finding
	cmp     al, cl
	je      Found
	inc     esi
	jmp     SingleCharNeedle                            ; loop through haystack
strstrGeneric endp

; CPU dispatching for strstr. This is executed only once
$align 16
strstrCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strstrGeneric
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	; Point to SSE2 version of strstr
	mov     ecx, offset strstrSSE2
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q100
	; SSE4.2 supported
	; Point to SSE4.2 version of strstr
	mov     ecx, offset strstrSSE42

Q100:
	mov     dword ptr [strstrDispatch], ecx
	; Continue in appropriate version of strstr
	jmp     ecx
strstrCPUDispatch endp

end
