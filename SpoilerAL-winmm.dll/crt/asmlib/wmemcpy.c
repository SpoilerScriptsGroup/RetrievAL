#pragma function(wmemcpy)
#pragma function(memcpy)

extern int __cdecl InstructionSet();

static wchar_t * __cdecl wmemcpy686(wchar_t *dest, const wchar_t *src, size_t count);
static wchar_t * __cdecl wmemcpy386(wchar_t *dest, const wchar_t *src, size_t count);
static wchar_t * __cdecl wmemcpyCPUDispatch(wchar_t *dest, const wchar_t *src, size_t count);

static wchar_t *(__cdecl * wmemcpyDispatch)(wchar_t *dest, const wchar_t *src, size_t count) = wmemcpyCPUDispatch;

// wmemcpy function
__declspec(naked) wchar_t * __cdecl wmemcpy(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemcpyDispatch]                 // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) static wchar_t * __cdecl wmemcpy686(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]                   // count
		mov     ecx, -1                                     // maximum bytes
		add     eax, eax                                    // count * 2
		cmovc   eax, ecx                                    // number of bytes
		mov     dword ptr [esp + 12], eax
		jmp     memcpy
	}
}

// generic version
__declspec(naked) static wchar_t * __cdecl wmemcpy386(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]                   // count
		add     eax, eax                                    // count * 2
		jc      L1
		mov     dword ptr [esp + 12], eax                   // number of bytes
		jmp     memcpy
	L1:
		mov     dword ptr [esp + 12], -1                    // maximum bytes
		jmp     memcpy
	}
}

// CPU dispatching for wmemcpy. This is executed only once
__declspec(naked) static wchar_t * __cdecl wmemcpyCPUDispatch(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet
		// Point to generic version of wmemcpy
		mov     ecx, offset wmemcpy386
		cmp     eax, 4                                      // check SSE2
		jb      Q100
		// SSE4.2 supported
		// Point to SSE2 version of wmemcpy
		mov     ecx, offset wmemcpy686

	Q100:
		mov     dword ptr [wmemcpyDispatch], ecx
		// Continue in appropriate version of wmemcpy
		jmp     ecx
	}
}
