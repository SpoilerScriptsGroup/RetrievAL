#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"

#if OPTIMIZE_ALLOCATOR

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90
#define NOP_X2    (WORD)0x9090

void __cdecl OptimizeAllocator()
{
	// ::operator delete(void *p);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 005D4484 _ 8B. 4C 24, 04
		jmp     _allocator_deallocate                   ; 005D4488 _ E9, ????????
	*/
	*(LPDWORD)0x005D4484 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005D4488 = JMP_REL32;
	*(LPDWORD)0x005D4489 = (DWORD)_allocator_deallocate - (0x005D4489 + sizeof(DWORD));
	*(LPBYTE )0x005D448D = NOP;
	*(LPWORD )0x005D448E = NOP_X2;

	// ::operator new(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 005D44B8 _ 8B. 4C 24, 04
		jmp     allocator_allocate                      ; 005D44BC _ E9, ????????
	*/
	*(LPDWORD)0x005D44B8 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005D44BC = JMP_REL32;
	*(LPDWORD)0x005D44BD = (DWORD)allocator_allocate - (0x005D44BD + sizeof(DWORD));
	*(LPWORD )0x005D44C1 = NOP_X2;

	// std::allocator::node_alloc::allocate(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 005F43F0 _ 8B. 4C 24, 04
		jmp     allocator_allocate                      ; 005F43F4 _ E9, ????????
	*/
	*(LPDWORD)0x005F43F0 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005F43F4 = JMP_REL32;
	*(LPDWORD)0x005F43F5 = (DWORD)allocator_allocate - (0x005F43F5 + sizeof(DWORD));
	*(LPWORD )0x005F43F9 = NOP_X2;

	// std::allocator::node_alloc::deallocate(void *p, size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 005F47A0 _ 8B. 4C 24, 04
		jmp     _allocator_deallocate                   ; 005F47A4 _ E9, ????????
	*/
	*(LPDWORD)0x005F47A0 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005F47A4 = JMP_REL32;
	*(LPDWORD)0x005F47A5 = (DWORD)_allocator_deallocate - (0x005F47A5 + sizeof(DWORD));
	*(LPWORD )0x005F47A9 = NOP_X2;
}
#endif
