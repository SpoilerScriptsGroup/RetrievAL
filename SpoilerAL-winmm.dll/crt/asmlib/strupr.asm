.686
.xmm
.model flat

public __strupr

extern InstructionSet: near
extern struprlwrSSE42: near
extern casebit: near

.const

align   16

azlow   db 'azazazazazazazaz'                           ; define range for lower case

.data

; Pointer to appropriate version. Initially points to dispatcher
struprDispatch dd struprCPUDispatch

.code

align 16
__strupr proc near
	jmp     dword ptr [struprDispatch]                  ; Go to appropriate version, depending on instruction set
__strupr endp

align 16
struprSSE42 proc near
	movdqa  xmm1, xmmword ptr [azlow]                   ; define range a-z
	movdqa  xmm3, xmmword ptr [casebit]                 ; bit to change
	jmp     struprlwrSSE42
struprSSE42 endp

; 386 version
align 16
struprGeneric proc near
	mov     edx, dword ptr [esp + 4]                    ; string

B100:
	; loop
	mov     al, byte ptr [edx]
	test    al, al
	jz      B900                                        ; end of string
	sub     al, 'a'
	cmp     al, 'z' - 'a'
	jbe     B200                                        ; is lower case
	inc     edx
	jmp     B100                                        ; loop to next character

B200:
	; convert to upper case
	add     al, 'A'
	mov     byte ptr [edx], al
	inc     edx
	jmp     B100

B900:
	mov     eax, dword ptr [esp + 4]                    ; string
	ret
struprGeneric endp

; CPU dispatching for strupr. This is executed only once
align 16
struprCPUDispatch proc near
	; get supported instruction set
	call    InstructionSet

	; Point to generic version
	mov     ecx, offset struprGeneric
	cmp     eax, 10                                     ; check SSE4.2
	jb      Q200

	; SSE4.2 supported
	; Point to SSE4.2 version
	mov     ecx, offset struprSSE42

Q200:
	mov     dword ptr [struprDispatch], ecx

	; Continue in appropriate version
	jmp     ecx
struprCPUDispatch endp

end
