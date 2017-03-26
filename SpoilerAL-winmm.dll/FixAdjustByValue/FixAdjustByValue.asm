.486
.model flat, c

public FixAdjustByValue

.code

align 16

FixAdjustByValue proc near

	mov     cl, byte ptr [ebx + 78H]
	mov     edx, 004AE7A0H
	cmp     cl, 2
	jb      L2
	je      L3
	cmp     cl, 3
	ja      L1
	shl     eax, 8
L1:
	bswap   eax
	mov     dword ptr [esp + 8], eax
L2:
	jmp     edx
L3:
	mov     byte ptr [esp + 8], ah
	mov     byte ptr [esp + 9], al
	mov     word ptr [esp + 10], 0
	jmp     edx

FixAdjustByValue endp

end
