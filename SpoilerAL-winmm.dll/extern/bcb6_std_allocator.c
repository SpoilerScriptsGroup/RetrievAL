#include "bcb6_std_allocator.h"
#include "bcb6_malloc.h"
#include "TWinControl.h"
#include "TMainForm.h"

void *(__cdecl *bcb6_std_node_alloc_allocate)(size_t n) = (LPVOID)0x005F43F0;
void (__cdecl *bcb6_std_node_alloc_deallocate)(void *p, size_t n) = (LPVOID)0x005F47A0;

#define MAX_BYTES          bcb6_std_allocator_MAX_BYTES
#define lpApplicationTitle (LPCSTR)0x006020D8

__declspec(naked) void * __fastcall bcb6_std_allocator_allocate(size_t n)
{
	__asm
	{
		cmp     ecx, MAX_BYTES
		push    ecx
		mov     eax, ecx
		jbe     L1
		call    dword ptr [bcb6_global_operator_new]
		jmp     L2
	L1:
		test    ecx, ecx
		jz      L3
		call    dword ptr [bcb6_std_node_alloc_allocate]
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

__declspec(naked) void __fastcall bcb6_std_allocator_deallocate(void *p, size_t n)
{
	__asm
	{
		test    ecx, ecx
		jz      L3
		cmp     edx, MAX_BYTES
		push    edx
		push    ecx
		jbe     L1
		call    dword ptr [bcb6_global_operator_delete]
		jmp     L2
	L1:
		call    dword ptr [bcb6_std_node_alloc_deallocate]
	L2:
		add     esp, 8
	L3:
		ret
	}
}

void * __fastcall bcb6_std_allocator_reallocate(void *p, size_t from, size_t to)
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
				p = bcb6_std_allocator_allocate(to);
				if (from)
				{
					memcpy(p, src, min(from, to));
					bcb6_std_allocator_deallocate(src, from);
				}
			}
		}
	}
	else
	{
		if (from)
			bcb6_std_allocator_deallocate(p, from);
		p = NULL;
	}
	return p;
}

void __cdecl bad_alloc()
{
	char lpText[256];

	FormatMessageA(
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		ERROR_OUTOFMEMORY,
		LANG_USER_DEFAULT,
		lpText,
		_countof(lpText) - 1,
		NULL);
	MessageBoxA(TWinControl_GetHandle(MainForm), lpText, lpApplicationTitle, MB_ICONERROR);
	ExitProcess(3);
}
