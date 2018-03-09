.686
.model flat, c

includelib kernel32.lib

extrn _imp__IsProcessorFeaturePresent@4:dword

public InitializeProcessorFeaturePresent
public __isa_available
public __isa_enabled
public __favor

__ISA_AVAILABLE_X86        equ 0
__ISA_AVAILABLE_SSE2       equ 1
__ISA_AVAILABLE_SSE42      equ 2
__ISA_AVAILABLE_AVX        equ 3
__ISA_AVAILABLE_ENFSTRG    equ 4
__ISA_AVAILABLE_AVX2       equ 5

__ISA_AVAILABLE_ARMNT      equ 0
__ISA_AVAILABLE_NEON       equ 1
__ISA_AVAILABLE_NEON_ARM64 equ 2

__ISA_ENABLED_X86          equ 00000001H
__ISA_ENABLED_SSE2         equ 00000002H
__ISA_ENABLED_SSE42        equ 00000004H
__ISA_ENABLED_AVX          equ 00000008H
__ISA_ENABLED_AVX2         equ 00000020H

__FAVOR_ATOM               equ 0
__FAVOR_ENFSTRG            equ 1
__FAVOR_XMMLOOP            equ 2

.data

__isa_available dword 0
__isa_enabled   dword __ISA_ENABLED_X86
__favor         dword 0

.code

BSWAP32 macro value
	local result
	result = ((value shr 24) and 0FFH) or ((value shr 8) and 0FF00H) or ((value shl 8) and 000FF0000H) or ((value shl 24) and 0FF000000H)
	exitm %result
endm

BIT_SCAN_FORWARD macro value
	local bits
	bits = 1
	while value shr bits shl bits eq value
		bits = bits + 1
	endm
	exitm %bits - 1
endm

align 16

; void __cdecl InitializeProcessorFeaturePresent()
InitializeProcessorFeaturePresent proc near

	PF_XMMI64_INSTRUCTIONS_AVAILABLE equ 10

	ERMS    equ 00000200H
	SSE42   equ 00100000H
	OSXSAVE equ 08000000H
	AVX     equ 10000000H
	AVX2    equ 00000020H

	cpuid_0_eax equ eax
	cpuid_1_ecx equ ecx
	cpuid_7_ebx equ ebx

	mov     dword ptr [__favor], 0
	push    PF_XMMI64_INSTRUCTIONS_AVAILABLE
	call    dword ptr [_imp__IsProcessorFeaturePresent@4]
	test    eax, eax
	jz      L4
	push    ebx
	xor     eax, eax
	cpuid
	push    cpuid_0_eax
	xor     ebx, BSWAP32('Genu')
	xor     edx, BSWAP32('ineI')
	xor     ecx, BSWAP32('ntel')
	or      ebx, edx
	mov     eax, 1
	or      ebx, ecx
	cpuid
	jnz     L2
	and     eax, 0FFF3FF0H
	jz      L2
	sub     eax, 000106C0H
	jz      L1
	sub     eax, 00020660H - 000106C0H
	jz      L1
	sub     eax, 00020670H - 00020660H
	jz      L1
	sub     eax, 00030650H - 00020670H
	jz      L1
	sub     eax, 00030660H - 00030650H
	jz      L1
	cmp     eax, 00030670H - 00030660H
	jne     L2
L1:
	mov     dword ptr [__favor], 1 shl __FAVOR_ATOM
L2:
	pop     cpuid_0_eax
	xor     cpuid_7_ebx, cpuid_7_ebx
	cmp     cpuid_0_eax, 7
	jl      L3
	push    cpuid_1_ecx
	mov     eax, 7
	xor     ecx, ecx
	cpuid
	mov     ecx, cpuid_7_ebx
	and     ebx, ERMS
	shr     ebx, BIT_SCAN_FORWARD(ERMS) - __FAVOR_ENFSTRG
	mov     eax, dword ptr [__favor]
	or      eax, ebx
	mov     cpuid_7_ebx, ecx
	mov     dword ptr [__favor], eax
	pop     cpuid_1_ecx
L3:
	test    cpuid_1_ecx, SSE42
	jz      L5
	and     ecx, OSXSAVE or AVX
	cmp     ecx, OSXSAVE or AVX
	jne     L6
	xor     ecx, ecx
	xgetbv
	and     eax, 6
	cmp     eax, 6
	jne     L6
	test    cpuid_7_ebx, AVX2
	jz      L7
	mov     dword ptr [__isa_available], __ISA_AVAILABLE_AVX2
	mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42 or __ISA_ENABLED_AVX or __ISA_ENABLED_AVX2
	pop     ebx
	ret
L4:
	mov     dword ptr [__isa_available], 0
	mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86
	ret
L5:
	mov     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
	mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2
	pop     ebx
	ret
L6:
	mov     dword ptr [__isa_available], __ISA_AVAILABLE_SSE42
	mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42
	pop     ebx
	ret
L7:
	mov     dword ptr [__isa_available], __ISA_AVAILABLE_AVX
	mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42 or __ISA_ENABLED_AVX
	pop     ebx
	ret

InitializeProcessorFeaturePresent endp

end
