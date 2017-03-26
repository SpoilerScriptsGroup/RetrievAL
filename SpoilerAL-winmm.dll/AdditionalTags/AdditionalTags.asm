.486
.model flat, c

extrn Attribute_expr@16:proc
extrn Attribute_define@16:proc
extrn Attribute_undef@16:proc
extrn Attribute_allcate@16:proc
extrn Attribute_variable_open@16:proc
extrn Attribute_variable_close@16:proc
extrn Attribute_error_skip_open@16:proc
extrn Attribute_error_skip_close@16:proc

public AdditionalTags@16

BSWAP16 macro value
	local result
	result = ((value shr 8) and 0FFH) or ((value shl 8) and 0FF00H)
	exitm %result
endm

BSWAP24 macro value
	local result
	result = ((value shr 16) and 0FFH) or (value and 0FF00H) or ((value shl 16) and 000FF0000H)
	exitm %result
endm

BSWAP32 macro value
	local result
	result = ((value shr 24) and 0FFH) or ((value shr 8) and 0FF00H) or ((value shl 8) and 000FF0000H) or ((value shl 24) and 0FF000000H)
	exitm %result
endm

MAKESTRING macro args:vararg
	local value, bits
	value = 0
	bits = 0
	for char, <args>
		value = value or (char shl bits)
		bits = bits + 8
	endm
	exitm %value
endm

.const

JmupTable label byte
	dword   L8
	dword   L11
	dword   L12
	dword   L13

.code

align 16

AdditionalTags@16 proc near

	SSGCtrl equ <esp +  4>
	parent  equ <esp +  8>
	prefix  equ <esp + 12>
	code    equ <esp + 16>

	mov     ecx, dword ptr [prefix]
	mov     eax, dword ptr [ecx]
	mov     ecx, dword ptr [ecx + 4]
	sub     ecx, eax
	jbe     L1
	cmp     ecx, 5
	jb      L1
	mov     edx, dword ptr [eax]
	je      L2
	cmp     edx, BSWAP32('[und')
	je      L4
	sub     ecx, 9
	jb      L1
	cmp     edx, BSWAP32('[def')
	je      L6
	cmp     ecx, 12 - 9
	ja      L1
	jmp     dword ptr [JmupTable + ecx * 4]
L1:
	ret     16

	align   16
L2:
	mov     cl, byte ptr [eax + 4]
	xor     edx, BSWAP32('[exp')
	xor     ecx, 'r'
	or      ecx, edx
	jz      Attribute_expr@16
L3:
	ret     16

	align   16
L4:
	cmp     ecx, 8
	jb      L5
	mov     cx, word ptr [eax + 4]
	mov     dl, byte ptr [eax + 6]
	cmp     cx, BSWAP16('ef')
	jne     L5
	cmp     dl, ' '
	je      Attribute_undef@16
	cmp     dl, 09H
	jb      L5
	cmp     dl, 0DH
	jbe     Attribute_undef@16
L5:
	ret     16

	align   16
L6:
	mov     eax, dword ptr [eax + 4]
	cmp     eax, BSWAP32('ine ')
	je      Attribute_define@16
	rol     eax, 8
	cmp     eax, MAKESTRING(09H, 'i', 'n', 'e')
	jb      L7
	cmp     eax, MAKESTRING(0DH, 'i', 'n', 'e')
	jbe     Attribute_define@16
L7:
	ret     16

	align   16
L8 label byte
	cmp     edx, BSWAP32('[all')
	jne     L10
	mov     edx, dword ptr [eax + 4]
	mov     cl, byte ptr [eax + 8]
	xor     edx, BSWAP32('ocat')
	xor     ecx, 'e'
	or      ecx, edx
	jz      Attribute_allcate@16
L9:
	ret     16

	align   16
L10:
	mov     cl, byte ptr [eax + 8]
	mov     eax, dword ptr [eax + 4]
	xor     edx, BSWAP32('[var')
	xor     eax, BSWAP32('iabl')
	xor     ecx, 'e'
	or      eax, edx
	or      eax, ecx
	jz      Attribute_variable_open@16
	ret     16

	align   16
L11 label byte
	mov     cx, word ptr [eax + 8]
	mov     eax, dword ptr [eax + 4]
	xor     edx, BSWAP32('[/va')
	xor     eax, BSWAP32('riab')
	xor     ecx, BSWAP16('le')
	or      eax, edx
	or      eax, ecx
	jz      Attribute_variable_close@16
	ret     16

	align   16
L12 label byte
	mov     ecx, dword ptr [eax + 4]
	mov     eax, dword ptr [eax + 8]
	xor     edx, BSWAP32('[err')
	xor     ecx, BSWAP32('or_s')
	xor     eax, BSWAP24('kip')
	or      ecx, edx
	or      ecx, eax
	jz      Attribute_error_skip_open@16
	ret     16

	align   16
L13 label byte
	mov     ecx, dword ptr [eax + 4]
	mov     eax, dword ptr [eax + 8]
	xor     edx, BSWAP32('[/er')
	xor     ecx, BSWAP32('ror_')
	xor     eax, BSWAP32('skip')
	or      ecx, edx
	or      ecx, eax
	jz      Attribute_error_skip_close@16
	ret     16

AdditionalTags@16 endp

end
