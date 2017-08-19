#include <windows.h>
#include "intrinsic.h"
#include "OptimizeAllocator.h"

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90

#if OPTIMIZE_ALLOCATOR
EXTERN_C void * __cdecl bcb6_malloc(size_t size);
EXTERN_C void __cdecl bcb6_free(void *memblock);

void __cdecl OptimizeAllocator()
{
	// ::operator delete(void *p);
	*(LPBYTE )0x005D4484 = JMP_REL32;
	*(LPDWORD)0x005D4485 = (DWORD)bcb6_free - (0x005D4485 + sizeof(DWORD));
	*(LPBYTE )0x005D4489 = NOP;

	// ::operator new(size_t n);
	*(LPBYTE )0x005D44B8 = JMP_REL32;
	*(LPDWORD)0x005D44B9 = (DWORD)bcb6_malloc - (0x005D44B9 + sizeof(DWORD));
	*(LPBYTE )0x005D44BD = NOP;

	// std::allocator::node_alloc::allocate(size_t n);
	*(LPBYTE )0x005F43F0 = JMP_REL32;
	*(LPDWORD)0x005F43F1 = (DWORD)bcb6_malloc - (0x005F43F1 + sizeof(DWORD));
	*(LPBYTE )0x005F43F5 = NOP;

	// std::allocator::node_alloc::deallocate(void *p, size_t n);
	*(LPBYTE )0x005F47A0 = JMP_REL32;
	*(LPDWORD)0x005F47A1 = (DWORD)bcb6_free - (0x005F47A1 + sizeof(DWORD));
	*(LPBYTE )0x005F47A5 = NOP;
}
#endif
