#define wmemchr inline_wmemchr
#define wmemcmp inline_wmemcmp
#include <wchar.h>
#undef wmemchr
#undef wmemcmp
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count);
extern int __cdecl wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#include <memory.h>
extern void * __cdecl _memichr(const void *buffer, int c, size_t count);
extern void * __cdecl _memrchr(const void *buffer, int c, size_t count);
extern void * __cdecl _memrichr(const void *buffer, int c, size_t count);
#include <tchar.h>
#include <xmmintrin.h>

#ifndef _UNICODE
# pragma function(memcmp)
# define  TYPE                  void
# define  MEMCMP                memcmp
# define  MEMICMP               _memicmp
# ifndef REVERSE
#  define MEMMEM                _memmem
#  define MEMIMEM               _memimem
#  define MEMMEM_SSE2           memmemSSE2
#  define MEMIMEM_SSE2          memimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_memmemSSE2
#  define MEMMEM_386            memmem386
#  define MEMIMEM_386           memimem386
#  define INTERNAL_MEMMEM_386   internal_memmem386
#  define MEMMEM_CPU_DISPATCH   memmemCPUDispatch
#  define MEMIMEM_CPU_DISPATCH  memimemCPUDispatch
#  define MEMMEM_DISPATCH       memmemDispatch
#  define MEMIMEM_DISPATCH      memimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_memchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_memichrSSE2
#  define INTERNAL_MEMCHR_386   internal_memchr386
#  define INTERNAL_MEMICHR_386  internal_memichr386
#  define MEMCHR                memchr
#  define MEMICHR               _memichr
# else
#  define MEMMEM                _memrmem
#  define MEMIMEM               _memrimem
#  define MEMMEM_SSE2           memrmemSSE2
#  define MEMIMEM_SSE2          memrimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_memrmemSSE2
#  define MEMMEM_386            memrmem386
#  define MEMIMEM_386           memrimem386
#  define INTERNAL_MEMMEM_386   internal_memrmem386
#  define MEMMEM_CPU_DISPATCH   memrmemCPUDispatch
#  define MEMIMEM_CPU_DISPATCH  memrimemCPUDispatch
#  define MEMMEM_DISPATCH       memrmemDispatch
#  define MEMIMEM_DISPATCH      memrimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_memrchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_memrichrSSE2
#  define INTERNAL_MEMCHR_386   internal_memrchr386
#  define INTERNAL_MEMICHR_386  internal_memrichr386
#  define MEMRCHR               _memrchr
#  define MEMRICHR              _memrichr
# endif
#else
# define  TYPE                  wchar_t
# define  MEMCMP                wmemcmp
# define  MEMICMP               _wmemicmp
# ifndef REVERSE
#  define MEMMEM                _wmemmem
#  define MEMIMEM               _wmemimem
#  define MEMMEM_SSE2           wmemmemSSE2
#  define MEMIMEM_SSE2          wmemimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_wmemmemSSE2
#  define MEMMEM_386            wmemmem386
#  define MEMIMEM_386           wmemimem386
#  define INTERNAL_MEMMEM_386   internal_wmemmem386
#  define MEMMEM_CPU_DISPATCH   wmemmemCPUDispatch
#  define MEMIMEM_CPU_DISPATCH  wmemimemCPUDispatch
#  define MEMMEM_DISPATCH       wmemmemDispatch
#  define MEMIMEM_DISPATCH      wmemimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_wmemchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_wmemichrSSE2
#  define INTERNAL_MEMCHR_386   wmemchr
#  define INTERNAL_MEMICHR_386  _wmemichr
#  define MEMCHR                wmemchr
#  define MEMICHR               _wmemichr
# else
#  define MEMMEM                _wmemrmem
#  define MEMIMEM               _wmemrimem
#  define MEMMEM_SSE2           wmemrmemSSE2
#  define MEMIMEM_SSE2          wmemrimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_wmemrmemSSE2
#  define MEMMEM_386            wmemrmem386
#  define MEMIMEM_386           wmemrimem386
#  define INTERNAL_MEMMEM_386   internal_wmemrmem386
#  define MEMMEM_CPU_DISPATCH   wmemrmemCPUDispatch
#  define MEMIMEM_CPU_DISPATCH  wmemrimemCPUDispatch
#  define MEMMEM_DISPATCH       wmemrmemDispatch
#  define MEMIMEM_DISPATCH      wmemrimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_wmemrchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_wmemrichrSSE2
#  define INTERNAL_MEMCHR_386   _wmemrchr
#  define INTERNAL_MEMICHR_386  _wmemrichr
#  define MEMRCHR               _wmemrchr
#  define MEMRICHR              _wmemrichr
# endif
#endif

#ifndef _M_IX86
TYPE * __cdecl MEMMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	TCHAR *first, *last, c;

	if (!needlelen)
		return (TCHAR *)haystack;
	if (haystacklen < needlelen)
		return NULL;
	last = (first = (TCHAR *)haystack) + haystacklen - needlelen + 1;
	c = *(TCHAR *)needle;
#ifndef REVERSE
	do
		if (!(first = MEMCHR(first, c, last - first)))
			break;
		else if (MEMCMP(first, needle, needlelen) == 0)
			return first;
	while (last > ++first);
#else
	do
		if (!(last = MEMRCHR(first, c, last - first)))
			break;
		else if (MEMCMP(last, needle, needlelen) == 0)
			return last;
	while (last > first);
#endif
	return NULL;
}

TYPE * __cdecl MEMIMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	TCHAR *first, *last, c;

	if (!needlelen)
		return (TCHAR *)haystack;
	if (haystacklen < needlelen)
		return NULL;
	last = (first = (TCHAR *)haystack) + haystacklen - needlelen + 1;
	c = *(TCHAR *)needle;
#ifndef REVERSE
	do
		if (!(first = MEMICHR(first, c, last - first)))
			break;
		else if (MEMICMP(first, needle, needlelen) == 0)
			return first;
	while (last > ++first);
#else
	do
		if (!(last = MEMRICHR(first, c, last - first)))
			break;
		else if (MEMICMP(last, needle, needlelen) == 0)
			return last;
	while (last > first);
#endif
	return NULL;
}
#else
static TYPE * __cdecl MEMMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMMEM_CPU_DISPATCH(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMIMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMIMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMIMEM_CPU_DISPATCH(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl INTERNAL_MEMMEM_SSE2(unsigned long c, TYPE *(__vectorcall *memchr)(const TYPE *, __m128, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
#ifndef _UNICODE
static TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__fastcall *memchr)(const TYPE *, unsigned long, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
#else
static TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__cdecl *memchr)(const TYPE *, TYPE, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
#endif

static TYPE *(__cdecl *MEMMEM_DISPATCH)(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen) = MEMMEM_CPU_DISPATCH;
static TYPE *(__cdecl *MEMIMEM_DISPATCH)(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen) = MEMIMEM_CPU_DISPATCH;

__declspec(naked) TYPE * __cdecl MEMMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	__asm
	{
		jmp     dword ptr [MEMMEM_DISPATCH]
	}
}

__declspec(naked) TYPE * __cdecl MEMIMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	__asm
	{
		jmp     dword ptr [MEMIMEM_DISPATCH]
	}
}

__declspec(naked) static TYPE * __cdecl MEMMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	extern TYPE * __vectorcall INTERNAL_MEMCHR_SSE2(const TYPE *buffer, __m128 c, size_t count);

#ifndef _UNICODE
	#define TCHAR_PTR byte ptr
	#define TA        al
	#define TC        cl
#else
	#define TCHAR_PTR word ptr
	#define TA        ax
	#define TC        cx
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]                  // eax = needlelen
		mov     ecx, dword ptr [needle]                     // ecx = needle
		test    eax, eax                                    // check if needlelen == 0
		jz      empty_needle                                // if needlelen == 0, leave
		mov     TA, TCHAR_PTR [ecx]
		xor     ecx, ecx
		mov     TC, TA
		push    offset MEMCMP
		push    offset INTERNAL_MEMCHR_SSE2
		push    ecx
		call    INTERNAL_MEMMEM_SSE2
		add     esp, 12
		ret

		align   16
	empty_needle:
		mov     eax, dword ptr [haystack]
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef TCHAR_PTR
	#undef TA
	#undef TC
}

__declspec(naked) static TYPE * __cdecl MEMIMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	extern TYPE * __vectorcall INTERNAL_MEMCHR_SSE2(const TYPE *buffer, __m128 c, size_t count);
	extern TYPE * __vectorcall INTERNAL_MEMICHR_SSE2(const TYPE *buffer, __m128 c, size_t count);

#ifndef _UNICODE
	#define TCHAR_PTR byte ptr
	#define TA        al
	#define TC        cl
#else
	#define TCHAR_PTR word ptr
	#define TA        ax
	#define TC        cx
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]                  // eax = needlelen
		mov     ecx, dword ptr [needle]                     // ecx = needle
		test    eax, eax                                    // check if needlelen == 0
		jz      empty_needle                                // if needlelen == 0, leave
		mov     TA, TCHAR_PTR [ecx]
		xor     ecx, ecx
		mov     TC, TA
		or      TA, 'a' - 'A'
		sub     TA, 'a'
		mov     edx, offset INTERNAL_MEMCHR_SSE2
		cmp     TA, 'z' - 'a' + 1
		jae     changed_to_lowercase
		mov     edx, offset INTERNAL_MEMICHR_SSE2
		or      ecx, 'a' - 'A'
	changed_to_lowercase:
		push    offset MEMICMP
		push    edx
		push    ecx
		call    INTERNAL_MEMMEM_SSE2
		add     esp, 12
		ret

		align   16
	empty_needle:
		mov     eax, dword ptr [haystack]                   // eax = haystack
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef TCHAR_PTR
	#undef TA
	#undef TC
}

__declspec(naked) static TYPE * __cdecl INTERNAL_MEMMEM_SSE2(unsigned long c, TYPE *(__vectorcall *memchr)(const TYPE *, __m128, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	__asm
	{
		#define c           (esp + 4)
		#define memchr      (esp + 8)
		#define memcmp      (esp + 12)
		#define reserved    (esp + 16)
		#define haystack    (esp + 20)
		#define haystacklen (esp + 24)
		#define needle      (esp + 28)
		#define needlelen   (esp + 32)

		mov     edx, dword ptr [haystacklen]                // edx = haystacklen
		mov     eax, dword ptr [needlelen]                  // eax = needlelen
		sub     edx, eax                                    // check if haystacklen < needlelen
		jae     above_or_equal                              // if haystacklen < needlelen, leave
		xor     eax, eax
		ret

		align   16
	above_or_equal:
		push    ebp                                         // preserve ebp
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
#ifdef _UNICODE
		lea     edx, [edx + edx + 2]                        // edx = (haystacklen - needlelen + 1) * sizeof(wchar_t)
#else
		inc     edx                                         // edx = haystacklen - needlelen + 1
#endif
		mov     ebp, esp
		sub     esp, 32

		#undef c
		#undef memchr
		#undef memcmp
		#undef reserved
		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen

		#define c           (ebp + 16)
		#define memchr      (ebp + 20)
		#define memcmp      (ebp + 24)
		#define reserved    (ebp + 28)
		#define haystack    (ebp + 32)
		#define haystacklen (ebp + 36)
		#define needle      (ebp + 40)
		#define needlelen   (ebp + 44)

		mov     esi, dword ptr [haystack]                   // esi = haystack
		and     esp, -16
		movd    xmm0, dword ptr [c]                         // xmm0 = first needle char
#ifndef _UNICODE
		punpcklbw xmm0, xmm0
#endif
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		movdqa  xmmword ptr [esp + 16], xmm0
		lea     edi, [esi + edx]                            // edi = haystack + haystacklen - needlelen + 1

		align   16
	loop_begin:
#ifdef _UNICODE
		shr     edx, 1
#endif
		mov     ecx, esi
		movdqa  xmm0, xmmword ptr [esp + 16]
		call    dword ptr [memchr]
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
		call    dword ptr [memcmp]
		test    eax, eax
		jz      found
		mov     edx, edi
#ifndef REVERSE
#ifdef _UNICODE
		add     esi, 2
#else
		inc     esi
#endif
#endif
		sub     edx, esi
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
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebp                                         // restore ebp
		ret

		#undef c
		#undef memchr
		#undef memcmp
		#undef reserved
		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}
}

__declspec(naked) static TYPE * __cdecl MEMIMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
#ifndef _UNICODE
	extern TYPE * __fastcall INTERNAL_MEMCHR_386(const TYPE *buffer, unsigned long c, size_t count);
	extern TYPE * __fastcall INTERNAL_MEMICHR_386(const TYPE *buffer, unsigned long c, size_t count);
#endif

#ifndef _UNICODE
	#define TCHAR_PTR byte ptr
	#define TA        al
	#define TC        cl
#else
	#define TCHAR_PTR word ptr
	#define TA        ax
	#define TC        cx
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]                  // eax = needlelen
		mov     ecx, dword ptr [needle]                     // ecx = needle
		test    eax, eax                                    // check if needlelen == 0
		jz      empty_needle                                // if needlelen == 0, leave
		mov     TA, TCHAR_PTR [ecx]
		xor     ecx, ecx
		mov     TC, TA
		or      TA, 'a' - 'A'
		sub     TA, 'a'
		mov     edx, offset INTERNAL_MEMCHR_386
		cmp     TA, 'z' - 'a' + 1
		jae     changed_to_lowercase
		mov     edx, offset INTERNAL_MEMICHR_386
		or      ecx, 'a' - 'A'
	changed_to_lowercase:
		push    offset MEMICMP
		push    edx
		push    ecx
		call    INTERNAL_MEMMEM_386
		add     esp, 12
		ret

		align   16
	empty_needle:
		mov     eax, dword ptr [haystack]                   // eax = haystack
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef TCHAR_PTR
	#undef TA
	#undef TC
}

__declspec(naked) static TYPE * __cdecl MEMMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
#ifndef _UNICODE
	extern TYPE * __fastcall INTERNAL_MEMCHR_386(const TYPE *buffer, unsigned long c, size_t count);
#endif

#ifndef _UNICODE
	#define TCHAR_PTR byte ptr
	#define TA        al
	#define TC        cl
#else
	#define TCHAR_PTR word ptr
	#define TA        ax
	#define TC        cx
#endif

	__asm
	{
		#define haystack    (esp + 4)
		#define haystacklen (esp + 8)
		#define needle      (esp + 12)
		#define needlelen   (esp + 16)

		mov     eax, dword ptr [needlelen]                  // eax = needlelen
		mov     ecx, dword ptr [needle]                     // ecx = needle
		test    eax, eax                                    // check if needlelen == 0
		jz      empty_needle                                // if needlelen == 0, leave
		mov     TA, TCHAR_PTR [ecx]
		xor     ecx, ecx
		mov     TC, TA
		push    offset MEMCMP
		push    offset INTERNAL_MEMCHR_386
		push    ecx
		call    INTERNAL_MEMMEM_386
		add     esp, 12
		ret

		align   16
	empty_needle:
		mov     eax, dword ptr [haystack]
		ret

		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}

	#undef TCHAR_PTR
	#undef TA
	#undef TC
}

#ifndef _UNICODE
__declspec(naked) static TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__fastcall *memchr)(const TYPE *, unsigned long, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
#else
__declspec(naked) static TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__cdecl *memchr)(const TYPE *, TYPE, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
#endif
{
	__asm
	{
		#define c           (esp + 4)
		#define memchr      (esp + 8)
		#define memcmp      (esp + 12)
		#define reserved    (esp + 16)
		#define haystack    (esp + 20)
		#define haystacklen (esp + 24)
		#define needle      (esp + 28)
		#define needlelen   (esp + 32)

		mov     eax, dword ptr [haystacklen]                // eax = haystacklen
		mov     ecx, dword ptr [needlelen]                  // ecx = needlelen
		sub     eax, ecx                                    // check if haystacklen < needlelen
		jae     above_or_equal                              // if haystacklen < needlelen, leave
		xor     eax, eax
		ret

		align   16
	above_or_equal:
#ifndef _UNICODE
		                                                    // set all 4 bytes of ebx to [value]
		mov     ecx, dword ptr [c]                          // ecx = c
		mov     edx, dword ptr [haystack]                   // edx = haystack
		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     ebx, ecx                                    // ebx = 0/0/0/c
		shl     ecx, 8                                      // ecx = 0/0/c/0
		inc     eax                                         // eax = haystacklen - needlelen + 1
		or      ecx, ebx                                    // ecx = 0/0/c/c
		mov     esi, edx                                    // esi = haystack
		mov     ebx, ecx                                    // ebx = 0/0/c/c
		lea     edi, [edx + eax]                            // edi = haystack + haystacklen - needlelen + 1
		shl     ecx, 16                                     // ecx = c/c/0/0
		sub     esp, 12
		or      ebx, ecx                                    // ebx = all 4 bytes = [search char]

		align   16
	loop_begin:
		mov     ecx, esi
		mov     edx, ebx
		push    eax
		call    dword ptr [memchr + 28]
#else
		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		lea     eax, [eax + eax + 2]                        // eax = (haystacklen - needlelen + 1) * sizeof(wchar_t)
		mov     esi, dword ptr [haystack + 12]              // esi = haystack
		mov     ebx, dword ptr [c + 12]                     // ebx = first needle char
		sub     esp, 12
		lea     edi, [esi + eax]                            // edi = haystack + haystacklen - needlelen + 1

		align   16
	loop_begin:
		shr     eax, 1
		mov     dword ptr [esp], esi
		mov     dword ptr [esp + 4], ebx
		mov     dword ptr [esp + 8], eax
		call    dword ptr [memchr + 24]
#endif
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
		call    dword ptr [memcmp + 24]
		test    eax, eax
		jz      found
		mov     eax, edi
#ifndef REVERSE
#ifndef _UNICODE
		inc     esi
#else
		add     esi, 2
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
		add     esp, 12
#ifndef REVERSE
		mov     eax, esi
#else
		mov     eax, edi
#endif
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret

		#undef c
		#undef memchr
		#undef memcmp
		#undef reserved
		#undef haystack
		#undef haystacklen
		#undef needle
		#undef needlelen
	}
}

__declspec(naked) static TYPE * __cdecl MEMMEM_CPU_DISPATCH(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [MEMMEM_DISPATCH], offset MEMMEM_386
		jmp     MEMMEM_386

	L1:
		mov     dword ptr [MEMMEM_DISPATCH], offset MEMMEM_SSE2
		jmp     MEMMEM_SSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}

__declspec(naked) static TYPE * __cdecl MEMIMEM_CPU_DISPATCH(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [MEMIMEM_DISPATCH], offset MEMIMEM_386
		jmp     MEMIMEM_386

	L1:
		mov     dword ptr [MEMIMEM_DISPATCH], offset MEMIMEM_SSE2
		jmp     MEMIMEM_SSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif

#undef TYPE
#undef MEMCMP
#undef MEMICMP
#undef MEMMEM
#undef MEMIMEM
#undef MEMMEM_SSE2
#undef MEMIMEM_SSE2
#undef INTERNAL_MEMMEM_SSE2
#undef MEMMEM_386
#undef MEMIMEM_386
#undef INTERNAL_MEMMEM_386
#undef MEMMEM_CPU_DISPATCH
#undef MEMIMEM_CPU_DISPATCH
#undef MEMMEM_DISPATCH
#undef MEMIMEM_DISPATCH
#undef INTERNAL_MEMCHR_SSE2
#undef INTERNAL_MEMICHR_SSE2
#undef INTERNAL_MEMCHR_386
#undef INTERNAL_MEMICHR_386
#undef MEMCHR
#undef MEMICHR
#undef MEMRCHR
#undef MEMRICHR
