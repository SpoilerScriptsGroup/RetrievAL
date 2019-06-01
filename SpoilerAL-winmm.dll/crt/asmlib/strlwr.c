extern int __cdecl InstructionSet();
extern char * __cdecl struprlwrSSE42(char *string);
extern const char struprlwrSSE42_casebit[16];

__declspec(align(16)) static const char azhigh[16] = {      // define range for upper case
	'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z'
};

static char * __cdecl strlwrSSE42(char *string);
static char * __cdecl strlwrGeneric(char *string);
static char * __cdecl strlwrCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl * strlwrDispatch)(char *string) = strlwrCPUDispatch;

__declspec(naked) char * __cdecl _strlwr(char *string)
{
	__asm
	{
		jmp     dword ptr [strlwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) static char * __cdecl strlwrSSE42(char *string)
{
	__asm
	{
		movdqa  xmm1, xmmword ptr [azhigh]                  // define range A-Z
		movdqa  xmm3, xmmword ptr [struprlwrSSE42_casebit]  // bit to change
		jmp     struprlwrSSE42
	}
}

// 386 version
__declspec(naked) static char * __cdecl strlwrGeneric(char *string)
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

	A100:
		// loop
		mov     al, byte ptr [edx]
		test    al, al
		jz      A900                                        // end of string
		sub     al, 'A'
		cmp     al, 'Z' - 'A'
		jbe     A200                                        // is upper case
		inc     edx
		jmp     A100                                        // loop to next character

	A200:
		// convert to lower case
		add     al, 'a'
		mov     byte ptr [edx], al
		inc     edx
		jmp     A100

	A900:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

// CPU dispatching for strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset strlwrGeneric
		cmp     eax, 10                                     // check SSE4.2
		jb      Q100

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset strlwrSSE42

	Q100:
		mov     dword ptr [strlwrDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
