#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "bcb6_std_vector_string.h"

#if OPTIMIZE_ALLOCATOR

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90
#define NOP_X2    (WORD)0x9090

static __inline void __cdecl OptimizeString()
{
	// std::string::string();
	/*
		mov     ecx, dword ptr [esp + 4]                ; 00402590 _ 8B. 4C 24, 04
		jmp     string_ctor                             ; 00402594 _ E9, ????????
	*/
	*(LPDWORD)0x00402590 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x00402594 = JMP_REL32;
	*(LPDWORD)0x00402595 = (DWORD)string_ctor - (0x00402595 + sizeof(DWORD));
	*(LPWORD )0x00402599 = NOP_X2;

	// std::string::assign(const char *first, const char *last);
	/*
		mov     ecx, dword ptr [esp + 12]               ; 004159D4 _ 8B. 4C 24, 0C
		pop     eax                                     ; 004159D8 _ 58
		push    ecx                                     ; 004159D9 _ 51
		push    eax                                     ; 004159DA _ 50
		mov     ecx, dword ptr [esp +  8]               ; 004159DB _ 8B. 4C 24, 08
		mov     edx, dword ptr [esp + 12]               ; 004159DF _ 8B. 54 24, 0C
		jmp     string_assign_range                     ; 004159E3 _ E9, ????????
	*/
	*(LPDWORD)0x004159D4 = BSWAP32(0x8B4C240C);
	*(LPDWORD)0x004159D8 = BSWAP32(0x5851508B);
	*(LPDWORD)0x004159DC = BSWAP32(0x4C24088B);
	*(LPDWORD)0x004159E0 = BSWAP32(0x54240CE9);
	*(LPDWORD)0x004159E4 = (DWORD)string_assign_range - (0x004159E4 + sizeof(DWORD));
	*(LPBYTE )0x004159E8 = NOP;

	// std::string::string(const char *s);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 004165E8 _ 8B. 4C 24, 04
		mov     edx, dword ptr [esp + 8]                ; 004165EC _ 8B. 54 24, 08
		jmp     string_ctor_assign_cstr                 ; 004165F0 _ E9, ????????
	*/
	*(LPDWORD)0x004165E8 = BSWAP32(0x8B4C2404);
	*(LPDWORD)0x004165EC = BSWAP32(0x8B542408);
	*(LPBYTE )0x004165F0 = JMP_REL32;
	*(LPDWORD)0x004165F1 = (DWORD)string_ctor_assign_cstr - (0x004165F1 + sizeof(DWORD));

	// std::string::string(string &s);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 004166F0 _ 8B. 4C 24, 04
		mov     edx, dword ptr [esp + 8]                ; 004166F4 _ 8B. 54 24, 08
		jmp     string_ctor_assign                      ; 004166F8 _ E9, ????????
	*/
	*(LPDWORD)0x004166F0 = BSWAP32(0x8B4C2404);
	*(LPDWORD)0x004166F4 = BSWAP32(0x8B542408);
	*(LPBYTE )0x004166F8 = JMP_REL32;
	*(LPDWORD)0x004166F9 = (DWORD)string_ctor_assign - (0x004166F9 + sizeof(DWORD));

	// std::string::append(const char *first, const char *last);
	/*
		mov     ecx, dword ptr [esp + 12]               ; 00418610 _ 8B. 4C 24, 0C
		pop     eax                                     ; 00418614 _ 58
		push    ecx                                     ; 00418615 _ 51
		push    eax                                     ; 00418616 _ 50
		mov     ecx, dword ptr [esp +  8]               ; 00418617 _ 8B. 4C 24, 08
		mov     edx, dword ptr [esp + 12]               ; 0041861B _ 8B. 54 24, 0C
		jmp     string_append_range                     ; 0041861F _ E9, ????????
	*/
	*(LPDWORD)0x00418610 = BSWAP32(0x8B4C240C);
	*(LPDWORD)0x00418614 = BSWAP32(0x5851508B);
	*(LPDWORD)0x00418618 = BSWAP32(0x4C24088B);
	*(LPDWORD)0x0041861C = BSWAP32(0x54240CE9);
	*(LPDWORD)0x00418620 = (DWORD)string_append_range - (0x00418620 + sizeof(DWORD));
	*(LPWORD )0x00418624 = NOP_X2;

	// std::string::reserve(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 004462DC _ 8B. 4C 24, 04
		mov     edx, dword ptr [esp + 8]                ; 004462E0 _ 8B. 54 24, 08
		jmp     string_reserve                          ; 004462E4 _ E9, ????????
	*/
	*(LPDWORD)0x004462DC = BSWAP32(0x8B4C2404);
	*(LPDWORD)0x004462E0 = BSWAP32(0x8B542408);
	*(LPBYTE )0x004462E4 = JMP_REL32;
	*(LPDWORD)0x004462E5 = (DWORD)string_reserve - (0x004462E5 + sizeof(DWORD));

	// std::string::append(size_t n, char c);
	/*
		mov     ecx, dword ptr [esp + 12]               ; 00462FFC _ 8B. 4C 24, 0C
		pop     eax                                     ; 00463000 _ 58
		push    ecx                                     ; 00463001 _ 51
		push    eax                                     ; 00463002 _ 50
		mov     ecx, dword ptr [esp +  8]               ; 00463003 _ 8B. 4C 24, 08
		mov     edx, dword ptr [esp + 12]               ; 00463007 _ 8B. 54 24, 0C
		jmp     string_append                           ; 0046300B _ E9, ????????
	*/
	*(LPDWORD)0x00462FFC = BSWAP32(0x8B4C240C);
	*(LPDWORD)0x00463000 = BSWAP32(0x5851508B);
	*(LPDWORD)0x00463004 = BSWAP32(0x4C24088B);
	*(LPDWORD)0x00463008 = BSWAP32(0x54240CE9);
	*(LPDWORD)0x0046300C = (DWORD)string_append - (0x0046300C + sizeof(DWORD));
	*(LPWORD )0x00463010 = NOP_X2;
	*(LPBYTE )0x00463012 = NOP;
}

static __inline void __cdecl OptimizeVector()
{
	// std::vector<std::string>::reserve(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 00473564 _ 8B. 4C 24, 04
		mov     edx, dword ptr [esp + 8]                ; 00473568 _ 8B. 54 24, 08
		jmp     vector_string_reserve                   ; 0047356C _ E9, ????????
	*/
	*(LPDWORD)0x00473564 = BSWAP32(0x8B4C2404);
	*(LPDWORD)0x00473568 = BSWAP32(0x8B542408);
	*(LPBYTE )0x0047356C = JMP_REL32;
	*(LPDWORD)0x0047356D = (DWORD)vector_string_reserve - (0x0047356D + sizeof(DWORD));
	*(LPWORD )0x00473571 = NOP_X2;

	// std::vector<BYTE>::reserve(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 0050F128 _ 8B. 4C 24, 04
		mov     edx, dword ptr [esp + 8]                ; 0050F12C _ 8B. 54 24, 08
		jmp     vector_byte_reserve                     ; 0050F130 _ E9, ????????
	*/
	*(LPDWORD)0x0050F128 = BSWAP32(0x8B4C2404);
	*(LPDWORD)0x0050F12C = BSWAP32(0x8B542408);
	*(LPBYTE )0x0050F130 = JMP_REL32;
	*(LPDWORD)0x0050F131 = (DWORD)vector_byte_reserve - (0x0050F131 + sizeof(DWORD));
	*(LPWORD )0x0050F135 = NOP_X2;
}

void __cdecl OptimizeAllocator()
{
	OptimizeString();
	OptimizeVector();

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
		jmp     _allocator_allocate                     ; 005D44BC _ E9, ????????
	*/
	*(LPDWORD)0x005D44B8 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005D44BC = JMP_REL32;
	*(LPDWORD)0x005D44BD = (DWORD)_allocator_allocate - (0x005D44BD + sizeof(DWORD));
	*(LPWORD )0x005D44C1 = NOP_X2;

	// std::allocator::node_alloc::allocate(size_t n);
	/*
		mov     ecx, dword ptr [esp + 4]                ; 005F43F0 _ 8B. 4C 24, 04
		jmp     _allocator_allocate                     ; 005F43F4 _ E9, ????????
	*/
	*(LPDWORD)0x005F43F0 = BSWAP32(0x8B4C2404);
	*(LPBYTE )0x005F43F4 = JMP_REL32;
	*(LPDWORD)0x005F43F5 = (DWORD)_allocator_allocate - (0x005F43F5 + sizeof(DWORD));
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
