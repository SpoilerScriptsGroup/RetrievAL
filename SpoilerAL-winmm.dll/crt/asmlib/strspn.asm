.686
.xmm
.model flat

include align.inc

public _strspn

extern InstructionSet: near

.data

strspnDispatch dd strspnCPUDispatch

.code

; function dispatching
_strspn proc near
	jmp     dword ptr [strspnDispatch]                  ; Go to appropriate version, depending on instruction set
	$align  16
_strspn endp

; SSE4.2 version
strspnSSE42 proc near
	push    esi
	push    edi
	mov     esi, dword ptr [esp + 12]                   ; str
	mov     edi, dword ptr [esp + 16]                   ; set
	xor     ecx, ecx                                    ; span counter

str_next:
	movdqu  xmm2, xmmword ptr [esi]                     ; str
	movdqu  xmm1, xmmword ptr [edi]                     ; set
	pcmpistrm xmm1, xmm2, 00000000b                     ; find in set, return bit mask in xmm0
	movd    eax, xmm0
	jns     set_extends

set_finished:
	cmp     ax, -1
	jne     str_finished
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add     esi, 16                                     ; next 16 bytes of str
	add     ecx, 16                                     ; count span
	jmp     str_next

str_finished:
	not     eax
	bsf     eax, eax
	add     eax, ecx
	pop     edi
	pop     esi
	ret

set_loop:
	or      eax, edx                                    ; accumulate matches

set_extends:
	; the set is more than 16 bytes
	add     edi, 16
	movdqu  xmm1, xmmword ptr [edi]                     ; next part of set
	pcmpistrm xmm1, xmm2, 00000000b                     ; find in set, return bit mask in xmm0
	movd    edx, xmm0
	jns     set_loop
	mov     edi, dword ptr [esp + 16]                   ; restore set pointer
	or      eax, edx                                    ; accumulate matches
	jmp     set_finished
	$align  16
strspnSSE42 endp

; Generic version
;
; Algorithm:
;	int __cdecl strspn(const char *string, const char *control)
;	{
;		unsigned char map[256 / 8] = { 0 }, c;
;		size_t        index;
;
;		for (; *control; control++)
;			map[(unsigned char)*control >> 3] |= (1 << (*control & 7));
;		index = -1;
;		string++;
;		do
;			c = string[index++];
;		while (map[c >> 3] & (1 << (c & 7)));
;		return index;
;	}
;
strspnGeneric proc near
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
	push    ecx                                         ;  32

	map     equ (esp)

	; Set control char bits in map
	jmp     listinit

	$align  16
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
	jc      dstnext                                     ; found char, continue

	; Return code
	add     esp, 32
	ret                                                 ; __cdecl return
strspnGeneric endp

; CPU dispatching for strspn. This is executed only once
strspnCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strspnGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q100
	; SSE4.2 supported
	; Point to SSE4.2 version of strstr
	mov     ecx, offset strspnSSE42

Q100:
	mov     dword ptr [strspnDispatch], ecx
	; Continue in appropriate version
	jmp     ecx
	$align  16
strspnCPUDispatch endp

end
