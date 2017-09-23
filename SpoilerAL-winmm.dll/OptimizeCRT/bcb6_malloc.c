#include <windows.h>
#include "bcb6_errno.h"

extern HANDLE hHeap;

void * __cdecl bcb6_calloc(size_t num, size_t size)
{
	void *memblock;

	if (!(size *= num))
		size++;
	memblock = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
	if (!memblock)
		bcb6_errno = BCB6_ENOMEM;
	return memblock;
}

void * __cdecl bcb6_malloc(size_t size)
{
	void *memblock;

	if (!size)
		size++;
	memblock = HeapAlloc(hHeap, 0, size);
	if (!memblock)
		bcb6_errno = BCB6_ENOMEM;
	return memblock;
}

void __cdecl bcb6_free(void *memblock)
{
	if (memblock)
		HeapFree(hHeap, 0, memblock);
}

void * __cdecl bcb6_realloc(void *memblock, size_t size)
{
	if (size)
	{
		memblock = memblock ?
			HeapReAlloc(hHeap, 0, memblock, size) :
			HeapAlloc(hHeap, 0, size);
		if (!memblock)
			bcb6_errno = BCB6_ENOMEM;
	}
	else
	{
		if (memblock)
			HeapFree(hHeap, 0, memblock);
		memblock = NULL;
	}
	return memblock;
}
