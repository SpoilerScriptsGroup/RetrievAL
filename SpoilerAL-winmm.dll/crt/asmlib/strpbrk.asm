.686
.xmm
.model flat

include align.inc

public _strpbrk

extern InstructionSet: near

.data

strpbrkDispatch dd strpbrkCPUDispatch

.code

; function dispatching
_strpbrk proc near
	jmp     dword ptr [strpbrkDispatch]                 ; Go to appropriate version, depending on instruction set
	$align  16
_strpbrk endp

; SSE4.2 version
strpbrkSSE42 proc near
	push    esi
	mov     ecx, dword ptr [esp + 8]                    ; str
	mov     esi, dword ptr [esp + 12]                   ; set

str_next2:
	movdqu  xmm2, xmmword ptr [ecx]                     ; str
	movdqu  xmm1, xmmword ptr [esi]                     ; set
	pcmpistrm xmm1, xmm2, 00110000b                     ; find in set, invert valid bits, return bit mask in xmm0
	movd    eax, xmm0
	jns     set_extends2

set_finished2:
	cmp     ax, -1
	jne     str_finished2
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add     ecx, 16                                     ; next 16 bytes of str
	jmp     str_next2

str_finished2:
	xor     eax, -1
	xor     edx, edx
	bsf     eax, eax
	mov     dl, byte ptr [eax + ecx]
	add     eax, ecx
	test    dl, dl
	pop     esi
	cmovz   eax, edx
	ret

set_loop2:
	and     eax, edx                                    ; accumulate matches

set_extends2:
	; the set is more than 16 bytes
	add     esi, 16
	movdqu  xmm1, xmmword ptr [esi]                     ; next part of set
	pcmpistrm xmm1, xmm2, 00110000b                     ; find in set, invert valid bits, return bit mask in xmm0
	movd    edx, xmm0
	jns     set_loop2
	mov     esi, dword ptr [esp + 16]                   ; restore set pointer
	and     eax, edx                                    ; accumulate matches
	jmp     set_finished2
	$align  16
strpbrkSSE42 endp

; Generic version
;
; Algorithm:
;	char * __cdecl strpbrk(const char *string, const char *control)
;	{
;		unsigned char map[256 / 8];
;		size_t        index;
;
;		((size_t *)map)[0] = 1;
;		for (index = 1; index < (sizeof(map) / sizeof(size_t)); index++)
;			((size_t *)map)[index] = 0;
;		for (; *control; control++)
;			map[(unsigned char)*control >> 3] |= (1 << (*control & 7));
;		while (!(map[(unsigned char)*string >> 3] & (1 << (*string & 7))))
;			string++;
;		return *string ? string : NULL;
;	}
;
strpbrkGeneric proc near
	string  equ (esp + 4)
	control equ (esp + 8)

	; create and zero out char bit map
	mov     eax, dword ptr [string]                     ; eax = string
	mov     edx, dword ptr [control]                    ; edx = control
	xor     ecx, ecx
	push    1                                           ; 32
	push    ecx
	push    ecx
	push    ecx                                         ; 128
	push    ecx
	push    ecx
	push    ecx
	push    ecx                                         ; 256

	map     equ (esp)

	; Set control char bits in map
	jmp     listinit

	$align  16
listnext:
	; init char bit map
	bts     dword ptr [map], ecx

listinit:
	mov     cl, byte ptr [edx]
	inc     edx
	test    cl, cl
	jnz     listnext

	; Loop through comparing source string with control bits
	align   16
dstnext:
	mov     cl, byte ptr [eax]
	inc     eax
	bt      dword ptr [map], ecx
	jnc     dstnext                                     ; did not find char, continue

	; Return code
dstdone:
	dec     eax
	test    ecx, ecx
	cmovz   eax, ecx
	add     esp, 32
	ret                                                 ; _cdecl return
strpbrkGeneric endp

; CPU dispatching for strpbrk. This is executed only once
strpbrkCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet
	; Point to generic version of strstr
	mov     ecx, offset strpbrkGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q200
	; SSE4.2 supported
	; Point to SSE4.2 version of strstr
	mov     ecx, offset strpbrkSSE42
Q200:
	mov     dword ptr [strpbrkDispatch], ecx
	; Continue in appropriate version
	jmp     ecx
	$align  16
strpbrkCPUDispatch endp

end
