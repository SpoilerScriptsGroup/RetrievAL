extern int __cdecl InstructionSet();
extern char * __cdecl struprlwrSSE42(char *string);
extern const char struprlwrSSE42_casebit[16];

__declspec(align(16)) static const char azlow[16] = {       // define range for lower case
	'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z'
};

static char * __cdecl struprSSE42(char *string);
static char * __cdecl struprGeneric(char *string);
static char * __cdecl struprCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl * struprDispatch)(char *string) = struprCPUDispatch;

__declspec(naked) char * __cdecl _strupr(char *string)
{
	__asm
	{
		jmp     dword ptr [struprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

__declspec(naked) static char * __cdecl struprSSE42(char *string)
{
	__asm
	{
		movdqa  xmm1, xmmword ptr [azlow]                   // define range a-z
		movdqa  xmm3, xmmword ptr [struprlwrSSE42_casebit]  // bit to change
		jmp     struprlwrSSE42
	}
}

// 386 version
__declspec(naked) static char * __cdecl struprGeneric(char *string)
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

	B100:
		// loop
		mov     al, byte ptr [edx]
		test    al, al
		jz      B900                                        // end of string
		sub     al, 'a'
		cmp     al, 'z' - 'a'
		jbe     B200                                        // is lower case
		inc     edx
		jmp     B100                                        // loop to next character

	B200:
		// convert to upper case
		add     al, 'A'
		mov     byte ptr [edx], al
		inc     edx
		jmp     B100

	B900:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

// CPU dispatching for strupr. This is executed only once
__declspec(naked) static char * __cdecl struprCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset struprGeneric
		cmp     eax, 10                                     // check SSE4.2
		jb      Q200

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset struprSSE42

	Q200:
		mov     dword ptr [struprDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
