#include <stddef.h>

#ifndef _M_IX86
size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen);
static size_t __cdecl strnlen386(const char *string, size_t maxlen);
static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen);

static size_t(__cdecl * strnlenDispatch)(const char *string, size_t maxlen) = strnlenCPUDispatch;

__declspec(naked) size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [strnlenDispatch]
	}
}

__declspec(naked) static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // eax = maxlen
		mov     edx, dword ptr [string]                     // edx = string
		test    eax, eax                                    // check if maxlen=0
		jz      retzero                                     // if maxlen=0, leave
		pxor    xmm1, xmm1                                  // xmm1 = zero clear
		push    esi                                         // preserve esi
		lea     esi, [edx + eax]                            // esi = end of string
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      negate_count
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jz      negate_count
		xor     ecx, ecx
		sub     ecx, eax                                    // ecx = negative count
		jmp     found

	negate_count:
		sub     ecx, eax                                    // ecx = negative count
		jae     epilogue

		align   16                                          // already aligned
	loop_begin:
		movdqa  xmm0, xmmword ptr [esi + ecx]
		pcmpeqb xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 16
		jnc     loop_begin
		pop     esi                                         // restore esi
	retzero:
		ret

		align   16
	found:
		bsf     edx, edx
		add     ecx, edx
		jc      epilogue
		add     eax, ecx
	epilogue:
		pop     esi                                         // restore esi
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlen386(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // 00000000 _ 8B. 44 24, 08
		mov     edx, dword ptr [string]                     // 00000004 _ 8B. 54 24, 04
		test    eax, eax                                    // 00000008 _ 85. C0
		jz      maxlen_equal_zero                           // 0000000A _ 74, 1D
		push    esi                                         // 0000000C _ 56
		push    edi                                         // 0000000D _ 57
		mov     esi, edx                                    // 0000000E _ 8B. F2
		mov     edi, eax                                    // 00000010 _ 8B. F8
		and     edx, 3                                      // 00000012 _ 83. E2, 03
		jz      loop_begin                                  // 00000015 _ 74, 59
		dec     edx                                         // 00000017 _ 4A
		jz      modulo1                                     // 00000018 _ 74, 36
		dec     edx                                         // 0000001A _ 4A
		jz      modulo2                                     // 0000001B _ 74, 13
		mov     cl, byte ptr [esi]                          // 0000001D _ 8A. 0E
		inc     esi                                         // 0000001F _ 46
		test    cl, cl                                      // 00000020 _ 84. C9
		jz      return_zero                                 // 00000022 _ 74, 06
		dec     edi                                         // 00000024 _ 4F
		jnz     loop_begin                                  // 00000025 _ 75, 49
		pop     edi                                         // 00000027 _ 5F
		pop     esi                                         // 00000028 _ 5E
	maxlen_equal_zero:
		ret                                                 // 00000029 _ C3

	return_zero:
		xor     eax, eax                                    // 0000002A _ 33. C0
		pop     edi                                         // 0000002C _ 5F
		pop     esi                                         // 0000002D _ 5E
		ret                                                 // 0000002E _ C3

		align   16                                          // 0000002F _ 90
	modulo2:
		mov     ecx, dword ptr [esi - 2]                    // 00000030 _ 8B. 4E, FE
		add     esi, 2                                      // 00000033 _ 83. C6, 02
		lea     edx, [ecx - 01010000H]                      // 00000036 _ 8D. 91, FEFF0000
		xor     ecx, -1                                     // 0000003C _ 83. F1, FF
		and     edx, 80800000H                              // 0000003F _ 81. E2, 80800000
		and     ecx, edx                                    // 00000045 _ 23. CA
		jnz     found_at_high_word                          // 00000047 _ 75, 57
		sub     edi, 2                                      // 00000049 _ 83. EF, 02
		ja      loop_begin                                  // 0000004C _ 77, 22
		jmp     return_maxlen                               // 0000004E _ EB, 3D

		align   16
	modulo1:
		mov     ecx, dword ptr [esi - 1]                    // 00000050 _ 8B. 4E, FF
		add     esi, 3                                      // 00000053 _ 83. C6, 03
		mov     edx, ecx                                    // 00000056 _ 8B. D1
		xor     ecx, -1                                     // 00000058 _ 83. F1, FF
		sub     edx, 01010100H                              // 0000005B _ 81. EA, 01010100
		and     ecx, 80808000H                              // 00000061 _ 81. E1, 80808000
		and     ecx, edx                                    // 00000067 _ 23. CA
		jnz     found_at_second_byte_or_later               // 00000069 _ 75, 25
		sub     edi, 3                                      // 0000006B _ 83. EF, 03
		jbe     return_maxlen                               // 0000006E _ 76, 1D

		align   16
	loop_begin:
		mov     ecx, dword ptr [esi]                        // 00000070 _ 8B. 0E
		add     esi, 4                                      // 00000072 _ 83. C6, 04
		lea     edx, [ecx - 01010101H]                      // 00000075 _ 8D. 91, FEFEFEFF
		xor     ecx, -1                                     // 0000007B _ 83. F1, FF
		and     edx, 80808080H                              // 0000007E _ 81. E2, 80808080
		and     ecx, edx                                    // 00000084 _ 23. CA
		jnz     found                                       // 00000086 _ 75, 0B
		sub     edi, 4                                      // 00000088 _ 83. EF, 04
		ja      loop_begin                                  // 0000008B _ 77, E3
	return_maxlen:
		pop     edi                                         // 0000008D _ 5F
		pop     esi                                         // 0000008E _ 5E
		ret                                                 // 0000008F _ C3

		align   16
	found_at_second_byte_or_later:
		shr     ecx, 8                                      // 00000090 _ C1. E9, 08
	found:
		test    cx, cx                                      // 00000093 _ 66: 85. C9
		jnz     found_at_low_word                           // 00000096 _ 75, 0B
		sub     edi, 2                                      // 00000098 _ 83. EF, 02
		ja      found_at_high_word                          // 0000009B _ 77, 03
		pop     edi                                         // 0000009D _ 5F
		pop     esi                                         // 0000009E _ 5E
		ret                                                 // 0000009F _ C3

		align   16
	found_at_high_word:
		shr     ecx, 16                                     // 000000A0 _ C1. E9, 10
	found_at_low_word:
		sub     eax, edi                                    // 000000A3 _ 2B. C7
		cmp     cl, 1                                       // 000000A5 _ 80. F9, 01
		pop     edi                                         // 000000A8 _ 5F
		adc     eax, 0                                      // 000000A9 _ 83. D0, 00
		pop     esi                                         // 000000AC _ 5E
		ret                                                 // 000000AD _ C3

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strnlenDispatch], offset strnlen386
		jmp     strnlen386

	L1:
		mov     dword ptr [strnlenDispatch], offset strnlenSSE2
		jmp     strnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
