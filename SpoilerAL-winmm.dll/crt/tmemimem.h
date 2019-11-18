#include <wchar.h>
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#include <memory.h>
#include <tchar.h>
#include <xmmintrin.h>

#ifndef _UNICODE
# define  type                 void
# ifdef REVERSE
#  define _memimem             _memrimem
#  define memimemSSE2          memrimemSSE2
#  define memimem386           memrimem386
#  define memimemCPUDispatch   memrimemCPUDispatch
#  define memimemDispatch      memrimemDispatch
#  define internal_memchrSSE2  internal_memrchrSSE2
#  define internal_memichrSSE2 internal_memrichrSSE2
#  define _memichr             _memrichr
   extern void * __cdecl _memrichr(const void *buffer, int c, size_t count);
# endif
#else
# define  type                 wchar_t
# define  _memicmp             _wmemicmp
# ifndef REVERSE
#  define _memimem             _wmemimem
#  define memimemSSE2          wmemimemSSE2
#  define memimem386           wmemimem386
#  define memimemCPUDispatch   wmemimemCPUDispatch
#  define memimemDispatch      wmemimemDispatch
#  define internal_memchrSSE2  internal_wmemchrSSE2
#  define internal_memichrSSE2 internal_wmemichrSSE2
#  define _memichr             _wmemichr
# else
#  define _memimem             _wmemrimem
#  define memimemSSE2          wmemrimemSSE2
#  define memimem386           wmemrimem386
#  define memimemCPUDispatch   wmemrimemCPUDispatch
#  define memimemDispatch      wmemrimemDispatch
#  define internal_memchrSSE2  internal_wmemrchrSSE2
#  define internal_memichrSSE2 internal_wmemrichrSSE2
#  define _memichr             _wmemrichr
# endif
#endif

#ifndef _M_IX86
type * __cdecl _memimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *first, *last, c;

		last = (first = (TCHAR *)haystack) + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
#ifndef REVERSE
		do
			if (!(first = _memichr(first, c, last - first)))
				break;
			else if (_memicmp(first, needle, needlelen) == 0)
				return first;
		while (last > ++first);
#else
		do
			if (!(last = _memrichr(first, c, last - first)))
				break;
			else if (_memicmp(last, needle, needlelen) == 0)
				return last;
		while (last > first);
#endif
	}
	return NULL;
}
#else
#pragma warning(disable:4414)

static type * __cdecl memimemSSE2(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);
static type * __cdecl memimem386(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);
static type * __cdecl memimemCPUDispatch(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen);

static type *(__cdecl * memimemDispatch)(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen) = memimemCPUDispatch;

__declspec(naked) type * __cdecl _memimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	__asm
	{
		jmp     dword ptr [memimemDispatch]
	}
}

__declspec(naked) static type * __cdecl memimemSSE2(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	extern type * __vectorcall internal_memchrSSE2(const type *buffer, __m128 c, size_t count);
	extern type * __vectorcall internal_memichrSSE2(const type *buffer, __m128 c, size_t count);

#ifdef _UNICODE
	#define tchar_ptr word ptr
	#define ta        ax
	#define tc        cx
#else
	#define tchar_ptr byte ptr
	#define ta        al
	#define tc        cl
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]              // eax = needlelen
		mov     edx, dword ptr [needle]                 // edx = needle
		test    eax, eax                                // check if needlelen == 0
		jz      needlelen_equal_zero                    // if needlelen == 0, leave
		push    ebx                                     // preserve ebx
		push    ebp                                     // preserve ebp
		push    esi                                     // preserve esi
		push    edi                                     // preserve edi
		mov     ta, tchar_ptr [edx]
		mov     ebx, offset internal_memichrSSE2
		or      ta, 'a' - 'A'
		xor     ecx, ecx
		mov     tc, ta
		sub     ta, 'a'
		cmp     ta, 'z' - 'a'
		jbe     changed_to_lowercase
		mov     tc, tchar_ptr [edx]
		mov     ebx, offset internal_memchrSSE2
	changed_to_lowercase:
		mov     eax, dword ptr [haystacklen + 16]       // eax = haystacklen
		mov     edx, dword ptr [needlelen + 16]         // edx = needlelen
		mov     ebp, esp
		sub     esp, 32

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen

		#define haystack    (ebp + 20)
		#define haystacklen (ebp + 24)
		#define needle      (ebp + 28)
		#define needlelen   (ebp + 32)

		sub     eax, edx                                // check if haystacklen < needlelen
		jb      not_found                               // if haystacklen < needlelen, leave
		mov     edi, dword ptr [haystack]               // edi = haystack
		and     esp, -16
		movd    xmm0, ecx                               // xmm0 = first needle char
#ifndef _UNICODE
		punpcklbw xmm0, xmm0
#endif
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		movdqa  xmmword ptr [esp + 16], xmm0
		mov     esi, edi                                // esi = haystack
#ifdef _UNICODE
		lea     eax, [eax + eax + 2]                    // eax = (haystacklen - needlelen + 1) * sizeof(wchar_t)
#else
		inc     eax                                     // eax = haystacklen - needlelen + 1
#endif
		add     edi, eax                                // edi = haystack + haystacklen - needlelen + 1

		align   16
	loop_begin:
#ifdef _UNICODE
		shr     eax, 1
#endif
		mov     ecx, esi
		mov     edx, eax
		movdqa  xmm0, xmmword ptr [esp + 16]
		call    ebx
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
		call    _memicmp
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
		pop     ebx                                     // restore ebx
	needlelen_equal_zero:
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef tchar_ptr
	#undef ta
	#undef tc
}

__declspec(naked) static type * __cdecl memimem386(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
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
		call    _wmemichr
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
		call    _wmemicmp
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
	extern type * __fastcall internal_memichr386(const type *buffer, unsigned long c, size_t count);

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]              // eax = needlelen
		mov     edx, dword ptr [needle]                 // ecx = needle
		test    eax, eax                                // check if needlelen == 0
		jz      needlelen_equal_zero                    // if needlelen == 0, leave
		push    ebx                                     // preserve ebx
		push    ebp                                     // preserve ebp
		mov     al, byte ptr [edx]
		mov     ebp, offset internal_memichr386
		or      al, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, al
		sub     al, 'a'
		cmp     al, 'z' - 'a'
		jbe     changed_to_lowercase
		mov     cl, byte ptr [edx]
		mov     ebp, offset internal_memchr386
	changed_to_lowercase:
		                                                // set all 4 bytes of ebx to [value]
		mov     ebx, ecx                                // ebx = 0/0/0/c
		push    esi                                     // preserve esi
		shl     ecx, 8                                  // ecx = 0/0/c/0
		push    edi                                     // preserve edi
		or      ebx, ecx                                // ebx = 0/0/c/c
		sub     esp, 12
		mov     eax, dword ptr [haystacklen + 28]       // eax = haystacklen
		mov     ecx, dword ptr [needlelen + 28]         // ecx = needlelen
		sub     eax, ecx                                // check if haystacklen < needlelen
		jb      not_found                               // if haystacklen < needlelen, leave
		mov     esi, ebx                                // esi = 0/0/c/c
		mov     edi, dword ptr [haystack + 28]          // edi = haystack
		shl     ebx, 16                                 // ebx = c/c/0/0
		inc     eax                                     // eax = haystacklen - needlelen + 1
		or      ebx, esi                                // ebx = all 4 bytes = [search char]
		mov     esi, edi                                // esi = haystack
		add     edi, eax                                // edi = haystack + haystacklen - needlelen + 1

		align   16
	loop_begin:
		mov     ecx, esi
		mov     edx, ebx
		push    eax
		call    ebp
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
		call    _memicmp
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

__declspec(naked) static type * __cdecl memimemCPUDispatch(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memimemDispatch], offset memimem386
		jmp     memimem386
	L1:
		mov     dword ptr [memimemDispatch], offset memimemSSE2
		jmp     memimemSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
