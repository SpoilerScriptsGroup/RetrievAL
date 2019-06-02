extern int __cdecl InstructionSet();
extern void __cdecl CpuType(int *vendor, int *family, int *model);

__declspec(naked) int __cdecl Store256BitIsFaster()
{
	__asm
	{
		call    InstructionSet
		cmp     eax, 11                                     // AVX supported
		jb      S90
		push    0                                           // vendor
		push    0                                           // family
		push    0                                           // model
		mov     eax, esp
		push    eax                                         // &model
		add     eax, 4
		push    eax                                         // &family
		add     eax, 4
		push    eax                                         // &vendor
		call    CpuType                                     // get vendor, family, model
		add     esp, 12
		pop     edx                                         // model
		pop     ecx                                         // family
		pop     eax                                         // vendor

		cmp     eax, 1                                      // Intel
		je      S_Intel
		cmp     eax, 2                                      // AMD
		je      S_AMD
#if 0
		cmp     eax, 3                                      // VIA
		je      S_VIA
#endif
		jmp     S91                                         // other vendor, not known

	S_Intel:
		cmp     ecx, 6
		jne     S92                                         // unknown family. possibly future model
		// model 2AH Sandy Bridge
		// model 3AH Ivy Bridge
		// model 3CH Haswell
		// Sandy Bridge and Ivy Bridge are slightly faster with 128 than with 256 bit moves on large data blocks
		// Haswell is much faster with 256 bit moves
		cmp     edx, 3AH
		jbe     S90
		jmp     S92

	S_AMD:
		// AMD
		cmp     ecx, 15H                                    // family 15h = Bulldozer, Piledriver
		ja      S92                                         // assume future AMD families are faster
		// model 1 = Bulldozer is a little slower on 256 bit write
		// model 2 = Piledriver is terribly slow on 256 bit write
		// assume future models 3-4 are like Bulldozer
		cmp     edx, 4
		jbe     S90
		jmp     S91                                         // later models: don't know

#if 0
	S_VIA:
		jmp     S91                                         // don't know
#endif

	S90:
		xor     eax, eax                                    // return 0
		ret

	S91:
		mov     eax, 1                                      // return 1
		ret

	S92:
		mov     eax, 2                                      // return 2
		ret
	}
}
