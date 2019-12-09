#include <windows.h>
#include <limits.h>

extern HANDLE hHeap;

unsigned long *utf16map = NULL;	// 8 kilo byte

#ifndef _M_IX86
unsigned long *alloc_utf16map()
{
	return utf16map ? utf16map : (utf16map = (unsigned long *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 0x10000 / CHAR_BIT));
}
#else
__declspec(naked) unsigned long *alloc_utf16map()
{
	__asm
	{
		mov     eax, dword ptr [utf16map]
		mov     ecx, dword ptr [hHeap]
		test    eax, eax
		jnz     map_is_not_null
		push    0x10000 / CHAR_BIT
		push    HEAP_ZERO_MEMORY
		push    ecx
		call    HeapAlloc
		mov     dword ptr [utf16map], eax
	map_is_not_null:
		ret
	}
}
#endif

#ifndef _M_IX86
void __cdecl free_utf16map()
{
	LPVOID lpMem;

	if (lpMem = utf16map)
	{
		utf16map = NULL;
		HeapFree(hHeap, 0, lpMem);
	}
}
#else
__declspec(naked) void __cdecl free_utf16map()
{
	__asm
	{
		mov     eax, dword ptr [utf16map]
		mov     ecx, dword ptr [hHeap]
		test    eax, eax
		jz      map_is_null
		mov     dword ptr [utf16map], 0
		push    eax
		push    0
		push    ecx
		call    HeapFree
	map_is_null:
		ret
	}
}
#endif
