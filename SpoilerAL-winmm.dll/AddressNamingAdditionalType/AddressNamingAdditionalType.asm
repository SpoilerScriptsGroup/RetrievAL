.486
.model flat, c

extrn AddressNamingFromUnicode@8:proc
extrn AddressNamingFEPNumber@20:proc
extrn AddressNamingFEPList@20:proc
extrn AddressNamingFEPFreeList@20:proc

public AddressNamingAdditionalType

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

.code

align 16

AddressNamingAdditionalType proc near

	ReturnAddress equ 00506578H
	SSGCtrl       equ <edi>
	SSGS          equ <ebp + 16>
	tmpV          equ <esi>
	DataSize      equ <ebp - 01ECH>
	tmpC          equ <ebp - 01E8H>
	sizeof_string equ 24

	mov     eax, dword ptr [esi]
	mov     edx, dword ptr [tmpC]
	mov     ecx, dword ptr [eax + 4CH]
	mov     eax, dword ptr [eax + 48H]                  ;	const char *p = tmpV[3].c_str();
	sub     ecx, eax
	sub     ecx, 7                                      ;	switch (tmpV[3].length())
	ja      L3                                          ;	{
	jb      L5                                          ;
	mov     ecx, dword ptr [eax]                        ;	case 7:
	mov     eax, dword ptr [eax + 4]                    ;
	cmp     ecx, BSWAP32('unic')                        ;		if (*(LPDWORD)p == BSWAP32('unic'))
	jne     L2                                          ;		{
	cmp     eax, BSWAP24('ode')                         ;			if (*(LPDWORD)(p + 4) != BSWAP32('ode\0'))
	jne     L5                                          ;				break;
	mov     eax, dword ptr [tmpV]                       ;			tmpV[3].clear();
	push    edx                                         ;			AddressNamingFromUnicode(DataSize, tmpC);
	add     eax, sizeof_string * 3
	mov     edx, dword ptr [DataSize]
	mov     ecx, dword ptr [eax]
	push    edx
	mov     dword ptr [eax + 4], ecx
	mov     byte ptr [ecx], 0
	push    ReturnAddress                               ;			break;
	jmp     AddressNamingFromUnicode@8                  ;		}
L2:
	xor     ecx, BSWAP32('fep_')                        ;		if (*(LPDWORD)p != BSWAP32('fep_'))
	xor     eax, BSWAP24('num')                         ;			break;
	or      eax, ecx                                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('num\0'))
	jnz     L5                                          ;			break;
	mov     ecx, dword ptr [DataSize]                   ;		AddressNamingFEPNumber(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
	mov     eax, dword ptr [SSGS]
	push    edx
	push    ecx
	push    tmpV
	push    eax
	push    SSGCtrl
	push    ReturnAddress
	jmp     AddressNamingFEPNumber@20                   ;		break;
L3:
	cmp     dword ptr [eax], BSWAP32('fep_')
	jne     L5
	dec     ecx                                         ;	case 8:
	jnz     L4                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
                                                        ;			break;
	cmp     dword ptr [eax + 4], BSWAP32('list')        ;		if (*(LPDWORD)(p + 4) != BSWAP32('list'))
	jne     L5                                          ;			break;
	mov     eax, dword ptr [DataSize]                   ;		AddressNamingFEPList(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
	mov     ecx, dword ptr [SSGS]
	push    edx
	push    eax
	push    tmpV
	push    ecx
	push    SSGCtrl
	push    ReturnAddress
	jmp     AddressNamingFEPList@20                     ;		break;
L4:
	cmp     ecx, 13 - 8                                 ;	case 13:
	jne     L5                                          ;		if (*(LPDWORD)p != BSWAP32('fep_'))
                                                        ;			break;
	cmp     byte ptr [eax + 12], 't'                    ;		if (*(LPDWORD)(p + 4) != BSWAP32('free'))
	jne     L5                                          ;			break;
	mov     ecx, dword ptr [eax + 4]                    ;		if (*(LPDWORD)(p + 8) != BSWAP32('_lis'))
	mov     eax, dword ptr [eax + 8]                    ;			break;
	xor     ecx, BSWAP32('free')                        ;		if (p[12] != 't')
	xor     eax, BSWAP32('_lis')                        ;			break;
	or      eax, ecx
	jnz     L5
	mov     eax, dword ptr [DataSize]                   ;		AddressNamingFEPFreeList(SSGCtrl, SSGS, tmpV, DataSize, tmpC);
	mov     ecx, dword ptr [SSGS]
	push    edx
	push    eax
	push    tmpV
	push    ecx
	push    SSGCtrl
	push    ReturnAddress
	jmp     AddressNamingFEPFreeList@20                 ;		break;
L5:                                                     ;	}
	mov     eax, ReturnAddress
	jmp     eax

AddressNamingAdditionalType endp

end
