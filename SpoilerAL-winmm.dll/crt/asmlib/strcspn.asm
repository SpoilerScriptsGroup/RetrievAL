.686
.xmm
.model flat

public _strcspn

extern InstructionSet: near

.data

strcspnDispatch dd strcspnCPUDispatch

.code

; function dispatching
align 16
_strcspn proc near
	jmp     dword ptr [strcspnDispatch]                 ; Go to appropriate version, depending on instruction set
_strcspn endp

; SSE4.2 version
align 16
strcspnSSE42 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; str
	mov     edi, dword ptr [esp + 16]                   ; set
	xor     ecx, ecx                                    ; span counter

str_next2:
	movdqu  xmm2, xmmword ptr [esi]                     ; str
	movdqu  xmm1, xmmword ptr [edi]                     ; set
	pcmpistrm xmm1, xmm2, 00110000b                     ; find in set, invert valid bits, return bit mask in xmm0
	movd    eax, xmm0
	jns     set_extends2

set_finished2:
	cmp     ax, -1
	jne     str_finished2
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add     esi, 16                                     ; next 16 bytes of str
	add     ecx, 16                                     ; count span
	jmp     str_next2

str_finished2:
	not     eax
	bsf     eax, eax
	add     eax, ecx
	pop     edi
	pop     esi
	ret

set_loop2:
	and     eax, edx                                    ; accumulate matches

set_extends2:
	; the set is more than 16 bytes
	add     edi, 16
	movdqu  xmm1, xmmword ptr [edi]                     ; next part of set
	pcmpistrm xmm1, xmm2, 00110000b                     ; find in set, invert valid bits, return bit mask in xmm0
	movd    edx, xmm0
	jns     set_loop2
	mov     edi, dword ptr [esp + 16]                   ; restore set pointer
	and     eax, edx                                    ; accumulate matches
	jmp     set_finished2
strcspnSSE42 endp

; Generic version
;
; Algorithm:
;	int __cdecl strcspn(const char *string, const char *control)
;	{
;		unsigned char map[256 / 8] = { 1, 0 }, c;
;		size_t        index;
;
;		for (; *control; control++)
;			map[(unsigned char)*control >> 3] |= (1 << (*control & 7));
;		index = -1;
;		string++;
;		do
;			c = string[index++];
;		while (!(map[c >> 3] & (1 << (c & 7))));
;		return index;
;	}
;
align 16
strcspnGeneric proc near
	string  equ (esp + 4)
	control equ (esp + 8)

	; create and zero out char bit map
	mov     edx, dword ptr [string]                     ; edx = string
	mov     eax, dword ptr [control]                    ; eax = control
	xor     ecx, ecx
	push    0                                           ; 256
	push    ecx                                         ; 224
	push    ecx                                         ; 192
	push    ecx                                         ; 160
	push    ecx                                         ; 128
	push    ecx                                         ;  96
	push    ecx                                         ;  64
	push    1                                           ;  32

	map     equ (esp)

	; Set control char bits in map
	jmp     listinit

	align   16
listnext:
	; init char bit map
	bts     dword ptr [map], ecx

listinit:
	mov     cl, byte ptr [eax]
	inc     eax
	test    cl, cl
	jnz     listnext

	; Loop through comparing source string with control bits
	or      eax, -1                                     ; set eax to -1
	inc     edx                                         ; edx = string + 1

	align   16
dstnext:
	mov     cl, byte ptr [eax + edx]
	inc     eax
	bt      dword ptr [map], ecx
	jnc     dstnext                                     ; did not find char, continue

	; Return code
	add     esp, 32
	ret                                                 ; __cdecl return
strcspnGeneric endp

; CPU dispatching for strcspn. This is executed only once
align 16
strcspnCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strcspnGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q200
	; SSE4.2 supported
	; Point to SSE4.2 version of strstr
	mov     ecx, offset strcspnSSE42
Q200:
	mov     dword ptr [strcspnDispatch], ecx
	; Continue in appropriate version
	jmp     ecx
strcspnCPUDispatch endp

end
