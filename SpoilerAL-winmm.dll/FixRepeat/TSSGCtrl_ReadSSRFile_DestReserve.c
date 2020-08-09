
__declspec(naked) long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(int unsign)
{
	__asm
	{
		#define Step       (ebp - 0xF8)

		jecxz   SIGNED
		xor     edx, edx
		div     dword ptr [Step]
		ret

		align   16
	SIGNED:
		cdq
		idiv    dword ptr [Step]
		ret

		#undef Step
	}
}
