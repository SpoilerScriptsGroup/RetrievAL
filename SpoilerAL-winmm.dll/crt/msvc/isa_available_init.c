#include <windows.h>

#define __ISA_AVAILABLE_X86        0
#define __ISA_AVAILABLE_SSE2       1
#define __ISA_AVAILABLE_SSE42      2
#define __ISA_AVAILABLE_AVX        3
#define __ISA_AVAILABLE_ENFSTRG    4
#define __ISA_AVAILABLE_AVX2       5
#define __ISA_AVAILABLE_AVX512     6

#define __ISA_AVAILABLE_ARMNT      0
#define __ISA_AVAILABLE_NEON       1
#define __ISA_AVAILABLE_NEON_ARM64 2

#define __ISA_ENABLED_X86          0x00000001
#define __ISA_ENABLED_SSE2         0x00000002
#define __ISA_ENABLED_SSE42        0x00000004
#define __ISA_ENABLED_AVX          0x00000008
#define __ISA_ENABLED_AVX2         0x00000020
#define __ISA_ENABLED_AVX512       0x00000040

#define __FAVOR_ATOM               0
#define __FAVOR_ENFSTRG            1
#define __FAVOR_XMMLOOP            2

unsigned int __isa_available = __ISA_AVAILABLE_X86;
unsigned int __isa_enabled   = __ISA_ENABLED_X86;
unsigned int __favor         = 0;

#ifndef _M_IX86
#include "intrinsic.h"
#include <immintrin.h>
#pragma intrinsic(__cpuid)
#pragma intrinsic(__cpuidex)

void __cdecl __isa_available_init()
{
	#define ERMS     0x00000200
	#define SSE42    0x00100000
	#define OSXSAVE  0x08000000
	#define AVX      0x10000000
	#define AVX2     0x00000020
	#define AVX512F  0x00010000
	#define AVX512DQ 0x00020000
	#define AVX512CD 0x10000000
	#define AVX512BW 0x40000000
	#define AVX512VL 0x80000000
	#define AVX512   (AVX512F | AVX512DQ | AVX512CD | AVX512BW | AVX512VL)

	struct {
		int eax;
		int ebx;
		int ecx;
		int edx;
	}             cpuInfo;
	int           cpuid_0_eax;
	int           cpuid_1_ecx;
	int           cpuid_7_ebx;
	int           xgetbv_eax;
	unsigned char intel_inside;

	__favor = 0;
	if (!IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
		goto ISA_AVAILABLE_X86;
	__cpuid((int *)&cpuInfo, 0);
	cpuid_0_eax = cpuInfo.eax;
	intel_inside = (
		(cpuInfo.ebx ^ BSWAP32('Genu')) |
		(cpuInfo.edx ^ BSWAP32('ineI')) |
		(cpuInfo.ecx ^ BSWAP32('ntel'))) == 0;
	__cpuid((int *)&cpuInfo, 1);
	cpuid_1_ecx = cpuInfo.ecx;
	if (intel_inside)
		switch (cpuInfo.eax & 0xFFF3FF0) {
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
		__favor |= ((cpuid_7_ebx = cpuInfo.ebx) & ERMS) >> (BSF(ERMS) - __FAVOR_ENFSTRG);
	}
	if (cpuid_1_ecx & SSE42)
		if ((cpuid_1_ecx & (OSXSAVE | AVX)) == (OSXSAVE | AVX) && ((xgetbv_eax = (int)_xgetbv(0)) & 6) == 6)
			if (cpuid_7_ebx & AVX2)
				if ((cpuid_7_ebx & AVX512) == AVX512 && (xgetbv_eax & 0xE0) == 0xE0)
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

	#undef ERMS
	#undef SSE42
	#undef OSXSAVE
	#undef AVX
	#undef AVX2
	#undef AVX512F
	#undef AVX512DQ
	#undef AVX512CD
	#undef AVX512BW
	#undef AVX512VL
	#undef AVX512
}
#else
#define _BSWAP32(value) (            \
    (((value) >> 24) & 0x000000FF) | \
    (((value) >>  8) & 0x0000FF00) | \
    (((value) <<  8) & 0x00FF0000) | \
    (((value) << 24) & 0xFF000000))

#define _BSF(value) (                                                  -1 + \
      ((value) & 0x00000001)                                         *  1 + \
    ((((value) & 0x00000002) >>  1) & (((value) & 0x00000001) == 0)) *  2 + \
    ((((value) & 0x00000004) >>  2) & (((value) & 0x00000003) == 0)) *  3 + \
    ((((value) & 0x00000008) >>  3) & (((value) & 0x00000007) == 0)) *  4 + \
    ((((value) & 0x00000010) >>  4) & (((value) & 0x0000000F) == 0)) *  5 + \
    ((((value) & 0x00000020) >>  5) & (((value) & 0x0000001F) == 0)) *  6 + \
    ((((value) & 0x00000040) >>  6) & (((value) & 0x0000003F) == 0)) *  7 + \
    ((((value) & 0x00000080) >>  7) & (((value) & 0x0000007F) == 0)) *  8 + \
    ((((value) & 0x00000100) >>  8) & (((value) & 0x000000FF) == 0)) *  9 + \
    ((((value) & 0x00000200) >>  9) & (((value) & 0x000001FF) == 0)) * 10 + \
    ((((value) & 0x00000400) >> 10) & (((value) & 0x000003FF) == 0)) * 11 + \
    ((((value) & 0x00000800) >> 11) & (((value) & 0x000007FF) == 0)) * 12 + \
    ((((value) & 0x00001000) >> 12) & (((value) & 0x00000FFF) == 0)) * 13 + \
    ((((value) & 0x00002000) >> 13) & (((value) & 0x00001FFF) == 0)) * 14 + \
    ((((value) & 0x00004000) >> 14) & (((value) & 0x00003FFF) == 0)) * 15 + \
    ((((value) & 0x00008000) >> 15) & (((value) & 0x00007FFF) == 0)) * 16 + \
    ((((value) & 0x00010000) >> 16) & (((value) & 0x0000FFFF) == 0)) * 17 + \
    ((((value) & 0x00020000) >> 17) & (((value) & 0x0001FFFF) == 0)) * 18 + \
    ((((value) & 0x00040000) >> 18) & (((value) & 0x0003FFFF) == 0)) * 19 + \
    ((((value) & 0x00080000) >> 19) & (((value) & 0x0007FFFF) == 0)) * 20 + \
    ((((value) & 0x00100000) >> 20) & (((value) & 0x000FFFFF) == 0)) * 21 + \
    ((((value) & 0x00200000) >> 21) & (((value) & 0x001FFFFF) == 0)) * 22 + \
    ((((value) & 0x00400000) >> 22) & (((value) & 0x003FFFFF) == 0)) * 23 + \
    ((((value) & 0x00800000) >> 23) & (((value) & 0x007FFFFF) == 0)) * 24 + \
    ((((value) & 0x01000000) >> 24) & (((value) & 0x00FFFFFF) == 0)) * 25 + \
    ((((value) & 0x02000000) >> 25) & (((value) & 0x01FFFFFF) == 0)) * 26 + \
    ((((value) & 0x04000000) >> 26) & (((value) & 0x03FFFFFF) == 0)) * 27 + \
    ((((value) & 0x08000000) >> 27) & (((value) & 0x07FFFFFF) == 0)) * 28 + \
    ((((value) & 0x10000000) >> 28) & (((value) & 0x0FFFFFFF) == 0)) * 29 + \
    ((((value) & 0x20000000) >> 29) & (((value) & 0x1FFFFFFF) == 0)) * 30 + \
    ((((value) & 0x40000000) >> 30) & (((value) & 0x3FFFFFFF) == 0)) * 31 + \
    ((((value) & 0x80000000) >> 31) & (((value) & 0x7FFFFFFF) == 0)) * 32)

__declspec(naked) void __cdecl __isa_available_init()
{
	__asm
	{
		#define ERMS     0x00000200
		#define SSE42    0x00100000
		#define OSXSAVE  0x08000000
		#define AVX      0x10000000
		#define AVX2     0x00000020
		#define AVX512F  0x00010000
		#define AVX512DQ 0x00020000
		#define AVX512CD 0x10000000
		#define AVX512BW 0x40000000
		#define AVX512VL 0x80000000
		#define AVX512   (AVX512F or AVX512DQ or AVX512CD or AVX512BW or AVX512VL)

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
		push    cpuid_0_eax
		xor     ebx, _BSWAP32('Genu')
		xor     edx, _BSWAP32('ineI')
		xor     ecx, _BSWAP32('ntel')
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
		shr     ebx, _BSF(ERMS) - __FAVOR_ENFSTRG
		mov     eax, dword ptr [__favor]
		or      eax, ebx
		mov     cpuid_7_ebx, ecx
		mov     dword ptr [__favor], eax
		pop     cpuid_1_ecx
	L3:
		mov     eax, cpuid_1_ecx
		and     ecx, OSXSAVE or AVX
		test    eax, SSE42
		jz      ISA_AVAILABLE_SSE2
		cmp     ecx, OSXSAVE or AVX
		jne     ISA_AVAILABLE_SSE42
		xor     ecx, ecx
		xgetbv
		mov     ecx, eax
		and     eax, 6
		cmp     eax, 6
		jne     ISA_AVAILABLE_SSE42
		test    cpuid_7_ebx, AVX2
		jz      ISA_AVAILABLE_AVX
		and     ebx, AVX512
		and     ecx, 0xE0
		cmp     ebx, AVX512
		jne     ISA_AVAILABLE_AVX2
		cmp     ecx, 0xE0
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

		#undef ERMS
		#undef SSE42
		#undef OSXSAVE
		#undef AVX
		#undef AVX2
		#undef AVX512F
		#undef AVX512DQ
		#undef AVX512CD
		#undef AVX512BW
		#undef AVX512VL
		#undef AVX512
		#undef cpuid_0_eax
		#undef cpuid_1_ecx
		#undef cpuid_7_ebx
	}
}
#endif
