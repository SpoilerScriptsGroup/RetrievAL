#define USING_NAMESPACE_BCB6_GLOBAL
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "TWinControl.h"
#include "TMainForm.h"

void *(__cdecl *node_alloc_allocate)(size_t n) = (LPVOID)0x005F43F0;
void (__cdecl *node_alloc_deallocate)(void *p, size_t n) = (LPVOID)0x005F47A0;

#undef allocator_allocate
#undef allocator_deallocate
#undef allocator_reallocate
#define allocator_allocate   _bcb6_std_allocator_allocate
#define allocator_deallocate _bcb6_std_allocator_deallocate
#define allocator_reallocate _bcb6_std_allocator_reallocate

#if !OPTIMIZE_ALLOCATOR
#define MAX_BYTES 128
void * __cdecl bcb6_realloc(void *memblock, size_t size);
#else
extern HANDLE hHeap;
#endif

#define lpApplicationTitle (LPCSTR)0x006020D8

#if !OPTIMIZE_ALLOCATOR
__declspec(naked) void * __fastcall allocator_allocate(size_t n)
{
	__asm
	{
		cmp     ecx, MAX_BYTES
		push    ecx
		mov     eax, ecx
		jbe     L1
		call    dword ptr [operator_new]
		jmp     L2
	L1:
		test    ecx, ecx
		jz      L3
		call    dword ptr [node_alloc_allocate]
	L2:
		test    eax, eax
		jz      L4
	L3:
		pop     ecx
		ret

		align   16
	L4:
		pop     ecx
		call    bad_alloc
		xor     eax, eax
		ret
	}
}
#else
void * __fastcall allocator_allocate(size_t n)
{
	if (n)
	{
		void *p = HeapAlloc(hHeap, 0, n);
		if (!p)
			bad_alloc();
		return p;
	}
	else
	{
		return NULL;
	}
}
#endif

#if !OPTIMIZE_ALLOCATOR
__declspec(naked) void __fastcall allocator_deallocate(void *p, size_t n)
{
	__asm
	{
		test    ecx, ecx
		jz      L3
		cmp     edx, MAX_BYTES
		push    edx
		push    ecx
		jbe     L1
		call    dword ptr [operator_delete]
		jmp     L2
	L1:
		call    dword ptr [node_alloc_deallocate]
	L2:
		add     esp, 8
	L3:
		ret
	}
}
#else
void __fastcall allocator_deallocate(void *p)
{
	if (p)
		HeapFree(hHeap, 0, p);
}
#endif

#if !OPTIMIZE_ALLOCATOR
void * __fastcall allocator_reallocate(void *p, size_t from, size_t to)
{
	if (to)
	{
		if (from != to)
		{
			if (from > MAX_BYTES && to > MAX_BYTES)
			{
				p = bcb6_realloc(p, to);
				if (!p)
					bad_alloc();
			}
			else
			{
				void *src = p;
				p = allocator_allocate(to);
				if (from)
				{
					memcpy(p, src, min(from, to));
					allocator_deallocate(src, from);
				}
			}
		}
	}
	else
	{
		if (from)
			allocator_deallocate(p, from);
		p = NULL;
	}
	return p;
}
#else
void * __fastcall allocator_reallocate(void *p, size_t n)
{
	if (n)
	{
		p = p ? HeapReAlloc(hHeap, 0, p, n) : HeapAlloc(hHeap, 0, n);
		if (!p)
			bad_alloc();
	}
	else
	{
		if (p)
			HeapFree(hHeap, 0, p);
		p = NULL;
	}
	return p;
}
#endif

void __cdecl bad_alloc()
{
	char lpText[256];

	FormatMessageA(
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		ERROR_OUTOFMEMORY,
		LANGIDFROMLCID(GetThreadLocale()),
		lpText,
		_countof(lpText),
		NULL);
	MessageBoxA(TWinControl_GetHandle(MainForm), lpText, lpApplicationTitle, MB_ICONERROR);
	ExitProcess(3);
}
