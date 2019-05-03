.686
.xmm
.model flat

public _strstr

extern InstructionSet: near

.data

strstrDispatch dd strstrCPUDispatch

.code

; function dispatching
_strstr proc near
	jmp     dword ptr [strstrDispatch]              ; Go to appropriate version, depending on instruction set
_strstr endp

; SSE4.2 version
strstrSSE42 proc near
	push    ebx
	push    esi
	mov     esi, dword ptr [esp + 12]               ; haystack
	mov     eax, dword ptr [esp + 16]               ; needle
	movdqu  xmm1, xmmword ptr [eax]                 ; needle

	align   16
haystacknext:   
	; [esi] = haystack
	pcmpistrm xmm1, xmmword ptr [esi], 00001100b    ; unsigned byte search, equal ordered, return mask in xmm0
	jc      matchbegin                              ; found beginning of a match
	jz      nomatch                                 ; end of haystack found, no match
	add     esi, 16
	jmp     haystacknext

matchbegin:
	jz      foundshort                              ; haystack ends here, a short match is found
	movd    eax, xmm0                               ; bit mask of possible matches

nextindexbit:
	bsf     ecx, eax                                ; index of first bit in mask of possible matches

	; compare strings for full match
	lea     ebx, [esi + ecx]                        ; haystack + index
	mov     edx, dword ptr [esp + 16]               ; needle

compareloop:
	; compare loop for long match
	movdqu  xmm2, [edx]                             ; paragraph of needle
	pcmpistrm xmm2, xmmword ptr [ebx], 00001100B    ; unsigned bytes, equal ordered, modifies xmm0
	; (can't use "equal each, masked" because it inverts when past end of needle, but not when past end of both)

	jno     longmatchfail                           ; difference found after extending partial match
	js      longmatchsuccess                        ; end of needle found, and no difference
	add     edx, 16
	add     ebx, 16
	jmp     compareloop                             ; loop to next 16 bytes

longmatchfail:
	; remove index bit of first partial match
	btr     eax, ecx
	test    eax, eax
	jnz     nextindexbit                            ; mask contains more index bits, loop to next bit in eax mask
	; mask exhausted for possible matches, continue to next haystack paragraph
	add     esi, 16
	jmp     haystacknext                            ; loop to next paragraph of haystack

longmatchsuccess:
	; match found over more than one paragraph
	lea     eax, [esi + ecx]                        ; haystack + index to begin of long match
	pop     esi
	pop     ebx
	ret

foundshort:
	; match found within single paragraph
	movd    eax, xmm0                               ; bit mask of matches
	bsf     eax, eax                                ; index of first match
	add     eax, esi                                ; pointer to first match
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

; generic version
strstrGeneric proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]               ; haystack
	mov     edi, dword ptr [esp + 16]               ; needle

	mov     ax, word ptr [edi]
	test    al, al
	jz      Found                                   ; a zero-length needle is always found
	test    ah, ah
	jz      SingleCharNeedle

SearchLoop:
	; search for first character match
	mov     cl, byte ptr [esi]
	test    cl, cl
	jz      NotFound                                ; end of haystack reached without finding
	cmp     al, cl
	je      FirstCharMatch                          ; first character match

IncompleteMatch:
	inc     esi
	jmp     SearchLoop                              ; loop through haystack

FirstCharMatch:
	mov     ecx, esi                                ; begin of match position

MatchLoop:
	inc     ecx
	inc     edi
	mov     al, byte ptr [edi]
	test    al, al
	jz      Found                                   ; end of needle. match ok
	cmp     al, byte ptr [ecx] 
	je      MatchLoop
	; match failed, recover and continue
	mov     edi, dword ptr [esp + 16]               ; needle
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
	jz      NotFound                                ; end of haystack reached without finding
	cmp     al, cl
	je      Found
	inc     esi
	jmp     SingleCharNeedle                        ; loop through haystack
strstrGeneric endp

; CPU dispatching for strstr. This is executed only once
strstrCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strstrGeneric
	cmp     eax, 10                ; check SSE4.2
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
