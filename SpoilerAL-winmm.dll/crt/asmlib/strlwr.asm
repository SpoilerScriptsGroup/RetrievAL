.686
.xmm
.model flat

include align.inc

public __strlwr

extern InstructionSet: near
extern struprlwrSSE42: near
extern casebit: near

.const

align   16

azhigh  db 'AZAZAZAZAZAZAZAZ'                           ; define range for upper case

.data

; Pointer to appropriate version. Initially points to dispatcher
strlwrDispatch dd strlwrCPUDispatch

.code

$align 16
__strlwr proc near
	jmp     dword ptr [strlwrDispatch]                  ; Go to appropriate version, depending on instruction set
__strlwr endp

$align 16
strlwrSSE42 proc near
	movdqa  xmm1, xmmword ptr [azhigh]                  ; define range A-Z
	movdqa  xmm3, xmmword ptr [casebit]                 ; bit to change
	jmp     struprlwrSSE42
strlwrSSE42 endp

; 386 version
$align 16
strlwrGeneric proc near
	mov     edx, dword ptr [esp + 4]                    ; string

A100:
	; loop
	mov     al, byte ptr [edx]
	test    al, al
	jz      A900                                        ; end of string
	sub     al, 'A'
	cmp     al, 'Z' - 'A'
	jbe     A200                                        ; is upper case
	inc     edx
	jmp     A100                                        ; loop to next character

A200:
	; convert to lower case
	add     al, 'a'
	mov     byte ptr [edx], al
	inc     edx
	jmp     A100

A900:
	mov     eax, dword ptr [esp + 4]                    ; string
	ret
strlwrGeneric endp

; CPU dispatching for strlwr. This is executed only once
$align 16
strlwrCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet

	; Point to generic version
	mov     ecx, offset strlwrGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q100

	; SSE4.2 supported
	; Point to SSE4.2 version
	mov     ecx, offset strlwrSSE42

Q100:
	mov     dword ptr [strlwrDispatch], ecx

	; Continue in appropriate version
	jmp     ecx
strlwrCPUDispatch endp

end
