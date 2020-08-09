
__declspec(naked) void __fastcall TSSGCtrl_ReadSSRFile_CompareLoopCounter(int unsign)
{
	__asm
	{// loop if ecx is 0
		#define Step      (ebp - 0xF8)
		#define End       (ebp - 0xF4)
		#define i          esi

		jecxz   SIGNED

		cmp     i  , dword ptr [End]
		setae   cl
		movzx   ecx, cl
		ret

		align   16
	SIGNED:
		mov     eax, i
		mov     edx, dword ptr [End]
		cmp     dword ptr [Step], 0
		cmovs   eax, edx
		cmovs   edx, i
		cmp     eax, edx
		setge   cl
		movzx   ecx, cl
		ret
			
		#undef i
		#undef End
		#undef Step
	}
}
