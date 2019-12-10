extern int __cdecl InstructionSet();

static char * __cdecl strpbrkSSE42(const char *string, const char *control);
static char * __cdecl strpbrkGeneric(const char *string, const char *control);
static char * __cdecl strpbrkCPUDispatch(const char *string, const char *control);

static char *(__cdecl *strpbrkDispatch)(const char *string, const char *control) = strpbrkCPUDispatch;

// function dispatching
__declspec(naked) char * __cdecl strpbrk(const char *string, const char *control)
{
	__asm
	{
		jmp     dword ptr [strpbrkDispatch]                 // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strpbrkSSE42(const char *string, const char *control)
{
	__asm
	{
		push    esi
		mov     ecx, dword ptr [esp + 8]                    // str
		mov     esi, dword ptr [esp + 12]                   // set

	str_next2:
		movdqu  xmm2, xmmword ptr [ecx]                     // str
		movdqu  xmm1, xmmword ptr [esi]                     // set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    eax, xmm0
		jns     set_extends2

	set_finished2:
		cmp     ax, -1
		jne     str_finished2
		// first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
		add     ecx, 16                                     // next 16 bytes of str
		jmp     str_next2

	str_finished2:
		xor     eax, -1
		xor     edx, edx
		bsf     eax, eax
		mov     dl, byte ptr [eax + ecx]
		add     eax, ecx
		test    dl, dl
		pop     esi
		cmovz   eax, edx
		ret

	set_loop2:
		and     eax, edx                                    // accumulate matches

	set_extends2:
		// the set is more than 16 bytes
		add     esi, 16
		movdqu  xmm1, xmmword ptr [esi]                     // next part of set
		pcmpistrm xmm1, xmm2, 00110000B                     // find in set, invert valid bits, return bit mask in xmm0
		movd    edx, xmm0
		jns     set_loop2
		mov     esi, dword ptr [esp + 16]                   // restore set pointer
		and     eax, edx                                    // accumulate matches
		jmp     set_finished2
	}
}

// Generic version
__declspec(naked) static char * __cdecl strpbrkGeneric(const char *string, const char *control)
{
	// Algorithm:
	//	char * __cdecl strpbrk(const char *string, const char *control)
	//	{
	//		unsigned long map[0x100 / 32] = { 1 };
	//		unsigned char c;
	//
	//		while (c = *(control++))
	//			_bittestandset(map, c);
	//		do
	//			c = *(string++);
	//		while (!_bittest(map, c));
	//		return c ? (char *)string - 1 : NULL;
	//	}
	//
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     eax, dword ptr [string]                     // eax = string
		mov     edx, dword ptr [control]                    // edx = control
		xor     ecx, ecx                                    // create and zero out char bit map
		push    0                                           // 256
		push    ecx                                         // 224
		push    ecx                                         // 192
		push    ecx                                         // 160
		push    ecx                                         // 128
		push    ecx                                         //  96
		push    ecx                                         //  64
		push    1                                           //  32
		jmp     listinit

		#define map     (esp)

		// Set control char bits in map
		align   16
	listnext:
		bts     dword ptr [map], ecx                        // init char bit map
	listinit:
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jnz     listnext

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cl, byte ptr [eax]
		inc     eax
		bt      dword ptr [map], ecx
		jnc     dstnext                                     // did not find char, continue

		// Return code
		xor     edx, edx
		cmp     ecx, 1
		adc     edx, -1
		dec     eax
		add     esp, 32
		and     eax, edx
		ret                                                 // __cdecl return

		#undef string
		#undef control
		#undef map
	}
}

// CPU dispatching for strpbrk. This is executed only once
__declspec(naked) static char * __cdecl strpbrkCPUDispatch(const char *string, const char *control)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version of strpbrk
		mov     ecx, offset strpbrkGeneric

		cmp     eax, 10                                     // check SSE4.2
		jb      Q200

		// SSE4.2 supported
		// Point to SSE4.2 version of strpbrk
		mov     ecx, offset strpbrkSSE42

	Q200:
		mov     dword ptr [strpbrkDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
