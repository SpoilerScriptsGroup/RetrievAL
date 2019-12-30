#include <windows.h>
#include <stdint.h>
#include "intrinsic.h"

#define __ISA_AVAILABLE_X86         0
#define __ISA_AVAILABLE_SSE2        1
#define __ISA_AVAILABLE_SSE42       2
#define __ISA_AVAILABLE_AVX         3
#define __ISA_AVAILABLE_ENFSTRG     4
#define __ISA_AVAILABLE_AVX2        5
#define __ISA_AVAILABLE_AVX512      6

#define __ISA_AVAILABLE_ARMNT       0
#define __ISA_AVAILABLE_NEON        1
#define __ISA_AVAILABLE_NEON_ARM64  2

#define __ISA_ENABLED_X86           0x00000001
#define __ISA_ENABLED_SSE2          0x00000002
#define __ISA_ENABLED_SSE42         0x00000004
#define __ISA_ENABLED_AVX           0x00000008
#define __ISA_ENABLED_AVX2          0x00000020
#define __ISA_ENABLED_AVX512        0x00000040

#define __FAVOR_ATOM                0
#define __FAVOR_ENFSTRG             1
#define __FAVOR_XMMLOOP             2

/* Features in ecx for leaf 1 */
#define CF_SSE42                    0x00100000
#define CF_OSXSAVE                  0x08000000
#define CF_AVX                      0x10000000

/* Features in ebx for leaf 7 sub-leaf 0 */
#define CX_AVX2                     0x00000020
#define CX_ERMS                     0x00000200
#define CX_AVX512F                  0x00010000
#define CX_AVX512DQ                 0x00020000
#define CX_AVX512CD                 0x10000000
#define CX_AVX512BW                 0x40000000
#define CX_AVX512VL                 0x80000000

/* Get XCR_XFEATURE_ENABLED_MASK register with xgetbv. */
#define XCR_XFEATURE_ENABLED_MASK   0x00000000
#define XSTATE_FP                   0x00000001
#define XSTATE_SSE                  0x00000002
#define XSTATE_YMM                  0x00000004
#define XSTATE_OPMASK               0x00000020
#define XSTATE_ZMM                  0x00000040
#define XSTATE_HI_ZMM               0x00000080

unsigned int __isa_available = __ISA_AVAILABLE_X86;
unsigned int __isa_enabled   = __ISA_ENABLED_X86;
unsigned int __favor         = 0;

#ifndef _M_IX86
#pragma intrinsic(__cpuid)
#pragma intrinsic(__cpuidex)

void __cdecl __isa_available_init()
{
	#define C1_AVX                   (CF_OSXSAVE | CF_AVX)
	#define C7_AVX512                (CX_AVX512F | CX_AVX512DQ | CX_AVX512CD | CX_AVX512BW | CX_AVX512VL)
	#define XCR_AVX_ENABLED_MASK     (XSTATE_SSE | XSTATE_YMM)
	#define XCR_AVX512F_ENABLED_MASK (XSTATE_OPMASK | XSTATE_ZMM | XSTATE_HI_ZMM)

	struct {
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
	}        cpuInfo;
	uint32_t cpuid_0_eax;
	uint32_t cpuid_1_ecx;
	uint32_t cpuid_7_ebx;
	uint32_t xgetbv_eax;
	uint32_t intel_outside;

	__favor = 0;
	if (!IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		goto ISA_AVAILABLE_X86;
	__cpuid((int *)&cpuInfo, 0);
	cpuid_0_eax = cpuInfo.eax;
	intel_outside =
		(cpuInfo.ebx ^ BSWAP32('Genu')) |
		(cpuInfo.edx ^ BSWAP32('ineI')) |
		(cpuInfo.ecx ^ BSWAP32('ntel'));
	__cpuid((int *)&cpuInfo, 1);
	cpuid_1_ecx = cpuInfo.ecx;
	if (!intel_outside)
		switch (cpuInfo.eax & 0x0FFF3FF0) {
		case 0x000106C0:
		case 0x00020660:
		case 0x00020670:
		case 0x00030650:
		case 0x00030660:
		case 0x00030670:
			__favor = 1 << __FAVOR_ATOM;
		}
	cpuid_7_ebx = 0;
	if (cpuid_0_eax >= 7) {
		__cpuidex((int *)&cpuInfo, 7, 0);
		__favor |= ((cpuid_7_ebx = cpuInfo.ebx) & CX_ERMS) >> (BSF32(CX_ERMS) - __FAVOR_ENFSTRG);
	}
	if (cpuid_1_ecx & CF_SSE42)
		if ((cpuid_1_ecx & C1_AVX) == C1_AVX && ((xgetbv_eax = (uint32_t)_xgetbv(0)) & XCR_AVX_ENABLED_MASK) == XCR_AVX_ENABLED_MASK)
			if (cpuid_7_ebx & CX_AVX2)
				if ((cpuid_7_ebx & C7_AVX512) == C7_AVX512 && (xgetbv_eax & XCR_AVX512F_ENABLED_MASK) == XCR_AVX512F_ENABLED_MASK)
					goto ISA_AVAILABLE_AVX512;
				else
					goto ISA_AVAILABLE_AVX2;
			else
				goto ISA_AVAILABLE_AVX;
		else
			goto ISA_AVAILABLE_SSE42;
	else
		goto ISA_AVAILABLE_SSE2;

ISA_AVAILABLE_X86:
	__isa_available = __ISA_AVAILABLE_X86;
	__isa_enabled = __ISA_ENABLED_X86;
	return;

ISA_AVAILABLE_SSE2:
	__isa_available = __ISA_AVAILABLE_SSE2;
	__isa_enabled = __ISA_ENABLED_X86 | __ISA_ENABLED_SSE2;
	return;

ISA_AVAILABLE_SSE42:
	__isa_available = __ISA_AVAILABLE_SSE42;
	__isa_enabled = __ISA_ENABLED_X86 | __ISA_ENABLED_SSE2 | __ISA_ENABLED_SSE42;
	return;

ISA_AVAILABLE_AVX:
	__isa_available = __ISA_AVAILABLE_AVX;
	__isa_enabled = __ISA_ENABLED_X86 | __ISA_ENABLED_SSE2 | __ISA_ENABLED_SSE42 | __ISA_ENABLED_AVX;
	return;

ISA_AVAILABLE_AVX2:
	__isa_available = __ISA_AVAILABLE_AVX2;
	__isa_enabled = __ISA_ENABLED_X86 | __ISA_ENABLED_SSE2 | __ISA_ENABLED_SSE42 | __ISA_ENABLED_AVX | __ISA_ENABLED_AVX2;
	return;

ISA_AVAILABLE_AVX512:
	__isa_available = __ISA_AVAILABLE_AVX512;
	__isa_enabled = __ISA_ENABLED_X86 | __ISA_ENABLED_SSE2 | __ISA_ENABLED_SSE42 | __ISA_ENABLED_AVX | __ISA_ENABLED_AVX2 | __ISA_ENABLED_AVX512;
	return;

	#undef C1_AVX
	#undef C7_AVX512
	#undef XCR_AVX_ENABLED_MASK
	#undef XCR_AVX512F_ENABLED_MASK
}
#else
__declspec(naked) void __cdecl __isa_available_init()
{
	__asm
	{
		#define C1_AVX                   (CF_OSXSAVE or CF_AVX)
		#define C7_AVX512                (CX_AVX512F or CX_AVX512DQ or CX_AVX512CD or CX_AVX512BW or CX_AVX512VL)
		#define XCR_AVX_ENABLED_MASK     (XSTATE_SSE or XSTATE_YMM)
		#define XCR_AVX512F_ENABLED_MASK (XSTATE_OPMASK or XSTATE_ZMM or XSTATE_HI_ZMM)

		#define cpuid_0_eax eax
		#define cpuid_1_ecx ecx
		#define cpuid_7_ebx ebx

		mov     dword ptr [__favor], 0
		push    PF_XMMI64_INSTRUCTIONS_AVAILABLE
		call    IsProcessorFeaturePresent
		test    eax, eax
		jz      ISA_AVAILABLE_X86
		push    ebx
		xor     eax, eax
		cpuid
		xor     ebx, MASM_BSWAP32('Genu')
		xor     edx, MASM_BSWAP32('ineI')
		xor     ecx, MASM_BSWAP32('ntel')
		or      ebx, edx
		push    cpuid_0_eax
		or      ebx, ecx
		mov     eax, 1
		push    ebx
		cpuid
		pop     ebx
		mov     edx, eax
		and     eax, 0FFF3FF0H and (not (000106C0H or 00020660H or 00020670H or 00030650H or 00030660H or 00030670H))
		and     edx, 000106C0H or 00020660H or 00020670H or 00030650H or 00030660H or 00030670H
		or      eax, ebx
		jnz     L2
		mov     eax, edx
		and     edx, 000106C0H and 00020660H and 00020670H and 00030650H and 00030660H and 00030670H
		cmp     edx, 000106C0H and 00020660H and 00020670H and 00030650H and 00030660H and 00030670H
		jne     L2
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
		jb      L3
		push    cpuid_1_ecx
		mov     eax, 7
		xor     ecx, ecx
		cpuid
		mov     ecx, cpuid_7_ebx
		and     ebx, CX_ERMS
		shr     ebx, MASM_BSF32(CX_ERMS) - __FAVOR_ENFSTRG
		mov     eax, dword ptr [__favor]
		or      eax, ebx
		mov     cpuid_7_ebx, ecx
		mov     dword ptr [__favor], eax
		pop     cpuid_1_ecx
	L3:
		mov     eax, cpuid_1_ecx
		and     ecx, C1_AVX
		test    eax, CF_SSE42
		jz      ISA_AVAILABLE_SSE2
		cmp     ecx, C1_AVX
		jne     ISA_AVAILABLE_SSE42
		xor     ecx, ecx
		xgetbv
		mov     ecx, eax
		and     eax, XCR_AVX_ENABLED_MASK
		cmp     eax, XCR_AVX_ENABLED_MASK
		jne     ISA_AVAILABLE_SSE42
		test    cpuid_7_ebx, CX_AVX2
		jz      ISA_AVAILABLE_AVX
		and     ebx, C7_AVX512
		and     ecx, XCR_AVX512F_ENABLED_MASK
		cmp     ebx, C7_AVX512
		jne     ISA_AVAILABLE_AVX2
		cmp     ecx, XCR_AVX512F_ENABLED_MASK
		jne     ISA_AVAILABLE_AVX2
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_AVX512
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42 or __ISA_ENABLED_AVX or __ISA_ENABLED_AVX2 or __ISA_ENABLED_AVX512
		pop     ebx
		ret

		align   16
	ISA_AVAILABLE_X86:
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86
		ret

		align   16
	ISA_AVAILABLE_SSE2:
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2
		pop     ebx
		ret

		align   16
	ISA_AVAILABLE_SSE42:
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_SSE42
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42
		pop     ebx
		ret

		align   16
	ISA_AVAILABLE_AVX:
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_AVX
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42 or __ISA_ENABLED_AVX
		pop     ebx
		ret

		align   16
	ISA_AVAILABLE_AVX2:
		mov     dword ptr [__isa_available], __ISA_AVAILABLE_AVX2
		mov     dword ptr [__isa_enabled], __ISA_ENABLED_X86 or __ISA_ENABLED_SSE2 or __ISA_ENABLED_SSE42 or __ISA_ENABLED_AVX or __ISA_ENABLED_AVX2
		pop     ebx
		ret

		#undef C1_AVX
		#undef C7_AVX512
		#undef XCR_AVX_ENABLED_MASK
		#undef XCR_AVX512F_ENABLED_MASK
		#undef cpuid_0_eax
		#undef cpuid_1_ecx
		#undef cpuid_7_ebx
	}
}
#endif
