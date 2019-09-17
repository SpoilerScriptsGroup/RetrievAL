#pragma function(wmemmove)
#pragma function(memmove)

extern int __cdecl InstructionSet();

static wchar_t * __cdecl wmemmove686(wchar_t *dest, const wchar_t *src, size_t count);
static wchar_t * __cdecl wmemmove386(wchar_t *dest, const wchar_t *src, size_t count);
static wchar_t * __cdecl wmemmoveCPUDispatch(wchar_t *dest, const wchar_t *src, size_t count);

static wchar_t *(__cdecl * wmemmoveDispatch)(wchar_t *dest, const wchar_t *src, size_t count) = wmemmoveCPUDispatch;

// wmemmove function
__declspec(naked) wchar_t * __cdecl wmemmove(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemmoveDispatch]                // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) static wchar_t * __cdecl wmemmove686(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]                   // count
		mov     ecx, -1                                     // maximum bytes
		add     eax, eax                                    // count * 2
		cmovc   eax, ecx                                    // number of bytes
		mov     dword ptr [esp + 12], eax
		jmp     memmove
	}
}

// generic version
__declspec(naked) static wchar_t * __cdecl wmemmove386(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]                   // count
		add     eax, eax                                    // count * 2
		jc      L1
		mov     dword ptr [esp + 12], eax                   // number of bytes
		jmp     memmove
	L1:
		mov     dword ptr [esp + 12], -1                    // maximum bytes
		jmp     memmove
	}
}

// CPU dispatching for wmemmove. This is executed only once
__declspec(naked) static wchar_t * __cdecl wmemmoveCPUDispatch(wchar_t *dest, const wchar_t *src, size_t count)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet
		// Point to generic version of wmemmove
		mov     ecx, offset wmemmove386
		cmp     eax, 4                                      // check SSE2
		jb      Q100
		// SSE4.2 supported
		// Point to SSE2 version of wmemmove
		mov     ecx, offset wmemmove686

	Q100:
		mov     dword ptr [wmemmoveDispatch], ecx
		// Continue in appropriate version of wmemmove
		jmp     ecx
	}
}
