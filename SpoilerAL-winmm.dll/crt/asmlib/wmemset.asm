.686
.model flat

public _wmemset

extern memsetAVX512BW_entry: near
extern memsetAVX512F_entry: near
extern memsetAVX_entry: near
extern memsetSSE2_entry: near
extern memset386_entry: near
extern GetMemsetCacheLimit: near
extern _InstructionSet: near
extern _Store256BitIsFaster: near

.data

wmemsetDispatch dd wmemsetCPUDispatch

.code

; Function entry:
align 16
_wmemset proc near
	jmp     dword ptr [wmemsetDispatch]                 ; Go to appropriate version, depending on instruction set
_wmemset endp

; AVX512BW version. Use zmm register
align 16
wmemsetAVX512BW proc near
	mov     ecx, dword ptr [esp + 12]                   ; count
	mov     edx, dword ptr [esp + 4]                    ; dest
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     L001
	int     3
	align   16

L001:
	movzx   eax, word ptr [esp + 8]                     ; c
	imul    eax, 00010001H                              ; Broadcast c into all words of eax
	jmp     memsetAVX512BW_entry
wmemsetAVX512BW endp

; AVX512F version
align 16
wmemsetAVX512F proc near
	mov     ecx, dword ptr [esp + 12]                   ; count
	mov     edx, dword ptr [esp + 4]                    ; dest
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     A001
	int     3
	align   16

A001:
	movzx   eax, word ptr [esp + 8]                     ; c
	imul    eax, 00010001H                              ; Broadcast c into all words of eax
	jmp     memsetAVX512F_entry
wmemsetAVX512F endp

; AVX version. Use ymm register
align 16
wmemsetAVX proc near
	mov     ecx, dword ptr [esp + 12]                   ; count
	mov     edx, dword ptr [esp + 4]                    ; dest
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     B001
	int     3
	align   16

B001:
	movzx   eax, word ptr [esp + 8]                     ; c
	imul    eax, 00010001H                              ; Broadcast c into all words of eax
	jmp     memsetAVX_entry
wmemsetAVX endp

; SSE2 Version
align 16
wmemsetSSE2 proc near
	mov     ecx, dword ptr [esp + 12]                   ; count
	mov     edx, dword ptr [esp + 4]                    ; dest
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     M001
	int     3
	align   16

M001:
	movzx   eax, word ptr [esp + 8]                     ; c
	imul    eax, 00010001H                              ; Broadcast c into all words of eax
	jmp     memsetSSE2_entry
wmemsetSSE2 endp

; 80386 Version
align 16
wmemset386 proc near
	mov     ecx, dword ptr [esp + 12]                   ; count
	xor     eax, eax
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     N001
	int     3
	align   16

N001:
	mov     ax, word ptr [esp + 8]                      ; c
	mov     edx, dword ptr [esp + 4]                    ; dest
	push    edi
	mov     edi, eax
	shl     eax, 16
	or      eax, edi
	mov     edi, edx
	jmp     memset386_entry
wmemset386 endp

; CPU dispatching for wmemset. This is executed only once
align 16
wmemsetCPUDispatch proc near
	pushad
	call    GetMemsetCacheLimit                         ; calculate cache limit
	call    _InstructionSet                             ; get supported instruction set
	mov     ebx, eax
	; Point to generic version of wmemset
	mov     dword ptr [wmemsetDispatch], offset wmemset386
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	; Point to SSE2 version of wmemset
	mov     dword ptr [wmemsetDispatch], offset wmemsetSSE2
	call    _Store256BitIsFaster                        ; check if 256-bit stores are available and faster
	test    eax, eax
	jz      Q100
	mov     dword ptr [wmemsetDispatch], offset wmemsetAVX
	cmp     ebx, 15
	jb      Q100
	mov     dword ptr [wmemsetDispatch], offset wmemsetAVX512F
	cmp     ebx, 16
	jb      Q100
	mov     dword ptr [wmemsetDispatch], offset wmemsetAVX512BW

Q100:
	popad
	; Continue in appropriate version of wmemset
	jmp     dword ptr [wmemsetDispatch]
wmemsetCPUDispatch endp

end
