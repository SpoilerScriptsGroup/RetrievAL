#define wmemchr inline_wmemchr
#include <wchar.h>
#undef wmemchr
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);
extern wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#include <memory.h>
extern void * __cdecl _memrichr(const void *buffer, int c, size_t count);
#include <tchar.h>
#include <xmmintrin.h>

#ifndef _UNICODE
# define  TYPE                  void
# define  MEMICMP               _memicmp
# ifndef REVERSE
#  define MEMIMEM               _memimem
#  define MEMIMEM_SSE2          memimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_memmemSSE2
#  define MEMIMEM_386           memimem386
#  define INTERNAL_MEMMEM_386   internal_memmem386
#  define MEMIMEM_CPU_DISPATCH  memimemCPUDispatch
#  define MEMIMEM_DISPATCH      memimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_memchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_memichrSSE2
#  define INTERNAL_MEMCHR_386   internal_memchr386
#  define INTERNAL_MEMICHR_386  internal_memichr386
#  define MEMICHR               _memichr
# else
#  define MEMIMEM               _memrimem
#  define MEMIMEM_SSE2          memrimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_memrmemSSE2
#  define MEMIMEM_386           memrimem386
#  define INTERNAL_MEMMEM_386   internal_memrmem386
#  define MEMIMEM_CPU_DISPATCH  memrimemCPUDispatch
#  define MEMIMEM_DISPATCH      memrimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_memrchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_memrichrSSE2
#  define INTERNAL_MEMCHR_386   internal_memrchr386
#  define INTERNAL_MEMICHR_386  internal_memrichr386
#  define MEMICHR               _memrichr
# endif
#else
# define  TYPE                  wchar_t
# define  MEMICMP               _wmemicmp
# ifndef REVERSE
#  define MEMIMEM               _wmemimem
#  define MEMIMEM_SSE2          wmemimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_wmemmemSSE2
#  define MEMIMEM_386           wmemimem386
#  define INTERNAL_MEMMEM_386   internal_wmemmem386
#  define MEMIMEM_CPU_DISPATCH  wmemimemCPUDispatch
#  define MEMIMEM_DISPATCH      wmemimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_wmemchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_wmemichrSSE2
#  define INTERNAL_MEMCHR_386   wmemchr
#  define INTERNAL_MEMICHR_386  _wmemichr
#  define MEMICHR               _wmemichr
# else
#  define MEMIMEM               _wmemrimem
#  define MEMIMEM_SSE2          wmemrimemSSE2
#  define INTERNAL_MEMMEM_SSE2  internal_wmemrmemSSE2
#  define MEMIMEM_386           wmemrimem386
#  define INTERNAL_MEMMEM_386   internal_wmemrmem386
#  define MEMIMEM_CPU_DISPATCH  wmemrimemCPUDispatch
#  define MEMIMEM_DISPATCH      wmemrimemDispatch
#  define INTERNAL_MEMCHR_SSE2  internal_wmemrchrSSE2
#  define INTERNAL_MEMICHR_SSE2 internal_wmemrichrSSE2
#  define INTERNAL_MEMCHR_386   _wmemrchr
#  define INTERNAL_MEMICHR_386  _wmemrichr
#  define MEMICHR               _wmemrichr
# endif
#endif

#ifndef _M_IX86
TYPE * __cdecl MEMIMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *first, *last, c;

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
			if (!(last = MEMICHR(first, c, last - first)))
				break;
			else if (MEMICMP(last, needle, needlelen) == 0)
				return last;
		while (last > first);
#endif
	}
	return NULL;
}
#else
#pragma warning(disable:4414)

static TYPE * __cdecl MEMIMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMIMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
static TYPE * __cdecl MEMIMEM_CPU_DISPATCH(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);

static TYPE *(__cdecl *MEMIMEM_DISPATCH)(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen) = MEMIMEM_CPU_DISPATCH;

__declspec(naked) TYPE * __cdecl MEMIMEM(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	__asm
	{
		jmp     dword ptr [MEMIMEM_DISPATCH]
	}
}

__declspec(naked) static TYPE * __cdecl MEMIMEM_SSE2(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
	extern TYPE * __vectorcall INTERNAL_MEMCHR_SSE2(const TYPE *buffer, __m128 c, size_t count);
	extern TYPE * __vectorcall INTERNAL_MEMICHR_SSE2(const TYPE *buffer, __m128 c, size_t count);
	extern TYPE * __cdecl INTERNAL_MEMMEM_SSE2(unsigned long c, TYPE *(__vectorcall *memchr)(const TYPE *, __m128, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);

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

		mov     eax, dword ptr [needlelen]              // eax = needlelen
		mov     ecx, dword ptr [needle]                 // ecx = needle
		test    eax, eax                                // check if needlelen == 0
		jz      needlelen_equal_zero                    // if needlelen == 0, leave
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
	needlelen_equal_zero:
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

__declspec(naked) static TYPE * __cdecl MEMIMEM_386(const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen)
{
#ifndef _UNICODE
	extern TYPE * __fastcall INTERNAL_MEMCHR_386(const TYPE *buffer, unsigned long c, size_t count);
	extern TYPE * __fastcall INTERNAL_MEMICHR_386(const TYPE *buffer, unsigned long c, size_t count);
	extern TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__fastcall *memchr)(const TYPE *, unsigned long, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
#else
	extern TYPE * __cdecl INTERNAL_MEMMEM_386(unsigned long c, TYPE *(__cdecl *memchr)(const TYPE *, TYPE, size_t), int *(__cdecl *memcmp)(const TYPE *, const TYPE *, size_t), void *reserved, const TYPE *haystack, size_t haystacklen, const TYPE *needle, size_t needlelen);
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

		mov     eax, dword ptr [needlelen]              // eax = needlelen
		mov     ecx, dword ptr [needle]                 // ecx = needle
		test    eax, eax                                // check if needlelen == 0
		jz      needlelen_equal_zero                    // if needlelen == 0, leave
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
	needlelen_equal_zero:
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
#undef MEMICMP
#undef MEMIMEM
#undef MEMIMEM_SSE2
#undef INTERNAL_MEMMEM_SSE2
#undef MEMIMEM_386
#undef INTERNAL_MEMMEM_386
#undef MEMIMEM_CPU_DISPATCH
#undef MEMIMEM_DISPATCH
#undef INTERNAL_MEMCHR_SSE2
#undef INTERNAL_MEMICHR_SSE2
#undef INTERNAL_MEMCHR_386
#undef INTERNAL_MEMICHR_386
#undef MEMICHR
