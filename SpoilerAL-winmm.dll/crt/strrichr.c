#include <string.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
char * __cdecl _strrichr(const char *string, int c)
{
	const char *p;
	char       c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return strrchr(string, c);
	p = string - 1;
	do
		if (((c2 = *(++p)) | ('a' - 'A')) == c1)
			goto DONE;
	while (c2);
	p = NULL;
DONE:
	return (char *)p;
}
#else
extern const char casebitA[16];

static char * __cdecl strrichrSSE2(const char *string, int c);
static char * __cdecl strrichr386(const char *string, int c);
static char * __cdecl strrichrCPUDispatch(const char *string, int c);

static char *(__cdecl * strrichrDispatch)(const char *string, int c) = strrichrCPUDispatch;

__declspec(naked) char * __cdecl _strrichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrichrDispatch]
	}
}

__declspec(naked) static char * __cdecl strrichrSSE2(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a'
		ja      strrchr
		push    ebx
		xor     ebx, ebx
		pxor    xmm1, xmm1
		movd    xmm2, edx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebitA]
		test    eax, 15
		jz      main_loop_entry
		mov     ecx, eax
		and     eax, -16
		and     ecx, 15
		dec     ebx
		shl     ebx, cl
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb edx, xmm0
		pxor    xmm1, xmm1
		and     ecx, ebx
		and     edx, ebx
		xor     ebx, ebx
		or      edx, ecx
		jz      main_loop_increment
		test    ecx, ecx
		jnz     null_is_found

		align   16
	main_loop:
		bsr     edx, edx
		lea     ebx, [eax + edx]
	main_loop_increment:
		add     eax, 16
	main_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jz      main_loop_increment
		pmovmskb ecx, xmm1
		test    ecx, ecx
		jz      main_loop
	null_is_found:
		xor     edx, ecx
		jz      epilogue
		bsf     ecx, ecx
		add     eax, ecx
		xor     ecx, 15
		shl     edx, cl
		and     edx, 7FFFH
		jz      epilogue
		bsr     edx, edx
		lea     eax, [eax + edx - 15]
		pop     ebx
		ret

		align   16
	epilogue:
		mov     eax, ebx
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrichr386(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		or      edx, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, dl
		sub     edx, 'a'
		cmp     dl, 'z' - 'a'
		ja      strrchr
		mov     edx, ecx
		push    ebx
		shl     ecx, 8
		push    ebp
		or      ecx, edx
		push    esi
		mov     ebx, ecx
		push    edi
		shl     ecx, 16
		xor     ebp, ebp
		or      ebx, ecx

		align   16
	misaligned_loop:
		test    eax, 3
		jz      main_loop
		mov     cl, byte ptr [eax]
		inc     eax
		mov     dl, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		jne     is_null
		lea     ebp, [eax - 1]
		jmp     misaligned_loop
	is_null:
		test    dl, dl
		jnz     misaligned_loop
		mov     eax, ebp
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	main_loop:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     esi, ecx
		or      ecx, 20202020H
		mov     edx, ecx
		xor     ecx, ebx
		sub     esi, 01010101H
		lea     edi, [ecx + 7EFEFEFFH]
		xor     edx, -1
		xor     ecx, -1
		and     esi, edx
		xor     ecx, edi
		and     esi, 80808080H
		jnz     null_is_found
		and     ecx, 81010100H
		jz      main_loop
		and     ecx, 01010100H
		jnz     chr_is_found
		test    edi, edi
		js      main_loop
	chr_is_found:
		mov     ebp, eax
		jmp     main_loop

		align   16
	null_is_found:
		and     ecx, 01010100H
		jz      process_stored_pointer
		not     edx
		bswap   edx
		add     esi, esi
		jz      compare_byte_2
		shl     esi, 8
		jz      compare_byte_1
		shr     edx, 16
		jmp     compare_byte_0

		align   16
	compare_byte_2:
		cmp     dh, bl
		je      byte_2
	compare_byte_1:
		shr     edx, 16
		nop                                             // padding 1 byte
		cmp     dl, bl
		je      byte_1
	compare_byte_0:
		cmp     dh, bl
		je      byte_0

		// 16 byte aligned
	process_stored_pointer:
		mov     eax, ebp
		test    ebp, ebp
		jz      restore_register
		test    eax, 3                                  // append 3 byte (and ebp,imm -> test eax,imm)
		jnz     restore_register
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		or      ecx, 20202020H
		align   16                                      // padding 8 byte
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		nop                                             // padding 1 byte
		cmp     cl, bl
		je      byte_1

		// 16 byte aligned
	byte_0:
		sub     eax, 4
		jmp     restore_register

	byte_1:
		sub     eax, 3
		jmp     restore_register

	byte_2:
		sub     eax, 2
		jmp     restore_register

	byte_3:
		dec     eax

		// 16 byte aligned
	restore_register:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strrichrDispatch], offset strrichr386
		jmp     strrichr386
	L1:
		mov     dword ptr [strrichrDispatch], offset strrichrSSE2
		jmp     strrichrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
