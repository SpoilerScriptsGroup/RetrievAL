#include <wchar.h>

#pragma function(wcslen)

#ifndef _M_IX86
size_t __cdecl wcslen(const wchar_t *string)
{
	size_t length;

	length = -1;
	string++;
	while (string[length++]);
	return length;
}
#else
static size_t __cdecl wcslenSSE2(const wchar_t *string);
static size_t __cdecl wcslen386(const wchar_t *string);
static size_t __cdecl wcslenCPUDispatch(const wchar_t *string);

static size_t(__cdecl * wcslenDispatch)(const wchar_t *string) = wcslenCPUDispatch;

__declspec(naked) size_t __cdecl wcslen(const wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslenDispatch]
	}
}

__declspec(naked) static size_t __cdecl wcslenSSE2(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     eax, dword ptr [string]                     // get pointer to string
		mov     ecx, 15                                     // set lower 4 bits mask
		pxor    xmm1, xmm1                                  // set to zero
		or      edx, -1                                     // fill mask bits
		test    eax, 1                                      // is aligned to word?
		jnz     unaligned                                   // jump if not aligned to word
		and     ecx, eax                                    // get lower 4 bits indicate misalignment
		jz      aligned_loop_entry                          // jump if aligned to wmmword
		shl     edx, cl                                     // shift out false bits
		sub     eax, ecx                                    // align pointer by 16
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		add     eax, 16                                     // increment pointer by 16
		or      edx, -1                                     // fill mask bits
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]                     // read 16 bytes aligned
		pcmpeqw xmm0, xmm1                                  // compare 8 words with zero
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      aligned_loop                                // loop if not found
		jmp     found

		align   16
	unaligned:
		mov     ecx, eax                                    // copy pointer
		and     eax, -16                                    // align pointer by 16
		inc     ecx                                         // add 1 byte
		dec     eax                                         // sub 1 byte
		and     ecx, 15                                     // compute (pointer + 1 byte) % 16
		jz      unaligned_loop                              // jump if pointer % 16 == 15
		movdqa  xmm0, xmmword ptr [eax + 1]                 // read 16 bytes aligned
		pslldq  xmm0, 1                                     // shift 1 byte for words compare
		shl     edx, cl                                     // shift out false bits
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		add     eax, 16                                     // increment pointer by 16
		or      edx, -1                                     // fill mask bits
		movdqu  xmm0, xmmword ptr [eax]                     // read 16 bytes unaligned
	unaligned_loop_entry:
		pcmpeqw xmm0, xmm1                                  // compare 8 words with zero
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      unaligned_loop                              // loop if not found

		align   16
	found:
		bsf     ecx, ecx                                    // get first bit index of result
		mov     edx, dword ptr [string]                     // get pointer to string
		add     eax, ecx                                    // add byte index
		sub     eax, edx                                    // subtract start address
		shr     eax, 1                                      // get number of words
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslen386(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]
		mov     eax, -1
		add     ecx, 2

		align   16
	loop_begin:
		mov     dx, word ptr [ecx + eax * 2]
		inc     eax
		test    dx, dx
		jnz     loop_begin
		ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslenCPUDispatch(const wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcslenDispatch], offset wcslen386
		jmp     wcslen386

	L1:
		mov     dword ptr [wcslenDispatch], offset wcslenSSE2
		jmp     wcslenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
