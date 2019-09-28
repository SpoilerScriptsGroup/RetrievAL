#pragma function(strcmp)

#include "PageSize.h"

extern int __cdecl InstructionSet();

static int __cdecl strcmpSSE42(const char *string1, const char *string2);
static int __cdecl strcmpSSE2(const char *string1, const char *string2);
static int __cdecl strcmpGeneric(const char *string1, const char *string2);
static int __cdecl strcmpCPUDispatch(const char *string1, const char *string2);

static int(__cdecl * strcmpDispatch)(const char *string1, const char *string2) = strcmpCPUDispatch;

// strcmp function
__declspec(naked) int __cdecl strcmp(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [strcmpDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) static int __cdecl strcmpSSE42(const char *string1, const char *string2)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]                    // string 1
		mov     edx, dword ptr [esp + 8]                    // string 2
		push    ebx
		mov     ebx, -16                                    // offset counter

	compareloop:
		add     ebx, 16                                     // increment offset
		movdqu  xmm1, xmmword ptr [eax + ebx]               // read 16 bytes of string 1
		pcmpistri xmm1, xmmword ptr [edx + ebx], 00011000B  // unsigned bytes, equal each, invert. returns index in ecx
		jnbe    compareloop                                 // jump if not carry flag and not zero flag
		jnc     equal

	//notequal:
		// strings are not equal
		add     ecx, ebx                                    // offset to first differing byte
		movzx   eax, byte ptr [eax + ecx]                   // compare bytes
		movzx   edx, byte ptr [edx + ecx]
		sub     eax, edx
		pop     ebx
		ret

	equal:
		xor     eax, eax                                    // strings are equal
		pop     ebx
		ret
	}
}

// SSE2 version
__declspec(naked) static int __cdecl strcmpSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax                                    // esi = string2 - string1
		pxor    xmm2, xmm2
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		lea     edi, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     edi, PAGE_SIZE - 1

		align   16
	dword_loop:
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     edi, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		add     eax, 4
		sub     ecx, 01010101H
		xor     edx, -1
		lea     edi, [eax + esi]
		and     ecx, 80808080H
		and     edi, PAGE_SIZE - 1
		and     edx, ecx
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		align   16
	xmmword_loop:
		cmp     edi, PAGE_SIZE - 16
		ja      dword_check_cross_pages                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [eax + esi]
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		lea     edi, [eax + esi + 16]
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edi, 0FFFFH
		xor     ecx, 15
		shr     edi, cl
		and     edx, edi
		jz      return_equal
	xmmword_has_not_null:
		bsf     edx, edx
		add     edx, eax
		movzx   eax, byte ptr [edx + esi]
		movzx   ecx, byte ptr [edx]
		sub     eax, ecx
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}
}

// generic version
__declspec(naked) static int __cdecl strcmpGeneric(const char *string1, const char *string2)
{
#if 0
	__asm
	{
		// This is a very simple solution. There is not much gained by using SSE2 or anything complicated
		mov     ecx, dword ptr [esp + 4]                    // string 1
		mov     edx, dword ptr [esp + 8]                    // string 2

	compareloop:
		mov     al, byte ptr [ecx]
		cmp     al, byte ptr [edx]
		jne     notequal
		test    al, al
		jz      equal
		inc     ecx
		inc     edx
		jmp     compareloop

	equal:
		xor     eax, eax                                    // strings are equal
		ret

	notequal:
		// strings are not equal
		movzx   eax, byte ptr [ecx]                         // compare first differing byte
		movzx   edx, byte ptr [edx]
		sub     eax, edx
		ret
	}
#else
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     eax, dword ptr [string2 + 8]                // eax = string2
		sub     esi, eax
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
		lea     ebx, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     ebx, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     ebx, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     ecx, dword ptr [eax + esi]
		mov     edx, dword ptr [eax]
		cmp     ecx, edx
		jne     byte_loop                                   // not equal
		add     eax, 4
		sub     edx, 01010101H
		xor     ecx, -1
		lea     ebx, [eax + esi]
		and     edx, 80808080H
		and     ebx, PAGE_SIZE - 1
		and     edx, ecx
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
#endif
}

// CPU dispatching for strcmp. This is executed only once
__declspec(naked) static int __cdecl strcmpCPUDispatch(const char *string1, const char *string2)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version of strcmp
		mov     ecx, offset strcmpGeneric
		cmp     eax, 4                                      // check SSE2
		jb      Q100

		// SSE2 supported
		// Point to SSE2 version of strcmp
		mov     ecx, offset strcmpSSE2
		cmp     eax, 10                                     // check SSE4.2
		jb      Q100

		// SSE4.2 supported
		// Point to SSE4.2 version of strcmp
		mov     ecx, offset strcmpSSE42

	Q100:
		mov     dword ptr [strcmpDispatch], ecx

		// Continue in appropriate version of strcmp
		jmp     ecx
	}
}
