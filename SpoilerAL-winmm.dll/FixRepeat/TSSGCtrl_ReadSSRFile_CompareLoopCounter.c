
__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter()
{
	__asm
	{
		#define signedLoop esp
		#define Step       (ebp - 0F8H)
		#define _End       (ebp - 0F4H)
		#define i          esi

		mov     eax, dword ptr [signedLoop]
		mov     ecx, dword ptr [_End]
		test    eax, eax
		mov     edx, dword ptr [Step]
		mov     eax, 004FF2F1H
		jnz     L1
		cmp     i, ecx
		jb      L4
		jmp     L3
	L1:
		test    edx, edx
		js      L2
		cmp     i, ecx
		jl      L4
		jmp     L3
	L2:
		cmp     i, ecx
		jg      L4
	L3:
		mov     eax, 004FF3A4H
	L4:
		jmp     eax

		#undef signedLoop
		#undef Step
		#undef _End
		#undef i
	}
}
