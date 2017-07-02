.486
.model flat, c

IWORD equ <dword>

CODESEG macro
	.code
	align 16
endm

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

