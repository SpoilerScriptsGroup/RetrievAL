__declspec(align(16)) const char struprlwrSSE42_casebit[16] = {
	                                                        // bit to change when changing case
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};

__declspec(naked) char * __cdecl struprlwrSSE42(char *string)
{
	__asm
	{
		// common code for strupr and strlwr
		mov     edx, dword ptr [esp + 4]                    // string

	next:
		// loop
		movdqu  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm1, xmm2, 01000100b                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm2, xmm0                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm2                     // write changed value
		add     edx, 16
		jmp     next                                        // next 16 bytes

	last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     finish                                      // nothing changed, no need to write
		pand    xmm3, xmm0                                  // mask and case bit
		pxor    xmm2, xmm3                                  // change case bit

		// less elegant alternative, but probably faster if data needed again soon
		// write 8-4-2-1 bytes, if necessary
		pmovmskb eax, xmm0                                  // create bit mask
		cmp     eax, 10000000b
		jb      L10

		// there are at least 8 bytes to write
		movq    qword ptr [edx], xmm2
		psrldq  xmm2, 8
		add     edx, 8
		shr     eax, 8

	L10:
		cmp     eax, 1000b
		jb      L20

		// there are at least 4 bytes to write
		movd    dword ptr [edx], xmm2
		psrldq  xmm2, 4
		add     edx, 4
		shr     eax, 4

	L20:
		movd    ecx, xmm2                                   // use ecx for last 3 bytes
		cmp     eax, 10b
		jb      L30

		// there are at least 2 bytes to write
		mov     word ptr [edx], cx
		shr     ecx, 16
		add     edx, 2
		shr     eax, 2

	L30:
		cmp     eax, 1
		jb      finish

		// there is one more byte to write
		mov     byte ptr [edx], cl

	finish:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}
