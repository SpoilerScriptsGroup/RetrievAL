
__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter()
{
	__asm
	{
		#define signedLoop esp
		#define Step       (ebp - 0F8H)
		#define _End       (ebp - 0F4H)
		#define i          esi

		mov     eax, 0x004FF3A4// break
		mov     ecx, i
		mov     edx, dword ptr [_End]
		cmp     dword ptr [Step], 0
		bt      dword ptr [signedLoop], 0
		jc      SIGNED
		cmp     ecx, edx
		mov     ecx, 0x004FF2F1
		cmovb   eax, ecx
		jmp     eax

		align   16
	SIGNED:
		cmovs   ecx, edx
		cmovs   edx, i
		cmp     ecx, edx
		mov     ecx, 0x004FF2F1
		cmovl   eax, ecx
		jmp     eax

		#undef signedLoop
		#undef Step
		#undef _End
		#undef i
	}
}
