#define wmemchr inline_wmemchr
#define wmemcmp inline_wmemcmp
#include <wchar.h>
#undef wmemchr
#undef wmemcmp
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);
extern int __cdecl wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#include <memory.h>
#include <tchar.h>
#include <xmmintrin.h>

#ifndef _UNICODE
# pragma function(memcmp)
# define  type                void
# ifdef REVERSE
#  define _memmem             _memrmem
#  define memmemSSE2          memrmemSSE2
#  define memmem386           memrmem386
#  define memmemCPUDispatch   memrmemCPUDispatch
#  define memmemDispatch      memrmemDispatch
#  define internal_memchrSSE2 internal_memrchrSSE2
#  define _memchr             _memrchr
   extern void * __cdecl _memrchr(const void *buffer, int c, size_t count);
# endif
#else
# define  type                wchar_t
# define  memcmp              wmemcmp
# ifndef REVERSE
#  define _memmem             _wmemmem
#  define memmemSSE2          wmemmemSSE2
#  define memmem386           wmemmem386
#  define memmemCPUDispatch   wmemmemCPUDispatch
#  define memmemDispatch      wmemmemDispatch
#  define internal_memchrSSE2 internal_wmemchrSSE2
#  define memchr              wmemchr
# else
#  define _memmem             _wmemrmem
#  define memmemSSE2          wmemrmemSSE2
#  define memmem386           wmemrmem386
#  define memmemCPUDispatch   wmemrmemCPUDispatch
#  define memmemDispatch      wmemrmemDispatch
#  define internal_memchrSSE2 internal_wmemrchrSSE2
#  define _memchr             _wmemrchr
# endif
#endif

#ifndef _M_IX86
type * __cdecl _memmem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *first, *last, c;

		last = (first = (TCHAR *)haystack) + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
#ifndef REVERSE
		do
			if (!(first = memchr(first, c, last - first)))
				break;
			else if (memcmp(first, needle, needlelen) == 0)
				return first;
		while (last > ++first);
#else
		do
			if (!(last = _memrchr(first, c, last - first)))
				break;
			else if (memcmp(last, needle, needlelen) == 0)
				return last;
		while (last > first);
#endif
	}
	return NULL;
}
#else
static type * __cdecl memmemSSE2(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);
static type * __cdecl memmem386(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);
static type * __cdecl memmemCPUDispatch(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);

static type *(__cdecl * memmemDispatch)(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen) = memmemCPUDispatch;

__declspec(naked) type * __cdecl _memmem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	__asm
	{
		jmp     dword ptr [memmemDispatch]
	}
}

__declspec(naked) static type * __cdecl memmemSSE2(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	extern type * __vectorcall internal_memchrSSE2(const type *buffer, __m128 c, size_t count);

#ifdef _UNICODE
	#define tchar_ptr word ptr
	#define ta        ax
#else
	#define tchar_ptr byte ptr
	#define ta        al
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]              // eax = needlelen
		mov     ecx, dword ptr [haystacklen]            // ecx = haystacklen
		test    eax, eax
		jz      needlelen_equal_zero
		push    ebp                                     // preserve ebp
		push    esi                                     // preserve esi
		push    edi                                     // preserve edi
		mov     esi, eax                                // esi = needlelen
		mov     ebp, esp
		mov     edx, dword ptr [needle + 12]            // edx = needle

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen

		#define haystack    (ebp + 16)
		#define haystacklen (ebp + 20)
		#define needle      (ebp + 24)
		#define needlelen   (ebp + 28)

		sub     ecx, esi                                // check if haystacklen < needlelen
		jb      not_found                               // if haystacklen < needlelen, leave
		xor     eax, eax
		sub     esp, 32
		mov     ta, tchar_ptr [edx]
		and     esp, -16
		movd    xmm0, eax                               // xmm0 = first needle char
#ifndef _UNICODE
		punpcklbw xmm0, xmm0
#endif
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		movdqa  xmmword ptr [esp + 16], xmm0
		mov     esi, dword ptr [haystack]               // esi = haystack
#ifdef _UNICODE
		lea     eax, [ecx + ecx + 2]                    // eax = (haystacklen - needlelen + 1) * sizeof(wchar_t)
#else
		lea     eax, [ecx + 1]                          // eax = haystacklen - needlelen + 1
#endif
		lea     edi, [esi + eax]                        // edi = haystack + haystacklen - needlelen + 1

		align   16
	loop_begin:
#ifdef _UNICODE
		shr     eax, 1
#endif
		mov     ecx, esi
		mov     edx, eax
		movdqa  xmm0, xmmword ptr [esp + 16]
		call    internal_memchrSSE2
		test    eax, eax
		jz      not_found
		mov     ecx, dword ptr [needle]
		mov     edx, dword ptr [needlelen]
#ifndef REVERSE
		mov     esi, eax
#else
		mov     edi, eax
#endif
		mov     dword ptr [esp], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp + 8], edx
		call    memcmp
		test    eax, eax
		jz      found
		mov     eax, edi
#ifndef REVERSE
#ifdef _UNICODE
		add     esi, 2
#else
		inc     esi
#endif
#endif
		sub     eax, esi
		ja      loop_begin
	not_found:
#ifndef REVERSE
		xor     esi, esi
#else
		xor     edi, edi
#endif
	found:
		mov     esp, ebp
#ifndef REVERSE
		mov     eax, esi
#else
		mov     eax, edi
#endif
		pop     edi                                     // restore edi
		pop     esi                                     // restore esi
		pop     ebp                                     // restore ebp
	needlelen_equal_zero:
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef tchar_ptr
	#undef ta
}

__declspec(naked) static type * __cdecl memmem386(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
#ifdef _UNICODE
	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		push    ebx
		push    esi
		push    edi
		xor     ebx, ebx
		mov     ecx, dword ptr [needlelen + 12]
		sub     esp, 12
		test    ecx, ecx
		jz      not_found
		mov     eax, dword ptr [haystacklen + 24]
		mov     edx, dword ptr [needle + 24]
		sub     eax, ecx
		jb      not_found
		mov     esi, dword ptr [haystack + 24]
		lea     eax, [eax + eax + 2]
		mov     bx, word ptr [edx]
		lea     edi, [eax + esi]

		align   16
	loop_begin:
		shr     eax, 1
		mov     dword ptr [esp], esi
		mov     dword ptr [esp + 4], ebx
		mov     dword ptr [esp + 8], eax
		call    wmemchr
		test    eax, eax
		jz      not_found
		mov     ecx, dword ptr [needle + 24]
		mov     edx, dword ptr [needlelen + 24]
#ifndef REVERSE
		mov     esi, eax
#else
		mov     edi, eax
#endif
		mov     dword ptr [esp], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp + 8], edx
		call    wmemcmp
		test    eax, eax
		jz      found
		mov     eax, edi
#ifndef REVERSE
		add     esi, 2
#endif
		sub     eax, esi
		ja      loop_begin
	not_found:
#ifndef REVERSE
		xor     esi, esi
#else
		xor     edi, edi
#endif
	found:
		add     esp, 12
#ifndef REVERSE
		mov     eax, esi
#else
		mov     eax, edi
#endif
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}
#else
	extern type * __fastcall internal_memchr386(const type *buffer, unsigned long c, size_t count);

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     ecx, dword ptr [needlelen]              // ecx = needlelen
		xor     eax, eax                                // eax = 0
		test    ecx, ecx                                // check if needlelen == 0
		jz      needlelen_equal_zero                    // if needlelen == 0, leave
		push    ebx                                     // preserve ebx
		push    ebp                                     // preserve ebp
		push    esi                                     // preserve esi
		push    edi                                     // preserve edi
		mov     eax, dword ptr [haystacklen + 16]       // eax = haystacklen
		sub     esp, 12
		sub     eax, ecx                                // check if haystacklen < needlelen
		jb      not_found                               // if haystacklen < needlelen, leave
		                                                // set all 4 bytes of ebx to [value]
		mov     edx, dword ptr [needle + 28]            // edx = needle
		mov     ebp, ecx                                // ebp = needlelen
		xor     ecx, ecx                                // ecx = 0
		mov     esi, dword ptr [haystack + 28]          // esi = haystack
		mov     cl, byte ptr [edx]                      // cl = search char
		inc     eax                                     // eax = haystacklen - needlelen + 1
		mov     ebx, ecx                                // ebx = 0/0/0/c
		mov     edi, esi                                // edi = haystack
		shl     ecx, 8                                  // ecx = 0/0/c/0
		add     edi, eax                                // edi = haystack + haystacklen - needlelen + 1
		or      ecx, ebx                                // ecx = 0/0/c/c
		mov     ebx, ecx                                // ebx = 0/0/c/c
		shl     ecx, 16                                 // ecx = c/c/0/0
		or      ebx, ecx                                // ebx = all 4 bytes = [search char]

		align   16
	loop_begin:
		mov     ecx, esi
		mov     edx, ebx
		push    eax
		call    internal_memchr386
		test    eax, eax
		jz      not_found
		mov     ecx, dword ptr [needle + 28]
		mov     edx, dword ptr [needlelen + 28]
#ifndef REVERSE
		mov     esi, eax
#else
		mov     edi, eax
#endif
		mov     dword ptr [esp], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp + 8], edx
		call    memcmp
		test    eax, eax
		jz      found
		mov     eax, edi
#ifndef REVERSE
		inc     esi
#endif
		sub     eax, esi
		ja      loop_begin
	not_found:
#ifndef REVERSE
		xor     esi, esi
#else
		xor     edi, edi
#endif
	found:
		add     esp, 12
#ifndef REVERSE
		mov     eax, esi
#else
		mov     eax, edi
#endif
		pop     edi                                     // restore edi
		pop     esi                                     // restore esi
		pop     ebp                                     // restore ebp
		pop     ebx                                     // restore ebx
	needlelen_equal_zero:
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}
#endif
}

__declspec(naked) static type * __cdecl memmemCPUDispatch(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memmemDispatch], offset memmem386
		jmp     memmem386
	L1:
		mov     dword ptr [memmemDispatch], offset memmemSSE2
		jmp     memmemSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
